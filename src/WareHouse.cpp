﻿#include "../include/WareHouse.h"

WareHouse::WareHouse(const string &configFilePath) : isOpen(false), actionsLog{}, volunteers{}, pendingOrders{}, inProcessOrders{},
                                                     completedOrders{}, customers{}, customerCounter(0), volunteerCounter(0), orderCounter(0), defaultCustomer(new SoldierCustomer(-1, "default", -1, -1)),
                                                     defaultVolunteer(new CollectorVolunteer(-1, "default", -1)), defaultOrder(new Order(-1, -1, -1))
{
    OpenConfigFile(configFilePath);
}

void WareHouse::OpenConfigFile(const string &configFilePath)
{
    string line;
    ifstream file;
    file.open(configFilePath);
    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file, line);
            string firstWord = line.substr(0, line.find(" "));
            if (firstWord.compare("customer") == 0)
            {
                InitializeCustomer(line);
            }

            else if (firstWord.compare("volunteer") == 0)
            {
                InitializeVolunteer(line);
            }
        }
    }
    file.close();
}

void WareHouse::InitializeCustomer(string commandLine)
{
    string word;
    size_t i = 0;

    Customer *customer;
    int id = customerCounter, locationDistance, maxOrders, index = 0;
    string name, type;

    while ((i = commandLine.find(" ")) != string::npos)
    {
        word = commandLine.substr(0, i);
        commandLine.erase(0, i + 1);
        switch (index)
        {
        case 1:
            name = word;
            break;
        case 2:
            type = word;
            break;
        case 3:
            locationDistance = stoi(word);
            break;
        case 4:
            maxOrders = stoi(word);
            break;
        default:
            break;
        }
        index += 1;
    }

    if (type.compare("soldier") == 0)
        customer = new SoldierCustomer(id, name, locationDistance, maxOrders);
    if (type.compare("civilian") == 0)
        customer = new CivilianCustomer(id, name, locationDistance, maxOrders);

    addCustomer(customer);
}

void WareHouse::InitializeVolunteer(string commandLine)
{
    string word;
    size_t i = 0;

    Volunteer *volunteer;
    int id = volunteerCounter, coolDown, maxOrders, maxDistance, distancePerStep, index = 0;
    string name, type;

    while ((i = commandLine.find(" ")) != string::npos)
    {
        word = commandLine.substr(0, i);
        commandLine.erase(0, i + 1);
        switch (index)
        {
        case 1:
            name = word;
            break;
        case 2:
            type = word;
            break;
        case 3:
            coolDown = stoi(word);
            maxDistance = stoi(word);
            break;
        case 4:
            if ((type.compare("limited_collector") == 0) | (type.compare("driver") == 0) | (type.compare("limited_driver") == 0))
            {
                maxOrders = stoi(word);
                distancePerStep = stoi(word);
            }
            break;
        case 5:
            if (type.compare("limited_driver") == 0)
            {
                maxOrders = stoi(word);
            }
            break;
        default:
            break;
        }
        index += 1;
    }

    if (type.compare("collector") == 0)
        volunteer = new CollectorVolunteer(id, name, coolDown);
    if (type.compare("limited_collector") == 0)
        volunteer = new LimitedCollectorVolunteer(id, name, coolDown, maxOrders);
    if (type.compare("driver") == 0)
        volunteer = new DriverVolunteer(id, name, maxDistance, distancePerStep);
    if (type.compare("limited_driver") == 0)
        volunteer = new LimitedDriverVolunteer(id, name, maxDistance, distancePerStep, maxOrders);

    addVolunteer(volunteer);
}

void WareHouse::start()
{
    open();
    std::cout << "Warehouse is open!" << std::endl;
    while (isOpen)
    {
        string input;
        std::cout << "Enter an action to execute:" << std::endl;
        std::getline(std::cin, input);
        activateAction(input);
    }
}

