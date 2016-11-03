#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::createTable()
{
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE ratings(name TEXT PRIMARY KEY, stars INTEGER);");

    if (!query.exec())
    {
        qDebug() << "Couldn't create the table 'ratings': one might already exist.";
        success = false;
    }
    else
        success = true;

    return success;
}

bool DbManager::addTrack(const QString& path, const QString& trackname, const QString& album, const QString& artist)
{
    bool success = false;
    if (!path.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO tracks (path, trackname, album, artist) VALUES (:path, :trackname, :album, :artist)");
        queryAdd.bindValue(":path", path);
        queryAdd.bindValue(":trackname", trackname);
        queryAdd.bindValue(":album", album);
        queryAdd.bindValue(":artist", artist);

        QSqlQuery queryAdd2;
        queryAdd2.prepare("INSERT INTO ratings (path, rating) VALUES (:path, :rating)");
        queryAdd2.bindValue(":path", path);
        queryAdd2.bindValue(":rating", 0);

        if(queryAdd.exec() && queryAdd2.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "add track failed: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "add track failed: name cannot be empty";
    }

    return success;
}

bool DbManager::addRating(const QString& path, const int rating)
{
    bool success = false;
    if (!path.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO ratings (path, rating) VALUES (:path, :rating)");
        queryAdd.bindValue(":path", path);
        queryAdd.bindValue(":rating", rating);

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "add track failed: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "add track failed: name cannot be empty";
    }

    return success;
}

bool DbManager::removeTrack(const QString& name)
{
    bool success = false;

    if (trackExists(name))
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM ratings WHERE name = (:name)");
        queryDelete.bindValue(":name", name);
        success = queryDelete.exec();

        if(!success)
        {
            qDebug() << "remove track failed: " << queryDelete.lastError();
        }
    }
    else
    {
        qDebug() << "remove track failed: track doesnt exist";
    }

    return success;
}

QStringList DbManager::printAllArtists() const
{
    QStringList returnstringlist;
    QSqlQuery query("SELECT DISTINCT artist FROM tracks");
    int idName = query.record().indexOf("artist");
    while (query.next())
    {
        QString name = query.value(idName).toString();
        returnstringlist.append(name);
    }
    return returnstringlist;
}

QStringList DbManager::printAllAlbums() const
{
    QStringList returnstringlist;
    QSqlQuery query("SELECT DISTINCT album FROM tracks");
    int idName = query.record().indexOf("album");
    while (query.next())
    {
        QString name = query.value(idName).toString();
        returnstringlist.append(name);
    }
    return returnstringlist;
}

QStringList DbManager::printSomeAlbums(QString artist)
{
    QStringList returnstringlist;
    QSqlQuery query;

    query.prepare("SELECT DISTINCT album FROM tracks WHERE artist = (:artist)");
    query.bindValue(":artist", artist);
    query.exec();

    int idName = query.record().indexOf("album");
    while (query.next())
    {
        QString name = query.value(idName).toString();
        returnstringlist.append(name);
    }
    return returnstringlist;
}

QStringList DbManager::printSomeTracks(QString str, int flag)
{
    QStringList returnstringlist;
    QSqlQuery query;

    if(flag == 1)
    {
        query.prepare("SELECT DISTINCT trackname FROM tracks WHERE artist = (:artist)");
        query.bindValue(":artist", str);
    }

    if(flag == 2)
    {
        query.prepare("SELECT DISTINCT trackname FROM tracks WHERE album = (:album)");
        query.bindValue(":album", str);
    }

    query.exec();

    int idName = query.record().indexOf("trackname");
    while (query.next())
    {
        QString name = query.value(idName).toString();
        returnstringlist.append(name);
    }
    return returnstringlist;
}

QString DbManager::getPathOfTrack(QString trackname)
{
    QSqlQuery query;
    query.prepare("SELECT path FROM tracks WHERE trackname = (:trackname)");
    query.bindValue(":trackname", trackname);
    query.exec();

    int idName = query.record().indexOf("path");
    query.next();
    QString name = query.value(idName).toString();

    return name;
}

int DbManager::getRatingOfTrack(QString path)
{
    QSqlQuery query;
    query.prepare("SELECT rating FROM ratings WHERE path = (:path)");
    query.bindValue(":path", path);
    query.exec();

    int idName = query.record().indexOf("rating");
    query.next();
    int rating = query.value(idName).toInt();

    return rating;
}

bool DbManager::setRatingOfTrack(QString path, int rating)
{
    QSqlQuery query;
    query.prepare("UPDATE ratings SET rating = (:rating) WHERE path = (:path)");
    query.bindValue(":rating", rating);
    query.bindValue(":path", path);
    qDebug() << "rating set to " << rating;
    qDebug() << query.exec();

    qDebug() << query.lastError();

    return false;
}

QStringList DbManager::printAllTracks() const
{
    QStringList returnstringlist;
    QSqlQuery query("SELECT DISTINCT trackname FROM tracks");
    int idName = query.record().indexOf("trackname");
    while (query.next())
    {
        QString name = query.value(idName).toString();
        returnstringlist.append(name);
    }
    return returnstringlist;
}

bool DbManager::trackExists(const QString& name) const
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT name FROM ratings WHERE name = (:name)");
    checkQuery.bindValue(":name", name);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else
    {
        qDebug() << "track exists failed: " << checkQuery.lastError();
    }

    return exists;
}

bool DbManager::removeAllTracks()
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM ratings");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all tracks failed: " << removeQuery.lastError();
    }

    return success;
}

