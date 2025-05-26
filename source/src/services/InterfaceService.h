#ifndef INTERFACE_SERVICE_H
#define INTERFACE_SERVICE_H

#include "../core/exceptions/Result.h"
#include <vector>

template <typename T>
class IService
{
public:
    virtual ~IService() = default;

    // Basic CRUD operations
    virtual Result<T> getById(const int &id) = 0;
    virtual Result<std::vector<T>> getAll() = 0;
    virtual Result<bool> exists(const int &id) = 0;
    virtual Result<size_t> count() = 0;
    virtual Result<T> create(const T &entity) = 0;
    virtual Result<T> update(const T &entity) = 0;
    virtual Result<bool> deleteById(const int &id) = 0;
};

#endif // INTERFACE_SERVICE_H
