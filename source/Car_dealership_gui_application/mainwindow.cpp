#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sqlite.h"
#include <thread>
#include <chrono>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Function that makes the program start in the main menu.
void MainWindow::start_in_main_menu()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//Fills the combo box for assigning cars.
void MainWindow::set_combo_box_assign_car()
{
    //Clears the combo box every time to avoid duplicate entries.
    ui->comboBox_assign_car->clear();

    //Prepares a select statement that gets every car that has the status available.
    QSqlQuery query;
    query.prepare("SELECT * FROM car WHERE availability = 'available'");
    query.exec();

    //Adds an empty entry at the top of the combo box.
    QStringList list=(QStringList()<<"");
    ui->comboBox_assign_car->addItems(list);

    //While loop that adds the registration number of every available car into the combo box.
    while (query.next())
        {
        QStringList list=(QStringList()<<query.value("reg_nr").toString());
        ui->comboBox_assign_car->addItems(list);
        }
}

//Fills the combo box for assigning customers
void MainWindow::set_combo_box_assign_customer()
{
    //Clears the combo box every time to avoid duplicate entries.
    ui->comboBox_assign_customer->clear();

    //Prepares a select statement that gets every customer in the system.
    QSqlQuery query;
    query.prepare("SELECT * FROM customer");
    query.exec();

    //Adds an empty entry at the top of the combo box.
    QStringList list=(QStringList()<<"");
    ui->comboBox_assign_customer->addItems(list);

    //While loop that adds the social security number of every available customer into the combo box.
    while (query.next())
        {
        QStringList list=(QStringList()<<query.value("social_security_number").toString());
        ui->comboBox_assign_customer->addItems(list);
        }
}

//Fills the combo box for unassigning cars
void MainWindow::set_combo_box_unassign_car()
{
    //Clears the combo box every time to avoid duplicate entries.
    ui->comboBox_unassign_car->clear();

    //Prepares a select statement that gets every car that is currently being rented.
    QSqlQuery query;
    query.prepare("SELECT * FROM car WHERE availability = 'unavailable'");
    query.exec();

    //Adds an empty entry at the top of the combo box.
    QStringList list=(QStringList()<<"");
    ui->comboBox_unassign_car->addItems(list);

    //While loop that adds the registration number of every available car into the combo box.
    while (query.next())
        {
        QStringList list=(QStringList()<<query.value("reg_nr").toString());
        ui->comboBox_unassign_car->addItems(list);
        }
}

//Goes from the main menu to the manage cars page
void MainWindow::on_pushButton_manage_cars_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

//Goes from the manage cars page to the main menu
void MainWindow::on_pushButton_car_to_main_menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//Goes from the main menu to the manage customers page
void MainWindow::on_pushButton_manage_customers_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//Goes from the manage customers page to the main menu
void MainWindow::on_pushButton_customer_to_main_menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//Goes from the main menu to the manage rentals page
void MainWindow::on_pushButton_manage_rentals_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

//Goes from the manage rentals page to the main menu
void MainWindow::on_pushButton_rental_to_main_menu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//Lists all the cars in the car table visible to the user.
void MainWindow::on_pushButton_list_cars_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM car");
    query.exec();
    model->setQuery(std::move(query));
    ui->tableView_car->setModel(model);
    ui->tableView_car->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//Adds a car to the car table.