// This function finds the correct action and executes it.
void WareHouse::activateAction(string &input)
{
    string firstWord = input.substr(0, input.find(" "));
    vector<string> command;
    std::stringstream ss(input);
    string token;

    while (getline(ss, token, ' '))
    {
        command.push_back(token);
    }

    if (firstWord == "step")
    {
        if (command.size() >= 2)
        {
            SimulateStep step_action = SimulateStep(stoi(command.at(1)));
            step_action.act(*this);
        }
        else
        {
            cout << "Usage: step <number>" << endl;
        }
    }
    else if (firstWord == "order")
    {
        if (command.size() >= 2)
        {
            AddOrder order_action = AddOrder(stoi(command.at(1)));
            order_action.act(*this);
        }
        else
        {
            cout << "Usage: order <customer_id>" << endl;
        }
    }
    else if (firstWord == "customer")
    {
        if (command.size() >= 5)
        {
            AddCustomer customer_action = AddCustomer(command.at(1), command.at(2), stoi(command.at(3)), stoi(command.at(4)));
            customer_action.act(*this);
        }
        else
        {
            cout << "Usage: customer <name> <type> <distance> <max_orders>" << endl;
        }
    }
    else if (firstWord == "orderStatus")
    {
        if (command.size() >= 2)
        {
            PrintOrderStatus order_status_action = PrintOrderStatus(stoi(command.at(1)));
            order_status_action.act(*this);
        }
        else
        {
            cout << "Usage: orderStatus <order_id>" << endl;
        }
    }
    else if (firstWord == "customerStatus")
    {
        if (command.size() >= 2)
        {
            PrintCustomerStatus customer_status_action = PrintCustomerStatus(stoi(command.at(1)));
            customer_status_action.act(*this);
        }
        else
        {
            cout << "Usage: customerStatus <customer_id>" << endl;
        }
    }
    else if (firstWord == "volunteerStatus")
    {
        if (command.size() >= 2)
        {
            PrintVolunteerStatus volunteer_status_action = PrintVolunteerStatus(stoi(command.at(1)));
            volunteer_status_action.act(*this);
        }
        else
        {
            cout << "Usage: volunteerStatus <volunteer_id>" << endl;
        }
    }
    else if (firstWord == "log")
    {
        PrintActionsLog log_action = PrintActionsLog();
        log_action.act(*this);
    }
    else if (firstWord == "close")
    {
        Close close_action = Close();
        close_action.act(*this);
    }
    else if (firstWord == "backup")
    {
        BackupWareHouse backup_action = BackupWareHouse();
        backup_action.act(*this);
    }
    else if (firstWord == "restore")
    {
        RestoreWareHouse restore_action = RestoreWareHouse();
        restore_action.act(*this);
    }
    else
    {
        cout << "Please enter a valid action" << endl;
    }
}

void WareHouse::addOrder(Order *order)
{
    addPendingOrders(order);
    orderCounter++;
}

