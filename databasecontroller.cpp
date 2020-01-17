#include "databasecontroller.h"

DatabaseController::DatabaseController()
{
    dbPath = QDir::currentPath();
    dbPath += QString("/mediaDB.db");
}

bool DatabaseController::ConnectDB()
{
    //qDebug() << dbPath;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    return db.open();
}

void DatabaseController::CloseDB()
{
    db.close();
    //db.removeDatabase(QSqlDatabase::defaultConnection);
    db.removeDatabase(dbPath);
}
