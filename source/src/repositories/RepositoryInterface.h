/**
 * @file RepositoryInterface.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef REPOSITORY_INTERFACE_H
 #define REPOSITORY_INTERFACE_H
 
 #include <vector>
 #include <optional>
 
 template <typename T, typename IdType = int>
 class IRepository {
 public:
     virtual ~IRepository() = default;
     
     /**
      * @brief Lấy tất cả các thuộc tính của thực thể và lưu vào vector
      * 
      * @return std::vector<T> Với T là thực thể tương ứng
      */
     virtual std::vector<T> findAll() = 0;
 
     /**
      * @brief Tìm các giá trị thuộc tính theo Id
      * 
      * @param id Khoá chính của thực thể
      * @return std::optional<T> Bộ giá trị của thực thể
      */
     virtual std::optional<T> findById(IdType id) = 0;
     
     virtual bool save(T& entity) = 0;
     
     virtual bool update(const T& entity) = 0;
 
     virtual bool remove(IdType id) = 0;
 };
 
 #endif