void WareHouse::addPendingOrders(Order *order)
{ // Add an existing order to pending orders
    pendingOrders.push_back(order);
}
void WareHouse::addInProcessOrder(Order *order)
{ // Add an existing order to in process orders
    inProcessOrders.push_back(order);
}
void WareHouse::addCompletedOrder(Order *order)
{ // Add an existing order to completed orders
    completedOrders.push_back(order);
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

Customer &WareHouse::getCustomer(int customerId) const
{
    for (Customer *customer : customers)
    {
        if (customer->getId() == customerId)
        {
            return *customer;
        }
    }
    return *defaultCustomer;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const
{
    for (Volunteer *volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return *volunteer;
        }
    }
    return *defaultVolunteer;
}

Order &WareHouse::getOrder(int orderId) const
{
    for (Order *order : pendingOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : inProcessOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : completedOrders)
    {
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    return *defaultOrder;
}

const vector<BaseAction *> &WareHouse::getActions() const
{
    return actionsLog;
}

void WareHouse::close()
{
    for (Order *order : pendingOrders)
    {
        cout << "OrderId: " << order->getId() << ", CustomerID: " << order->getCustomerId() << ", Status: " << order->stringOrderStatus() << endl;
    }
    for (Order *order : inProcessOrders)
    {
        cout << "OrderId: " << order->getId() << ", CustomerID: " << order->getCustomerId() << ", Status: " << order->stringOrderStatus() << endl;
    }
    for (Order *order : completedOrders)
    {
        cout << "OrderId: " << order->getId() << ", CustomerID: " << order->getCustomerId() << ", Status: " << order->stringOrderStatus() << endl;
    }
    isOpen = false;
}

void WareHouse::open()
{
    isOpen = true;
}

int WareHouse::getCustomerCounter() const
{
    return customerCounter;
}
int WareHouse::getVolunteerCounter() const
{
    return volunteerCounter;
}

int WareHouse::getOrderCounter() const
{
    return orderCounter;
}
void WareHouse::addCustomer(Customer *customer)
{
    customers.push_back(customer);
    customerCounter++;
}

void WareHouse::addVolunteer(Volunteer *volunteer)
{
    volunteers.push_back(volunteer);
    volunteerCounter++;
}

void WareHouse::checkPendingOrders()
{ // Go through all pending orders each step
    for (int i = 0; (unsigned)i < pendingOrders.size(); i++)
    {
        Order *order = pendingOrders.at(i);
        bool foundVol = false;
        if (order->getStatus() == OrderStatus::PENDING)
        { // Should be transfered to collector:
            for (Volunteer *volunteer : volunteers)
            { // Going through volunteers and finding a collector for the order
                if (!foundVol && volunteer->getType().compare("collector") == 0)
                {
                    if (volunteer->canTakeOrder(*order))
                    {
                        order->setStatus(OrderStatus::COLLECTING);
                        order->setCollectorId(volunteer->getId());
                        volunteer->acceptOrder(*order);
                        // Move from pendingOrders to inProcessOrders :
                        addInProcessOrder(order);
                        pendingOrders.erase(pendingOrders.begin() + i);
                        i--;
                        foundVol = true;
                    }
                }
            }
        }
        else if (order->getStatus() == OrderStatus::COLLECTING)
        { // Should be transfered to driver:
            for (Volunteer *volunteer : volunteers)
            { // Going through volunteers and finding a driver for the order
                if (!foundVol && volunteer->getType().compare("driver") == 0)
                {
                    if (volunteer->canTakeOrder(*order))
                    {
                        order->setStatus(OrderStatus::DELIVERING);
                        order->setDriverId(volunteer->getId());
                        volunteer->acceptOrder(*order);
                        // Move from pendingOrders to inProcessOrders :
                        addInProcessOrder(order);
                        pendingOrders.erase(pendingOrders.begin() + i);
                        i--;
                        foundVol = true;
                    }
                }
            }
        }
    }
}

void WareHouse::volunteersStep()
{
    for (int i = 0; (unsigned)i < volunteers.size(); i++)
    {
        Volunteer *volunteer = volunteers.at(i);
        volunteer->step();
    }
}

void WareHouse::checkInProcessOrders()
{
    for (int i = 0; (unsigned)i < inProcessOrders.size(); i++)
    {
        Order *order = inProcessOrders.at(i);
        if (order->getStatus() == OrderStatus::COLLECTING)
        { // Collector volunteer is handling this process:
            Volunteer *collector = &(getVolunteer((*order).getCollectorId()));
            int completedOrderID = collector->getCompletedOrderId();
            if (completedOrderID == order->getId())
            { // If there is an orderID in completedOrderID, the volunteer finished to process it in the recent step
                // Move from inProcessOrders to pendingOrders:
                addPendingOrders(order);
                inProcessOrders.erase(inProcessOrders.begin() + i);
                i--;

                // Check if the volunteer needs to be deleted
                if (!collector->hasOrdersLeft())
                {
                    deleteVolunteer(collector->getId());
                }
            }
        }
        else if (order->getStatus() == OrderStatus::DELIVERING)
        { // Driver volunteer is handling this process:
            Volunteer *driver = &(getVolunteer((*order).getDriverId()));
            int completedOrderID = driver->getCompletedOrderId();
            if (completedOrderID == order->getId())
            { // If there is an orderID in completedOrderID, the volunteer finished to process it in the recent step
                // Move from inProcessOrders to completedOrders:
                order->setStatus(OrderStatus::COMPLETED);
                addCompletedOrder(order);
                inProcessOrders.erase(inProcessOrders.begin() + i);
                i--;

                // Check if the volunteer needs to be deleted
                if (!driver->hasOrdersLeft())
                {
                    deleteVolunteer(driver->getId());
                }
            }
        }
    }
}

void WareHouse::deleteVolunteer(int volunteerId)
{
    // Finding the position of the volunteer in the vector and deleting it:
    for (int i = 0; (unsigned)i < volunteers.size(); i++)
    {
        if ((*volunteers.at(i)).getId() == volunteerId)
        {
            delete volunteers.at(i);
            volunteers.erase(volunteers.begin() + i);
            break;
        }
    }
}

// Copy Assignment Operator:
WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (&other != this)
    {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        // Tere is no need to change defaultCustomer, defaultVolunteer, defaultOrder

        // delete old actions:
        for (BaseAction *action : actionsLog)
        {
            delete action;
        }
        // clear old addresses
        actionsLog.clear();
        // add other actions:
        for (BaseAction *action : other.actionsLog)
        {
            actionsLog.push_back(action->clone());
        }

        // delete old volunteers:
        for (Volunteer *volunteer : volunteers)
        {
            delete volunteer;
        }
        // clear old volunteers
        volunteers.clear();
        // add other volunteers:
        for (Volunteer *volunteer : other.volunteers)
        {
            volunteers.push_back(volunteer->clone());
        }

        // delete old pendingOrders:
        for (Order *order : pendingOrders)
        {
            delete order;
        }
        // clear old pendingOrders
        pendingOrders.clear();
        // add other pendingOrders:
        for (Order *order : other.pendingOrders)
        {
            pendingOrders.push_back(order->clone());
        }

        // delete old inProcessOrders:
        for (Order *order : inProcessOrders)
        {
            delete order;
        }
        // clear old inProcessOrders
        inProcessOrders.clear();
        // add other inProcessOrders:
        for (Order *order : other.inProcessOrders)
        {
            inProcessOrders.push_back(order->clone());
        }

        // delete old completedOrders:
        for (Order *order : completedOrders)
        {
            delete order;
        }
        // clear old completedOrders
        completedOrders.clear();
        // add other completedOrders:
        for (Order *order : other.completedOrders)
        {
            completedOrders.push_back(order->clone());
        }

        // delete old customers:
        for (Customer *customer : customers)
        {
            delete customer;
        }
        // clear old customers
        customers.clear();
        // add other customers:
        for (Customer *customer : other.customers)
        {
            customers.push_back(customer->clone());
        }
    }
    return *this;
}

// Copy Constructor:
WareHouse::WareHouse(const WareHouse &other) : isOpen(other.isOpen), actionsLog{}, volunteers{}, pendingOrders{}, inProcessOrders{},
                                               completedOrders{}, customers{}, customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter), defaultCustomer(new SoldierCustomer(-1, "default", -1, -1)),
                                               defaultVolunteer(new CollectorVolunteer(-1, "default", -1)), defaultOrder(new Order(-1, -1, -1))
{

    // add other actions:
    for (BaseAction *action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }

    // add other volunteers:
    for (Volunteer *volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer->clone());
    }

    // add other pendingOrders:
    for (Order *order : other.pendingOrders)
    {
        pendingOrders.push_back(order->clone());
    }

    // add other inProcessOrders:
    for (Order *order : other.inProcessOrders)
    {
        inProcessOrders.push_back(order->clone());
    }

    // add other completedOrders:
    for (Order *order : other.completedOrders)
    {
        completedOrders.push_back(order->clone());
    }

    // add other customers:
    for (Customer *customer : other.customers)
    {
        customers.push_back(customer->clone());
    }
}

