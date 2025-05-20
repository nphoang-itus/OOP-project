#ifndef INTERFACE_REPOSITORY_H
#define INTERFACE_REPOSITORY_H

#include <vector>
#include "../core/exceptions/Result.h"

// Base repository interface for read operations
template<typename T, typename IdType = int>
class IRepository {
public:
    virtual ~IRepository() = default;
    
    virtual Result<T> findById(const IdType& id) = 0;
    virtual Result<std::vector<T>> findAll() = 0;
    virtual Result<bool> exists(const IdType& id) = 0;
    virtual Result<size_t> count() = 0;

    virtual Result<T> create(const T& entity) = 0;
    virtual Result<T> update(const T& entity) = 0;
    virtual Result<bool> deleteById(const IdType& id) = 0;
};

#endif // INTERFACE_REPOSITORY_H
