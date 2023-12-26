#ifndef SQLITE_H
#define SQLITE_H
#include <QtSql>
#include <QDebug>
#include <QApplication>

void database_connection();

void create_table_car();
void list_all_cars();
void add_car(QString reg_nr, QString brand, QString model, int reg_year, int mileage);
void edit_car(QString reg_nr, QString brand, QString model, int reg_year, int mileage);
void remove_car(QString reg_nr);
void search_car(QString searchword);

void create_table_customer();
void list_all_customers();
void add_customer(QString social_security_number, QString full_name, QString date_of_birth, QString email, int phone_number);
void edit_customer(QString customer_id, QString full_name, QString date_of_birth, QString email, int phone_number);
void remove_customer(QString customer_id);
void search_customer(QString searchword);

void create_table_rental();
void list_all_rentals();
void assign_car(QString social_security_number, QString reg_nr);
void unassign_car(QString reg_nr);

#endif // SQLITE_H
