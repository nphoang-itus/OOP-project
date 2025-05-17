#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
protected:
    std::string _id;

public:
    Entity() = default;
    explicit Entity(std::string id) : _id(std::move(id)) {}
    virtual ~Entity() = default;

    const std::string& getId() const { return _id; }
    void setId(const std::string& newId) { _id = newId; }
};

#endif