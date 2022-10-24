#include "databasemanager.h"
#include <QMessageBox>
#include<iostream>
#include <fstream>
#include <string>
#include "utils.h"
DataTypes::DataTable DataBaseManager::selectedRows;
char* DataBaseManager::dataBaseFileName;
DataBaseManager::DataBaseManager(char* _dataBaseFileName)
{
    dataBaseFileName = _dataBaseFileName;
    OpenDataBaseConnection();
}
DataBaseManager::DataBaseManager(std::string _dataBaseFileName)
{
    dataBaseFileName = (char*)_dataBaseFileName.c_str();
    OpenDataBaseConnection();
}
DataBaseManager::~DataBaseManager(){
    sqlite3_close(db);
}
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void DataBaseManager::LoadSavedCategories(QStringList &categories){
    std::ifstream f;
    f.open("categories.save");
    if(f.is_open()){
        std::string line;
        while(std::getline(f,line)){
            categories << QString::fromStdString(line);
        }
    }

}
void DataBaseManager::SaveCategories(QStringList &categories){
    std::ofstream f;
    f.open("categories.save");
    if(f.is_open()){
        for(QString str : categories){
            f << str.toStdString() << "\n";
        }
    }
}
void DataBaseManager::ExecuteTransaction(char sql[])
{
    char *zErrMsg = 0;
    int rc;
    const char* data = "Callback function called";
    selectedRows.clear();
    rc = sqlite3_exec(db, sql, selectCallback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
       } else {
          fprintf(stdout, "Records created successfully\n");
       }
}
void DataBaseManager::ExecuteSelectTransaction(){
    char *zErrMsg = 0;
    int rc;
    const char* data = "Callback function called";
    char sql[] ="SELECT * FROM ACCOUNT";
    selectedRows.clear();
    rc = sqlite3_exec(db, sql, selectCallback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
       } else {
          fprintf(stdout, "Records created successfully\n");
       }
}

void DataBaseManager::ExecuteInsertTransaction(std::string date,std::string label, std::string priceperunit ,std::string quantity,std::string price,std::string category)
{
    std::string str;
    //Get number of rows to calculate next id
    ExecuteSelectTransaction();
    int id = selectedRows.size()+1;
    std::string id_str;
    id_str = std::to_string(id);

    //Format the sql Query | 7 is the number of arguments
    str = Utils::format("INSERT INTO ACCOUNT (ID,DATE,LABEL,PRICEPERUNIT,QUANTITY,PRICE,CATEGORY) "  \
          "VALUES (#,#,#,#,#,#,#);",7,id_str.c_str(),date.c_str(),label.c_str(),priceperunit.c_str(),quantity.c_str(),price.c_str(),category.c_str());
    char sql[str.size()];
    for (int i = 0; i < sizeof(sql); i++) {
            sql[i] = str[i];
    }

    //Execute the query
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
       } else {
          fprintf(stdout, "Records created successfully\n");
       }
}
void DataBaseManager::CheckDataBaseState(int rc){
    if(rc){
        QMessageBox messageBox;
        std::string message;
        std::string errorMessage = sqlite3_errmsg(db);
        message = "An error has occured !\n" + errorMessage;
        messageBox.critical(nullptr,"Error", QString::fromStdString(message));
        messageBox.setFixedSize(500,200);
        messageBox.show();
    }else{

    }
}
void DataBaseManager::ExecuteDataBaseOperation(char sqlStatement[]){
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db, sqlStatement, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
       } else {
          fprintf(stdout, "Records created successfully\n");
       }
}
void DataBaseManager::OpenDataBaseConnection(){
    int rc;
    if (FILE *file = fopen(dataBaseFileName, "r")) {
            //Database already exists -> just open it
            fclose(file);
            rc = sqlite3_open(dataBaseFileName,&db);
            CheckDataBaseState(rc);
    } else {
        //Database created for the first time -> open it
            rc = sqlite3_open(dataBaseFileName,&db);
            CheckDataBaseState(rc);
            char sql[] = "CREATE TABLE ACCOUNT (ID INT PRIMARY KEY NOT NULL," \
                                               "DATE DATE NOT NULL," \
                                               "LABEL TEXT," \
                                               "PRICEPERUNIT FLOAT NOT NULL," \
                                               "QUANTITY INT NOT NULL," \
                                               "PRICE FLOAT NOT NULL," \
                                               "CATEGORY TEXT NOT NULL)";
            ExecuteDataBaseOperation(sql);
    }
}
