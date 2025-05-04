# AIRLINES RESERVATION SYSTEM
---
## I. Thành viên nhóm:
| Họ tên | MSSV | Nhóm trưởng |
|-|:-:|:-:|
| Nguyễn Phúc Hoàng | 23120264 | `X` |
| Dương Nhật Cường | 23120223 | |

---
## II. Phân công công việc
Phân công chi tiết được trình bày trong spreadsheet  [milestone](https://docs.google.com/spreadsheets/d/10Fg3uumLuMZ6uxxLJZAJpDxn2EyuMy1-oPprR4_YdoU/edit?usp=sharing)

### Mô tả spreadsheet phân công:

| Tên sheet         | Nội dung chính|
|:-:|-|
| **M1**            | Phân tích yêu cầu & thiết kế hệ thống                                    |
| **M2.1**          | Thiết kế cơ sở dữ liệu và kết nối dữ liệu với chức năng                  |
| **M2.2**          | Cài đặt các chức năng (CRUD)                                             |
| **M3**            | Kiểm thử và sửa lỗi                                                      |
| **M4**            | Báo cáo và hoàn thiện hệ thống                                           |
| **ĐÁNH GIÁ**      | Thống kê số task mỗi thành viên nhận và hoàn thành (tính tỷ lệ đóng góp) |

### Tỉ lệ điểm của các thành viên:
*(Dựa vào tỉ lệ đóng góp)*

---
## III. Cách thức đánh giá
### 1. Teamwork (15%):
#### 1.1. Giao tiếp:
- Trao đổi thông tin qua và họp qua Discord.
- Nhóm tổ chức họp định kỳ mỗi tuần một lần (trừ các tuần dính lịch thi học kỳ).
- Thư mục lưu trữ biên bản họp: [BIÊN BẢN HỌP]()

#### 1.2. Theo dõi tiến độ:
- Thực hiện phân công theo mô hình phân chia [milestone](https://fastwork.vn/milestone-la-gi/).
- Lập kế hoạch: Chia nhỏ công việc theo các Milestone (1-2 tuần), theo dõi tiến độ bằng Google Sheet.
- Kế hoạch dự án: [KẾ HOẠCH DỰ ÁN]()
- Phân công công việc: [PHÂN CÔNG CÔNG VIỆC]()
- Workspace: [ĐỒ ÁN OOP]()

#### 1.3. Quản lý mã nguồn bằng GIT:
- GitHub được sử dụng để quản lý source code, phân nhánh theo tính năng và thành viên lập trình.
- Link Github: https://github.com/nphoang-itus/OOP-project.git
#### Mô tả thống kê git:
###### a. Theo dõi đóng góp Git
- Để đảm bảo phân công công việc hiệu quả, nhóm sử dụng Git để theo dõi tiến độ. Dưới đây là thống kê commit của từng thành viên:

    | Thành viên | Tổng số commit | Các commit chính | Tỷ lệ đóng góp |Ngày Commit |
    | - | :-: | :-: | - | - |
    | Nguyễn Phúc Hoàng | | | |
    | Dương Nhật Cường | | | |

*(Bổ sung sau: Thống kê git theo tuần và tạo lập biểu đồ)*

### 2. UI/UX:
#### 2.1. Mô tả chủ đề
**Airline Reservation System** là một chương trình C++ kết nối với cơ sở dữ liệu MySQL, giúp quản lý một hệ thống đặt vé máy bay cho khách hàng. Hệ thống cho phép người dùng thực hiện các chức năng như tìm kiếm chuyến bay, đặt vé, hủy vé và quản lý thông tin hành khách. Chương trình sử dụng MySQL để lưu trữ thông tin về chuyến bay, hành khách và đơn hàng.

*Link tham khảo mã nguồn: https://www.codewithc.com/airlines-reservation-system-in-c-with-mysql/*
##### Các chức năng chính của hệ thống

- **Reverse Seat**: Cho phép hành khách đặt chỗ trên chuyến bay, hủy chỗ và quản lý thông tin hành khách (Tên, Số điện thoại, Email, Passport, ...).
- **User Ticket**: Cho phép tìm kiếm thông tin vé theo hành khách và hiển thị thông tin vé máy bay đã đặt của hành khách, bao gồm thông tin chuyến bay, ghế ngồi, thời gian bay, và trạng thái vé (đã xác nhận, đã hủy).
- **Flight Schedule**: Hiển thị lịch trình chuyến bay (gồm số hiệu, điểm đến, giờ khởi hành, giờ dự kiến đến, ...).
- **Display Passenger**: Hiển thị danh sách của các hành khách đã đặt vé trên chuyến bay cụ thể.
- **Add New Flight**: Thêm một chuyến bay mới.
- **Edit Flight**: Chỉnh sửa thông tin chuyến bay.
- **Delete Flight**: Xóa chuyến bay khỏi hệ thống.
- **Edlit Flight Leave and Arrive**: Sửa thông tin chuyến bay (thời gian khởi hành và thời gian đến).

#### 2.2. Cài đặt lại chức năng của mã nguồn mẫu theo hướng đối tượng (3%)
**a. Quản lý chuyến bay**:
- Xem danh sách chuyến bay
- Tìm kiếm chuyến bay theo mã
- Tìm kiếm chuyến bay theo điểm đi/đến
- Thêm/sửa/xóa chuyến bay

**b. Quản lý hành khách**:
- Xem danh sách hành khách
- Tìm kiếm hành khách theo mã
- Tìm kiếm hành khách theo số hộ chiếu
- Thêm/sửa/xóa hành khách

**c. Quản lý đặt chỗ**:
- Xem danh sách đặt chỗ
- Tìm kiếm đặt chỗ theo mã
- Tìm kiếm đặt chỗ theo mã chuyến bay
- Tìm kiếm đặt chỗ theo số hộ chiếu
- Thêm/sửa/xóa đặt chỗ

#### 2.3. Các chức năng thêm và cải tiến (14%)
***(Dự kiến)***

**a. Hệ thống xác thực người dùng**:
- Đăng nhập/đăng xuất
- Phân quyền (Admin, Staff, User)
- Quản lý tài khoản

**b. Quản lý giá vé**:
- Hỗ trợ nhiều loại giá vé (Economy, Business, First Class)
- Áp dụng khuyến mãi theo thời gian
- Tính toán giá vé dựa trên thời gian đặt

**c. Quản lý ghế ngồi**:
- Hiển thị sơ đồ ghế ngồi
- Cho phép chọn ghế khi đặt vé
- Quản lý trạng thái ghế

**d. Thống kê và báo cáo**:
- Thống kê doanh thu theo thời gian
- Thống kê số lượng hành khách theo chuyến bay
- Báo cáo tỉ lệ lấp đầy của các chuyến bay

**e. Quản lý thanh toán**:
- Hỗ trợ nhiều phương thức thanh toán
- Lưu trữ lịch sử thanh toán
- Xuất hóa đơn

**f. Thông báo và nhắc nhở**:
- Gửi email xác nhận đặt vé
- Nhắc nhở check-in
- Thông báo thay đổi lịch bay

**g. Tìm kiếm nâng cao**:
- Tìm kiếm chuyến bay theo nhiều tiêu chí
- Lọc kết quả theo giá, thời gian, hãng hàng không
- Sắp xếp kết quả theo nhiều tiêu chí

### 2.4. Giao diện bố trí hợp lý (3%)
*(Bổ sung sau: Có thể làm interface sử dụng thư viên đồ hoạ SFML)*

### 3. Kiến trúc phần mềm + design pattern (20%)
#### 3.1. Mô tả kiến trúc phần mềm
Chúng em đã áp dụng **kiến trúc phân lớp (Layered Architecture)** với các lớp sau:

##### Presentation Layer (UI Layer):
- Chứa các lớp UI như `Menu`, `FlightUI`, `PassengerUI`, và `ReservationUI`.
- Chịu trách nhiệm tương tác với người dùng.

##### Service Layer (Business Logic Layer):
- Chứa các lớp Service như `FlightService`, `PassengerService`, và `ReservationService`.
- Chịu trách nhiệm xử lý logic nghiệp vụ.

##### **Repository Layer (Data Access Layer)**:
- Chứa các lớp Repository như `FlightRepository`, `PassengerRepository`, và `ReservationRepository`.
- Chịu trách nhiệm truy cập dữ liệu.

##### **Domain Layer (Core Layer)**:
- Chứa các lớp Domain như `Flight`, `Passenger`, và `Reservation`.
- Đại diện cho các thực thể chính của hệ thống.

##### **Infrastructure Layer**:
- Chứa các lớp Infrastructure như `DatabaseConnection`, `MySQLConnection`, `Logger`, và các lớp tiện ích khác.
- Cung cấp các dịch vụ cơ sở cho các lớp khác.

##### Cấu trúc thư mục phản ánh kiến trúc này:

```plaintext
src/
├── core/           # Domain Layer
├── database/       # Infrastructure Layer (Database)
├── repositories/   # Repository Layer
├── services/       # Service Layer
├── ui/             # Presentation Layer
└── utils/          # Infrastructure Layer (Utilities)
```

#### 3.2. Nguyên tắc và nguyên lý OOP
##### **Tính đóng gói (Encapsulation)**:

- Tất cả các thuộc tính của lớp đều được khai báo là `private`
- Chỉ có thể truy cập thông qua các phương thức getter/setter
- Ví dụ: Lớp `Flight` có thuộc tính `private` và các phương thức `getFlightNo()`, `setFlightNo()`

##### **Tính kế thừa (Inheritance)**:
- Sử dụng kế thừa để tái sử dụng code
- Ví dụ: `MySQLConnection` kế thừa từ `DatabaseConnection`

##### **Tính đa hình (Polymorphism)**:
- Sử dụng đa hình để xử lý các đối tượng khác nhau theo cùng một cách
- Ví dụ: `DatabaseConnection` có thể là `MySQLConnection` hoặc `PostgreSQLConnection`

##### **Tính trừu tượng (Abstraction)**:
- Sử dụng interface và lớp trừu tượng để định nghĩa hành vi
- Ví dụ: `DatabaseConnection` là một interface định nghĩa các phương thức cần thiết

#### 3.3. Nguyên tắc SOLID

- S: Single Responsibility Principle (SRP): Mỗi lớp chỉ có một trách nhiệm duy nhất.
- Ví dụ: Lớp `Flight` chỉ quản lý thông tin chuyến bay. Lớp Passenger chỉ quản lý thông tin hành khách.

- O: Open/Closed Principle (OCP): Các lớp nên mở cho việc mở rộng nhưng đóng cho việc sửa đổi.
- Ví dụ: Thêm một lớp `FlightService` mới mà không cần sửa đổi lớp `Flight`.

- L: Liskov Substitution Principle (LSP): Các lớp con nên có thể thay thế cho lớp cha mà không làm thay đổi tính đúng đắn của chương trình.

- Ví dụ: `MySQLConnection` có thể thay thế cho `DatabaseConnection` mà không làm thay đổi cách thức hoạt động của chương trình.

- I: Interface Segregation Principle (ISP): Không nên ép buộc một lớp phải triển khai các phương thức mà nó không sử dụng.

- Ví dụ: `DatabaseConnection` chỉ định nghĩa các phương thức cần thiết cho việc kết nối cơ sở dữ liệu.

- D: Dependency Inversion Principle (DIP): Các lớp cao cấp không nên phụ thuộc vào các lớp thấp cấp. Cả hai nên phụ thuộc vào các abstraction.

- Ví dụ: `FlightService` phụ thuộc vào `FlightRepository` thông qua interface `RepositoryInterface`, không phụ thuộc vào lớp cụ thể.

#### 3.4. Design Patterns

##### **Repository Pattern**:
- Tách biệt logic truy cập dữ liệu khỏi logic nghiệp vụ
- Ví dụ: `FlightRepository`, `PassengerRepository`, `ReservationRepository`
*(Note: Bổ sung lại cụ thể ko nêu ví dụ)*

##### **Singleton Pattern**:
- Đảm bảo chỉ có một thể hiện duy nhất của một lớp
- Ví dụ: `Logger` được triển khai như một singleton
*(Note: Bổ sung lại cụ thể ko nêu ví dụ)*

##### **Factory Method Pattern**:
- Tạo đối tượng mà không cần chỉ định lớp cụ thể
- Ví dụ: `DatabaseConnectionFactory` tạo `DatabaseConnection` dựa trên cấu hình
*(Note: Bổ sung lại cụ thể ko nêu ví dụ)*

### 4. Đảm bảo chất lượng (20%)
#### 4.1. Unit Test
#### Test Plan
###### Mục tiêu:
- Kiểm tra tính đúng đắn của các lớp và phương thức
- Đảm bảo các lớp hoạt động đúng khi tích hợp với nhau
- Phát hiện và sửa lỗi sớm trong quá trình phát triển

###### Phạm vi:
- Unit tests cho các lớp core: `Flight`, `Passenger`, `Reservation`
- Unit tests cho các lớp service: `FlightService`, `PassengerService`, `ReservationService`
- Unit tests cho các lớp repository: `FlightRepository`, `PassengerRepository`, `ReservationRepository`
- Unit tests cho các lớp utility: `Logger`, `InputValidator`

###### Công cụ:
- Google Test (GTest) cho unit testing
- CMake để build và chạy tests

#### Test Cases
*(Bổ sung sau)*

#### Test Report
*(Bổ sung sau)*

#### 4.2. Coding Convention

**Quy ước đặt tên**
| Đối tượng | Quy ước | Ví dụ |
|-|-|-|
| Biến thường, biến public | `camelCase` | `exitSurity`, `userName`|
| Biến private, protected | `_underlineCamelCase` | `_flightName`, `_passport`|
| Hằng số | `UPPER_SNAKE_CASE` | `MYSQL_ROW` |
| Hàm | `camelCase` | `DisplayPassenger()`, `ReserveSeat()`|
| Lớp | `PascalCase` | `Ticket`, `Flight` |
| Tên folder và hàm `main.cpp` | `lowercase` | `presentation`, `business`, `data`, `models`|
| Tên file | `PascalCase` | `FlightSystem.cpp`, `ApplicationUI.h`|
| Namespace | `snake_case` | `music_store::utils` |
| Enum class | `PascalCase` | `enum class MusicType { Pop, Rock }` |

**Cấu trúc và định dạng mã nguồn**
- Dùng tab với kích thước **4 khoảng trắng**.
    ```cpp
    if (a == b) {
        //Code here
    }
    ```
- Nên đóng/ mở code blocks theo style:
    ```cpp
    if (a == b) {
        // Code here
    } 
    else {
        // Do sth
    }
    ```
- Nên thêm 1 dòng trống giữa các hàm và các khối logic rõ ràng.
    ```cpp
    int first_number;
    int second_number;
    int sum_of_two_numbers;

    first_number = 5;
    sencond_number = 10;

    sum_of_two_numbers = first_number + second_number;
    ```
- Không nên viết nhiều câu lệnh trên cùng 1 dòng.
    ```cpp
    int a, b, c; // Không nên

    // Nên
    int a;
    int b;
    int c;
    ```
- Khối if/else/for/while nên có {} dù chỉ có 1 dòng.
    ```cpp
    if (a > b) {
        return a;
    }
    ```
- Không dùng `using namespace std;`.
    ```cpp
    std::cout << "Hello world!\n";
    int a = 0;
    std::cin >> a;
    ```
- Nên khởi tạo giá trị cho biến trước khi dùng:
    ```cpp
    // Không nên
    int a;
    std::cin >> a;

    // Nên
    int a = 0;
    std::cin >> a;
    ```
- Operators và operands phải có được ngăn cách bởi dấu ' '
    ```cpp
    // Không nên
    for (int i=0;i<n;++i) 
    {
        // Do sth
    }

    // Nên
    for (int i = 0; i < n; ++i) 
    {
        // Do sth
    }
    ```
- Code có nhiều điều kiện thì nên xuống dòng để dễ kiểm tra
    ```cpp
    // Không nên
    if (a == 1 && b == 2 && c == 3 && d == 4) 
    {
        // Do sth
    }

    // Nên
    if (a == 1 && 
        b == 2 && 
        c == 3 && 
        d == 4) 
    {
        // Do sth
    }
    ```
\
**Quy ước tổ chức file**
```plaintext
├── CMakeLists.txt                      # Tệp cấu hình CMake để biên dịch dự án
├── docker-compose.yml                  # Cấu hình Docker Compose để triển khai MySQL server
├── schema.sql                          # Tệp SQL khởi tạo cơ sở dữ liệu và các bảng
├── Doxyfile                            # Tệp cấu hình cho Doxygen để tạo tài liệu mã nguồn
├── build/                              # Thư mục chứa kết quả build (do CMake tạo)
├── docs/                               # Thư mục chứa tài liệu (được tạo bởi Doxygen hoặc người viết tay)
├── src/                                # Mã nguồn chính của ứng dụng
│   ├── Main.cpp                        # Điểm bắt đầu (entry point) của chương trình
│   ├── core/                           # Các lớp thực thể (domain entities)
│   │   ├── Flight.h                    # Lớp đại diện chuyến bay
│   │   ├── Flight.cpp
│   │   ├── Passenger.h                 # Lớp đại diện hành khách
│   │   ├── Passenger.cpp
│   │   ├── Reservation.h               # Lớp đại diện đặt vé
│   │   └── Reservation.cpp
│   ├── repositories/                   # Tầng truy xuất dữ liệu (Data Access Layer)
│   │   ├── RepositoryInterface.h       # Interface chung cho các repository
│   │   ├── FlightRepository.h          # Repository của chuyến bay
│   │   ├── FlightRepository.cpp
│   │   ├── PassengerRepository.h       # Repository của hành khách
│   │   ├── PassengerRepository.cpp
│   │   ├── ReservationRepository.h     # Repository của đặt vé
│   │   └── ReservationRepository.cpp
│   ├── services/                       # Tầng xử lý nghiệp vụ (Business Logic Layer)
│   │   ├── FlightService.h             # Xử lý logic liên quan đến chuyến bay
│   │   ├── FlightService.cpp
│   │   ├── ReservationService.h        # Xử lý logic đặt vé
│   │   └── ReservationService.cpp
│   ├── database/                       # Hạ tầng kết nối cơ sở dữ liệu
│   │   ├── DatabaseConnection.h        # Interface kết nối CSDL
│   │   ├── MySQLConnection.h           # Cài đặt cụ thể sử dụng MySQL
│   │   └── MySQLConnection.cpp
│   ├── ui/                             # Giao diện người dùng (User Interface Layer)
│   │   ├── Menu.h                      # Hệ thống menu điều hướng
│   │   ├── Menu.cpp
│   │   ├── FlightUI.h                  # Giao diện quản lý chuyến bay
│   │   ├── FlightUI.cpp
│   │   ├── ReservationUI.h             # Giao diện đặt vé
│   │   └── ReservationUI.cpp
│   └── utils/                          # Các tiện ích dùng chung
│       ├── InputValidator.h            # Hàm kiểm tra đầu vào
│       ├── InputValidator.cpp
│       ├── Exceptions.h                # Định nghĩa exception tùy chỉnh
│       └── Logger.h                    # Tiện ích ghi log
└── tests/                              # Kiểm thử đơn vị (Unit Tests)
    ├── FlightTests.cpp
    ├── ReservationTests.cpp
    └── RepositoryTests.cpp
```
**Comment Code**:
- Sử dụng comment Doxygen cho tất cả các lớp, phương thức, và thuộc tính.
- Mỗi file bắt đầu với một comment Doxygen mô tả file.
- Mỗi lớp bắt đầu với một comment Doxygen mô tả lớp.
- Mỗi phương thức bắt đầu với một comment Doxygen mô tả phương thức, tham số, và giá trị trả về.

**Xử lý lỗi**:
- Sử dụng ngoại lệ (exceptions) thay vì mã lỗi.
- Bắt và xử lý ngoại lệ ở mức thích hợp.
- Ghi log tất cả các ngoại lệ.

**Quản lý bộ nhớ**:
- Sử dụng smart pointers (`std::unique_ptr`, `std::shared_ptr`) thay vì raw pointers
- Tuân thủ [Rule of Three/Five/Zero](https://en.cppreference.com/w/cpp/language/rule_of_three).
- Tránh rò rỉ bộ nhớ bằng cách giải phóng tài nguyên trong destructor (Ví dụ như hàm `disconnect` để ngắt kết nối database, tránh việc vẫn còn kết nối khi chương trình kết thúc hoặc trong các trường hợp sử dụng raw pointer).

#### 4.3. Quản lý mã nguồn với Git
Chúng em đã sử dụng "Feature Branch Workflow" để quản lý mã nguồn với Git trên nền tảng Github:
##### **Main Branch**:
- Branch `main` chứa code ổn định nhất và sẵn sàng để triển khai.

##### **Develop Branch**:
- Branch `develop` chứa code đang phát triển với những fearture đã được hoàn thiện.

##### **Feature Branches**:
- Mỗi tính năng mới được phát triển trên một branch riêng biệt, ví dụ:
    - `feature/database_connection`: Triển khai Interface Database Connection và phát triển XAPIDev của MySQL Connector C++.
    - `feature/logger`: Triển khai Logger để ghi log của chương trình, giúp lập trình viên dễ dàng trong việc debug và theo dõi luồng hoạt động của chương trình.
    - `feature/repo_flight`: Triển khai Fight Repository hỗ trợ truy cập vào dữ liệu lấy từ Database.
    ...

##### **Pull Requests**:
- Khi một tính năng hoàn thành, tạo một *Pull Request* để merge vào branch `develop`.
- Khi toàn bộ chương trình được phát triển hoàn thiện sẽ thực hiện test trên branch `develop` và tạo một *Pull Request* để merge vào branch `main` nếu như không có bug xảy ra trong quá trình test.
- *Pull Request* được review bởi các thành viên khác trong nhóm trước khi merge.

<!-- ##### **Commit Messages**:
- Commit messages tuân theo format: `<type>: <description>`
- Các loại `<type>` bao gồm: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`
- Ví dụ: `feat: add MySQL Connector/C++ support` -->


### 5. Tài liệu mô tả kiến trúc phần mềm và các module (10%)
#### 5.1. Tài liệu sinh từ Doxygen:
- Bổ sung comment code theo tài liệu Doxygen trong mã nguồn để sinh tài liệu mô tả mã nguồn.
*(Bổ sung sau)*

#### 5.2. Mô hình hoá bằng mermaid
##### a. Class Diagram
*(NOTE: Bổ sung)*

##### b. Sơ đồ Use-case
*(NOTE: Bổ sung)*

##### c. Lược đồ ER
*(NOTE: Bổ sung)*

### 6. Các chủ đề nâng cao (15%)
*(NOTE: Bổ sung sau)*

<!-- #### 6.1. Connection Pooling
##### **Mô tả**:
- Triển khai connection pooling để tái sử dụng kết nối cơ sở dữ liệu
- Giảm thời gian thiết lập kết nối
- Tăng hiệu suất hệ thống

##### **Triển khai**:
- Sử dụng lớp `ConnectionPool` để quản lý pool kết nối
- Tự động tạo kết nối mới khi cần
- Tự động đóng kết nối không sử dụng

##### **Kết quả**:
- Giảm 70% thời gian thiết lập kết nối
- Tăng 50% số lượng truy vấn đồng thời
- Giảm 30% thời gian phản hồi

#### 6.2. Dependency Injection Container
##### **Mô tả**:
- Triển khai dependency injection container để quản lý các dependency
- Tự động tạo và inject các dependency
- Giảm sự phụ thuộc giữa các lớp

##### **Triển khai**:
- Sử dụng lớp `DIContainer` để quản lý các dependency
- Đăng ký các dependency với container
- Tự động resolve các dependency khi cần

##### **Kết quả**:
- Giảm sự phụ thuộc giữa các lớp
- Dễ dàng thay đổi triển khai của các dependency
- Dễ dàng test các lớp với mock objects

#### 6.3. Transaction Management
#### **Mô tả**:
- Triển khai quản lý transaction để đảm bảo tính toàn vẹn dữ liệu
- Tự động rollback khi có lỗi
- Hỗ trợ nested transactions

#### **Triển khai**:
- Sử dụng lớp `TransactionManager` để quản lý transactions
- Sử dụng RAII (Resource Acquisition Is Initialization) để tự động commit/rollback
- Hỗ trợ savepoints cho nested transactions

#### **Kết quả**:
- Đảm bảo tính toàn vẹn dữ liệu
- Giảm lỗi khi xử lý nhiều thao tác cơ sở dữ liệu
- Dễ dàng debug khi có lỗi

#### 6.4. Caching
##### **Mô tả**:
- Triển khai caching để giảm số lượng truy vấn cơ sở dữ liệu
- Tự động invalidate cache khi dữ liệu thay đổi
- Hỗ trợ nhiều cấp độ cache (memory, disk)

##### **Triển khai**:
- Sử dụng lớp `CacheManager` để quản lý cache
- Sử dụng LRU (Least Recently Used) algorithm để quản lý bộ nhớ cache
- Hỗ trợ time-to-live (TTL) cho các mục cache

##### **Kết quả**:
- Giảm 80% số lượng truy vấn cơ sở dữ liệu cho các dữ liệu thường xuyên truy cập
- Giảm 60% thời gian phản hồi cho các truy vấn phổ biến
- Tăng khả năng mở rộng của hệ thống

#### 6.5. Multithreading
##### **Mô tả**:
- Triển khai multithreading để tăng hiệu suất hệ thống
- Xử lý nhiều yêu cầu đồng thời
- Tránh blocking I/O

##### **Triển khai**:
- Sử dụng thread pool để quản lý các thread
- Sử dụng std::async và std::future để xử lý các tác vụ bất đồng bộ
- Sử dụng std::mutex và std::condition_variable để đồng bộ hóa

##### **Kết quả**:
- Tăng 200% số lượng yêu cầu đồng thời
- Giảm 50% thời gian phản hồi cho các tác vụ I/O-bound
- Tăng khả năng mở rộng của hệ thống -->
---
## IV. Hướng dẫn chạy chương trình
*(Bổ sung sau)*
---
## V. Link video demo
*(Bổ sung sau)*