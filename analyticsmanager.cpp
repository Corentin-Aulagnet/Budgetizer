#include "analyticsmanager.h"
#include "utils.h"
#include <QStringListModel>
#include <QString>
#include <string>
#include <set>
#include<QBarCategoryAxis>
#include <QStackedBarSeries>
#include <QValueAxis>
AnalyticsManager::AnalyticsManager(Ui::MainWindow *_ui,DataBaseManager *_dataManager):
    ui(_ui),
    dataManager(_dataManager)
{
    ui->monthEdit->setDate(QDate::currentDate());
    ui->yearEdit->setDate(QDate::currentDate());
    updateChartsView();
}

AnalyticsManager::~AnalyticsManager(){
    delete ui->monthsListView->selectionModel();
}
QChart *AnalyticsManager::createPieChart(SummedDataTotalPair datas)
{
    QChart *chart = new QChart();
    chart->setTitle(tr("All expenses"));
    QPieSeries *series = new QPieSeries(chart);

    for(DataTypes::sumedCategorizedData data : datas.first){
        QPieSlice *slice = series->append(data.first,data.second/datas.second);
        slice->setLabelVisible();
    }
    chart->addSeries(series);
    return chart;
}

QChart *AnalyticsManager::createMonthlyPieChart(SummedDataTotalPair datas){
    QChart *chart = new QChart();
    chart->setTitle(tr("Monthly Expenses"));
    QPieSeries *series = new QPieSeries(chart);
    for(DataTypes::sumedCategorizedData data : datas.first){
        QPieSlice *slice = series->append(data.first,data.second/datas.second);
        slice->setLabelVisible();
    }
    chart->addSeries(series);
    return chart;
}

QChart* AnalyticsManager::createMonthlyBarChart(QList<QBarSet*> setList,QStringList months,float max){
    QChart *chart = new QChart();
    //Add series to the graph
    QStackedBarSeries *series = new QStackedBarSeries();
    for(QList<QBarSet*>::ConstIterator it = setList.begin();it != setList.end();it++){
        series->append(*it);
    }
    chart->addSeries(series);
    //Add categories to the XAxis of the chart
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(months);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    //Add YAxis to the chart
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,max);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}
std::map<QString, float> getYearlyCategoryValueFromExtrac(DataTypes::DataTable rows, QDate selectedYear){
    float value;
    QString cat;
    QDate date;
    std::map<QString, float> output;
    //Extract values to sum on the categories
    for(int row = 0; row < rows.size();row++){
        //ID,DATE(d/m/y),LABEL,PRICEPERUNIT,QUANTITY,PRICE,CATEGORY
        value = rows[row][5].toFloat();
        cat = rows[row][6];
        //Process date
        QString dateStr = rows[row][1];
        QStringList dmy = dateStr.split(QChar('/'));
        date = QDate(dmy[2].toInt(),dmy[1].toInt(),dmy[0].toInt());
        if(date.year() == selectedYear.year()){
            output[cat]+=value;
        }

    }
    return output;
}
SummedDataTotalPair AnalyticsManager::gatherDataForYearlyPieCharts(QDate selectedYear){
    dataManager->ExecuteSelectTransaction();
    DataTypes::DataTable rows = DataBaseManager::GetSelectedRows();

    std::map<QString, float> categoryToSum = getYearlyCategoryValueFromExtrac(rows, selectedYear);

    std::map<QString, float>::iterator it = categoryToSum.begin();
    DataTypes::SummedCategorizedDataList summedCategorizedDataList;
    float total=0;
    while (it != categoryToSum.end()){
        float value = it->second;
        total +=value;
        QString catName = it->first;
        DataTypes::sumedCategorizedData data = DataTypes::sumedCategorizedData(catName,value);
        summedCategorizedDataList.append(data);
        it++;
    }
    return SummedDataTotalPair(summedCategorizedDataList,total);
}

