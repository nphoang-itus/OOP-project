/**
 * @file DatabaseConnection.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Giao diện trừu tượng cho lớp kết nối cơ sở dữ liệu và xử lý kết quả truy vấn.
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 * Giao diện này định nghĩa các phương thức cần thiết cho việc giao tiếp với cơ sở dữ liệu,
 * bao gồm thực thi truy vấn, quản lý prepared statement, và xử lý transaction.
 * Thiết kế tuân theo nguyên lý SOLID, đặc biệt là Dependency Inversion Principle,
 * giúp tách biệt giữa tầng xử lý nghiệp vụ và tầng dữ liệu.
 */

#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <string>
#include <memory>
#include <ctime>
#include "../core/exceptions/Result.h"

/**
* @interface IDatabaseResult
* @brief Đại diện cho kết quả trả về từ truy vấn SELECT trong cơ sở dữ liệu.
*
* Cung cấp các phương thức để duyệt dữ liệu hàng loạt và truy xuất các giá trị theo chỉ số hoặc tên cột.
* 
* @details
* Interface này hỗ trợ hai cách truy xuất dữ liệu:
* - Theo chỉ số cột (0-based indexing)
* - Theo tên cột (case-sensitive)
* 
* Các kiểu dữ liệu được hỗ trợ:
* - std::string: Chuỗi ký tự
* - int: Số nguyên 32-bit
* - double: Số thực double precision
* - std::tm: Thời gian và ngày tháng
* 
* @note Trước khi truy xuất dữ liệu, phải gọi next() để di chuyển đến hàng đầu tiên.
*/
class IDatabaseResult {
public:
    /**
     * @brief Destructor ảo để đảm bảo giải phóng tài nguyên đúng cách.
     */
    virtual ~IDatabaseResult() = default;

    /**
    * @brief Di chuyển đến hàng tiếp theo trong kết quả truy vấn.
    * 
    * @return Result<bool> 
    *         - Success(true): Có hàng tiếp theo, có thể truy xuất dữ liệu
    *         - Success(false): Đã hết hàng, không còn dữ liệu
    *         - Failure: Có lỗi xảy ra trong quá trình di chuyển
    * 
    * @note Phải gọi phương thức này trước khi truy xuất dữ liệu từ hàng đầu tiên.
    * @warning Sau khi trả về false, không nên gọi các phương thức get*().
    */
    virtual Result<bool> next() = 0;

    /// @name Truy xuất dữ liệu theo chỉ số cột
    /// @{
    
    /**
     * @brief Lấy giá trị chuỗi từ cột theo chỉ số.
     * 
     * @param columnIndex Chỉ số cột (bắt đầu từ 0)
     * @return Result<std::string> 
     *         - Success: Giá trị chuỗi của cột
     *         - Failure: Lỗi chỉ số không hợp lệ hoặc không thể chuyển đổi kiểu
     * 
     * @pre Phải gọi next() và trả về true trước khi gọi phương thức này
     * @note Nếu giá trị NULL, sẽ trả về chuỗi rỗng
     */
    virtual Result<std::string> getString(const int& columnIndex) = 0;
    
    /**
     * @brief Lấy giá trị số nguyên từ cột theo chỉ số.
     * 
     * @param columnIndex Chỉ số cột (bắt đầu từ 0)
     * @return Result<int> 
     *         - Success: Giá trị số nguyên của cột
     *         - Failure: Lỗi chỉ số không hợp lệ, NULL, hoặc không thể chuyển đổi
     * 
     * @pre Phải gọi next() và trả về true trước khi gọi phương thức này
     * @warning Nếu giá trị vượt quá phạm vi int, có thể xảy ra overflow
     */
    virtual Result<int> getInt(const int& columnIndex) = 0;
    
    /**
     * @brief Lấy giá trị số thực từ cột theo chỉ số.
     * 
     * @param columnIndex Chỉ số cột (bắt đầu từ 0)
     * @return Result<double> 
     *         - Success: Giá trị số thực của cột
     *         - Failure: Lỗi chỉ số không hợp lệ, NULL, hoặc không thể chuyển đổi
     * 
     * @pre Phải gọi next() và trả về true trước khi gọi phương thức này
     * @note Hỗ trợ chuyển đổi từ các kiểu số khác (int, float, decimal)
     */
    virtual Result<double> getDouble(const int& columnIndex) = 0;
    
