# AIRLINES RESERVATION SYSTEM
## I. Thành viên nhóm:
| Họ tên | MSSV | Nhóm trưởng |
|-|:-:|:-:|
| Nguyễn Phúc Hoàng | 23120264 | `X` |
| Dương Nhật Cường | 23120223 | |

## II. Phân công công việc
### 1. Phân công chi tiết
|STT|Công việc|Phân công|Tỷ lệ hoàn thiện|
|:-:|-|:-:|:-:|
|1|A|A|100%|

### 2. Tỉ lệ đóng góp vào dự án

### 3. Tỉ lệ điểm của các thành viên:

## III. Cách thức đánh giá
### 1. Mô tả chủ đề

### 2. Mô tả áp dụng Lập trình hướng đối tượng

### 3. Mô tả các cải tiến áp dụng
#### 3.1. Nguyên tắc SOLID

#### 3.2. Design Pattern

#### 3.3. Nguyên lý Dependency Injection

### 4. Mô tả Kiến trúc phần mềm

### 5. Chất lượng mã nguồn
#### 5.1. Quản lý mã nguồn với Git

#### 5.2. Coding Convention

**Quy ước đặt tên**
| Đối tượng | Quy ước | Ví dụ |
|-|-|-|
| Biến thường, public | `camelCase` | `exitSurity`, `userName`|
| Biến private, protected | `_camelCase` | `_flightName`, `_passport`|
| Hằng số | `UPPER_SNAKE_CASE` | `MYSQL_ROW` |
| Hàm | `camelCase` | `DisplayPassenger()`, `ReserveSeat()`|
| Lớp | `PascalCase` | `Ticket`, `Flight` |
| Tên folder và hàm `main.cpp` | `lowercase` | `presentation`, `business`, `data`, `models`|
| Tên file | `PascalCase` | `FlightSystem.cpp`, `ApplicationUI.h`|
<!-- | Namespace | `snake_case` | `music_store::utils` |
| Enum class | `PascalCase` | `enum class MusicType { Pop, Rock }` | -->
\
**Cấu trúc và định dạng mã nguồn**
- Dùng tab với kích thước **4 khoảng trắng**
- Đóng/ mở code blocks theo style:
    ```cpp
    if (a == b) {
        // Code here
    } 
    else {
        // Do sth
    }
    ```
- Thêm 1 dòng trống giữa các hàm và các khối logic rõ ràng.
    ```cpp
    int first_number;
    int second_number;
    int sum_of_two_numbers;

    first_number = 5;
    sencond_number = 10;

    sum_of_two_numbers = first_number + second_number;
    ```
- Không viết nhiều câu lệnh trên cùng 1 dòng.
    ```cpp
    int a, b, c; // Không dùng

    // Dùng
    int a;
    int b;
    int c;
    ```
- Khối if/else/for/while luôn có {} dù chỉ có 1 dòng.
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
- Khởi tạo giá trị cho biến trước khi dùng:
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

#### 5.3. Kiểm thử Unit Test

### 6. Mô hình hoá
#### 6.1. Class Diagram

#### 6.2. Sơ đồ Use-case

#### 6.3. Lược đồ ER

### 7. Mô tả mã nguồn

## IV. Hướng dẫn chạy chương trình

## V. Link video demo