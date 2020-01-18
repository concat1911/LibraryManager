#ifndef MEDIAN_H
#define MEDIAN_H

#include "QString"
#include "QLabel"

#include "databasecontroller.h"
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
    DatabaseController *db;

public:
    Median();
};

class Book : public Median{
    int page;
    bool hasDigital;
    QString ISBN;

public:
    Book(QString objName,  QString objCategory, QString objAuthor, QString objLanguage, int objQuantity, QString objFormat, int objPage, bool objHasDigital, QString objISBN);

public:
    bool create();
    bool update(int objId);
};

class CDs: Median{
    float length;
    float size;

public:
    CDs(QString objName,  QString objCategory, QString objAuthor, QString objLanguage, int objQuantity, QString objFormat, float objLength, float objSize);

public:
    bool create();
};
#endif // MEDIAN_H
