#ifndef NEWCATEGORYFORM_H
#define NEWCATEGORYFORM_H

#include <QWidget>
#include "mainwindow.h"
namespace Ui {
class NewCategoryForm;
}

class NewCategoryForm : public QWidget
{

    Q_OBJECT

public:
    explicit NewCategoryForm(MainWindow *creator, QWidget *parent = nullptr);
    ~NewCategoryForm();

private:
    Ui::NewCategoryForm *ui;
    MainWindow *parent;

private slots:
    QString sendNewCategory();
};

#endif // NEWCATEGORYFORM_H
