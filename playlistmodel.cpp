#include "playlistmodel.h"

#include <QFileInfo>
#include <QUrl>
#include <QMediaPlaylist>

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractItemModel(parent)
    , tracks_playlist(0){
}

int PlaylistModel::rowCount(const QModelIndex &parent) const {
    return tracks_playlist && !parent.isValid() ? tracks_playlist->mediaCount() : 0;
}

int PlaylistModel::columnCount(const QModelIndex &parent) const {
    return !parent.isValid() ? ColumnCount : 0;
}

QModelIndex PlaylistModel::index(int row, int column, const QModelIndex &parent) const {
    return tracks_playlist && !parent.isValid()
            && row >= 0 && row < tracks_playlist->mediaCount()
            && column >= 0 && column < ColumnCount
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex PlaylistModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);

    return QModelIndex();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
    if (index.isValid() && role == Qt::DisplayRole) {
        QVariant value = track_data[index];
        if (!value.isValid() && index.column() == Title) {
            QUrl location = tracks_playlist->media(index.row()).canonicalUrl();
            return QFileInfo(location.path()).fileName();
        }
        return value;
    }
    return QVariant();
}

QMediaPlaylist *PlaylistModel::playlist() const {
    return tracks_playlist;
}

void PlaylistModel::setPlaylist(QMediaPlaylist *playlist) {
    if (tracks_playlist) {
        disconnect(tracks_playlist, SIGNAL(mediaAboutToBeInserted(int,int)), this, SLOT(beginInsertItems(int,int)));
        disconnect(tracks_playlist, SIGNAL(mediaInserted(int,int)), this, SLOT(endInsertItems()));
        disconnect(tracks_playlist, SIGNAL(mediaAboutToBeRemoved(int,int)), this, SLOT(beginRemoveItems(int,int)));
        disconnect(tracks_playlist, SIGNAL(mediaRemoved(int,int)), this, SLOT(endRemoveItems()));
        disconnect(tracks_playlist, SIGNAL(mediaChanged(int,int)), this, SLOT(changeItems(int,int)));
    }

    beginResetModel();
    tracks_playlist = playlist;

    if (tracks_playlist) {
        connect(tracks_playlist, SIGNAL(mediaAboutToBeInserted(int,int)), this, SLOT(beginInsertItems(int,int)));
        connect(tracks_playlist, SIGNAL(mediaInserted(int,int)), this, SLOT(endInsertItems()));
        connect(tracks_playlist, SIGNAL(mediaAboutToBeRemoved(int,int)), this, SLOT(beginRemoveItems(int,int)));
        connect(tracks_playlist, SIGNAL(mediaRemoved(int,int)), this, SLOT(endRemoveItems()));
        connect(tracks_playlist, SIGNAL(mediaChanged(int,int)), this, SLOT(changeItems(int,int)));
    }

    endResetModel();
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    Q_UNUSED(role);
    track_data[index] = value;
    emit dataChanged(index, index);
    return true;
}

void PlaylistModel::beginInsertItems(int start, int end) {
    track_data.clear();
    beginInsertRows(QModelIndex(), start, end);
}

void PlaylistModel::endInsertItems() {
    endInsertRows();
}

void PlaylistModel::beginRemoveItems(int start, int end) {
    track_data.clear();
    beginRemoveRows(QModelIndex(), start, end);
}

void PlaylistModel::endRemoveItems() {
    endInsertRows();
}

void PlaylistModel::changeItems(int start, int end) {
    track_data.clear();
    emit dataChanged(index(start,0), index(end,ColumnCount));
}
