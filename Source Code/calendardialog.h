#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>

#include "ui_calendardialog.h"
#include "hashTable.cpp"
#include "tree.cpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Calendar : public QDialog
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = 0);
    QString loadTable();
    void loadTree(QString currentLabel);

private:
    hashTable ht;
    tree bst;
    Ui::calendarDialog ui;
    QStringListModel *model;
    QDate getdate();
    QDate currentDate() const;
    void setCurrentDate(const QDate &d);
    void populateDate(const QStringList &l);
    void on_calendar_clicked(const QDate &date);
    int rdn(int y, int m, int d);
    vector<string> split(string in);

private slots:
    void on_MonthFrontButton_clicked();
    void on_MonthBackButton_clicked();
    void on_yearBackButton_clicked();
    void on_yearFrontButton_clicked();
    void on_OkButtonTable_clicked();
    void on_OkButtonTree_clicked();
    void on_mostLikedDaysTable_clicked();
    void on_leastLikedDaysTable_clicked();
    void on_mostLikedDaysTree_clicked();
    void on_leastLikedDaysTree_clicked();
};
#endif // CALENDARDIALOG_H
