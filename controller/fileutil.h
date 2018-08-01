#include <QSqlQueryModel>

#ifndef FILEUTIL_H
#define FILEUTIL_H

class FileUtil
{
public:
    FileUtil();
    bool addItem(QString fileName, qint64 fileSize, QString type, QString location);
    bool updateItem(QString fileName);
    bool deleteItem();
    bool deleteAll();
    void setModel();
    QSqlQueryModel *getModel();
    QSqlQueryModel *model;
};

#endif // FILEUTIL_H
