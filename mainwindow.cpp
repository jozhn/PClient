#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientthread.h"
#include <QFileDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <iostream>
#include <controller/database.h>
#include <QDebug>

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
    model->setHeaderData(4, Qt::Horizontal, tr("发送状态"));
    ResizeTableView(ui->tableView);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(6);
    ui->tableView->hideColumn(7);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    fileUtil->deleteAll();
}

void MainWindow::on_selectFile_clicked()
{
    fileList = QFileDialog::getOpenFileNames(this,tr("文件选择"),tr("/"),tr("图片文件(*.png *.jpg)"));
    if(!fileList.isEmpty()){
        for(int i=0;i<fileList.size();i++){
            QFileInfo info(fileList.at(i));
            if(fileUtil->addItem(info.fileName(),info.size(),ui->type->currentText())){
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
    //弹窗时发送按钮不能用
//    ui->pushButtonSend->setEnabled(false);
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
    initTable();
}
