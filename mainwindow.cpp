#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientthread.h"
#include <controller/database.h>
#include <form/queryhistorydialog.h>
#include <QFileDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QSqlQuery>
#include <iostream>
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QssMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/qss/app.png"));
    setWindowTitle("Client");
    model = new QSqlQueryModel(this);
    fileUtil = new FileUtil();
    initTable();
    fileUtil->addType(ui->type);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ResizeTableView(QTableView *tableview)
{
    tableview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自适应列宽
    tableview->resizeRowsToContents();//最小行高
}

void MainWindow::initTable()
{
    fileUtil->setModel();
    model = fileUtil->getModel();
    model->setHeaderData(1, Qt::Horizontal, tr("文件名"));
    model->setHeaderData(2, Qt::Horizontal, tr("文件大小"));
    model->setHeaderData(3, Qt::Horizontal, tr("类型"));
    model->setHeaderData(4, Qt::Horizontal, tr("地点"));
    model->setHeaderData(5, Qt::Horizontal, tr("发送状态"));
    ResizeTableView(ui->tableView);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    fileUtil->deleteAll();
}

void MainWindow::on_selectFile_clicked()
{
    QStringList tempList = QFileDialog::getOpenFileNames(this,tr("文件选择"),tr("/"),tr("图片文件(*.png *.jpg)"));
    fileList += tempList;
    if(!tempList.isEmpty()){
        for(int i=0;i<tempList.size();i++){
            QFileInfo info(tempList.at(i));
            if(fileUtil->addItem(info.fileName(),info.size(),ui->type->currentData().toInt(),ui->location->currentText(),QDateTime::currentDateTime())){
                initTable();
                continue;
            }
            else{
                qDebug()<<"error";
                break;
            }
        }
    }
}

void MainWindow::on_sendAll_clicked()
{
    if(fileList.isEmpty() || ui->ipEdit->text().isEmpty())
    {
        //如果ip和文件没选就按发送按钮，会弹窗，显示错误信息
        QssMessageBox::warn("文件不能为空",this,tr("警告"));
        return;
    }
    for(int i=0;i<fileList.size();i++){
        QString host = ui->ipEdit->text();
        quint16 port = ui->portEdit->text().toUInt();
        clientThread *client = new clientThread(host,port);
        connect(client, SIGNAL(SendFinish(QString)), this, SLOT(updateTableview(QString)));
        clientVec.push_back(client);
        clientVec.at(i)->sendFile(fileList.at(i));
        clientVec.at(i)->start();
    }
    clientVec.clear();
    fileList.clear();
}

void MainWindow::updateTableview(const QString &fileName)
{
    fileUtil->updateItem(fileName);
    initTable();
}

void MainWindow::on_clearButton_clicked()
{
    fileUtil->deleteAll();
    fileList.clear();
    initTable();
}

void MainWindow::on_queryButton_clicked()
{
    QueryHistoryDialog * q  = new QueryHistoryDialog();
    q->show();
}

void MainWindow::on_logoutButton_clicked()
{
    //
    this->closeWindow();
}
