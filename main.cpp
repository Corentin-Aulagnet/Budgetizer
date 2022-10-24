#include "mainwindow.h"
#include "databasemanager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <string>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Budgetizer_dev_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    std::string dataBaseFilename = "MyTestDataBase.db";
    DataBaseManager dataManager(dataBaseFilename);
    MainWindow w(&dataManager);
    w.show();
    return a.exec();
}
