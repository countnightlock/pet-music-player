#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "visualiser.h"
#include "playlistmodel.h"
#include "database.h"

visualiser::visualiser(QWidget *parent)
    : QWidget(parent)
{
    //placeholder constructor
    //can be used to initialise visualiser background details
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //draws the user interface
    ui->setupUi(this);

    //sets up the visualiser widget to draw bars on
    ui->gridLayout_5->addWidget(vis);

    //initialises the player object. This is the heart of the program
    player = new QMediaPlayer(this);

    //initialises new playlist object.
    playlist = new QMediaPlaylist();
    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
    ui->playlistListView->setModel(playlistModel);
    ui->playlistListView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));


    player->setPlaylist(playlist);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    // this allow the user to select the media it wants to play
      connect(ui->playlistListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(goToItem(QModelIndex)));
      connect(ui->playlistListView, SIGNAL(activated(QModelIndex)), this, SLOT(goToItem(QModelIndex)));

    //connecting various signals and slots here
    //when the position of the player changes, move the seek bar slider
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
    //when the song changes, update duration
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
    //when the song changes, update metadata shown in status bar
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::statusBarHandler);
    connect(playlist, &QMediaPlaylist::currentMediaChanged, this, &MainWindow::metadataHandler);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::metadataHandler);
//    connect(player, &QMediaPlayer::mediaChanged, this, &MainWindow::metadataHandler);

}

MainWindow::~MainWindow()
{
    player->stop();
    delete ui;
}

void MainWindow::on_sliderProgress_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::on_dialVolume_sliderMoved(int position)
{
    player->setVolume(position);
    MainWindow::volume = position;
}

void MainWindow::on_actionOpen_triggered()
{
    QIcon iconvoloff(":/icons/icons/ic_volume_off_black_48dp_2x.png");
    QIcon iconpause(":/icons/icons/ic_pause_black_48dp_2x.png");
    const QStringList usualPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    const QString filePath =
            QFileDialog::getOpenFileName(this, tr("Select a Track"),
            usualPaths.isEmpty() ? QDir::homePath() : usualPaths.first(),
            tr("MP3 files (*.mp3);;All files (*.*)"));
    playlist->addMedia(QUrl::fromLocalFile(filePath));

    playlistStringList << filePath;

    player->play();

    ui->actionPlayback->setIcon(iconpause);
    ui->actionPlayback->setToolTip("Pause");
    ui->actionMute->setIcon(iconvoloff);
    ui->actionMute->setToolTip("Mute");


    //qDebug() << player->errorString();
}

void MainWindow::goToItem(const QModelIndex &index){
  if (index.isValid()) {
    playlist->setCurrentIndex(index.row());
    player->play();
  }
}

void MainWindow::statusBarHandler()
{
    ui->lyricsPane->setText("Click the \"Download Lyrics\" button to Search for Lyrics on www.azlyrics.com");
    switch(player->state())
    {
    case QMediaPlayer::PlayingState :
        status_bar_message = "Playing";
        break;
    case QMediaPlayer::PausedState :
        status_bar_message = "Paused";
        break;
    case QMediaPlayer::StoppedState :
        status_bar_message = "Stopped";
        break;
    }
    ui->statusBar->showMessage(status_bar_message);
}

