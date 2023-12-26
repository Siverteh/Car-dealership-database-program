#include "sqlite.h"
#include <QtSql>
#include <QString>

//Function for establishing a database connection.
void database_connection()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("Car_dealership");

    //The programs throws an error if it cannot connect to the database.
    if (!db.open())
        {
          qDebug()<<"Problem opening database";
        }
}

//Function for creating the car table.
void create_table_car()
{
    //Prepares a create table query that creates the car table with the attributes reg_nr, brand, model, reg_year, mileage, and availability.
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS car("
                  "reg_nr TEXT PRIMARY KEY, "
                  "brand TEXT NOT NULL, "
                  "model TEXT NOT NULL, "
                  "reg_year INTEGER NOT NULL,"
                  "mileage INTEGER NOT NULL,"
                  "availability VARCHAR(45) DEFAULT available);");

    //Executetes the query and creates the car table, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error creating table: "<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function for adding a car to the car table.
void add_car(QString reg_nr, QString brand, QString model, int reg_year, int mileage)
{
    //Prepares an insert query that inserts a row into the car table. The values inserted are the parameteters of the function.
    QSqlQuery query;
    query.prepare("INSERT INTO car(reg_nr, brand, model, reg_year, mileage)"
                  "VALUES(:reg_nr, :brand, :model, :reg_year, :mileage)");
    query.bindValue(":reg_nr", reg_nr);
    query.bindValue(":brand", brand);
    query.bindValue(":model", model);
    query.bindValue(":reg_year", reg_year);
    query.bindValue(":mileage", mileage);

    //Executetes the query and adds a car to the table, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error adding car"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function for editing an existing car from the car table.
void edit_car(QString reg_nr, QString brand, QString model, int reg_year, int mileage)
{
    //Prepares an update query that updates every attribute from a row in the car table, except for the registration number of the car being updated. The new values are the parameters.
    QSqlQuery query;
    query.prepare("UPDATE car SET "
                  "(brand, model, reg_year, mileage) = (:brand, :model, :reg_year, :mileage)"
                  "WHERE reg_nr = :reg_nr");
    query.bindValue(":brand", brand);
    query.bindValue(":model", model);
    query.bindValue(":reg_year", reg_year);
    query.bindValue(":mileage", mileage);
    query.bindValue(":reg_nr", reg_nr);

    //Executetes the query and edits a car from the table, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error editing car"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function that removes an existing car from the car table.
void remove_car(QString reg_nr)
{
    //Prepares a delete query that deletes a car from the car table. The car being deleted is the one with the same registration_number as the parameter.
    QSqlQuery query;
    query.prepare("DELETE FROM car WHERE reg_nr = :reg_nr");
    query.bindValue(":reg_nr", reg_nr);

    //Executetes the query and removes a car from the table, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error removing car"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function for creating the customer table.
void create_table_customer()
{
    //Prepares a create table query that creates the customer table with the attributes social_security_number, full_name, date_of_birth, email, and phone_number.
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS customer("
                  "social_security_number TEXT PRIMARY KEY, "
                  "full_name TEXT NOT NULL, "
                  "date_of_birth TEXT NOT NULL,"
                  "email TEXT NOT NULL, "
                  "phone_number INTEGER NOT NULL);");

    //Executetes the query and creates the customer table, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error creating table"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function for adding a customer to the customer table.
void add_customer(QString social_security_number, QString full_name, QString date_of_birth, QString email, int phone_number)
{
    //Prepares an insert query that inserts a row into the customer table. The values inserted are the parameteters of the function.
    QSqlQuery query;
    query.prepare("INSERT INTO customer(social_security_number, full_name, date_of_birth, email, phone_number)"
                  "VALUES(:social_security_number, :full_name, :date_of_birth, :email, :phone_number)");
    query.bindValue(":social_security_number", social_security_number);
    query.bindValue(":full_name", full_name);
    query.bindValue(":date_of_birth", date_of_birth);
    query.bindValue(":email", email);
    query.bindValue(":phone_number", phone_number);

    //Executetes the query and adds a customer to the table, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error adding customer"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function that edits an existing customer from the customer table.
void edit_customer(QString social_security_number, QString full_name, QString date_of_birth, QString email, int phone_number)
{
    //Prepares an update query that updates every attribute from a row in the customer table, except for the social_security_number of the customer being updated.
    QSqlQuery query;
    query.prepare("UPDATE customer SET "
                  "(full_name, date_of_birth, email, phone_number) = "
                  "(:full_name, :date_of_birth, :email, :phone_number)"
                  "WHERE social_security_number = :social_security_number");
    query.bindValue(":full_name", full_name);
    query.bindValue(":date_of_birth", date_of_birth);
    query.bindValue(":email", email);
    query.bindValue(":phone_number", phone_number);
    query.bindValue(":social_security_number", social_security_number);

    //Executetes the query and edits a customer from the table, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error editing customer"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function that removes an existing customer from the customer table.
void remove_customer(QString social_security_number)
{
    //Prepares a delete query that deletes a customer from the customer table. The car being deleted is the one with the same registration_number as the parameter.
    QSqlQuery query;
    query.prepare("DELETE FROM customer WHERE social_security_number = :social_security_number");
    query.bindValue(":social_security_number", social_security_number);

    //Executetes the query and removes a customer from the customer table, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error removing customer"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function for creating the customer table.
void create_table_rental()
{
    //Prepares a create table query that creates the rental table with the attributes rental_id, social_security_number, and reg_nr.
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS rental ("
                  "rental_id INTEGER PRIMARY KEY, "
                  "social_security_number TEXT NOT NULL, "
                  "reg_nr TEXT NOT NULL, "
                  "FOREIGN KEY(social_security_number) REFERENCES customer(social_security_number),"
                  "FOREIGN KEY(reg_nr) REFERENCES car(reg_nr));");

    //Executetes the query and creates the rental table, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error creating table"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function for assigning a car to a customer.
void assign_car(QString social_security_number, QString reg_nr)
{
    //Prepares an insert query that inserts a row into the rental table. The values inserted are the parameteters of the function, and the rental id auto increments itself.
    QSqlQuery query;
    query.prepare("INSERT INTO rental(social_security_number, reg_nr) VALUES(:social_security_number, :reg_nr)");
    query.bindValue(":social_security_number", social_security_number);
    query.bindValue(":reg_nr", reg_nr);

    //Executetes the query and assigns the chosen car to the chosen customer, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error adding car"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}

//Function for unassigning a car from a customer.
void unassign_car(QString reg_nr)
{
    //Prepares a delete query that deletes a rental agreement from the rental table. In only needs to know the registration number being rented to unassign the car.
    QSqlQuery query;
    query.prepare("DELETE FROM rental WHERE reg_nr = :reg_nr");
    query.bindValue(":reg_nr", reg_nr);

    //Executetes the query and unassigns the chosen car, if this fails it prints out an error.
    if(!query.exec())
    {
        qDebug()<<"Error unassigning car"<<query.lastError().text()<<query.lastError().nativeErrorCode();
    }
}
