#include "Process.h"
#include <codecvt>
#include <fstream>
#include "HTMLparser.h"
#include <sstream>
#define CPU_CORES 4

using namespace std;
extern const std::locale empty_locale = std::locale::empty();
typedef std::codecvt_utf8<wchar_t> converter_type;
extern const converter_type* converter = new converter_type;
extern const std::locale utf8_locale = std::locale(empty_locale, converter);

int flag = 0;//已下载的网页数
int curent_worker = 0;//正在工作的下载线程数
int parse_flag = 0;
int current_parser = 0;
#define MAX_WORKER 8//最大线程数
mutex mtx;//全局线程锁
config g_conf;

QStringList g_urllist;
LinkList<PageInfo> pageinfo_list;
AVLTree<String> Process::dic_avl;
InvertedIndexDocument inverted_index_document;
String space(L"nbsp");

void Process::setconfig(config _conf)
{
    g_conf = _conf;
}

void Process::init()
{
    /*
     * 读取CSV文件并分离出url, 保存到urllist中
     */
    system("mkdir temp");
    QFile csvfile("input/url.csv");
    if (!csvfile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can not open input/url.csv";
        system("pause");
        exit(0);
    }

    QTextStream* in = new QTextStream(&csvfile);
    QStringList lines = in->readAll().split("\n");
    const auto rowcount = lines.size();
    for (int i = 1; i < rowcount; i++)
    {
        if (!lines[i].isEmpty())
        {
            auto line = lines[i].split(",");
            const auto url = line[1].mid(1, line[1].size() - 3);
            g_urllist.push_back(url);
        }
    }
    /*
     * 是否加载云词典
     */
    if (g_conf.DOWNLOAD_CLOUD_DIC)
    {
        cURL("http://sync.chenyijie.me/dictionary/cloud_dictionary.dic", "dictionary/cloud_dictionary.dic", false);
    }
    /*
     * 判断是否需下载网页
     */
    if (g_conf.DOWNLOAD_HTML)
        DownloadHTML(g_urllist);
}

void Process::DownloadHTML(QStringList urllist)
{
    for (int i = 0; i < urllist.size(); i++)
    {
        QString savefile = QString("temp/%1.html").arg(i);
        thread tr(cURL, urllist[i], savefile, true);
        tr.detach();
    }

    /*
    * 等待所有网页下载完成
    */
    while (true)
    {
        if (flag >= urllist.size())
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(200));
    }

    qDebug() << "All pages has been download";
}

/**
 * \brief 加载以平衡树形式存储的词典
 * \return 
 */
AVLTree<String> Process::initDictionary_avl()
{
    AVLTree<String> stringavltree;
    std::wifstream dic_fin("dictionary/dictionary.dic");
    dic_fin.imbue(utf8_locale);


    int total = 0;
    if (dic_fin)
    {
        wstring temp;
        while (getline(dic_fin, temp))
        {
            total++;
            if (total % 1000 == 0)
            {
                cout << "\r Loading Dictionary... [" << total << "] words";
            }
            String tmp(temp);
            stringavltree.insert(tmp);
        }
    }
    else
    {
        cout << "error";
        throw Error("fail to load dictionary");
    }
    std::wifstream extra_dic_fin("dictionary/extra_dictionary.dic");
    extra_dic_fin.imbue(utf8_locale);
    if (extra_dic_fin)
    {
        wstring temp;
        while (getline(extra_dic_fin, temp))
        {
            total++;
            if (total % 1000 == 0)
            {
                cout << "\r Loading Extra Dictionary... [" << total << "] words";
            }
            String tmp(temp);
            stringavltree.insert(tmp);
        }
    }
    else
    {
        cout << "error";
        throw Error("fail to load extra dictionary");
    }

    if (g_conf.DOWNLOAD_CLOUD_DIC)
    {
        std::wifstream cloud_dic_fin("dictionary/cloud_dictionary.dic");
        cloud_dic_fin.imbue(utf8_locale);
        if (cloud_dic_fin)
        {
            wstring temp;
            while (getline(cloud_dic_fin, temp))
            {
                total++;
                if (total % 1000 == 0)
                {
                    cout << "\r Loading Cloud Dictionary... [" << total << "] words";
                }
                String tmp(temp);
                stringavltree.insert(tmp);
            }
        }
        else
        {
            cout << "error\n";
            cout << "fail to load cloud dictionary";
        }
    }
    return stringavltree;
}

