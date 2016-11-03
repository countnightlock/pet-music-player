#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "httpdownloader.h"

void MainWindow::getLyrics(QString artist_name, QString track_name)
{
    QString lyrics_string("");

    artist_name = artist_name.toLower();

    if(artist_name.indexOf("the ") == 0)
        artist_name = artist_name.mid(3,-1);

    artist_name = artist_name.replace(" and "," & ");
    artist_name = artist_name.mid(0, artist_name.indexOf(" feat"));
    artist_name = artist_name.remove(QRegExp("[^a-zA-Z\\d\\s]"));
    artist_name = artist_name.replace(" ","");

    track_name = track_name.toLower();
    track_name = track_name.replace(" and "," & ");
    track_name = track_name.mid(0, track_name.indexOf("("));
    track_name = track_name.remove(QRegExp("[^a-zA-Z\\d\\s]"));
    track_name = track_name.replace(" ","");

    QString path = "downloaded_lyrics/" + artist_name + "/";
    QDir dir;
    QFile file(path + track_name);

    qDebug() << artist_name << track_name;

    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            lyrics_string = stream.readAll();
            qDebug() << "read from file";
        }
    }

    else
    {
        HTTPDownloader downloader;
        QString url = "http://azlyrics.com/lyrics/" + artist_name + "/" + track_name + ".html";
        std::string content = downloader.downloadUsingCurl(url.toStdString());
        QString qcontent = QString::fromStdString(content);

        int j1, j2, j3;
        j1 = qcontent.indexOf("<!-- Usage", 7000);
        j2 = qcontent.indexOf("-->", j1);
        j3 = qcontent.indexOf("</div>", j2);

        lyrics_string = qcontent.mid(j2 + 3,j3-j2-3);

        //censor expletives
        lyrics_string = lyrics_string.replace("fuck", "f***");
        lyrics_string = lyrics_string.replace("shit","s***");

        if(lyrics_string.size() > 10000)
            lyrics_string = "Lyrics not found. Metadata missing or invalid. Or track too Obscure, in which case, nice taste, I guess";
        else
        {
            QFile file(path + track_name);
            if(!dir.exists(path))
            {
                dir.mkpath(path);
                qDebug()<<"directory now exists";
            }
            if (file.open(QIODevice::ReadWrite) )
            {
                qDebug()<<"file now exists";
                QTextStream stream(&file);
                stream << lyrics_string << endl;
            }

            qDebug() << "read from azlyrics.com";
        }
    }
    ui->lyricsPane->setText(lyrics_string);
}

void MainWindow::on_actionDownloadLyrics_triggered()
{
    getLyrics(current_playing_artist_name,current_playing_title);
    ui->tabWidget->setCurrentIndex(3);
}
