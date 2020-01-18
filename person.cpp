#include "person.h"

Person::Person(QString objFName, QString objLName, QString objEmail, QString objContact, QString objType)
{
    firstName = objFName;
    lastName  = objLName;
    email     = objEmail;
    contact   = objContact;
    type  = objType;
}

bool Person::create(){
    db = new DatabaseController();
    if(!db->ConnectDB()) return false;

    qDebug() << firstName << lastName << email << contact << type;

    QSqlQuery query;
    query.prepare("INSERT INTO person ([First Name], [Last Name], Email, Contact, Type) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(email);
    query.addBindValue(contact);
    query.addBindValue(type);

    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().text();
        return false;
    }
}