void MainWindow::on_pushButton_add_car_clicked()
{
    //Gets the different line edit entries and stores them as variables.
    QString reg_nr = ui->lineEdit_reg_nr->text();
    QString brand = ui->lineEdit_brand->text();
    QString model = ui->lineEdit_model->text();
    int reg_year = ui->lineEdit_reg_year->text().toInt();
    int mileage = ui->lineEdit_mileage->text().toInt();

    //Prepares a select statement that gets every car currently in the system.
    QSqlQuery query;
    query.prepare("SELECT * FROM car");
    query.exec();

    //While loops that checks if the entered registration number already is in the system.
    while(query.next())
        {
        if(reg_nr == query.value("reg_nr").toString())
            {
            //If the car is already in the system an error will occur and all linedit boxes will be cleared.
            ui->progressBar_car->setFormat("Error: car already registered");
            ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
            ui->progressBar_car->setValue(100);
            std::this_thread::sleep_for (std::chrono::seconds(2));
            ui->progressBar_car->setFormat("");
            ui->progressBar_car->setValue(0);
            ui->lineEdit_reg_nr->setText("");
            ui->lineEdit_brand->setText("");
            ui->lineEdit_model->setText("");
            ui->lineEdit_reg_year->setText("");
            ui->lineEdit_mileage->setText("");
            return;
            }
        }

    //If statement that checks if any of the lineedits were not filled in.
    if(reg_nr == "" or brand == "" or model == ""
            or reg_year == 0 or mileage == 0)
    {
        //Throws an error if all lineedits are not filled.
        ui->progressBar_car->setFormat("Error: please fill all the required fields");
        ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_car->setValue(100);
    }
    else
    {
        //Else the entried information is approved and the car is entered into the table.
        ui->progressBar_car->setFormat("Car successfully added");
        ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}");
        ui->progressBar_car->setValue(100);

        //Calls the add car function, and updates the car table visible to the user.
        add_car(reg_nr, brand, model, reg_year, mileage);
        on_pushButton_list_cars_clicked();
        QStringList list=(QStringList()<<reg_nr);
        ui->comboBox_assign_car->addItems(list);
    }

    //Clears all lineedits after two seconds.
    std::this_thread::sleep_for (std::chrono::seconds(2));
    ui->progressBar_car->setFormat("");
    ui->progressBar_car->setValue(0);
    ui->lineEdit_reg_nr->setText("");
    ui->lineEdit_brand->setText("");
    ui->lineEdit_model->setText("");
    ui->lineEdit_reg_year->setText("");
    ui->lineEdit_mileage->setText("");

    //Updates the combo box in the rental window.
    set_combo_box_assign_car();
}

//Edits a car in the car table.
void MainWindow::on_pushButton_edit_car_clicked()
{
    //Gets the different line edit entries and stores them as variables.
    QString reg_nr = ui->lineEdit_reg_nr->text();
    QString brand = ui->lineEdit_brand->text();
    QString model = ui->lineEdit_model->text();
    int reg_year = ui->lineEdit_reg_year->text().toInt();
    int mileage = ui->lineEdit_mileage->text().toInt();

    //Prepares a select statement that gets all cars in the system,
    QSqlQuery query;
    query.prepare("SELECT * FROM car");
    query.exec();

    //While loop that checks if the entered registration number is in the table.
    bool in_table = false;
    while(query.next())
        {
        if(reg_nr == query.value("reg_nr").toString())
            {
                in_table = true;
            }
        }

    //Checks if all the required line edits are filled in.
    if(reg_nr == "" or brand == "" or model == ""
            or reg_year == 0 or mileage == 0)
    {
        ui->progressBar_car->setFormat("Error: please fill all the required fields");
        ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_car->setValue(100);
    }

    //Checks if the registration number is in the car table.
    else if(!in_table)
    {
        ui->progressBar_car->setFormat("Error: car not registered");
        ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_car->setValue(100);
    }

    //Else the entried information is approved and the car is successfully edited..
    else
    {
        ui->progressBar_car->setFormat("Car successfully edited");
        ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}");
        ui->progressBar_car->setValue(100);

        //Calls the edit car function and updates the car table visible to the user.
        edit_car(reg_nr, brand, model, reg_year, mileage);
        on_pushButton_list_cars_clicked();
    }

    //Clears all the line edits after two seconds.
    std::this_thread::sleep_for (std::chrono::seconds(2));
    ui->progressBar_car->setFormat("");
    ui->progressBar_car->setValue(0);
    ui->lineEdit_reg_nr->setText("");
    ui->lineEdit_brand->setText("");
    ui->lineEdit_model->setText("");
    ui->lineEdit_reg_year->setText("");
    ui->lineEdit_mileage->setText("");
}