    /**
     * @brief Lấy giá trị thời gian từ cột theo chỉ số.
     * 
     * @param columnIndex Chỉ số cột (bắt đầu từ 0)
     * @return Result<std::tm> 
     *         - Success: Cấu trúc thời gian đã được parse
     *         - Failure: Lỗi chỉ số không hợp lệ, NULL, hoặc format không đúng
     * 
     * @pre Phải gọi next() và trả về true trước khi gọi phương thức này
     * @note Hỗ trợ các định dạng: DATE, TIME, DATETIME, TIMESTAMP
     */
    virtual Result<std::tm> getDateTime(const int& columnIndex) = 0;
    /// @}

    /// @name Truy xuất dữ liệu theo tên cột
    /// @{
    
    /**
     * @brief Lấy giá trị chuỗi từ cột theo tên.
     * 
     * @param columnName Tên cột (phân biệt hoa thường)
     * @return Result<std::string> 
     *         - Success: Giá trị chuỗi của cột
     *         - Failure: Lỗi tên cột không tồn tại hoặc không thể chuyển đổi
     * 
     * @pre Phải gọi next() và trả về true trước khi gọi phương thức này
     * @note Tên cột phải khớp chính xác với tên trong SELECT statement
     */
    virtual Result<std::string> getString(const std::string& columnName) = 0;
    
    /**
     * @brief Lấy giá trị số nguyên từ cột theo tên.
     * 
     * @param columnName Tên cột (phân biệt hoa thường)
     * @return Result<int> 
     *         - Success: Giá trị số nguyên của cột
     *         - Failure: Lỗi tên cột không tồn tại, NULL, hoặc không thể chuyển đổi
     * 
     * @pre Phải gọi next() và trả về true trước khi gọi phương thức này
     * @note Tên cột phải khớp chính xác với tên trong SELECT statement
     */
    virtual Result<int> getInt(const std::string& columnName) = 0;
    
    /**
     * @brief Lấy giá trị số thực từ cột theo tên.
     * 
     * @param columnName Tên cột (phân biệt hoa thường)
     * @return Result<double> 
     *         - Success: Giá trị số thực của cột
     *         - Failure: Lỗi tên cột không tồn tại, NULL, hoặc không thể chuyển đổi
     * 
     * @pre Phải gọi next() và trả về true trước khi gọi phương thức này
     * @note Tên cột phải khớp chính xác với tên trong SELECT statement
     */
    virtual Result<double> getDouble(const std::string& columnName) = 0;
    
    /**
     * @brief Lấy giá trị thời gian từ cột theo tên.
     * 
     * @param columnName Tên cột (phân biệt hoa thường)
     * @return Result<std::tm> 
     *         - Success: Cấu trúc thời gian đã được parse
     *         - Failure: Lỗi tên cột không tồn tại, NULL, hoặc format không đúng
     * 
     * @pre Phải gọi next() và trả về true trước khi gọi phương thức này
     * @note Tên cột phải khớp chính xác với tên trong SELECT statement
     */
    virtual Result<std::tm> getDateTime(const std::string& columnName) = 0;
    /// @}
};

/**
* @interface IDatabaseConnection
* @brief Giao diện trừu tượng đại diện cho kết nối đến cơ sở dữ liệu.
* 
* Cho phép quản lý truy vấn SQL, transaction, và các thao tác với prepared statement.
* 
* @details
* Interface này cung cấp đầy đủ chức năng để:
* - Quản lý kết nối database (connect/disconnect)
* - Thực thi câu lệnh SQL trực tiếp
* - Quản lý prepared statements với parameter binding
* - Điều khiển transactions (begin/commit/rollback)
* - Xử lý lỗi và kiểm tra trạng thái
* 
* @note Thiết kế theo mẫu Strategy Pattern, cho phép thay đổi implementation
*       mà không ảnh hưởng đến code sử dụng.
*/
class IDatabaseConnection {
public:
    /**
     * @brief Destructor ảo để đảm bảo giải phóng tài nguyên đúng cách.
     */
    virtual ~IDatabaseConnection() = default;

