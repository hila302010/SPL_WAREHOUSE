﻿#include "../include/Action.h"
extern WareHouse *backup;

// Class Base Action:
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete() {}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    (*this).errorMsg = errorMsg;
    cout << "Error: " << errorMsg << endl;
}

string BaseAction::getErrorMsg() const { return errorMsg; }

// Class SimulateStep:

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse)
{

    for (int steps = numOfSteps; steps > 0; steps--)
    {                                     // Doing the loop numOfSteps Times:
        wareHouse.checkPendingOrders();   // Go through all pending orders each step
        wareHouse.volunteersStep();       // Make a step in all volunteers
        wareHouse.checkInProcessOrders(); // Go through all in process orders each step
    }
    // Logging the action:
    SimulateStep *myClone = clone();
    wareHouse.addAction(myClone);
}
std::string SimulateStep::toString() const
{
    string myStr = "simulateStep " + to_string(numOfSteps);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return myStr + " COMPLETED";
    }
    else
    {
        return myStr + " ERROR";
    }
}
SimulateStep *SimulateStep::clone() const { return new SimulateStep(*this); }

// Class AddOrder:
AddOrder::AddOrder(int id) : BaseAction(), customerId(id) {}

void AddOrder::act(WareHouse &wareHouse)
{
    if (customerId >= 0 && customerId < wareHouse.getCustomerCounter())
    {
        Customer &currentCustomer = wareHouse.getCustomer(customerId);
        if (currentCustomer.canMakeOrder())
        {
            int orderId = wareHouse.getOrderCounter();                                               // Getting new order id
            Order *newOrder = new Order(orderId, customerId, currentCustomer.getCustomerDistance()); // Initializing new order
            wareHouse.addOrder(newOrder);                                                            // Adding the order
            currentCustomer.addOrder(orderId);
        }
        else
        {
            error("Cannot place this order"); // The customer cannot make orders
        }
    }
    else
    {
        error("Cannot place this order"); // There is no such customer
    }
    // Logging the action:
    AddOrder *myClone = clone();
    wareHouse.addAction(myClone);
}

AddOrder *AddOrder::clone() const { return new AddOrder(*this); }

string AddOrder::toString() const
{
    string myStr = "order " + to_string(customerId);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return myStr + " COMPLETED";
    }
    else
    {
        return myStr + " ERROR";
    }
}

// Class AddCustomer:
AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders) : customerName(customerName), customerType((customerType == "Soldier") ? CustomerType::Soldier : CustomerType::Civilian),
                                                                                                  distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse)
{
    int customerId = wareHouse.getCustomerCounter(); // Getting new customer id
    Customer *newCustomer;
    if (customerType == CustomerType::Civilian)
    {
        newCustomer = new CivilianCustomer(customerId, customerName, distance, maxOrders); // Check string
    }
    else
    {
        newCustomer = new SoldierCustomer(customerId, customerName, distance, maxOrders); // Check string
    }
    wareHouse.addCustomer(newCustomer);

    // Logging the action:
    AddCustomer *myClone = clone();
    wareHouse.addAction(myClone);
}

AddCustomer *AddCustomer::clone() const { return new AddCustomer(*this); }

string AddCustomer::toString() const
{
    string type = (customerType == CustomerType::Soldier) ? "Soldier" : "Civilian";
    string myStr = "customer " + customerName + " " + type + " " + to_string(distance) + " " + to_string(maxOrders);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return myStr + " COMPLETED";
    }
    else
    {
        return myStr + " ERROR";
    }
}

// Class PrintOrderStatus:
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    if (orderId >= 0 && orderId < wareHouse.getOrderCounter())
    {
        cout << (wareHouse.getOrder(orderId)).toString() << endl;
    }
    else
    {
        error("Order doesn't exist");
    }
    // Logging the action:
    PrintOrderStatus *myClone = clone();
    wareHouse.addAction(myClone);
}

PrintOrderStatus *PrintOrderStatus::clone() const { return new PrintOrderStatus(*this); }