//Removes a car from the car table.
void MainWindow::on_pushButton_remove_car_clicked()
{
    //Gets the different line edit entries and stores them as variables.
    QString reg_nr = ui->lineEdit_reg_nr->text();

    //Prepares a select statement that gets all the cars in the car table.
    QSqlQuery query;
    query.prepare("SELECT * FROM car");
    query.exec();

    //While loop that checks if the entered registration number is in the car table.
    bool is_registered = false;
    while(query.next())
        {
        if(reg_nr == query.value("reg_nr").toString())
            {
                //Checks if the registration number entered is currently for rent. If it is the program throws an error. Cannot remove a car from the system that is currently for rent.
                if(query.value("availability").toString() == "unavailable")
                {
                    ui->progressBar_car->setFormat("Error: cannot remove a rented car");
                    ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
                    ui->progressBar_car->setValue(100);

                    //Clears all the line edits after two seconds and returns.
                    std::this_thread::sleep_for (std::chrono::seconds(2));
                    ui->progressBar_car->setFormat("");
                    ui->progressBar_car->setValue(0);
                    ui->lineEdit_customer_id->setText("");
                    ui->lineEdit_full_name->setText("");
                    ui->lineEdit_date_of_birth->setText("");
                    ui->lineEdit_email->setText("");
                    ui->lineEdit_phone_number->setText("");
                    return;
                }
                else
                {
                is_registered = true;
                }
            }
        }

    //Checks if the user has entered a registration number.
    if(reg_nr == "")
    {
        ui->progressBar_car->setFormat("Error: please enter a registration number");
        ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_car->setValue(100);
    }

    //Checks if the entered registration number is currently in the system.
    else if(!is_registered)
    {
        ui->progressBar_car->setFormat("Error: car not registered");
        ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_car->setValue(100);
    }

    //Else the entried information is approved and the car is removed from the system.
    else
    {
        ui->progressBar_car->setFormat("Car successfully removed");
        ui->progressBar_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}");
        ui->progressBar_car->setValue(100);

        //Calls the remove car function, and updates the car table visible to the user.
        remove_car(reg_nr);
        on_pushButton_list_cars_clicked();
    }

    //Clears all the line edits after two seconds.
    std::this_thread::sleep_for (std::chrono::seconds(2));
    ui->progressBar_car->setFormat("");
    ui->progressBar_car->setValue(0);
    ui->lineEdit_reg_nr->setText("");
    ui->lineEdit_brand->setText("");
    ui->lineEdit_model->setText("");
    ui->lineEdit_reg_year->setText("");
    ui->lineEdit_mileage->setText("");

    //Updates the assign car combo box in the rental window.
    set_combo_box_assign_car();
}

//Searches for a car in the car table.
void MainWindow::on_pushButton_search_car_clicked()
{
    //Gets the search bar line edit entry and stores it in a variable.
    QString search_word = ui->lineEdit_car_search_bar->text();

    //If the the user has entered a search word it will search the car table for it and update the car table visible to the user.
    if(search_word != "")
    {
    QSqlQueryModel *model = new QSqlQueryModel();

    //Prepares a select statement that gets all the cars that has a registration number, brand, model, or registration year similar to the entered search word.
    QSqlQuery query;
    query.prepare("SELECT * FROM car WHERE (reg_nr LIKE :search_word) or (brand LIKE :search_word) or (model LIKE :search_word) or (reg_year LIKE :search_word)");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.exec();

    //Updates the car table visible to the user.
    model->setQuery(std::move(query));
    ui->tableView_car->setModel(model);
    ui->tableView_car->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }

    //Clears the search line edit after one second.
    std::this_thread::sleep_for (std::chrono::seconds(1));
    ui->lineEdit_car_search_bar->setText("");
}

//Lists all the customers in the customer table visible to the user.
void MainWindow::on_pushButton_list_customers_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM customer");
    query.exec();
    model->setQuery(std::move(query));
    ui->tableView_customer->setModel(model);
    ui->tableView_customer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//Adds a customer to the customer table.