    /**
    * @brief Thiết lập kết nối đến cơ sở dữ liệu.
    * 
    * @param host Tên máy chủ hoặc địa chỉ IP của database server
    * @param user Tên người dùng để xác thực
    * @param password Mật khẩu của người dùng
    * @param database Tên cơ sở dữ liệu cần kết nối
    * @param port Cổng kết nối (mặc định là 33060 cho XAPIDev của MySQL)
    * 
    * @return Result<bool> 
    *         - Success(true): Kết nối thành công
    *         - Failure: Thông tin kết nối sai hoặc server không khả dụng
    * 
    * @pre Chưa có kết nối active nào khác
    * @post Nếu thành công, có thể thực hiện các operation database
    * 
    * @note Nếu đã có kết nối active, nên gọi disconnect() trước
    * @warning Thông tin xác thực sẽ được gửi qua mạng, cần đảm bảo bảo mật
    */
    virtual Result<bool> connect(const std::string& host, const std::string& user,
                         const std::string& password, const std::string& database,
                         const int& port = 33060) = 0;

    /**
    * @brief Ngắt kết nối hiện tại khỏi cơ sở dữ liệu.
    * 
    * @return VoidResult 
    *         - Success: Ngắt kết nối thành công hoặc chưa có kết nối
    *         - Failure: Có lỗi trong quá trình ngắt kết nối
    * 
    * @post Tất cả prepared statements sẽ bị hủy
    * @post Các transaction chưa commit sẽ bị rollback
    * 
    * @note An toàn khi gọi nhiều lần hoặc khi chưa có kết nối
    */
    virtual VoidResult disconnect() = 0;

    /**
    * @brief Thực thi câu lệnh SQL không có kết quả trả về (INSERT, UPDATE, DELETE).
    * 
    * @param query Câu truy vấn SQL cần thực thi
    * 
    * @return Result<bool> 
    *         - Success(true): Thực thi thành công
    *         - Failure: Syntax error, permission denied, hoặc constraint violation
    * 
    * @pre Phải có kết nối active đến database
    * @note Áp dụng cho các câu lệnh DML không trả về result set
    * @warning Không hỗ trợ parameter binding, cần cẩn thận với SQL injection
    */
    virtual Result<bool> execute(const std::string& query) = 0;

    /**
    * @brief Thực thi truy vấn SQL có kết quả trả về (SELECT).
    * 
    * @param query Câu truy vấn SQL SELECT
    * 
    * @return Result<std::unique_ptr<IDatabaseResult>> 
    *         - Success: Con trỏ tới đối tượng kết quả
    *         - Failure: Syntax error, permission denied, hoặc không có dữ liệu
    * 
    * @pre Phải có kết nối active đến database
    * @post Kết quả trả về cần được sử dụng ngay hoặc lưu trữ an toàn
    * 
    * @note Chỉ áp dụng cho các câu lệnh SELECT và stored procedures trả về result set
    * @warning Result object có thể bị invalidate khi connection bị đóng
    */
    virtual Result<std::unique_ptr<IDatabaseResult>> executeQuery(const std::string& query) = 0;

    /**
    * @brief Tạo prepared statement với các placeholder (? hoặc :param).
    * 
    * @param query Chuỗi SQL có placeholder để bind parameter
    * 
    * @return Result<int> 
    *         - Success: ID định danh duy nhất của prepared statement
    *         - Failure: Syntax error hoặc không thể parse statement
    * 
    * @pre Phải có kết nối active đến database
    * @post Statement được lưu trong memory để sử dụng nhiều lần
    * 
    * @note Placeholder sử dụng ký tự '?' (ví dụ: "SELECT * FROM users WHERE id = ?")
    * @note Statement ID có thể được sử dụng cho đến khi gọi freeStatement()
    */
    virtual Result<int> prepareStatement(const std::string& query) = 0;

    /// @name Gán giá trị tham số cho prepared statement
    /// @{
    
