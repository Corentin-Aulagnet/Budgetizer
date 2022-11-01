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
    /*!
     * \brief UpdateTableView
     * \details Update the view of the database with all entries in the database
     */
    void UpdateTableView();
    /*!
     * \brief SetUpModel
     * \details Sets up the model to be displayed in the table view
     * \param rows
     * \return
     */
    QStandardItemModel* SetUpModel(DataTypes::DataTable &rows);
    /*!
     * \brief RefreshCategories
     * \details Refreshes the categories displayed in the drop down menu, in the main tab
     */
    void RefreshCategories();
    /*!
     * \brief categories, list of the categories currently saved
     */
    QStringList categories;
private slots:
    /*!
     * \brief getNewCategoryFromFormSLOT
     * \details SLOT to display the window to create and save a new category \n
     * linked to the ... button on the main tab
     */
    void getNewCategoryFromFormSLOT();
    /*!
     * \brief addNewLineSLOT
     * \details SLOT to create a new entry in the database \n
     * linked to the Add Line button on the main tab
     */
    void addNewLineSLOT();
    /*!
     * \brief tabChangedSLOT
     * \details SLOT to update views when main tabs are changed
     */
    void tabChangedSLOT();
    /*!
     * \brief monthlyAnalyticsDateChangedSLOT
     * \details SLOT to update the monthly analytics graph when the month to see is changed \n
     * linked to the month date display int the graphs tab
     */
    void monthlyAnalyticsDateChangedSLOT();
    /*!
     * \brief yearlyAnalyticsDateChangedSLOT
     * \details SLOT to update the yearly analytics graph when the year to see is changed \n
     * linked to the year date display int the graphs tab
     */
    void yearlyAnalyticsDateChangedSLOT();
    /*!
     * \brief chartsViewTabChangedSLOT
     * \details SLOT to update the graphs when the displayed graph tab change
     */
    void chartsViewTabChangedSLOT();
    /*!
     * \brief monthViewItemClickedSLOT
     * \details SLOT to update the barchart when the user click a month in the list view to update the time period to be displayed
     */
    void monthViewItemClickedSLOT();
};
#endif // MAINWINDOW_H
