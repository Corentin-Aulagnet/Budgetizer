#include "newcategoryform.h"
#include "ui_newcategoryform.h"
#include "mainwindow.h"
NewCategoryForm::NewCategoryForm(MainWindow *creator, QWidget *_parent) :
    QWidget(_parent),
    ui(new Ui::NewCategoryForm)
{
    parent = creator;
    ui->setupUi(this);
}

NewCategoryForm::~NewCategoryForm()
{
    delete ui;
}

QString NewCategoryForm::sendNewCategory(){
    QString catName = ui->categoryName->text();
    parent->addNewCategory(catName);
    this->close();
    return catName;
}
