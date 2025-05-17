#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "Entity.h"
#include "../value_objects/AircraftSerial.h"

using AircraftModel = std::string;

class Aircraft : public Entity {
private:
    AircraftSerial _serial;
    AircraftModel _model;
    SeatClassMap seatLayout;

public:
    
};

#endif