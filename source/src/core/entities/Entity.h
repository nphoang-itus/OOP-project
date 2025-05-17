#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>

class Entity {
public:
    virtual ~Entity() = default;

    virtual std::string getId() const = 0;
    virtual std::string toString() const = 0;

    virtual bool equals(const Entity& other) const = 0;
    virtual std::unique_ptr<Entity> clone() const = 0;

protected:
    Entity() = default;
};

using EntityPtr = std::unique_ptr<Entity>;

#endif