std::map<QString, float> getMontlhyCategoryValueFromExtrac(DataTypes::DataTable rows, QDate endOfMonth){
    float value;
    QString cat;
    QDate date;
    std::map<QString, float> output;
    //Extract values to sum on the categories
    for(int row = 0; row < rows.size();row++){
        //ID,DATE(d/m/y),LABEL,PRICEPERUNIT,QUANTITY,PRICE,CATEGORY
        value = rows[row][5].toFloat();
        cat = rows[row][6];
        //Process date
        QString dateStr = rows[row][1];
        QStringList dmy = dateStr.split(QChar('/'));
        date = QDate(dmy[2].toInt(),dmy[1].toInt(),dmy[0].toInt());
        if(date.month() == endOfMonth.month()){
            output[cat]+=value;
        }

    }
    return output;
}
std::map<QString, float> getMontlhyCategoryValueFromExtrac(DataTypes::DataTable rows){
    float value;
    QString cat;
    QDate date;
    std::map<QString, float> output;
    //Extract values to sum on the categories
    for(int row = 0; row < rows.size();row++){
        //ID,DATE(d/m/y),LABEL,PRICEPERUNIT,QUANTITY,PRICE,CATEGORY
        value = rows[row][5].toFloat();
        cat = rows[row][6];
        //Process date
        QString dateStr = rows[row][1];
        QStringList dmy = dateStr.split(QChar('/'));
        date = QDate(dmy[2].toInt(),dmy[1].toInt(),dmy[0].toInt());
        output[cat]+=value;

    }
    return output;
}
SummedDataTotalPair AnalyticsManager::gatherDataForMonthlyPieCharts(QDate endOfMonth){
    dataManager->ExecuteSelectTransaction();
    DataTypes::DataTable rows = DataBaseManager::GetSelectedRows();
    std::map<QString, float> categoryToSum = getMontlhyCategoryValueFromExtrac(rows,endOfMonth);
    //Convert to SuumedDataTotalPair
    std::map<QString, float>::iterator it = categoryToSum.begin();
    DataTypes::SummedCategorizedDataList summedCategorizedDataList;
    float total=0;
    while (it != categoryToSum.end()){
        float value = it->second;
        total +=value;
        QString catName = it->first;
        DataTypes::sumedCategorizedData data = DataTypes::sumedCategorizedData(catName,value);
        summedCategorizedDataList.append(data);
        it++;
    }
    return SummedDataTotalPair(summedCategorizedDataList,total);
}

QStringList AnalyticsManager::gatherDataForMonthsListView(){
    //Gather all the mm/yyy in the data base
    char sql[] = "SELECT DISTINCT DATE FROM ACCOUNT";
    dataManager->ExecuteTransaction(sql);
    QStringList output;
    DataTypes::DataTable dataTable = dataManager->GetSelectedRows();
    std::map<int,QString> int2Month{{1, QString::fromStdString("January")},
                                    {2, QString::fromStdString("February")},
                                    {3, QString::fromStdString("March")},
                                    {4, QString::fromStdString("April")},
                                    {5, QString::fromStdString("May")},
                                    {6, QString::fromStdString("June")},
                                    {7, QString::fromStdString("July")},
                                    {8, QString::fromStdString("August")},
                                    {9, QString::fromStdString("September")},
                                    {10, QString::fromStdString("October")},
                                    {11, QString::fromStdString("November")},
                                    {12, QString::fromStdString("December")}};
    for(int i =0; i < dataTable.size();i++){
        QDate date = QDate::fromString(dataTable[i][0],"dd/MM/yyyy");
        QString str;
        str += int2Month[date.month()];
        str += " "+date.toString("yyyy");
        output.append(str);
    }
    return output;
}

