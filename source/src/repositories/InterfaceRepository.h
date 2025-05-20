#ifndef INTERFACE_REPOSITORY_H
#define INTERFACE_REPOSITORY_H

#include <memory>
#include <vector>
#include <optional>
#include "../core/entities/IEntity.h"
#include "../core/exceptions/Result.h"

// Base repository interface for read operations
template<typename T, typename IdType = int>
class IReadRepository {
public:
    virtual ~IReadRepository() = default;
    virtual Result<T> findById(const IdType& id) = 0;
    virtual Result<std::vector<T>> findAll() = 0;
    virtual Result<std::vector<T>> findByCondition(const std::function<bool(const T&)>& condition) = 0;
    virtual Result<bool> exists(const IdType& id) = 0;
    virtual Result<size_t> count() = 0;
    virtual Result<std::vector<T>> findWithPaging(size_t page, size_t pageSize) = 0;
};

// Extended interface for write operations
template<typename T, typename IdType = int>
class IWriteRepository {
public:
    virtual ~IWriteRepository() = default;
    virtual Result<T> create(const T& entity) = 0;
    virtual Result<T> update(const T& entity) = 0;
    virtual Result<bool> deleteById(const IdType& id) = 0;
    virtual Result<bool> deleteMany(const std::vector<IdType>& ids) = 0;
};

// Full repository interface combining read and write operations
template<typename T, typename IdType = int>
class IRepository : public IReadRepository<T, IdType>, public IWriteRepository<T, IdType> {
public:
    virtual ~IRepository() = default;
};

#endif // INTERFACE_REPOSITORY_H
