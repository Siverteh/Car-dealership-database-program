#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "sqlite.h"
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Car dealership");

    //Sets the theme of the application to dark mode from the QDarkStyle open source library.
    //Source: https://github.com/ColinDuquesnoy/QDarkStyleSheet
    QFile f(":qdarkstyle/dark/style.qss");

    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    w.show();

    //Connects to the database
    database_connection();

    //Creates the tables car, customer, and rental if they do not exist already
    create_table_car();
    create_table_customer();
    create_table_rental();

    // Makes sure the application starts in the main menu, and loads in the tables and combo boxes of the application.
    w.on_pushButton_list_cars_clicked();
    w.on_pushButton_list_customers_clicked();
    w.on_pushButton_car_rentals_clicked();
    w.set_combo_box_assign_car();
    w.set_combo_box_assign_customer();
    w.set_combo_box_unassign_car();
    w.start_in_main_menu();

    return a.exec();
}
