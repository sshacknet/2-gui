#include "Gui.h"
#include <QDebug>
#include <QFile>
#include "Process.h"
#include <QMap>
#include <QTime>

QString title = QString("<a style=\"color:#1a0dab\" href=\"%1\"><font size=\"20\">%2</font></a><br>\n");
QString url = QString("<span style=\"color:#006621;\">%1</span><br>");
QString content = QString("<p style=\"color:#808080\">%1</p><br>");
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
    result->setHtml(QString::fromLocal8Bit("词典建立时间:%1ms").arg(constructTime));
}

void guitest::resizeEvent(QResizeEvent* event)
{
    int width = this->width();
    int height = this->height();
    qDebug() << "resize " << width << height;
    this->result->resize(width - 120, height - 140);
}

void guitest::search()
{
    auto begin_time = QTime::currentTime().msec();
    Document doc;
    QString key_words = ui.lineEdit->text();
    QMap<int, int> docs;
    QStringList key_words_list = key_words.split(QString(" "));
    qDebug() << key_words_list;
    for (QString key_word : key_words_list)
    {
        if (!key_word.isEmpty() && key_word != QString(" ") && key_word != QString(""))
        {
            auto w_key = key_word.toStdWString();
            String key(w_key);
            doc = g_inverted_index_document.search(key);

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
    auto end_time = QTime::currentTime().msec();
    QString s = QString::fromLocal8Bit("共找到%1个结果，用时%2ms<br>").arg(temp.size()).arg(end_time - begin_time);
    searchresult += s;

    for (int i = 0; i < temp.size(); i++)
    {
        QString _url = g_urllist[temp[i].urlID - 1];
        QString _content = QString::fromStdWString(g_pageinfo_list[temp[i].urlID - 1].content.toWstring()).simplified();
        QString _title = QString::fromStdWString(g_pageinfo_list[temp[i].urlID - 1].title.toWstring());
        searchresult += title.arg(_url).arg(_title);
        searchresult += url.arg(_url);
        for (QString key_word : key_words_list)
        {
            if (key_word != QString(" ") && key_word != QString(""))
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