void MainWindow::metadataHandler()
{
    QIcon solidstar(":/icons/icons/ic_star_black_48dp_2x.png");
    QIcon hollowstar(":/icons/icons/ic_star_border_black_48dp_2x.png");

    db.current_path = player->currentMedia().canonicalUrl().toString();
    db.current_path.replace("file://","");
    artist_list = player->metaData(QMediaMetaData::ContributingArtist).toStringList();
    current_playing_title = player->metaData(QMediaMetaData::Title).toString();
    current_playing_album = player->metaData(QMediaMetaData::AlbumTitle).toString();
    current_playing_rating = db.getRatingOfTrack(db.current_path);

    switch(current_playing_rating)
    {
        case 0:
            ui->actionStar1->setIcon(hollowstar);
            ui->actionStar2->setIcon(hollowstar);
            ui->actionStar3->setIcon(hollowstar);
            ui->actionStar4->setIcon(hollowstar);
            ui->actionStar5->setIcon(hollowstar);
            break;
        case 1:
            ui->actionStar1->setIcon(solidstar);
            ui->actionStar2->setIcon(hollowstar);
            ui->actionStar3->setIcon(hollowstar);
            ui->actionStar4->setIcon(hollowstar);
            ui->actionStar5->setIcon(hollowstar);
            break;
        case 2:
            ui->actionStar1->setIcon(solidstar);
            ui->actionStar2->setIcon(solidstar);
            ui->actionStar3->setIcon(hollowstar);
            ui->actionStar4->setIcon(hollowstar);
            ui->actionStar5->setIcon(hollowstar);
            break;
        case 3:
            ui->actionStar1->setIcon(solidstar);
            ui->actionStar2->setIcon(solidstar);
            ui->actionStar3->setIcon(solidstar);
            ui->actionStar4->setIcon(hollowstar);
            ui->actionStar5->setIcon(hollowstar);
            break;
        case 4:
            ui->actionStar1->setIcon(solidstar);
            ui->actionStar2->setIcon(solidstar);
            ui->actionStar3->setIcon(solidstar);
            ui->actionStar4->setIcon(solidstar);
            ui->actionStar5->setIcon(hollowstar);
            break;
        case 5:
            ui->actionStar1->setIcon(solidstar);
            ui->actionStar2->setIcon(solidstar);
            ui->actionStar3->setIcon(solidstar);
            ui->actionStar4->setIcon(solidstar);
            ui->actionStar5->setIcon(solidstar);
            break;
    }


    QString artist_name1;
    if(artist_list.size() == 0)
        current_playing_artist_name = "Unknown Artist";
    else
    {
        artist_name1 = artist_list.at(0);
        current_playing_artist_name = artist_name1;
    }

    qDebug() << db.current_path << current_playing_title << current_playing_album << current_playing_artist_name;
    if(current_playing_title != "")
    {
        addTrackToDB();
        ui->titleLabel->setText(current_playing_title);
        ui->albumLabel->setText(current_playing_album);
        ui->artistLabel->setText(current_playing_artist_name);
        ui->genreLabel->setText(player->metaData(QMediaMetaData::Genre).toString());
        ui->ratingLabel->setText(QString::number(current_playing_rating) + " star" + ((current_playing_rating == 1) ?"":"s"));
        ui->bitrateLabel->setText("320000");
    }
}

void MainWindow::addTrackToDB()
{
    db.addTrack(db.current_path, current_playing_title, current_playing_album, current_playing_artist_name);
}

void MainWindow::on_actionPlayback_triggered()
{
    QIcon iconplay(":/icons/icons/ic_play_arrow_black_48dp_2x.png");
    QIcon iconpause(":/icons/icons/ic_pause_black_48dp_2x.png");
    if(player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
        ui->actionPlayback->setIcon(iconplay);
        ui->actionPlayback->setToolTip("Resume");
        ui->statusBar->showMessage("Paused");
    }
    else
    {
        player->play();
        ui->actionPlayback->setIcon(iconpause);
        ui->actionPlayback->setToolTip("Pause");
        ui->statusBar->showMessage("Playing");
    }


}

void MainWindow::on_actionStop_triggered()
{
    QIcon iconplay(":/icons/icons/ic_play_arrow_black_48dp_2x.png");
    ui->actionPlayback->setIcon(iconplay);
    ui->actionPlayback->setToolTip("Resume");
    player->stop();
}

void MainWindow::on_actionMute_triggered()
{
    QIcon iconvoloff(":/icons/icons/ic_volume_off_black_48dp_2x.png");
    QIcon iconvolon(":/icons/icons/ic_volume_up_black_48dp_2x.png");
    if(!MainWindow::muted)
    {
        player->setVolume(0);
        MainWindow::muted = true;
        ui->actionMute->setIcon(iconvolon);
        ui->actionMute->setToolTip("Unmute");
    }
    else
    {
        player->setVolume(MainWindow::volume);
        MainWindow::muted = false;
        ui->actionMute->setIcon(iconvoloff);
        ui->actionMute->setToolTip("Mute");
    }
}

