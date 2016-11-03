#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QMediaMetaData>
#include <QDebug>
#include <QUrl>
#include <QTime>
#include <QString>
#include <QStringList>
#include <QStandardPaths>
#include <QGridLayout>

#include <QInputDialog>
#include <QListWidget>
#include <QListWidgetItem>

#include "playlistmodel.h"
#include "visualiser.h"
#include "database.h"


#define ARTIST_CLICKED 1
#define ALBUM_CLICKED 2
/*curl headers*/


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool paused = false;
    bool muted = false;

    QStringList playlistStringList;

    QStringList all_artists;
    QStringList all_albums;
    QStringList all_tracks;

    QStringList artist_list;
    QString current_playing_artist_name = "";
    QString current_playing_title = "";
    QString current_playing_album = "";
    int current_playing_rating = 0;

    QString status_bar_message = "";
    int volume = 50;
    visualiser *vis = new visualiser;
    QMediaPlaylist *playlist = new QMediaPlaylist;
    PlaylistModel *playlistModel = new PlaylistModel;
//    DbManager db = DbManager(QUrl::fromLocalFile(":/db/musicdb.db").toString());
    DbManager db = DbManager("musicdb.db");
    ~MainWindow();

    QString returnCurrentPlayingArtist();

private slots:
    void on_sliderProgress_sliderMoved(int position);

    void on_durationChanged(qint64 position);

    void on_positionChanged(qint64 position);

    void on_dialVolume_sliderMoved(int position);

    void on_actionOpen_triggered();

    void on_actionPlayback_triggered();

    void on_actionStop_triggered();

    void on_actionMute_triggered();

    void statusBarHandler();

    void getLyrics(QString artist_name, QString track_name);

    void on_actionDownloadLyrics_triggered();

    void goToItem(const QModelIndex &index);

    void on_actionPrevious_triggered();

    void on_actionNext_triggered();

    void on_actionShuffle_triggered();

    void addTrackToDB();

    void metadataHandler();

    void on_savePlaylistButton_clicked();

    void on_loadPlaylistButton_clicked();

    void on_populateButton_clicked();

    void on_artistList_itemClicked(QListWidgetItem *item);

    void on_albumList_itemClicked(QListWidgetItem *item);

    void on_trackList_itemDoubleClicked(QListWidgetItem *item);

    void on_actionStar1_triggered();

    void on_actionStar2_triggered();

    void on_actionStar3_triggered();

    void on_actionStar4_triggered();

    void on_actionStar5_triggered();

    void on_colorListWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
//    QMediaPlaylist *playlist;
};

#endif // MAINWINDOW_H
