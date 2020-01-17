#include "median.h"

Median::Median()
{

}

Book::Book(QString objName, QString objCategory, QString objAuthor, QString objLanguage, int objQuantity, QString objFormat, int objPage, bool objHasDigital, QString objISBN){
    name=objName;
    type="Book";
    category=objCategory;
    author=objAuthor;
    language=objLanguage;
    quantity=objQuantity;
    format=objFormat;
    avaiable=quantity;

    page=objPage;
    hasDigital=objHasDigital;
    ISBN=objISBN;
}

bool Book::create(){
    db = new DatabaseController();
    if(!db->ConnectDB()) return false;

    qDebug() << name << type << category << author << language << quantity << avaiable << format << page << hasDigital << ISBN;

    QSqlQuery query;
    query.prepare("INSERT INTO media (name, type, category, author, language, quantity, avaiable, format, page, hasDigital, ISBN) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(type);
    query.addBindValue(category);
    query.addBindValue(author);
    query.addBindValue(language);
    query.addBindValue(quantity);
    query.addBindValue(avaiable);
    query.addBindValue(format);
    query.addBindValue(page);
    query.addBindValue(hasDigital);
    query.addBindValue(ISBN);

    if(query.exec()){
        db->CloseDB();
        return true;
    }else{
        db->CloseDB();
        qDebug() << query.lastError().text();
        return false;
    }
}

CDs::CDs(QString objName,  QString objCategory, QString objAuthor, QString objLanguage, int objQuantity, QString objFormat, float objLength, float objSize){
    name=objName;
    type="CDs";
    category=objCategory;
    author=objAuthor;
    language=objLanguage;
    quantity=objQuantity;
    format=objFormat;
    avaiable=quantity;

    size=objSize;
    length=objLength;
}


bool CDs::create(){
    db = new DatabaseController();
    if(!db->ConnectDB()) return false;

    qDebug() << name << type << category << author << language << quantity << avaiable << format << length << size;

    QSqlQuery query;
    query.prepare("INSERT INTO media (name, type, category, author, language, quantity, avaiable, format, [size (Mb)], [length (min.)]) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(type);
    query.addBindValue(category);
    query.addBindValue(author);
    query.addBindValue(language);
    query.addBindValue(quantity);
    query.addBindValue(avaiable);
    query.addBindValue(format);
    query.addBindValue(size);
    query.addBindValue(length);

    if(query.exec()){
        db->CloseDB();
        return true;
    }else{
        db->CloseDB();
        qDebug() << query.lastError().text();
        return false;
    }
}