/**
 * \brief 加载以哈希表形式存储的词典
 * \return 
 */
StringHashTable Process::initDictionary_hash()
{
    StringHashTable strhashtable;

    std::wifstream dic_fin("dictionary/dictionary.dic");
    dic_fin.imbue(utf8_locale);


    int total = 0;
    if (dic_fin)
    {
        wstring temp;
        while (getline(dic_fin, temp))
        {
            total++;
            if (total % 1000 == 0)
            {
                cout << "\r Loading Dictionary... [" << total << "] words";
            }
            String tmp(temp);
            strhashtable.insert(tmp);
        }
    }
    else
    {
        cout << "error";
        throw Error("fail to load dictionary");
    }
    std::wifstream extra_dic_fin("dictionary/extra_dictionary.dic");
    extra_dic_fin.imbue(utf8_locale);
    if (extra_dic_fin)
    {
        wstring temp;
        while (getline(extra_dic_fin, temp))
        {
            total++;
            if (total % 1000 == 0)
            {
                cout << "\r Loading Extra Dictionary... [" << total << "] words";
            }
            String tmp(temp);
            strhashtable.insert(tmp);
        }
    }
    else
    {
        cout << "error";
        throw Error("fail to load extra dictionary");
    }

    if (g_conf.DOWNLOAD_CLOUD_DIC)
    {
        std::wifstream cloud_dic_fin("dictionary/cloud_dictionary.dic");
        cloud_dic_fin.imbue(utf8_locale);
        if (cloud_dic_fin)
        {
            wstring temp;
            while (getline(cloud_dic_fin, temp))
            {
                total++;
                if (total % 1000 == 0)
                {
                    cout << "\r Loading Cloud Dictionary... [" << total << "] words";
                }
                String tmp(temp);
                strhashtable.insert(tmp);
            }
        }
        else
        {
            cout << "error\n";
            cout << "fail to load cloud dictionary";
        }
    }
    return strhashtable;
}

LinkList<String> Process::divideWords(String words, AVLTree<String> dic)
{
    LinkList<String> tempresult;
    words = removeChineseSymbol(words);
    if (words.empty())
        return tempresult;

    if (words.size() == 1)
    {
        tempresult.push_back(words);
        return tempresult;
    }
    String currentstring(words);
    while (!currentstring.empty())
    {
        int i;
        String substr;
        int break_flag = 0;
        for (i = currentstring.size(); i > 0; i--)
        {
            substr = currentstring.substr(0, i);
            if (dic.search(substr))
            {
                tempresult.insert(substr);
                if (i != currentstring.size())
                {
                    currentstring = currentstring.substr(i, currentstring.size());
                    break;
                }
                else
                {
                    break_flag = 1;

                    break;
                }
            }
        }
        if (break_flag == 1)
        {
            break;
        }
        if (i == 0)
        {
            substr = currentstring.substr(0, 1);

            tempresult.insert(substr);
            currentstring = currentstring.substr(1, currentstring.size());
        }
        if (currentstring.size() == 1)
        {
            break;
        }
    }
    LinkList<String> result;
    auto current = tempresult.head;
    String numbers;
    while (current)
    {
        if (current->_data.size() >= 2)
        {
            if (!numbers.empty())
            {
                result.push_back(numbers);
                numbers = String();
            }
            if (!(current->_data == space))
                result.push_back(current->_data);
        }
        else
        {
            if (is_letter_or_number(current->_data[0]))
            {
                numbers = numbers.concat(current->_data);
            }
        }
        current = current->_next;
    }
    if (!numbers.empty())
    {
        result.push_back(numbers);
    }

    return result;
}