void MainWindow::on_pushButton_add_customer_clicked()
{
    //Gets the different line edit entries and stores them as variables.
    QString social_security_number = ui->lineEdit_customer_id->text();
    QString full_name = ui->lineEdit_full_name->text();
    QString date_of_birth = ui->lineEdit_date_of_birth->text();
    QString email = ui->lineEdit_email->text();
    int phone_number = ui->lineEdit_phone_number->text().toInt();

    //For loop that cheks every index of the social security number entered, if any of it is anything else than a number the bool is_number is set to false.
    bool is_number = true;
    std::string social_security_number_is_letter_test = social_security_number.toStdString();
    for(char const &c : social_security_number_is_letter_test)
    {
        if(std::isdigit(c) == 0)
        {
            is_number = false;
            break;
        }
    }

    //Prepares a query that gets every customer from the customer table.
    QSqlQuery query;
    query.prepare("SELECT * FROM customer");
    query.exec();

    //While loop that checks if the social security numbered entered is already in the system.
    while(query.next())
        {
        if(social_security_number == query.value("social_security_number"))
            {
            //If the social security number entered is already in the customer table the program throws an error and returns.
            ui->progressBar_customer->setFormat("Error: customer already registered");
            ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
            ui->progressBar_customer->setValue(100);

            //Clears all the line edits after two seconds.
            std::this_thread::sleep_for (std::chrono::seconds(2));
            ui->progressBar_customer->setFormat("");
            ui->progressBar_customer->setValue(0);
            ui->lineEdit_customer_id->setText("");
            ui->lineEdit_full_name->setText("");
            ui->lineEdit_date_of_birth->setText("");
            ui->lineEdit_email->setText("");
            ui->lineEdit_phone_number->setText("");
            return;
            }
        }

    //If any of the line edits required are not filled the system throws an error and returns.
    if(social_security_number == "" or full_name == "" or date_of_birth == ""
            or email == "" or phone_number == 0)
    {
        ui->progressBar_customer->setFormat("Error: please fill all the required fields");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_customer->setValue(100);
    }

    //If the length of the social security is not 11 the system throws an error and returns.
    else if(social_security_number.length() != 11)
    {
        ui->progressBar_customer->setFormat("Please enter a valid social security number");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_customer->setValue(100);
    }

    //If the social security number contains anything other than numbers the system throws an error and returns.
    else if (!is_number)
    {
        ui->progressBar_customer->setFormat("Please enter a valid social security number");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_customer->setValue(100);
    }

    //Else the entried information is approved and the customer is added to the system.
    else
    {
        ui->progressBar_customer->setFormat("Customer successfully added");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}");
        ui->progressBar_customer->setValue(100);

        //Calls the add customer function and updates the customer table visible to the user.
        add_customer(social_security_number, full_name, date_of_birth, email, phone_number);
        on_pushButton_list_customers_clicked();

        //Updates the assign customer combo box in the rental window with the new customer.
        QStringList list=(QStringList()<<social_security_number);
        ui->comboBox_assign_customer->addItems(list);
        set_combo_box_assign_customer();
    }

    //Clears all the line edits after two seconds.
    std::this_thread::sleep_for (std::chrono::seconds(2));
    ui->progressBar_customer->setFormat("");
    ui->progressBar_customer->setValue(0);
    ui->lineEdit_customer_id->setText("");
    ui->lineEdit_full_name->setText("");
    ui->lineEdit_date_of_birth->setText("");
    ui->lineEdit_email->setText("");
    ui->lineEdit_phone_number->setText("");
}

