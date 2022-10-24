#ifndef ANALYTICSMANAGER_H
#define ANALYTICSMANAGER_H

#include <QPieSeries>
#include "databasemanager.h"
#include "ui_mainwindow.h"
#include <map>
#include <QChart>
#include <QChartView>
#include <QString>
#include <QDateTimeEdit>
#include <QBarSet>
/*!
 * \brief SummedDataTotalPair
 * \details QPair<DataTypes::SummedCategorizedDataList,float>, used to represent datas that are sorted by categories, with the total value of the data represented
 */
typedef QPair<DataTypes::SummedCategorizedDataList,float> SummedDataTotalPair;
/*!
 * \brief The AnalyticsManager class
 * \details This class is used to plot the datas and perform all the processing necessary to the plots
 */
class AnalyticsManager
{
    Q_DECLARE_TR_FUNCTIONS(AnalyticsManager)
public:
    ~AnalyticsManager();
    AnalyticsManager(Ui::MainWindow *_ui,DataBaseManager *_dataManager);
    /*!
     * \brief updateChartsView
     * \details Caller function used to update the monthly and yearly charts
     */
    void updateChartsView();
    /*!
     * \brief updateMonthlyChartView
     * \details Function used to update only the montlhy chart
     */
    void updateMonthlyChartView();
    /*!
     * \brief updateYearlyChartView
     * \details Function used to update only the yearly chart
     */
    void updateYearlyChartView();
    /*!
     * \brief updateMonthsListView
     * \details Function used to update the listView of the months to select wich months to display in the bar stacked chart
     */
    void updateMonthsListView();
    /*!
     * \brief updateMonthBarChartsView
     * \details Function used to update the stacked bar chart
     */
    void updateMonthBarChartsView();
private:
    Ui::MainWindow *ui;
    DataBaseManager *dataManager;
    /*!
     * \brief createPieChart
     * \details creates a Chart to be plotted, used for yearly pie charts
     * \param datas, the data to plot
     * \return QChart, a pointer to the newly created Pie Chart
     */
    QChart *createPieChart(SummedDataTotalPair datas);
    /*!
     * \brief createMonthlyPieChart
     * \details creates a Chart to be plotted, used for monthly pie charts
     * \param datas, the data to plot
     * \return QChart, a pointer to the newly created Pie Chart
     */
    QChart *createMonthlyPieChart(SummedDataTotalPair datas);
    /*!
     * \brief createMonthlyBarChart
     * \details creates a QChart to be plotted, used for the stacked bar chart
     * \param setList, the lists of all sets to be plotted. A set is a serie of data
     * \param months, the lists of months to be plotted on the XAxis, these are the categories of the the chart
     * \param max, the maximum value to be plotted, used to calibrate the display range of the chart
     * \return QChart, a pointer to the newly created stacked bar chart
     */
    QChart *createMonthlyBarChart(QList<QBarSet*>setList, QStringList months,float max);
    /*!
     * \brief gatherDataForYearlyPieCharts
     * \details Function to fetch and process datas for the yearly pie chart
     * \param selectedYear, a QDate used to indicate the year to select. Only selectedYear.year() is used, other info are redundent
     * \return SummedDataTotalPair, represents the data sorted by categories + the total value of the of the data
     */
    SummedDataTotalPair gatherDataForYearlyPieCharts(QDate selectedYear);
    /*!
     * \brief gatherDataForMonthlyPieCharts
     * \details Function to fetch and process datas for the yearly pie chart
     * \param endOfMonth, a QDate used to indicate the month to select. Only endOfMonth.month() is used, other info are redundent
     * \return SummedDataTotalPair, represents the data sorted by categories + the total value of the of the data
     */
    SummedDataTotalPair gatherDataForMonthlyPieCharts(QDate endOfMonth);
    /*!
     * \brief gatherDataForMonthlyBarCharts
     * \details fetches and process the data to be plotted in the bar stacked chart. Also creates the QBarSet
     * \param monthsList, the list of months from wich data are to be fetched
     * \param max, a reference to the maximum value found to be displayed
     * \return QList<QBarSet*> the list of set to be displayed, in the order of the month in monthsList
     */
    QList<QBarSet*> gatherDataForMonthlyBarCharts(QStringList monthsList, float& max);
    /*!
     * \brief gatherDataForMonthsListView
     * \details fetch all months in the databases to display the in the listView next to the bar chart
     * \return QStringList of the months in the datebase
     */
    QStringList gatherDataForMonthsListView();
};

#endif // ANALYTICSMANAGER_H