LinkList<String> Process::divideWords(String words, StringHashTable dic)
{
    LinkList<String> tempresult;
    words = removeChineseSymbol(words);
    if (words.empty())
        return tempresult;

    if (words.size() == 1)
    {
        tempresult.push_back(words);
        return tempresult;
    }
    String currentstring(words);
    while (!currentstring.empty())
    {
        int i;
        String substr;
        int break_flag = 0;
        for (i = currentstring.size(); i > 0; i--)
        {
            substr = currentstring.substr(0, i);
            if (dic[substr])
            {
                tempresult.insert(substr);
                if (i != currentstring.size())
                {
                    currentstring = currentstring.substr(i, currentstring.size());
                    break;
                }
                else
                {
                    break_flag = 1;

                    break;
                }
            }
        }
        if (break_flag == 1)
        {
            break;
        }
        if (i == 0)
        {
            substr = currentstring.substr(0, 1);

            tempresult.insert(substr);
            currentstring = currentstring.substr(1, currentstring.size());
        }
        if (currentstring.size() == 1)
        {
            break;
        }
    }
    LinkList<String> result;
    auto current = tempresult.head;
    String numbers;
    while (current)
    {
        if (current->_data.size() >= 2)
        {
            if (!numbers.empty())
            {
                result.push_back(numbers);
                numbers = String();
            }
            if (!(current->_data == space))
                result.push_back(current->_data);
        }
        else
        {
            if (is_letter_or_number(current->_data[0]))
            {
                numbers = numbers.concat(current->_data);
            }
        }
        current = current->_next;
    }
    if (!numbers.empty())
    {
        result.push_back(numbers);
    }


    return result;
}

/**
 * \brief 
 * \param filename 
 * \return 
 */
PageInfo Process::extractInfo_onepage(const std::string filename)
{
    
    HTMLparser parser(filename);
    parser.toknize();
    return parser.parse();


}

LinkList<PageInfo> Process::extractInfo(int pagenumbers)
{
    
    for (int i = 0; i < pagenumbers; i += CPU_CORES)
    {
        future<PageInfo> temp[4];
        for (int j = i; j < pagenumbers && j < (i + CPU_CORES); j++)
        {
            ostringstream sstream;
            sstream << j << ".html";
            temp[j - i] = async(extractInfo_onepage, sstream.str());
        }
        for (int j = i; j < pagenumbers && j < (i + CPU_CORES); j++)
        {
            auto tempinfo = temp[j - i].get();
            pageinfo_list.push_back(tempinfo);
            cout << "Extracting... " << j + 1 << "/" << pagenumbers << endl;
            //tempinfo.show();
        }
    }
    return pageinfo_list;
}

void Process::process()
{
    auto infos = extractInfo(g_urllist.size());
    generateDocument(infos);
}

/**
 * \brief 根据页面信息生成倒排文档
 * \param infos 
 * \return 
 */
InvertedIndexDocument Process::generateDocument(LinkList<PageInfo> infos)
{
    dic_avl = initDictionary_avl();
    //InvertedIndexDocument inverted_index_document;
    int index = 1;
    auto current = infos.head;
    while (current)
    {
        auto titlewords = divideWords(current->_data.title, dic_avl);
        auto contentwords = divideWords(current->_data.content, dic_avl);
        auto current_title_word = titlewords.head;
        while (current_title_word)
        {
            inverted_index_document.add(current_title_word->_data, index);
            current_title_word = current_title_word->_next;
        }
        auto current_content_word = contentwords.head;
        while (current_content_word)
        {
            inverted_index_document.add(current_content_word->_data, index);
            current_content_word = current_content_word->_next;
        }


        current = current->_next;
        index++;
    }
    return inverted_index_document;
}

/*
* \brief 下载网页文件主线程
* \param url 指定的URL
* \param filename 保存的文件路径
* \param is_download_html 是否是在下载html文件
*/
void Process::cURL(const QString url, const QString filename, bool is_download_html)
{
    /*
    * 并发线程数控制
    */
    while (true)
    {
        if (curent_worker <= MAX_WORKER)
        {
            mtx.lock();
            curent_worker++;
            mtx.unlock();
            break;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(200));
    }
    /*
    * 执行下载任务
    */
    QUrl __url(url);
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply* reply = manager.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    /*
    * 写入文件
    */
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        mtx.lock();
        qDebug() << "Cannot open the file: " << filename;
        mtx.unlock();
        return;
    }
    QDataStream out(&file);
    const QByteArray content = reply->readAll();
    out << content;
    file.close();

    mtx.lock();
    if (is_download_html)
    {
        qDebug() << "Successful download: " << filename << "    " << flag;
        flag++;
    }
    curent_worker--;
    mtx.unlock();
}