//Edits a customer in the customer table.
void MainWindow::on_pushButton_edit_customer_clicked()
{
    //Gets the different line edit entries and stores them as variables.
    QString social_security_number = ui->lineEdit_customer_id->text();
    QString full_name = ui->lineEdit_full_name->text();
    QString date_of_birth = ui->lineEdit_date_of_birth->text();
    QString email = ui->lineEdit_email->text();
    int phone_number = ui->lineEdit_phone_number->text().toInt();

    //Prepares a select statement that gets all the customers from the customer table.
    QSqlQuery query;
    query.prepare("SELECT * FROM customer");
    query.exec();

    //While loop that checks if the social security number entered is in the table, and if it is sets the bool in_table to true.
    bool in_table = false;
    while(query.next())
        {
        if(social_security_number == query.value("social_security_number"))
            {
                in_table = true;
            }
        }

    //If all the required line edits are not filled, throws an error and returns.
    if(social_security_number == "" or full_name == "" or date_of_birth == ""
            or email == "" or phone_number == 0)
    {
        ui->progressBar_customer->setFormat("Error: please fill all the required fields");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_customer->setValue(100);
    }

    //If the entered social security number is not in the table the program throws an error and returns.
    else if(!in_table)
    {
        ui->progressBar_customer->setFormat("Error: customer not registered");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_customer->setValue(100);
    }

    //Else the entried information is approved and the customer is successfully edited.
    else
    {
        ui->progressBar_customer->setFormat("Customer successfully edited");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}");
        ui->progressBar_customer->setValue(100);

        //Calls the edit customer function and updates the customer table visible to the user.
        edit_customer(social_security_number, full_name, date_of_birth, email, phone_number);
        on_pushButton_list_customers_clicked();
    }

    //Clears all the line edits after two seconds.
    std::this_thread::sleep_for (std::chrono::seconds(2));
    ui->progressBar_customer->setFormat("");
    ui->progressBar_customer->setValue(0);
    ui->lineEdit_customer_id->setText("");
    ui->lineEdit_full_name->setText("");
    ui->lineEdit_date_of_birth->setText("");
    ui->lineEdit_email->setText("");
    ui->lineEdit_phone_number->setText("");
}

//Removes a customer from the customer table.
void MainWindow::on_pushButton_remove_customer_clicked()
{
    //Gets the social security line edit entry and stores it as a variable.
    QString social_security_number = ui->lineEdit_customer_id->text();

    //Prepares a select statement that gets all the rental agreements from the rental table.
    QSqlQuery query;
    query.prepare("SELECT * FROM rental");
    query.exec();

    //While loop that checks if the entered social security number has any active rental agreements. If the customer has any active rental he/she cannot be removed and the program throws an error.
    while(query.next())
        {
        if(social_security_number == query.value("social_security_number"))
            {
            ui->progressBar_customer->setFormat("Cant remove customer with active rental");
            ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
            ui->progressBar_customer->setValue(100);

            //Clears all the line edits after two seconds and returns.
            std::this_thread::sleep_for (std::chrono::seconds(2));
            ui->progressBar_customer->setFormat("");
            ui->progressBar_customer->setValue(0);
            ui->lineEdit_customer_id->setText("");
            ui->lineEdit_full_name->setText("");
            ui->lineEdit_date_of_birth->setText("");
            ui->lineEdit_email->setText("");
            ui->lineEdit_phone_number->setText("");
            return;
            }
        }

    //Prepares a select statement that gets all the customers from the customer table.
    query.prepare("SELECT * FROM customer");
    query.exec();

    //While loop that checks if the entered social security number already is in the system. If it is the bool is_registered is set to true.
    bool is_registered = false;
    while(query.next())
        {
        if(social_security_number == query.value("social_security_number"))
            {
                is_registered = true;
            }
        }

    //If the social security line edit is empty the system throws an error and returns.
    if(social_security_number == "")
    {
        ui->progressBar_customer->setFormat("Please enter a social security number");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_customer->setValue(100);
    }

    //If the social security number entered is not in the system the system throws an error and returns.
    else if(!is_registered)
    {
        ui->progressBar_customer->setFormat("Customer not registered");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_customer->setValue(100);
    }
    //Else the entried information is approved and the customer is successfully removed.
    else
    {
        ui->progressBar_customer->setFormat("Customer successfully removed");
        ui->progressBar_customer->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}");
        ui->progressBar_customer->setValue(100);

        //Calls the remove customer function and updates the customer table visible to the user, and updates the assign customer combo box in the rental window.
        remove_customer(social_security_number);
        on_pushButton_list_customers_clicked();
        set_combo_box_assign_customer();
    }

    //Clears all the line edits after two seconds.
    std::this_thread::sleep_for (std::chrono::seconds(2));
    ui->progressBar_customer->setFormat("");
    ui->progressBar_customer->setValue(0);
    ui->lineEdit_customer_id->setText("");
    ui->lineEdit_full_name->setText("");
    ui->lineEdit_date_of_birth->setText("");
    ui->lineEdit_email->setText("");
    ui->lineEdit_phone_number->setText("");
}

