#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractItemModel>
#include <QMediaPlaylist>

/*
 * This class subclasses QAbstractItemModel, which is basically
 * a generalised model for any kind of data structure composed
 * of items. We make a table for a row-column type of thing.
 *
 * QModelIndex is used to index items of QVariant type, which
 * here are nothing but tracks.
 *
 * This model is derived heavily from BlokBox's playlist model.
 * Check out https://github.com/agostinhobritojr/blokbox
 *
*/

class PlaylistModel : public QAbstractItemModel
{
  Q_OBJECT

private:
  // stores the media playlist, since a model-view implementation,
  // although convenient, is not a part of Qt
  QMediaPlaylist *tracks_playlist;
  QMap<QModelIndex, QVariant> track_data;

public:
  // class constructor
  explicit PlaylistModel(QObject *parent = 0);
  enum column {
      Title = 0,
      ColumnCount
  };

signals:

public slots:
  // modelview assumes data is stored in a table structure
  // some of the methods below MUST BE IMPLEMENTED so the
  // data to be retrieved may be accessed correctly

  // returns the number of rows stored in model
  int rowCount(const QModelIndex &parent = QModelIndex()) const;

  // returns the number of columns stored in this model
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  // returns the model index of element at a given row/column
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

  // returns the parent of a given item
  QModelIndex parent(const QModelIndex &child) const;

  // takes index as input and gives data as output
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  // returns playlist to be played
  QMediaPlaylist *playlist() const;

  // tells what playlist shall be filled
  void setPlaylist(QMediaPlaylist *playlist);

  // A setData is used to insert itens into the playlist
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);

private slots:
  // these functions help in internal organisation of the data
  void beginInsertItems(int start, int end);
  void endInsertItems();
  void beginRemoveItems(int start, int end);
  void endRemoveItems();
  void changeItems(int start, int end);

};

#endif // PLAYLISTMODEL_H
