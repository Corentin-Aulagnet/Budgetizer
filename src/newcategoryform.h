#ifndef NEWCATEGORYFORM_H
#define NEWCATEGORYFORM_H

#include <QWidget>
#include "mainwindow.h"
namespace Ui {
class NewCategoryForm;
}
/*!
 * \brief The NewCategoryForm class
 * \details Handle the window prompt to create a new category
 */
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
    /*!
     * \brief sendNewCategory
     * \details SLOT to send the newly created category to the mainwindow \n
     * linked to the ok button
     * \return QString, the string with the new category
     */
    QString sendNewCategory();
};

#endif // NEWCATEGORYFORM_H
