#ifndef PASSENGER_H
#define PASSENGER_H

#include "IEntity.h"
#include "../value_objects/contact_info/ContactInfo.h"
#include "../value_objects/passport_number/PassportNumber.h"
#include "../exceptions/Result.h"

using Name = std::string;

class Passenger : public IEntity {
protected:
    Name _name;
    ContactInfo _contactInfo;
    PassportNumber _passport;

    Passenger(Name name, ContactInfo contactInfo, PassportNumber passport)
        : _name(std::move(name)), _contactInfo(std::move(contactInfo)), _passport(std::move(passport)) {}

public:
    static Result<Passenger> create(const Name& name, const ContactInfo& contactInfo, const PassportNumber& passport) {
        if (name.empty()) {
            return Failure<Passenger>(CoreError("Passenger name cannot be empty", "INVALID_NAME"));
        }
        return Success(Passenger(name, contactInfo, passport));
    }

    static Result<Passenger> create(const std::string& name, const std::string& contactInfo, const std::string& passport) {
        if (name.empty()) {
            return Failure<Passenger>(CoreError("Passenger name cannot be empty", "INVALID_NAME"));
        }

        auto contactInfoResult = ContactInfo::create(contactInfo);
        if (!contactInfoResult) {
            return Failure<Passenger>(CoreError("Invalid contact info: " + contactInfoResult.error().message, "INVALID_CONTACT_INFO"));
        }

        auto passportResult = PassportNumber::create(passport);
        if (!passportResult) {
            return Failure<Passenger>(CoreError("Invalid passport number: " + passportResult.error().message, "INVALID_PASSPORT"));
        }

        return Success(Passenger(name, *contactInfoResult, *passportResult));
    }

    int getId() const override {
        return _id;
    }

    std::string getStringId() const override {
        return _passport.toString();
    }

    void setId(int id) override {
        _id = id;
    }

    std::string toString() const override {
        return "Passenger{id=" + std::to_string(_id) + 
               ", name=" + _name + 
               ", contactInfo=" + _contactInfo.toString() + 
               ", passport=" + _passport.toString() + "}";
    }

    bool equals(const IEntity& other) const override {
        if (const auto* passenger = dynamic_cast<const Passenger*>(&other)) {
            return _id == passenger->_id;
        }
        return false;
    }

    std::unique_ptr<IEntity> clone() const override {
        auto clone = std::unique_ptr<Passenger>(new Passenger(_name, _contactInfo, _passport));
        clone->_id = _id;
        return clone;
    }

    const Name& getName() const { return _name; }
    const ContactInfo& getContactInfo() const { return _contactInfo; }
    const PassportNumber& getPassport() const { return _passport; }
};

#endif