//Searches for a customer in the customer table.
void MainWindow::on_pushButton_search_customer_clicked()
{
    //Gets the search word line edit entry and stores it as a variable.
    QString search_word = ui->lineEdit_customer_search_bar->text();

    QSqlQueryModel *model = new QSqlQueryModel();

    //Prepares a select statement that returns the customer with a full name similar to the search word.
    QSqlQuery query;
    query.prepare("SELECT * FROM customer WHERE (social_security_number LIKE :search_word) or (full_name LIKE :search_word)");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.exec();

    //Updates the customer table visible to the user with only the customers similar to the search word.
    model->setQuery(std::move(query));
    ui->tableView_customer->setModel(model);
    ui->tableView_customer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Clears the customer search bar after one second.
    std::this_thread::sleep_for (std::chrono::seconds(1));
    ui->lineEdit_customer_search_bar->setText("");
}

//Lists all the rentals in the rental table visible to the user.
void MainWindow::on_pushButton_car_rentals_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM rental");
    query.exec();
    model->setQuery(std::move(query));
    ui->tableView_car_rental->setModel(model);
    ui->tableView_car_rental->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//Assigns a car to a customer.
void MainWindow::on_pushButton_assign_car_clicked()
{
    //Gets the different combo box entries and stores them as variables.
    QString reg_nr = ui->comboBox_assign_car->currentText();
    QString social_security_number = ui->comboBox_assign_customer->currentText();

    //Checks that the combo boxes has a value selected.
    if(reg_nr == "" or social_security_number == "")
    {
        //Throws an error if the user has not selected a value with the combo boxes.
        ui->progressBar_assign_car->setFormat("Please select a car and a customer");
        ui->progressBar_assign_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_assign_car->setValue(100);

        //Clears the progress bar after two seconds and returns.
        std::this_thread::sleep_for (std::chrono::seconds(2));
        ui->progressBar_assign_car->setFormat("");
        ui->progressBar_assign_car->setValue(0);
        return;
    }

    //Calls the assign car function.
    assign_car(social_security_number, reg_nr);
    ui->progressBar_assign_car->setFormat("Car successfully assigned");
    ui->progressBar_assign_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}");
    ui->progressBar_assign_car->setValue(100);

    //Prepares a select statement that gets all the cars from the car table that are not currently being rented.
    QSqlQuery query;
    query.prepare("SELECT * FROM car WHERE availability = 'available'");
    query.exec();

    //While loop that updates the assign car combo box by removing the car that has been rented.
    int index = 1;
    while (query.next())
        {
        if(reg_nr == query.value("reg_nr").toString())
            {
                ui->comboBox_assign_car->removeItem(index);
                ui->comboBox_assign_customer->setCurrentIndex(0);
            }
        index++;
        }

    //Prepares an update query that sets the availability of the car rented to unavailable.
    query.prepare("UPDATE car SET availability = 'unavailable' WHERE reg_nr = :reg_nr");
    query.bindValue(":reg_nr", reg_nr);
    query.exec();

    //Clears the progress bar after two seconds.
    std::this_thread::sleep_for (std::chrono::seconds(2));
    ui->progressBar_assign_car->setFormat("");
    ui->progressBar_assign_car->setValue(0);

    //Updates the car, and rental table visible to the user.
    on_pushButton_list_cars_clicked();
    on_pushButton_car_rentals_clicked();

    //updates the assign car and unassign car combo boxes.
    set_combo_box_assign_car();
    set_combo_box_unassign_car();
}

