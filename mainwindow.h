#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qss/qss.h"
#include <QThread>
#include <QString>
#include "clientthread.h"
#include "controller/fileutil.h"
#include <QVector>
#include <QTableView>
#include <QEvent>

class clientThread;

namespace Ui {
class MainWindow;
}

class MainWindow : public QssMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ResizeTableView(QTableView *tableview);
    void initTable();

private:
    Ui::MainWindow *ui;
    FileUtil *fileUtil;
    QSqlQueryModel *model;
    QStringList fileList;
    QVector<clientThread*> clientVec;
    void closeEvent(QCloseEvent *e);

private slots:
    void on_selectFile_clicked();
    void on_sendAll_clicked();
    void updateTableview(const QString &fileName);
    void on_clearButton_clicked();
};

#endif // MAINWINDOW_H
