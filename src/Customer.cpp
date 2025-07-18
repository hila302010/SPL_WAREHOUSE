﻿
#include "../include/Customer.h"
#include "../include/Order.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance)
, maxOrders(maxOrders), ordersId{}{}

const string &Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}
int Customer::getCustomerDistance() const
{
    return locationDistance;
}
int Customer::getMaxOrders() const //Returns maxOrders
{
    return maxOrders;
}
int Customer::getNumOrders() const //Returns num of orders the customer has made so far
{
    return getOrdersIds().size();
}
bool Customer::canMakeOrder() const //Returns true if the customer didn't reach max orders
{
    bool _canMakeOrder = true;
    if(getNumOrders() >= getMaxOrders())
        _canMakeOrder = false;
    return _canMakeOrder;
}

const vector<int> &Customer::Customer::getOrdersIds() const
{
    return ordersId;

}
int Customer::addOrder(int orderId) //return OrderId if order was added successfully, -1 otherwise
{
    int ret = -1;
    if(canMakeOrder())
    {
        ordersId.push_back(orderId);
        ret = orderId;

    }
    return ret;
}

const string Customer::toString() const
{
    std::string var = "CustomerID: " + std::to_string(id);
    return var;

}

int Customer::getNumOrdersLeft() const{
    return maxOrders - getNumOrders();
}



SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id,name,locationDistance,maxOrders){

}
SoldierCustomer* SoldierCustomer::clone() const{return new SoldierCustomer(*this);}

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id,name,locationDistance,maxOrders){

}
CivilianCustomer* CivilianCustomer::clone() const{return new CivilianCustomer(*this);}