    /**
     * @brief Gán giá trị chuỗi cho parameter của prepared statement.
     * 
     * @param statementId ID của statement được tạo bởi prepareStatement()
     * @param paramIndex Chỉ số parameter (bắt đầu từ 1)
     * @param value Giá trị chuỗi cần gán
     * 
     * @return VoidResult 
     *         - Success: Gán giá trị thành công
     *         - Failure: Statement ID không hợp lệ hoặc param index sai
     * 
     * @pre Statement phải được tạo trước bằng prepareStatement()
     * @note Parameter index bắt đầu từ 1, không phải 0
     * @note Giá trị sẽ được tự động escape để tránh SQL injection
     */
    virtual VoidResult setString(const int& statementId, const int& paramIndex, const std::string& value) = 0;
    
    /**
     * @brief Gán giá trị số nguyên cho parameter của prepared statement.
     * 
     * @param statementId ID của statement được tạo bởi prepareStatement()
     * @param paramIndex Chỉ số parameter (bắt đầu từ 1)
     * @param value Giá trị số nguyên cần gán
     * 
     * @return VoidResult 
     *         - Success: Gán giá trị thành công
     *         - Failure: Statement ID không hợp lệ hoặc param index sai
     * 
     * @pre Statement phải được tạo trước bằng prepareStatement()
     * @note Parameter index bắt đầu từ 1, không phải 0
     */
    virtual VoidResult setInt(const int& statementId, const int& paramIndex, const int& value) = 0;
    
    /**
     * @brief Gán giá trị số thực cho parameter của prepared statement.
     * 
     * @param statementId ID của statement được tạo bởi prepareStatement()
     * @param paramIndex Chỉ số parameter (bắt đầu từ 1)
     * @param value Giá trị số thực cần gán
     * 
     * @return VoidResult 
     *         - Success: Gán giá trị thành công
     *         - Failure: Statement ID không hợp lệ hoặc param index sai
     * 
     * @pre Statement phải được tạo trước bằng prepareStatement()
     * @note Parameter index bắt đầu từ 1, không phải 0
     * @note Độ chính xác phụ thuộc vào định dạng cột trong database
     */
    virtual VoidResult setDouble(const int& statementId, const int& paramIndex, const double& value) = 0;
    
    /**
     * @brief Gán giá trị thời gian cho parameter của prepared statement.
     * 
     * @param statementId ID của statement được tạo bởi prepareStatement()
     * @param paramIndex Chỉ số parameter (bắt đầu từ 1)
     * @param value Cấu trúc thời gian std::tm
     * 
     * @return VoidResult 
     *         - Success: Gán giá trị thành công
     *         - Failure: Statement ID không hợp lệ, param index sai, hoặc thời gian không hợp lệ
     * 
     * @pre Statement phải được tạo trước bằng prepareStatement()
     * @note Parameter index bắt đầu từ 1, không phải 0
     * @note Thời gian sẽ được format theo chuẩn ISO (YYYY-MM-DD HH:MM:SS)
     */
    virtual VoidResult setDateTime(const int& statementId, const int& paramIndex, const std::tm& value) = 0;
    /// @}

    /**
    * @brief Thực thi prepared statement không trả về kết quả.
    * 
    * @param statementId ID của statement cần thực thi
    * 
    * @return Result<bool> 
    *         - Success(true): Thực thi thành công
    *         - Failure: Statement không tồn tại, thiếu parameter, hoặc SQL error
    * 
    * @pre Statement phải được prepare và tất cả parameter phải được bind
    * @note Áp dụng cho INSERT, UPDATE, DELETE statements
    * @note Statement có thể được thực thi nhiều lần với parameter khác nhau
    */
    virtual Result<bool> executeStatement(const int& statementId) = 0;

    /**
    * @brief Thực thi prepared statement có trả về kết quả.
    * 
    * @param statementId ID của statement cần thực thi
    * 
    * @return Result<std::unique_ptr<IDatabaseResult>> 
    *         - Success: Con trỏ tới kết quả trả về
    *         - Failure: Statement không tồn tại, thiếu parameter, hoặc SQL error
    * 
    * @pre Statement phải được prepare và tất cả parameter phải được bind
    * @post Kết quả trả về cần được xử lý ngay hoặc lưu trữ an toàn
    * 
    * @note Áp dụng cho SELECT statements và stored procedures
    * @note Statement có thể được thực thi nhiều lần với parameter khác nhau
    */
    virtual Result<std::unique_ptr<IDatabaseResult>> executeQueryStatement(const int& statementId) = 0;

