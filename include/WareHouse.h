﻿#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;


#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Action.h"
class BaseAction;
class Volunteer;



// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        //Functions we added:
        int getCustomerCounter() const;
        int getVolunteerCounter() const;
        int getOrderCounter() const;
        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);
        const vector<Order*> &getPendingOrders() const;
        const vector<Volunteer*> &getVolunteers() const; //To check if needed

        void activateAction(string& input); //find the correct action and executes it.
        void checkPendingOrders(); //Go through all pending orders each step
        void volunteersStep(); //Make a step in all volunteers
        void checkInProcessOrders(); //Go through all in process orders each step
        void deleteVolunteer(int volunteerId); //Delete a volunteer that finished his max orders
        
        WareHouse& operator=(const WareHouse& other); //Copy Assignment Operator
        WareHouse(const WareHouse& other); //Copy Constructor
        ~WareHouse(); //Destructor
        WareHouse(WareHouse&& other) noexcept; //Move Constructor
        WareHouse& operator=(WareHouse&& other) noexcept; //Move Assignment Operator



    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique orders IDs
        SoldierCustomer* defaultCustomer;
        CollectorVolunteer* defaultVolunteer;
        Order* defaultOrder;

        //Functions we added:
        void OpenConfigFile(const string &configFilePath); //Parse the config file
        void InitializeCustomer(string commandLine); //Add customer from config file
        void InitializeVolunteer(string commandLine); //Add volunteer from config file

        void addPendingOrders(Order* order); //Add an existing order to pending orders
        void addInProcessOrder(Order* order); //Add an existing order to in process orders
        void addCompletedOrder(Order* order); //Add an existing order to completed orders

       



};
