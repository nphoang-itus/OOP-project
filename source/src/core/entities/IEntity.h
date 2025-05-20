#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>

class IEntity {
protected:
    int _id;  // Database-generated ID

public:
    virtual ~IEntity() = default;

    virtual int getId() const = 0;  // Get database ID
    virtual std::string getStringId() const = 0;  // Get business identifier
    virtual std::string toString() const = 0;
    virtual bool equals(const IEntity& other) const = 0;
    virtual std::unique_ptr<IEntity> clone() const = 0;
    virtual void setId(int id) = 0;  // Set database ID after insertion

protected:
    IEntity() : _id(0) {}  // Initialize id to 0
};

using EntityPtr = std::unique_ptr<IEntity>;

#endif