    /**
    * @brief Giải phóng tài nguyên của prepared statement.
    * 
    * @param statementId ID của statement cần giải phóng
    * 
    * @return VoidResult 
    *         - Success: Giải phóng thành công hoặc statement không tồn tại
    *         - Failure: Có lỗi trong quá trình giải phóng
    * 
    * @post Statement ID sẽ không còn hợp lệ sau khi gọi
    * @note An toàn khi gọi với ID không tồn tại
    * @note Nên gọi để giải phóng memory khi không cần statement nữa
    */
    virtual VoidResult freeStatement(const int& statementId) = 0;

    /**
    * @brief Lấy ID của bản ghi được chèn gần nhất.
    * 
    * @return Result<int> 
    *         - Success: ID của bản ghi vừa INSERT (auto-increment)
    *         - Failure: Không có INSERT nào hoặc table không có auto-increment
    * 
    * @pre Phải có ít nhất một INSERT statement được thực thi thành công
    * @note Chỉ áp dụng cho tables có cột AUTO_INCREMENT
    * @note Giá trị chỉ có ý nghĩa trong session hiện tại
    */
    virtual Result<int> getLastInsertId() = 0;

    /**
    * @brief Kiểm tra trạng thái kết nối.
    * 
    * @return Result<bool> 
    *         - Success(true): Đang có kết nối active
    *         - Success(false): Chưa kết nối hoặc đã mất kết nối
    *         - Failure: Có lỗi khi kiểm tra trạng thái
    * 
    * @note Không ảnh hưởng đến trạng thái kết nối hiện tại
    * @note Có thể được gọi bất cứ lúc nào mà không cần lo lắng về exception
    */
    virtual Result<bool> isConnected() const = 0;

    /**
    * @brief Trả về thông báo lỗi gần nhất.
    * 
    * @return Result<std::string> 
    *         - Success: Chuỗi mô tả lỗi (có thể rỗng nếu không có lỗi)
    *         - Failure: Hiếm khi xảy ra, chỉ khi có lỗi hệ thống nghiêm trọng
    * 
    * @note Thông báo lỗi được cập nhật sau mỗi operation thất bại
    * @note Hữu ích cho debugging và logging
    */
    virtual Result<std::string> getLastError() const = 0;

    /// @name Transaction Control
    /// @{
    
    /**
     * @brief Bắt đầu một transaction mới.
     * 
     * @return Result<bool> 
     *         - Success(true): Transaction được bắt đầu thành công
     *         - Failure: Không có kết nối hoặc đã có transaction active
     * 
     * @pre Phải có kết nối active và chưa có transaction nào đang chạy
     * @post Tất cả operations tiếp theo sẽ nằm trong transaction
     * 
     * @note Transaction sẽ được rollback tự động nếu connection bị đóng
     * @note Nên luôn kết thúc transaction bằng commit() hoặc rollback()
     */
    virtual Result<bool> beginTransaction() = 0;
    
    /**
     * @brief Commit transaction hiện tại, lưu tất cả thay đổi.
     * 
     * @return Result<bool> 
     *         - Success(true): Commit thành công, thay đổi được lưu vĩnh viễn
     *         - Failure: Không có transaction active hoặc conflict xảy ra
     * 
     * @pre Phải có transaction active được tạo bởi beginTransaction()
     * @post Transaction kết thúc, có thể bắt đầu transaction mới
     * 
     * @note Một khi commit thành công, không thể rollback các thay đổi
     * @warning Nếu có conflict, transaction sẽ được rollback tự động
     */
    virtual Result<bool> commitTransaction() = 0;
    
    /**
     * @brief Rollback transaction hiện tại, hủy tất cả thay đổi.
     * 
     * @return Result<bool> 
     *         - Success(true): Rollback thành công, thay đổi được hủy
     *         - Failure: Không có transaction active hoặc lỗi hệ thống
     * 
     * @pre Phải có transaction active được tạo bởi beginTransaction()
     * @post Transaction kết thúc, database trở về trạng thái trước khi begin
     * 
     * @note An toàn khi gọi nhiều lần hoặc khi không có transaction
     * @note Tất cả thay đổi trong transaction sẽ bị hủy bỏ hoàn toàn
     */
    virtual Result<bool> rollbackTransaction() = 0;
    /// @}
};

#endif