//Unassigns a car
void MainWindow::on_pushButton_unassign_car_clicked()
{
    //Gets the different combo box entries and stores them as variables.
    QString reg_nr = ui->comboBox_unassign_car->currentText();

    //Checks that the user has selected a registration number to unassign.
    if(reg_nr == "")
    {
        //If the user has not selected a value the program throws and error and returns.
        ui->progressBar_unassign_car->setFormat("Please select a car");
        ui->progressBar_unassign_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}");
        ui->progressBar_unassign_car->setValue(100);
        std::this_thread::sleep_for (std::chrono::seconds(2));
        ui->progressBar_unassign_car->setFormat("");
        ui->progressBar_unassign_car->setValue(0);
        return;
    }

    //Calls the unassign car function.
    unassign_car(reg_nr);
    ui->progressBar_unassign_car->setFormat("Car successfully unassigned");
    ui->progressBar_unassign_car->setStyleSheet("QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}");
    ui->progressBar_unassign_car->setValue(100);

    //Prepares a select statement that gets all the cars currently being rented.
    QSqlQuery query;
    query.prepare("SELECT * FROM car WHERE availability = 'unavailable'");
    query.exec();

    //For loop that removes the car unassigned from the unassign car combo box.
    int index = 1;
    while (query.next())
        {
        if(reg_nr == query.value("reg_nr").toString())
            {
                ui->comboBox_unassign_car->removeItem(index);
            }
        index++;
        }

    //Prepares an update query that sets the unassigned car to available.
    query.prepare("UPDATE car SET availability = 'available' WHERE reg_nr = :reg_nr");
    query.bindValue(":reg_nr", reg_nr);
    query.exec();

    //Clears the progress bar after two seconds.
    std::this_thread::sleep_for (std::chrono::seconds(2));
    ui->progressBar_unassign_car->setFormat("");
    ui->progressBar_unassign_car->setValue(0);

    //Updates the car and rental tables visible to the user.
    on_pushButton_list_cars_clicked();
    on_pushButton_car_rentals_clicked();

    //Updates the assign car, and unassign car combo boxes.
    set_combo_box_assign_car();
    set_combo_box_unassign_car();

}

//Searches for a car in the rental page.
void MainWindow::on_pushButton_search_car_rental_clicked()
{
    //Gets the search word line edit entry and stores it as a variable.
    QString search_word = ui->lineEdit_search_car_rental->text();

    //Checks that the user has entered a search word.
    if(search_word != "")
    {
    QSqlQueryModel *model = new QSqlQueryModel();

    //Prepares a select statement that gets all the cars with the registration number, brand, model, or registration year similar to the search word.
    QSqlQuery query;
    query.prepare("SELECT * FROM car WHERE (reg_nr LIKE :search_word) or (brand LIKE :search_word) or (model LIKE :search_word) or (reg_year LIKE :search_word)");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.exec();

    //Updates the rental table visible to the user by listing all the cars similar to the search word.
    model->setQuery(std::move(query));
    ui->tableView_car_rental->setModel(model);
    ui->tableView_car_rental->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
}

//Searches for a customer in the rental page.
void MainWindow::on_pushButton_search_customer_rental_clicked()
{
    //Gets the search word line edit entry and stores it as a variable.
    QString search_word = ui->lineEdit_search_customer_rental->text();

    QSqlQueryModel *model = new QSqlQueryModel();

    //Prepares a select statement that gets all the
    QSqlQuery query;
    query.prepare("SELECT * FROM customer WHERE (social_security_number LIKE :search_word) or(full_name LIKE :search_word)");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.bindValue(":search_word", "%" + search_word + "%");
    query.exec();

    //Updates the table visible to the user with the customers similar to the search word.
    model->setQuery(std::move(query));
    ui->tableView_car_rental->setModel(model);
    ui->tableView_car_rental->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

