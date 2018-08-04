#include "queryhistorydialog.h"
#include "ui_queryhistorydialog.h"

#include <QSqlQuery>
#include <QDebug>
QueryHistoryDialog::QueryHistoryDialog(QWidget *parent) :
    QssDialog(parent),
    ui(new Ui::QueryHistoryDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/qss/app.png"));
    setWindowTitle("历史记录");
    model = new QSqlQueryModel();
    init();
}

QueryHistoryDialog::~QueryHistoryDialog()
{
    delete ui;
}

void QueryHistoryDialog::ResizeTableView(QTableView *tableview)
{
    tableview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自适应列宽
    tableview->resizeRowsToContents();//最小行高
}

void QueryHistoryDialog::init()
{
    model->setQuery("SELECT s.id,s.filename,t.type,s.location,s.send_state,s.datetime "
                    "FROM send_record s,type t where t.id=s.type "
                    "order by s.datetime desc");
    model->setHeaderData(0,Qt::Horizontal,tr("产品名字"));
    model->setHeaderData(1,Qt::Horizontal,tr("文件名"));
    model->setHeaderData(2,Qt::Horizontal,tr("类型"));
    model->setHeaderData(3,Qt::Horizontal,tr("地点"));
    model->setHeaderData(4,Qt::Horizontal,tr("发送状态"));
    model->setHeaderData(5,Qt::Horizontal,tr("发送日期"));
    ResizeTableView(ui->tableView);
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0,true);
}

void QueryHistoryDialog::on_deleteButton_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if(row!=-1){
        QString id = ui->tableView->model()->data(ui->tableView->model()->index(row,0)).toString();
        QSqlQuery query;
        query.exec(QString("delete from rec_record where file_id = '%1' ").arg(id));
        query.exec(QString("delete from send_record where id = '%1' ").arg(id));
        qDebug()<<"delete success";
        init();
        QssMessageBox::tips("删除成功",this,tr("提示"));
    }
    else{
        QssMessageBox::warn("未选中！",this,tr("警告"));
        qDebug()<<"no success";
    }
}
