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
    //qDebug() << name << type << category << author << language << quantity << avaiable << format << page << hasDigital << ISBN;

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
        return true;
    }else{
        qDebug() << query.lastError().text();
        return false;
    }
}

bool Book::update(int objId){
    //qDebug() << name << type << category << author << language << quantity << avaiable << format << page << hasDigital << ISBN;

    QSqlQuery query;
    query.prepare("UPDATE media SET name=?, category=?, author=?, language=?, quantity=?, format=?, page=?, hasDigital=?, ISBN=? WHERE id=?");
    query.addBindValue(name);
    query.addBindValue(category);
    query.addBindValue(author);
    query.addBindValue(language);
    query.addBindValue(quantity);
    query.addBindValue(format);
    query.addBindValue(page);
    query.addBindValue(hasDigital);
    query.addBindValue(ISBN);
    query.addBindValue(objId);

    if(query.exec()){
        return true;
    }else{
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
    //qDebug() << name << type << category << author << language << quantity << avaiable << format << length << size;

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
        return true;
    }else{
        qDebug() << query.lastError().text();
        return false;
    }
}

bool CDs::update(int objId){
    //qDebug() << name << type << category << author << language << quantity << avaiable << format << length << size;

    QSqlQuery query;
    query.prepare("UPDATE media SET name=?, category=?, author=?, language=?, quantity=?, format=?, [size (Mb)]=?, [length (min.)]=? WHERE id=?");
    query.addBindValue(name);
    query.addBindValue(category);
    query.addBindValue(author);
    query.addBindValue(language);
    query.addBindValue(quantity);
    query.addBindValue(format);
    query.addBindValue(size);
    query.addBindValue(length);
    query.addBindValue(objId);

    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().text();
        return false;
    }
}
