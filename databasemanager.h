#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "qcoreapplication.h"
#include <string>
#include <list>
#include <QString>
#include <QStringList>
#include <QPointF>
#include <sqlite3/sqlite3.h>
#include <QDate>

namespace DataTypes{
    typedef QPair<QPair<QDate,float>,QString> datedCategorizedDataPoint;
    typedef QPair<QString, float> sumedCategorizedData;
    typedef QList<sumedCategorizedData> SummedCategorizedDataList;
    //typedef QList<sumedCategorizedDataList> fDataTable;

    typedef QList<QStringList> DataTable;
}

class DataBaseManager
{
    Q_DECLARE_TR_FUNCTIONS(DataBaseManager)
    /*
     * Singleton methods and attributes
     */
public:
    void LoadSavedCategories(QStringList &categories);
    void SaveCategories(QStringList &categories);
public:
    DataBaseManager(char* _dataBaseFileName);
    DataBaseManager(std::string _dataBaseFileName);
    ~DataBaseManager();
    void Save();
    void Load();
    void ExecuteSelectTransaction();
    void ExecuteTransaction(char sql[]);
    void ExecuteInsertTransaction(std::string date,std::string label, std::string priceperunit ,std::string quantity,std::string price,std::string category);
    static DataTypes::DataTable GetSelectedRows(){
        return selectedRows;
    }
    static char* dataBaseFileName;
private:
    static DataTypes::DataTable selectedRows;
    void CheckDataBaseState(int rc);
    void ExecuteDataBaseOperation(char sqlStatement[]);
    void OpenDataBaseConnection();
    static int selectCallback(void *data, int colNumber, char **colValues, char **colNames){
       int i;
       fprintf(stderr, "%s: ", (const char*)data);
       QStringList strList;
       for(i = 0; i<colNumber; i++){
          //printf("%s = %s\n", colNames[i], colValues[i] ? colValues[i] : "NULL");

          std::string str = colValues[i];
          strList.push_back(QString::fromStdString(str));

       }
       selectedRows.push_back(strList);
       printf("\n");

       return 0;
    }

    //Reference to the database
    sqlite3 *db;
};

#endif // DATABASEMANAGER_H
