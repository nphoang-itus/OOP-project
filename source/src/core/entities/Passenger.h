#ifndef PASSENGER_H
#define PASSENGER_H

#include "IEntity.h"
#include "../value_objects/contact_info/ContactInfo.h"
#include "../value_objects/passport_number/PassportNumber.h"

using Name = std::string;

class Passenger : public IEntity {
protected:
    Name _name;
    ContactInfo _contactInfo;
    PassportNumber _passport;

public:
    
};

#endif