#include "median.h"

Median::Median(){}

void Median::SetBase(QString objName, QString objCategory, QString objAuthor, QString objLanguage, int objQuantity, QString objFormat){
    name=objName;
    category=objCategory;
    author=objAuthor;
    language=objLanguage;
    quantity=objQuantity;
    avaiable=quantity;
    format=objFormat;
}

void Median::BindValues(){
    query.addBindValue(name);
    query.addBindValue(type);
    query.addBindValue(category);
    query.addBindValue(author);
    query.addBindValue(language);
    query.addBindValue(quantity);
    query.addBindValue(avaiable);
    query.addBindValue(format);
}

bool Median::QueryExec(){
    if(query.exec()){
        return true;
    }else{
        qDebug() << query.lastError().text();
        return false;
    }
}

Book::Book(int objPage, bool objHasDigital, QString objISBN){
    type="Book";
    page=objPage;
    hasDigital=objHasDigital;
    ISBN=objISBN;
}

bool Book::create(){
    query.prepare("INSERT INTO media (name, type, category, author, language, quantity, avaiable, format, page, hasDigital, ISBN) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    Median::BindValues();
    query.addBindValue(page);
    query.addBindValue(hasDigital);
    query.addBindValue(ISBN);

    return Median::QueryExec();
}

bool Book::update(int objId){
    query.prepare("UPDATE media SET name=?, type=?, category=?, author=?, language=?, quantity=?, avaiable=?, format=?, page=?, hasDigital=?, ISBN=? WHERE id=?");
    Median::BindValues();
    query.addBindValue(page);
    query.addBindValue(hasDigital);
    query.addBindValue(ISBN);
    query.addBindValue(objId);

    return Median::QueryExec();
}

CDs::CDs(float objLength, float objSize){
    type="CDs";
    size=objSize;
    length=objLength;
}

bool CDs::create(){
    query.prepare("INSERT INTO media (name, type, category, author, language, quantity, avaiable, format, [size (Mb)], [length (min.)]) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    Median::BindValues();
    query.addBindValue(size);
    query.addBindValue(length);

    return Median::QueryExec();
}

bool CDs::update(int objId){
    query.prepare("UPDATE media SET name=?, type=?, category=?, author=?, language=?, quantity=?, avaiable=?, format=?, [size (Mb)]=?, [length (min.)]=? WHERE id=?");
    Median::BindValues();
    query.addBindValue(size);
    query.addBindValue(length);
    query.addBindValue(objId);

    return Median::QueryExec();
}