QList<QBarSet*> AnalyticsManager::gatherDataForMonthlyBarCharts(QStringList monthsList, float& max){
    QBarSet* set=0;
    QList<QBarSet*> setList;
    std::set<QString> categories;
    QList<std::map<QString, float>> categoryValuesListByMonth;
    for(QString mmyyyy : monthsList){
        //Gather and process data for each months/year in monthsList
        std::map<QString,std::string> month2Int{{QString::fromStdString("January"), "01"},
                                        {QString::fromStdString("February"), "02"},
                                        {QString::fromStdString("March"), "03"},
                                        {QString::fromStdString("April"), "04"},
                                        {QString::fromStdString("May"), "05"},
                                        {QString::fromStdString("June"), "06"},
                                        {QString::fromStdString("July"), "07"},
                                        {QString::fromStdString("August"), "08"},
                                        {QString::fromStdString("September"), "09"},
                                        {QString::fromStdString("October"), "10"},
                                        {QString::fromStdString("November"), "11"},
                                        {QString::fromStdString("December"), "12"}};
        std::string month = month2Int[mmyyyy.split(" ")[0]];
        std::string year=mmyyyy.split(" ")[1].toStdString();
        //Gather date from database
        std::string str = Utils::format("SELECT * FROM ACCOUNT WHERE strftime('%m', DATE) IN('#') AND strftime('%Y', DATE) IN('#')",2,month.c_str(),year.c_str());
        char sql[str.size()];
        for (int i = 0; i < sizeof(sql); i++) {
                sql[i] = str[i];
        }
        dataManager->ExecuteSelectTransaction();
        //Create set, process data to get values by categories

        DataTypes::DataTable rows = DataBaseManager::GetSelectedRows();
        std::map<QString, float> categoryValues = getMontlhyCategoryValueFromExtrac(rows,QDate(stoi(year),stoi(month),1));
        //Appends the set of categories to display
        for(std::map<QString,float>::iterator it = categoryValues.begin();it != categoryValues.end();it++){
            categories.insert(it->first);
        }
        categoryValuesListByMonth.append(categoryValues);
    }
    float tmp=0;
    for(std::set<QString>::iterator it = categories.begin(); it != categories.end();it++){
        tmp = 0;
        set = new QBarSet(*it); //set name is the category name, values ordered by months in monthsList
        for(int i =0; i<categoryValuesListByMonth.size();i++){
            *set << categoryValuesListByMonth[i][*it];
            tmp+= categoryValuesListByMonth[i][*it];
        }
        if(tmp > max) max = tmp;
        setList.append(set);
    }
    return setList;
}

void AnalyticsManager::updateChartsView(){
    updateYearlyChartView();
    updateMonthlyChartView();
}
void AnalyticsManager::updateMonthlyChartView(){
    SummedDataTotalPair datas= gatherDataForMonthlyPieCharts(ui->monthEdit->date());
    QChart* chart = createMonthlyPieChart(datas);
    QChart* oldChart = ui->graphicsView_2->chart();
    ui->graphicsView_2->setChart(chart);
    delete oldChart;
    ui->monthTotalLabel->setText(QString::fromStdString(Utils::doubleToStr(datas.second)+"€"));
}

void AnalyticsManager::updateYearlyChartView(){
    SummedDataTotalPair datas= gatherDataForYearlyPieCharts(ui->yearEdit->date());
    QChart* chart = createMonthlyPieChart(datas);
    QChart* oldChart = ui->graphicsView->chart();
    ui->graphicsView->setChart(chart);
    delete oldChart;
    ui->yearTotalLabel->setText(QString::fromStdString(Utils::doubleToStr(datas.second)+"€"));
}

void AnalyticsManager::updateMonthsListView(){
    //Update the view of the mm/yyyy available in the databases to be selected to display on the barchart
    QListView* listView = ui->monthsListView;
    QItemSelectionModel* m = listView->selectionModel();

    QStringList monthsList = gatherDataForMonthsListView();
    QStringListModel* model = new QStringListModel();
    model->setHeaderData(0,Qt::Horizontal,"mm/yyyy");
    model->setStringList(monthsList);
    listView->setModel(model);
    delete m;
}

void AnalyticsManager::updateMonthBarChartsView(){
    QList<QModelIndex> indexes = ui->monthsListView->selectionModel()->selectedRows().toList();
    QStringList monthsList;
    for(QModelIndex index : indexes){
        monthsList.append(index.data().toString());
    }
    float max=0;
    QList<QBarSet*> setList = gatherDataForMonthlyBarCharts(monthsList,max);
    QChart* chart = createMonthlyBarChart(setList,monthsList,max);
    QChart* oldChart = ui->graphicsView_3->chart();
    ui->graphicsView_3->setChart(chart);
    delete oldChart;
    //ui->yearTotalLabel->setText(QString::fromStdString(Utils::doubleToStr(datas.second)+"€"));
}
