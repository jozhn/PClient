#include "fileutil.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <controller/database.h>

FileUtil::FileUtil()
{
}

bool FileUtil::addItem(QString fileName,qint64 fileSize,QString type)
{
    QSqlQuery query;
    query.prepare("insert into send_record (filename,filesize,type) "
                  "values"
                   " (:filename,:filesize,:type);");
    query.bindValue(":filename",fileName);
    query.bindValue(":filesize",fileSize);
    query.bindValue(":type",type);
    bool success = query.exec();
    if(success){
        return true;
    }
    else
        return false;
}

bool FileUtil::updateItem(QString fileName)
{
    QSqlQuery query;
    QString sql = QString("update send_record set state='已发送' where filename='%1'").arg(fileName);
    bool success = query.exec(sql);
    if(success){
        return true;
    }
    else
        return false;
}

bool FileUtil::deleteItem()
{

}

bool FileUtil::deleteAll()
{
    QSqlQuery query;
    QString sql = "update send_record set flag=1 where flag=0";
    bool success = query.exec(sql);
    if(success){
        return true;
    }
    else
        return false;
}

bool FileUtil::importItem()
{

}

void FileUtil::setModel()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM send_record where flag=0");
}

QSqlQueryModel *FileUtil::getModel()
{
    return model;
}
