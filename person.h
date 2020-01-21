#ifndef PERSON_H
#define PERSON_H

#include "QString"
#include <QtSql>

class Person
{
    QString firstName;
    QString lastName;
    QString email;
    QString contact;
    QString type;

public:
    Person(QString objFName, QString objLName, QString objEmail, QString objContact, QString objType);

public:
    bool create();
    bool update(int objID);
    bool QueryExec();

private:
    QSqlQuery query;
};

#endif // PERSON_H
