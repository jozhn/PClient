#include <controller/fileutil.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <controller/database.h>

FileUtil::FileUtil()
{
}

bool FileUtil::addItem(QString fileName,qint64 fileSize,QString type,QString location)
{
    QSqlQuery query;
    query.prepare("insert into send_record (filename,filesize,type,location) "
                  "values"
                   " (:filename,:filesize,:type,:location);");
    query.bindValue(":filename",fileName);
    query.bindValue(":filesize",fileSize);
    query.bindValue(":type",type);
    query.bindValue(":location",location);
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
    QString sql = QString("update send_record set send_state='已发送',receive_state='已接收' where filename='%1'").arg(fileName);
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
    QString sql = "update send_record set send_flag=1 where send_flag=0";
    bool success = query.exec(sql);
    if(success){
        return true;
    }
    else
        return false;
}


void FileUtil::setModel()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT id,filename,filesize,type,location,send_state FROM send_record where send_flag=0");
}

QSqlQueryModel *FileUtil::getModel()
{
    return model;
}
