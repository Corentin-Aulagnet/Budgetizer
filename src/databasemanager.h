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
/*!
 *  \brief DataTypes, custom types to store processed or to be processed data
 */
namespace DataTypes{
    /*!
     * \brief datedCategorizedDataPoint
     * \details store a data point [(data,value),category]
     */
    typedef QPair<QPair<QDate,float>,QString> DatedCategorizedDataPoint;
/*!
     * \brief sumedCategorizedData
     * \details store a data point (category, sum of values from a time period)
     */
    typedef QPair<QString, float> SummedCategorizedData;
/*!
     * \brief SummedCategorizedDataList
     * \details store a list a data point
     */
    typedef QList<SummedCategorizedData> SummedCategorizedDataList;
/*!
     * \brief DataTable
     * \details store extracted data from the data base. Usual format will be CATEGORY1 | CATEGORY2 | ...\n
     *                                                                        value1    | value2    | ...\n
     *                                                                        ...
     */
    typedef QList<QStringList> DataTable;
}
/*!
 * \brief The DataBaseManager class
 * \details This class is used to perform request to the database
 */
class DataBaseManager
{
    Q_DECLARE_TR_FUNCTIONS(DataBaseManager)
    /*
     * Singleton methods and attributes
     */
public:
    /*!
     * \brief LoadSavedCategories
     * \param categories, reference QStringList of the categories to load
     * \details Load all the categories from the save file and load them to the dropdown menu
     */
    void LoadSavedCategories(QStringList &categories);
    /*!
     * \brief SaveCategories
     * \param categories, reference QStringList of the categories to save
     * \details Save all the categories from the dropdown menu
     */
    void SaveCategories(QStringList &categories);
public:
    DataBaseManager(char* _dataBaseFileName);
    DataBaseManager(std::string _dataBaseFileName);
    ~DataBaseManager();
    /*!
     * \brief Save, saves all important data
     * \param categoriesToSave, QStringList of the categories to save
     */
    void Save(QStringList categoriesToSave);
    /*!
     * \brief Load, loads all important data
     * \details not used, nor implemented
     */
    void Load();
    /*!
     * \brief ExecuteSelectTransaction
     * \details Execute a SELECT * Transaction on the database
     *
     */
    void ExecuteSelectTransaction();
    /*!
     * \brief ExecuteTransaction
     * \details Execute the transaction written in SQL passed in parameter
     * \param sql
     * \todo Make a function per type of transaction, with overload on multiple parameters
     */
    void ExecuteTransaction(char sql[]);
    /*!
     * \brief ExecuteInsertTransaction
     * \details Execute an INSERT transaction with the parameters passed in arguments
     * \param date, std::string
     * \param label, std::string
     * \param priceperunit, std::string, it's the unit price of the object to write in the database
     * \param quantity, std::string
     * \param price, std::string
     * \param category, std::string
     */
    void ExecuteInsertTransaction(std::string date,std::string label, std::string priceperunit ,std::string quantity,std::string price,std::string category);
    /*!
     * \brief GetSelectedRows
     * \return returns DataTypes::DataTable, the rows selected in the last SELECT Transaction
     */
    static DataTypes::DataTable GetSelectedRows(){
        return selectedRows;
    }
    /*!
     * \brief dataBaseFileName, the name of the file containing the database
     */
    static char* dataBaseFileName;
private:
    /*!
     * \brief selectedRows, the DataTypes::DataTable containing the rows from the last SELECT Transaction
     */
    static DataTypes::DataTable selectedRows;
    /*!
     * \brief CheckDataBaseState
     * \param rc, the value returned when opening the database
     * \details check the return value when opening the database
     */
    void CheckDataBaseState(int rc);
    /*!
     * \brief OpenDataBaseConnection
     * \details Opens a connection with the database, if no file is found, creates a new empty database
     */
    void OpenDataBaseConnection();
    /*!
     * \brief selectCallback
     * \details Callback function for all SELECT statement
     * \param data
     * \param colNumber, number of columns extracted
     * \param colValues, values of the columns extracted
     * \param colNames, names of the columns extracted
     * \return just returns an int, not very useful
     */
    static int selectCallback(void *data, int colNumber, char **colValues, char **colNames){
       int i;
       fprintf(stderr, "%s: ", (const char*)data);
       QStringList strList;
       for(i = 0; i<colNumber; i++){
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
