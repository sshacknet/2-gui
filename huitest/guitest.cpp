#include "guitest.h"
#include <QDebug>
#include <QFile>
#include "Process.h"
#include <QMap>


QString title = QString("<a style=\"color:#1a0dab\" href=\"%1\"><font size=\"20\" >%2</font></a><br>\n");
QString url = QString("<span style=\"color:#006621;\">%1</span> <br>");
QString content = QString("<p style=\"color:#808080\">%1</p><br><br>");
QString key_word_style = QString("<span style=\"color:red\">%1</span>");

guitest::guitest(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    result = new QTextBrowser(this);
    resize(600, 600);

    result->setOpenExternalLinks(true);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(search()));


    result->setGeometry(60, 70, geometry().width() - 30, geometry().height() - 35);
    result->resize(geometry().width() - 120, geometry().height() - 140);
    result->show();
}

void guitest::search()
{
    Document doc;
    QString key_words = ui.lineEdit->text();
    QMap<int, int> docs;
    QStringList key_words_list = key_words.split(QString(" "));

    for (QString key_word : key_words_list)
    {
        if (!key_word.isEmpty())
        {
            auto w_key = key_word.toStdWString();
            String key(w_key);
            doc = inverted_index_document.search(key);

            auto c = doc.documents.head;
            while (c)
            {
                int urlid = c->_data.urlID;
                int occurtimes = c->_data.occurTimes;
                if (!docs.contains(urlid))
                {
                    docs[urlid] += c->_data.occurTimes;
                }
                else
                {
                    docs.insert(urlid, occurtimes);
                }
                c = c->_next;
            }
        }
    }
    QList<QueryInfo> temp;
    for (auto it = docs.begin(); it != docs.end(); it++)
    {
        temp.push_back(QueryInfo(it.key(), it.value()));
    }
    qSort(temp.begin(), temp.end(), [](QueryInfo& q1, QueryInfo& q2) { return q1.occurTimes > q2.occurTimes; });
    QString searchresult;
    //    auto c = doc.documents.head;
    //    while (c)
    //    {
    //        qDebug() << c->_data.urlID << " " << c->_data.occurTimes << endl;
    //       QString url = g_urllist[c->_data.urlID-1];
    //        QString _content = QString::fromStdWString(pageinfo_list[c->_data.urlID -1].content.toWstring()).simplified();
    //        QString _title = QString::fromStdWString(pageinfo_list[c->_data.urlID-1].title.toWstring());
    //        searchresult += title.arg(url).arg(_title);
    //        searchresult += url.arg(url);
    //        searchresult += content.arg(_content);
    //        c = c->_next;
    //    }
    for (int i = 0; i < temp.size(); i++)
    {
        QString _url = g_urllist[temp[i].urlID - 1];
        QString _content = QString::fromStdWString(pageinfo_list[temp[i].urlID - 1].content.toWstring()).simplified();
        QString _title = QString::fromStdWString(pageinfo_list[temp[i].urlID - 1].title.toWstring());
        searchresult += title.arg(_url).arg(_title);
        searchresult += url.arg(_url);
        for (QString key_word : key_words_list)
        {
            _content = _content.replace(key_word, key_word_style.arg(key_word));
        }
        searchresult += content.arg(_content);
    }
    QFile fout("log");
    fout.open(QIODevice::WriteOnly);
    QTextStream out(&fout);
    out << searchresult;
    fout.close();
    result->setHtml(searchresult);
}
