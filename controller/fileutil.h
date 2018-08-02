#include <QSqlQueryModel>
#include <QComboBox>
#include <QDateTime>

#ifndef FILEUTIL_H
#define FILEUTIL_H

class FileUtil
{
public:
    FileUtil();
    void addType(QComboBox *typebox);
    bool addItem(QString fileName, qint64 fileSize, int type, QString location, QDateTime datetime);
    bool updateItem(QString fileName);
    bool deleteItem();
    bool deleteAll();
    void setModel();
    QSqlQueryModel *getModel();
    QSqlQueryModel *model;
};

#endif // FILEUTIL_H
