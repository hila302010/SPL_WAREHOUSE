﻿#include "../include/Volunteer.h"
#define NO_ORDER -1

//Class Volunteer:

Volunteer::Volunteer(int id, const string &name, const string &type): completedOrderId(NO_ORDER), activeOrderId(NO_ORDER),id(id), name(name), type(type){}
int Volunteer::getId() const{return id;}
const string& Volunteer::getName() const{return name;}
int Volunteer::getActiveOrderId() const{return activeOrderId;}
int Volunteer::getCompletedOrderId() const{return completedOrderId;}
const string& Volunteer::getType() const{return type;}
bool Volunteer::isBusy() const{return activeOrderId != NO_ORDER ;}    


// class CollectorVolunteer:: public Volunteer : 

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown): Volunteer(id, name, "collector"), coolDown(coolDown), timeLeft(0){}
CollectorVolunteer* CollectorVolunteer::clone() const {return new CollectorVolunteer(*this);}
void CollectorVolunteer::step(){
    if (isBusy()){ 
        if (decreaseCoolDown()){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }   
}
int CollectorVolunteer::getCoolDown() const{return coolDown;}       
int CollectorVolunteer::getTimeLeft() const{return timeLeft;}
bool CollectorVolunteer::decreaseCoolDown(){
    timeLeft--;
    return timeLeft==0;
}//Decrease timeLeft by 1,return true if timeLeft=0,false otherwise

bool CollectorVolunteer::hasOrdersLeft() const {return true;}
bool CollectorVolunteer::canTakeOrder(const Order &order) const{return hasOrdersLeft() && !isBusy();}
void CollectorVolunteer::acceptOrder(const Order &order){//Need to complete
    timeLeft = coolDown;
    activeOrderId = order.getId();
}
string CollectorVolunteer::toString() const{
    string var = "VolunteerID: " + std::to_string(getId()) +"\nisBusy: " + (isBusy() ? "True" : "False" )+ "\nOrderID: ";
    if (!isBusy()){
       var += "None \ntimeLeft: None";
    }else{
        var += std::to_string(getActiveOrderId())+ "\ntimeLeft: " + std::to_string(timeLeft);
    }
    var += "\nordersLeft: No Limit";

    return var;
}


//class LimitedCollectorVolunteer:: public CollectorVolunteer 

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders):CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders),ordersLeft(maxOrders){}
LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {return new LimitedCollectorVolunteer(*this);}
bool LimitedCollectorVolunteer::hasOrdersLeft() const{return ordersLeft > 0 ;}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{return hasOrdersLeft() && !isBusy();}
void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    CollectorVolunteer::acceptOrder(order);
    ordersLeft --;
}
int LimitedCollectorVolunteer::getMaxOrders() const{return maxOrders;}
int LimitedCollectorVolunteer::getNumOrdersLeft() const{return ordersLeft;}
string LimitedCollectorVolunteer::toString() const {
    string var = "VolunteerID: " + std::to_string(getId()) +"\nisBusy: " + (isBusy() ? "True" : "False" ) + "\nOrderID: ";
    if (!isBusy()){
       var += "None \ntimeLeft: None";
    }else{
        var += std::to_string(getActiveOrderId())+ "\ntimeLeft: " + std::to_string(getTimeLeft());
    }
    var += "\nordersLeft: " + std::to_string(getNumOrdersLeft());

    return var;
}
    


//class DriverVolunteer:: public Volunteer

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep): Volunteer(id,name, "driver"), maxDistance(maxDistance), distancePerStep(distancePerStep),distanceLeft(0){}
DriverVolunteer* DriverVolunteer::clone() const {return new DriverVolunteer(*this);}

int DriverVolunteer::getDistanceLeft() const{return distanceLeft;}
int DriverVolunteer::getMaxDistance() const{return maxDistance;}
int DriverVolunteer::getDistancePerStep() const{return distancePerStep;} 
bool DriverVolunteer::decreaseDistanceLeft(){
    distanceLeft = distanceLeft - distancePerStep;
    return distanceLeft <= 0 ;
} //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise /
bool DriverVolunteer::hasOrdersLeft() const {return true;}
bool DriverVolunteer::canTakeOrder(const Order &order) const {return hasOrdersLeft() && !isBusy() && (order.getDistance() <= getMaxDistance());} // Signal if the volunteer is not busy and the order is within the maxDistance
void DriverVolunteer::acceptOrder(const Order &order) {
    distanceLeft = order.getDistance();
    activeOrderId = order.getId();
} // Assign distanceLeft to order's distance
void DriverVolunteer::step() {
        if (isBusy()){ 
        if (decreaseDistanceLeft()){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
} // Decrease distanceLeft by distancePerStep
string DriverVolunteer::toString() const {
    string var = "VolunteerID: " + std::to_string(getId()) +"\nisBusy: " + (isBusy() ? "True" : "False" ) + "\nOrderID: ";
    if (!isBusy()){
       var += "None \ntimeLeft: None";
    }else{
        var += std::to_string(getActiveOrderId())+ "\ntimeLeft: " + std::to_string(getDistanceLeft());
    }
    var += "\nordersLeft: None ";

    return var;
}


//class LimitedDriverVolunteer:: public DriverVolunteer 

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders): DriverVolunteer(id, name, maxDistance, distancePerStep),maxOrders(maxOrders),ordersLeft(maxOrders){}
LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {return new LimitedDriverVolunteer(*this);}
int LimitedDriverVolunteer::getMaxOrders() const{return maxOrders;}
int LimitedDriverVolunteer::getNumOrdersLeft() const{return ordersLeft;}
bool LimitedDriverVolunteer::hasOrdersLeft() const {return ordersLeft>0;}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {// Signal if the volunteer is not busy, the order is within the maxDistance.
    return hasOrdersLeft() && !isBusy() && (order.getDistance() <= getMaxDistance());
}
void LimitedDriverVolunteer::acceptOrder(const Order &order) {// Assign distanceLeft to order's distance and decrease ordersLeft
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}
string LimitedDriverVolunteer::toString() const {
    string var = "VolunteerID: " + std::to_string(getId()) +"\nisBusy: " + (isBusy() ? "True" : "False" ) + "\nOrderID: ";
    if (!isBusy()){
       var += "None \ntimeLeft: None";
    }else{
        var += std::to_string(getActiveOrderId())+ "\ntimeLeft: " + std::to_string(getDistanceLeft());
    }
    var += "\nordersLeft: " + std::to_string(getNumOrdersLeft());

    return var;
}