void MainWindow::on_positionChanged(qint64 position)
{
    ui->sliderProgress->setValue(position);
    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
    ui->progressDone->setText(duration.toString(tr("mm:ss")));
    vis->repaint();
}

void MainWindow::on_durationChanged(qint64 position)
{
    ui->sliderProgress->setMaximum(position);
    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
    ui->progressTotal->setText(duration.toString(tr("mm:ss")));
}

void MainWindow::on_actionPrevious_triggered()
{
    playlist->previous();
}

void MainWindow::on_actionNext_triggered()
{
    playlist->next();
}

void MainWindow::on_actionShuffle_triggered()
{
    playlist->shuffle();
}

void MainWindow::on_savePlaylistButton_clicked()
{
    QString path = "playlists/";
    bool ok;
    QString fileName = QInputDialog::getText(this, tr("Enter Name of Playlist"),
                                             tr(""), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
    if (ok && !fileName.isEmpty())
    {
        QFile file(path + fileName + ".playlist");
        if (file.open(QIODevice::WriteOnly) )
        {
            QTextStream stream(&file);
            for(int i = 0; i < playlistStringList.length(); ++i)
                stream << playlistStringList.at(i) << "\n";

            file.close();
        }
    }

}

void MainWindow::on_loadPlaylistButton_clicked()
{
    const QString filePath =
            QFileDialog::getOpenFileName(this, tr("Select a Playlist"), ":playlists",
            tr("Playlist files (*.playlist);;All files (*.*)"));

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly) )
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
            playlistStringList += stream.readLine();

        file.close();
    }

    playlist->clear();
    for(int i = 0; i < playlistStringList.length(); ++i)
        playlist->addMedia(QUrl::fromLocalFile(playlistStringList.at(i)));
}

void MainWindow::on_populateButton_clicked()
{
    all_artists = db.printAllArtists();
    all_albums = db.printAllAlbums();
    all_tracks = db.printAllTracks();

    while(ui->artistList->count() > 0)
        ui->artistList->takeItem(0);
    while(ui->albumList->count() > 0)
        ui->albumList->takeItem(0);
    while(ui->trackList->count() > 0)
        ui->trackList->takeItem(0);

    ui->artistList->addItems(all_artists);
    ui->albumList->addItems(all_albums);
    ui->trackList->addItems(all_tracks);
}

void MainWindow::on_artistList_itemClicked(QListWidgetItem *item)
{
    QStringList albums = db.printSomeAlbums(item->text());
    QStringList tracks = db.printSomeTracks(item->text(), ARTIST_CLICKED);

    while(ui->albumList->count() > 0)
        ui->albumList->takeItem(0);
    while(ui->trackList->count() > 0)
        ui->trackList->takeItem(0);

    ui->albumList->addItems(albums);
    ui->trackList->addItems(tracks);
}

void MainWindow::on_albumList_itemClicked(QListWidgetItem *item)
{
    QStringList tracks = db.printSomeTracks(item->text(), ALBUM_CLICKED);

    while(ui->trackList->count() > 0)
        ui->trackList->takeItem(0);

    ui->trackList->addItems(tracks);
}

