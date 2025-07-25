﻿#include "../include/Order.h"


Order::Order(int id, int customerId, int distance) : id(id),
    customerId(customerId), distance(distance),status(OrderStatus::PENDING),collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER){}

int Order::getId() const{
    return id;
}
int Order::getCustomerId() const{
    return customerId;
}
void Order::setStatus(OrderStatus status)
{
    (*this).status = status;
}
void Order::setCollectorId(int collectorId)
{
    (*this).collectorId = collectorId;
}
void Order::setDriverId(int driverId)
{
    (*this).driverId = driverId;
}
int Order::getCollectorId() const
{
    return collectorId;
}
int Order::getDriverId() const
{
    return driverId;
}
OrderStatus Order::getStatus() const
{
    return status;
}
const string Order::toString() const
{
    std::string var = "OrderID: " + std::to_string(id) +"\nOrderStatus: " + stringOrderStatus() +
    "\nCustomerID: " + std::to_string(customerId)+ "\nCollector: ";
    if (collectorId == NO_VOLUNTEER){
       var += "None";
    }else{
        var += std::to_string(collectorId);
    }
    if (driverId == NO_VOLUNTEER){
       var += "\nDriver: None";
    }else{
        var += "\nDriver: " + std::to_string(driverId);
    }
    
    return var;
}

int Order::getDistance() const{
    return distance;
}

string Order::stringOrderStatus() const{
    if (status == OrderStatus::PENDING){
        return "PENDING";
    }else if (status == OrderStatus::COLLECTING){
        return "COLLECTING";
    }else if (status == OrderStatus::DELIVERING){
        return "DELIVERING";
    }else{
        return "COMPLETED";
    }
}
Order* Order::clone() const{return new Order(*this);}
