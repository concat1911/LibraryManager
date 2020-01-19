#ifndef MEDIAN_H
#define MEDIAN_H

#include "QString"
#include "QLabel"
#include <QtSql>

class Median
{
public:
    QString name;
    QString type;
    QString category;
    QString author;
    QString language;
    QString format;
    int quantity;
    int avaiable;

public:
    Median();
    virtual ~Median() {}

public:
    QSqlQuery query;

public:
    void SetBase(QString objName, QString objCategory, QString objAuthor, QString objLanguage, int objQuantity, QString objFormat);
    virtual void BindValues();
    virtual bool QueryExec();
};

class Book : public Median{
    int page;
    bool hasDigital;
    QString ISBN;

public:
    Book(int objPage, bool objHasDigital, QString objISBN);

public:
    bool create();
    bool update(int objId);
};

class CDs: public Median{
    float length;
    float size;

public:
    CDs(float objLength, float objSize);

public:
    bool create();
    bool update(int objId);
};
#endif // MEDIAN_H