void MainWindow::on_trackList_itemDoubleClicked(QListWidgetItem *item)
{
    QString path = db.getPathOfTrack(item->text());
    playlist->addMedia(QUrl::fromLocalFile(path));
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_actionStar1_triggered()
{
    QIcon solidstar(":/icons/icons/ic_star_black_48dp_2x.png");
    QIcon hollowstar(":/icons/icons/ic_star_border_black_48dp_2x.png");

    db.setRatingOfTrack(db.current_path, 1);
    ui->ratingLabel->setText("1 star");

    ui->actionStar1->setIcon(solidstar);
    ui->actionStar2->setIcon(hollowstar);
    ui->actionStar3->setIcon(hollowstar);
    ui->actionStar4->setIcon(hollowstar);
    ui->actionStar5->setIcon(hollowstar);
}

void MainWindow::on_actionStar2_triggered()
{
    QIcon solidstar(":/icons/icons/ic_star_black_48dp_2x.png");
    QIcon hollowstar(":/icons/icons/ic_star_border_black_48dp_2x.png");

    db.setRatingOfTrack(db.current_path, 2);
    ui->ratingLabel->setText("2 stars");

    ui->actionStar1->setIcon(solidstar);
    ui->actionStar2->setIcon(solidstar);
    ui->actionStar3->setIcon(hollowstar);
    ui->actionStar4->setIcon(hollowstar);
    ui->actionStar5->setIcon(hollowstar);
}

void MainWindow::on_actionStar3_triggered()
{
    QIcon solidstar(":/icons/icons/ic_star_black_48dp_2x.png");
    QIcon hollowstar(":/icons/icons/ic_star_border_black_48dp_2x.png");

    db.setRatingOfTrack(db.current_path, 3);
    ui->ratingLabel->setText("3 stars");

    ui->actionStar1->setIcon(solidstar);
    ui->actionStar2->setIcon(solidstar);
    ui->actionStar3->setIcon(solidstar);
    ui->actionStar4->setIcon(hollowstar);
    ui->actionStar5->setIcon(hollowstar);
}

void MainWindow::on_actionStar4_triggered()
{
    QIcon solidstar(":/icons/icons/ic_star_black_48dp_2x.png");
    QIcon hollowstar(":/icons/icons/ic_star_border_black_48dp_2x.png");

    db.setRatingOfTrack(db.current_path, 4);
    ui->ratingLabel->setText("4 stars");

    ui->actionStar1->setIcon(solidstar);
    ui->actionStar2->setIcon(solidstar);
    ui->actionStar3->setIcon(solidstar);
    ui->actionStar4->setIcon(solidstar);
    ui->actionStar5->setIcon(hollowstar);
}

void MainWindow::on_actionStar5_triggered()
{
    QIcon solidstar(":/icons/icons/ic_star_black_48dp_2x.png");

    db.setRatingOfTrack(db.current_path, 5);
    ui->ratingLabel->setText("5 stars");

    ui->actionStar1->setIcon(solidstar);
    ui->actionStar2->setIcon(solidstar);
    ui->actionStar3->setIcon(solidstar);
    ui->actionStar4->setIcon(solidstar);
    ui->actionStar5->setIcon(solidstar);
}

void MainWindow::on_colorListWidget_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    QString color = "";
    QString backgroundColor = "";

    QString stylesheet = "";

    if(item->text() == "Bulbasaur")
    {
        color = "#184a4a";
        backgroundColor = "#62d5b4";
    }
    else if(item->text() == "Charmander")
    {
        color = "#ffd57b";
        backgroundColor = "#8b2900";
    }
    else if(item->text() == "Squirtle")
    {
        color = "#297383";
        backgroundColor = "#8bc5cd";
    }
    else if(item->text() == "Gyarados")
    {
        color = "#d5def6";
        backgroundColor = "#184173";
    }
    else if(item->text() == "Pikachu")
    {
        color = "#c52018";
        backgroundColor = "#fff6a4";
    }
    else if(item->text() == "Chansey")
    {
        color = "#ffe6e6";
        backgroundColor = "#ee5a31";
    }
    else if(item->text() == "Growlithe")
    {
        color = "#6a6a6a";
        backgroundColor = "#ffb452";
    }


    stylesheet = "color:" + color + ";\nbackground-color:" + backgroundColor + ";";

    qDebug() << stylesheet;

    ui->centralWidget->setStyleSheet(stylesheet);
    ui->statusBar->setStyleSheet(stylesheet);
    ui->mainToolBar->setStyleSheet(stylesheet);
    vis->setStyleSheet(stylesheet);
}
