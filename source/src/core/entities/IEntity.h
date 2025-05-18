#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>

class IEntity {
public:
    virtual ~IEntity() = default;

    virtual std::string getId() const = 0;
    virtual std::string toString() const = 0;

    virtual bool equals(const IEntity& other) const = 0;
    virtual std::unique_ptr<IEntity> clone() const = 0;

protected:
    IEntity() = default;
};

using EntityPtr = std::unique_ptr<IEntity>;

#endif