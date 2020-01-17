#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QStandardPaths>

class DatabaseController
{
public: QString dbPath;
public:
    DatabaseController();

public:
    QSqlDatabase db;

    bool ConnectDB();
    void CloseDB();
};

#endif // DATABASECONTROLLER_H
