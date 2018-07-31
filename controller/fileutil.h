#include <QSqlQueryModel>

#ifndef FILEUTIL_H
#define FILEUTIL_H

class FileUtil
{
public:
    FileUtil();
    bool addItem(QString fileName,qint64 fileSize,QString type);
    bool updateItem(QString fileName);
    bool deleteItem();
    bool deleteAll();
    bool importItem();
    void setModel();
    QSqlQueryModel *getModel();
    QSqlQueryModel *model;
};

#endif // FILEUTIL_H
