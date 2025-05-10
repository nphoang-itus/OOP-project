/**
 * @file ServiceInterface.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef SERVICE_INTERFACE_H
#define SERVICE_INTERFACE_H

#include <vector>
#include <optional>

template <typename T, typename IdType = int>
class IService {
public:
    virtual ~IService = default;
    
    virtual std::vector<T> findAll() = 0;
    virtual std::optional<T> findById(const IdType& id) = 0;
    virtual bool save(T& entity) = 0;
    virtual bool remove(const IdType& id) = 0;
};

#endif