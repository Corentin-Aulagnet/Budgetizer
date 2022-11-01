#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newcategoryform.h"
#include "databasemanager.h"
#include <iostream>
#include <list>
#include <string>
#include "utils.h"
MainWindow::MainWindow(DataBaseManager* _databaseManager,QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      databaseManager(_databaseManager),
      categories()
{ 
    setWindowTitle("Budgetizer");

    databaseManager->LoadSavedCategories(categories);

    ui->setupUi(this);
    analyticsManager = new AnalyticsManager(ui,_databaseManager);
    ui->dateEdit->setDate(QDate::currentDate());

    RefreshCategories();
    UpdateTableView();

}
MainWindow::~MainWindow()
{
    databaseManager->Save(categories);
    ui->accountView->selectionModel()->clear();
    delete ui->accountView->selectionModel();
    delete ui;
    delete analyticsManager;
}
void MainWindow::monthlyAnalyticsDateChangedSLOT(){
    analyticsManager->updateMonthlyChartView();
}
void MainWindow::yearlyAnalyticsDateChangedSLOT(){
    analyticsManager->updateYearlyChartView();
}
void MainWindow::getNewCategoryFromFormSLOT(){
    NewCategoryForm* f = new NewCategoryForm(this);
    f->show();
}
void MainWindow::addNewLineSLOT(){
    //Gather information in the form
    std::string label;
    QString priceperunit;
    std::string quantity;
    std::string date;
    std::string category;
    std::string pricestr;
    label = ui->labelEditField->text().toStdString().append("'").insert(0,"'");
    priceperunit = ui->priceperunitEditField->text();
    quantity = ui->qtySpinBox->text().toStdString();
    date = ui->dateEdit->text().toStdString().append("'").insert(0,"'");
    category = ui->categoryCB->currentText().toStdString().append("'").insert(0,"'");

    int qty = stoi(quantity);
    double pricepunit = stod(priceperunit.replace('.',',').toStdString());
    double price = pricepunit * qty;
    pricestr = Utils::doubleToStr(price);
    databaseManager->ExecuteInsertTransaction(date,label,priceperunit.replace(',','.').toStdString(),quantity,pricestr,category);
    UpdateTableView();
    ui->labelEditField->clear();
    ui->priceperunitEditField->clear();
    ui->qtySpinBox->setValue(1);
    ui->dateEdit->setDate(QDate::currentDate());
}
void MainWindow::tabChangedSLOT(){
    if(ui->ViewsTabWidget->currentIndex()==0){
        //Expenses view activated
        RefreshCategories();
        UpdateTableView();
    }else if (ui->ViewsTabWidget->currentIndex()==1){
        //Analytics view activated
        analyticsManager->updateChartsView();
    }
}
void MainWindow::chartsViewTabChangedSLOT(){
    analyticsManager->updateMonthsListView();
}

void MainWindow::monthViewItemClickedSLOT(){
    analyticsManager->updateMonthBarChartsView();
}
void MainWindow::RefreshCategories(){
    ui->categoryCB->clear();
    for(QString cat : categories){
        ui->categoryCB->addItem(cat);
    }
}
void MainWindow::addNewCategory(QString categoryName){
    if(!categoryName.isEmpty()){
         ui->categoryCB->addItem(categoryName);
         categories << categoryName;
         databaseManager->SaveCategories(categories);
    }
    ui->categoryCB->setCurrentIndex(ui->categoryCB->count()-1);

}
QStandardItemModel* MainWindow::SetUpModel(DataTypes::DataTable& rows){
    QStandardItemModel* model = new QStandardItemModel(rows.size(), 6);
    //model->setHeaderData(0,Qt::Horizontal,tr("Id"));
    model->setHeaderData(0,Qt::Horizontal,tr("Date"));
    model->setHeaderData(1,Qt::Horizontal,tr("Label"));
    model->setHeaderData(2,Qt::Horizontal,tr("€/Unit"));
    model->setHeaderData(3,Qt::Horizontal,tr("Quantity"));
    model->setHeaderData(4,Qt::Horizontal,tr("Price"));
    model->setHeaderData(5,Qt::Horizontal,tr("Category"));
    for(int rowI =0;rowI<rows.size();rowI++){
        for(int colI = 1;colI<rows[rowI].size();colI++){
            QString str = rows[rowI][colI];
            str.append((colI==3 | colI == 5) ? " €" : "");
            QStandardItem *item = new QStandardItem(str);
            model->setItem(rowI,colI-1,item);
            }
        }

    return model;
}

void MainWindow::UpdateTableView(){
    QTableView* tableView = ui->accountView;
    QItemSelectionModel *m = tableView->selectionModel();
    databaseManager->ExecuteSelectTransaction();
    DataTypes::DataTable rows = DataBaseManager::GetSelectedRows();
    QStandardItemModel* model = SetUpModel(rows);
    tableView->setModel(model);
    delete m;
}