string PrintOrderStatus::toString() const
{
    string myStr = "orderStatus " + to_string(orderId);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return myStr + " COMPLETED";
    }
    else
    {
        return myStr + " ERROR";
    }
}

// Class PrintCustomerStatus:
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (customerId >= 0 && customerId < wareHouse.getCustomerCounter())
    {
        Customer &current = wareHouse.getCustomer(customerId);
        string result = current.toString();
        for (int orderid : current.getOrdersIds())
        {
            Order &order = wareHouse.getOrder(orderid);
            result += "\nOrderID: " + to_string(orderid) + "\nOrderStatus: " += order.stringOrderStatus();
        }
        result += "\nnumOrdersLeft: " + to_string(current.getNumOrdersLeft());
        cout << result << endl;
    }
    else
    {
        error("Customer doesn't exist");
    }

    // Logging the action:
    PrintCustomerStatus *myClone = clone();
    wareHouse.addAction(myClone);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const { return new PrintCustomerStatus(*this); } // To check with Hila

string PrintCustomerStatus::toString() const
{
    string myStr = "customerStatus " + to_string(customerId);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return myStr + " COMPLETED";
    }
    else
    {
        return myStr + " ERROR";
    }
}

// Class PrintVolunteerStatus:
PrintVolunteerStatus::PrintVolunteerStatus(int id) : VolunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    if (VolunteerId >= 0 && VolunteerId < wareHouse.getVolunteerCounter())
    {
        Volunteer &v = wareHouse.getVolunteer(VolunteerId);
        if (v.getId() != -1)
        {
            cout << v.toString() << endl;
        }
        else
        {
            error("Volunteer doesn't exist");
        }
    }
    else
    {
        error("Volunteer doesn't exist");
    }

    // Logging the action:
    PrintVolunteerStatus *myClone = clone();
    wareHouse.addAction(myClone);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const { return new PrintVolunteerStatus(*this); }

string PrintVolunteerStatus::toString() const
{
    string myStr = "volunteerStatus " + to_string(VolunteerId);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return myStr + " COMPLETED";
    }
    else
    {
        return myStr + " ERROR";
    }
}

// Class PrintActionsLog:
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    for (BaseAction *action : wareHouse.getActions())
    {
        cout << action->toString() << endl;
    }

    // Logging the action:
    PrintActionsLog *myClone = clone();
    wareHouse.addAction(myClone);
}

PrintActionsLog *PrintActionsLog::clone() const { return new PrintActionsLog(*this); }

string PrintActionsLog::toString() const
{
    return "log COMPLETED";
}

// Class Close:
Close::Close() {};
void Close::act(WareHouse &wareHouse)
{
    wareHouse.close();
}
Close *Close::clone() const { return new Close(*this); }

string Close::toString() const { return "close COMPLETED"; }

// Class BackupWareHouse:
BackupWareHouse::BackupWareHouse() {}
void BackupWareHouse::act(WareHouse &wareHouse)
{
    if (backup == nullptr)
    {
        backup = new WareHouse(wareHouse); // Using Copy Constructor
    }
    else
    {
        *backup = wareHouse; // Using Copy Assignment Operator
    }
    // Logging the action:
    BackupWareHouse *myClone = clone();
    wareHouse.addAction(myClone);
}
BackupWareHouse *BackupWareHouse::clone() const { return new BackupWareHouse(*this); }
string BackupWareHouse::toString() const { return "backup COMPLETED"; }

// Class RestoreWareHouse:
RestoreWareHouse::RestoreWareHouse() {}
void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup == nullptr)
    {
        error("No backup available");
    }
    else
    {
        wareHouse = *backup; // Using Copy Assignment Operator
    }
    // Logging the action:
    RestoreWareHouse *myClone = clone();
    wareHouse.addAction(myClone);
}
RestoreWareHouse *RestoreWareHouse::clone() const { return new RestoreWareHouse(*this); }
string RestoreWareHouse::toString() const
{
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return "restore COMPLETED";
    }
    return "restore ERROR";
}
