#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStandardItem>
#include "databasemanager.h"
#include "analyticsmanager.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DataBaseManager *_databaseManager,QWidget *parent = nullptr);
    ~MainWindow();
    void addNewCategory(QString categoryName);
private:

    Ui::MainWindow *ui;
    DataBaseManager *databaseManager;
    AnalyticsManager *analyticsManager;
    void UpdateTableView();
    QStandardItemModel* SetUpModel(DataTypes::DataTable &rows);
    void RefreshCategories();
    QStringList categories;
private slots:
    void getNewCategoryFromFormSLOT();
    void addNewLineSLOT();
    void tabChangedSLOT();
    void monthlyAnalyticsDateChangedSLOT();
    void yearlyAnalyticsDateChangedSLOT();
    void chartsViewTabChangedSLOT();
    void monthViewItemClickedSLOT();
};
#endif // MAINWINDOW_H
