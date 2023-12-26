#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //All pushButton functions:
private slots:

    void on_pushButton_edit_car_clicked();

    void on_pushButton_add_car_clicked();

    void on_pushButton_remove_car_clicked();

    void on_pushButton_search_car_clicked();

    void on_pushButton_add_customer_clicked();

    void on_pushButton_edit_customer_clicked();

    void on_pushButton_remove_customer_clicked();

    void on_pushButton_search_customer_clicked();

    void on_pushButton_manage_cars_clicked();

    void on_pushButton_car_to_main_menu_clicked();

    void on_pushButton_manage_customers_clicked();

    void on_pushButton_customer_to_main_menu_clicked();

    void on_pushButton_manage_rentals_clicked();

    void on_pushButton_assign_car_clicked();

    void on_pushButton_rental_to_main_menu_clicked();

    void on_pushButton_unassign_car_clicked();

    void on_pushButton_search_car_rental_clicked();

    void on_pushButton_search_customer_rental_clicked();

    //Public slots so they can be used in the main function.
public slots:
    void on_pushButton_list_cars_clicked();

    void start_in_main_menu();

    void set_combo_box_assign_car();

    void set_combo_box_assign_customer();

    void set_combo_box_unassign_car();

    void on_pushButton_list_customers_clicked();

    void on_pushButton_car_rentals_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
