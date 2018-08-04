#ifndef QUERYHISTORYDIALOG_H
#define QUERYHISTORYDIALOG_H

#include "qss/qss.h"
#include <QDialog>
#include <QSqlQueryModel>
#include <QTableView>

namespace Ui {
class QueryHistoryDialog;
}

class QueryHistoryDialog : public QssDialog
{
    Q_OBJECT

public:
    explicit QueryHistoryDialog(QWidget *parent = 0);
    ~QueryHistoryDialog();
    void ResizeTableView(QTableView *tableview);
    void init();

private slots:
    void on_deleteButton_clicked();

private:
    Ui::QueryHistoryDialog *ui;
    QSqlQueryModel * model;
};

#endif // QUERYHISTORYDIALOG_H