// Destructor:
WareHouse::~WareHouse()
{
    delete defaultCustomer;
    defaultCustomer = nullptr;
    delete defaultVolunteer;
    defaultVolunteer = nullptr;
    delete defaultOrder;
    defaultOrder = nullptr;

    // delete actions:
    for (BaseAction *action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();

    // delete volunteers:
    for (Volunteer *volunteer : volunteers)
    {
        delete volunteer;
    }
    volunteers.clear();

    // delete pendingOrders:
    for (Order *order : pendingOrders)
    {
        delete order;
    }
    pendingOrders.clear();
    // delete inProcessOrders:
    for (Order *order : inProcessOrders)
    {
        delete order;
    }
    inProcessOrders.clear();
    // delete completedOrders:
    for (Order *order : completedOrders)
    {
        delete order;
    }
    completedOrders.clear();
    // delete customers:
    for (Customer *customer : customers)
    {
        delete customer;
    }
    customers.clear();
}

// Move Constructor:
WareHouse::WareHouse(WareHouse &&other) noexcept
    : isOpen(other.isOpen),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter),
      defaultCustomer(other.defaultCustomer),
      defaultVolunteer(other.defaultVolunteer),
      defaultOrder(other.defaultOrder)
{

    other.defaultCustomer = nullptr;
    other.defaultVolunteer = nullptr;
    other.defaultOrder = nullptr;
}

// Move Assignment Operator
WareHouse &WareHouse::operator=(WareHouse &&other) noexcept
{
    if (this != &other)
    {
        // Free resources in the current object
        for (BaseAction *action : actionsLog)
        {
            delete action;
        }
        actionsLog.clear();

        for (Volunteer *volunteer : volunteers)
        {
            delete volunteer;
        }
        volunteers.clear();

        for (Order *order : pendingOrders)
        {
            delete order;
        }
        pendingOrders.clear();

        for (Order *order : inProcessOrders)
        {
            delete order;
        }
        inProcessOrders.clear();

        for (Order *order : completedOrders)
        {
            delete order;
        }
        completedOrders.clear();

        for (Customer *customer : customers)
        {
            delete customer;
        }
        customers.clear();

        delete defaultCustomer;
        delete defaultVolunteer;
        delete defaultOrder;

        // Move resources from the source object
        isOpen = other.isOpen;
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        defaultCustomer = other.defaultCustomer;
        defaultVolunteer = other.defaultVolunteer;
        defaultOrder = other.defaultOrder;

        // Set the source's pointers to nullptr or appropriate defaults
        other.defaultCustomer = nullptr;
        other.defaultVolunteer = nullptr;
        other.defaultOrder = nullptr;
    }

    return *this;
}
