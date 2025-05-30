## III. Hướng dẫn chạy chương trình

### 1. Yêu cầu hệ thống

#### Phần mềm cần thiết:
- **Docker và Docker Compose**: Để chạy cơ sở dữ liệu MySQL
- **CMake**: Phiên bản 3.10 trở lên
- **C++ Compiler**: Hỗ trợ C++23 (GCC 11+, Clang 14+, MSVC 2022+)
- **wxWidgets**: Thư viện GUI
- **MySQL Connector/C++**: Để kết nối cơ sở dữ liệu

#### Cài đặt dependencies theo hệ điều hành:

**macOS (sử dụng Homebrew):**
```bash
# Cài đặt Docker Desktop từ https://www.docker.com/products/docker-desktop
brew install cmake
brew install wxwidgets
brew install mysql-connector-c++
```

**Ubuntu/Debian:**
```bash
# Cài đặt Docker
sudo apt update
sudo apt install docker.io docker-compose-plugin
sudo systemctl start docker
sudo usermod -aG docker $USER

# Cài đặt các dependencies khác
sudo apt install cmake build-essential
sudo apt install libwxgtk3.0-gtk3-dev
sudo apt install libmysqlcppconn-dev
```

**Windows:**
```bash
# Cài đặt Docker Desktop từ https://www.docker.com/products/docker-desktop
# Cài đặt CMake từ https://cmake.org/download/
# Cài đặt wxWidgets từ https://www.wxwidgets.org/downloads/
# Cài đặt MySQL Connector/C++ từ https://dev.mysql.com/downloads/connector/cpp/
```

### 2. Thiết lập cơ sở dữ liệu

#### Bước 1: Khởi động MySQL bằng Docker
```bash
# Di chuyển đến thư mục source
cd source

# Khởi động container MySQL
docker-compose up -d

# Kiểm tra trạng thái container
docker-compose ps
```

#### Bước 2: Xác minh kết nối cơ sở dữ liệu
```bash
# Kiểm tra logs để đảm bảo MySQL đã khởi động thành công
docker-compose logs mysql

# Kết nối vào MySQL để kiểm tra
docker exec -it airlines_mysql mysql -u cuong116 -p1162005 airlines_db
```

#### Thông tin kết nối cơ sở dữ liệu:
- **Host**: localhost
- **Port**: 3306
- **Database**: airlines_db
- **Username**: cuong116
- **Password**: 1162005

### 3. Biên dịch chương trình

#### Bước 1: Tạo thư mục build
```bash
# Từ thư mục gốc của project
mkdir build
cd build
```

#### Bước 2: Cấu hình CMake
```bash
# Cấu hình project
cmake ../source

# Hoặc nếu cần chỉ định đường dẫn cụ thể cho dependencies:
cmake -DMYSQLCPPCONN_DIR=/path/to/mysql-connector-c++ ../source
```

#### Bước 3: Biên dịch
```bash
# Biên dịch chương trình chính
make AirlinesManagement

# Hoặc biên dịch tất cả (bao gồm tests nếu được bật)
make all
```

#### Biên dịch với tests (tùy chọn):
```bash
# Cài đặt Google Test trước
# macOS: brew install googletest
# Ubuntu: sudo apt install libgtest-dev
# Windows: vcpkg install gtest

# Cấu hình với tests
cmake -DBUILD_TESTS=ON ../source
make all

# Chạy tests
make test
# hoặc
ctest
```

### 4. Chạy chương trình

#### Bước 1: Khởi động ứng dụng
```bash
# Từ thư mục build
./AirlinesManagement
```

#### Bước 2: Sử dụng giao diện
- Giao diện wxWidgets sẽ mở ra
- Chương trình sẽ tự động kết nối đến cơ sở dữ liệu MySQL
- Dữ liệu mẫu đã được tải sẵn trong database

### 5. Xử lý sự cố

#### Lỗi kết nối cơ sở dữ liệu:
```bash
# Kiểm tra container MySQL có đang chạy không
docker-compose ps

# Restart container nếu cần
docker-compose restart mysql

# Kiểm tra logs
docker-compose logs mysql
```

#### Lỗi biên dịch MySQL Connector:
```bash
# Thiết lập biến môi trường nếu cần
export MYSQLCPPCONN_DIR=/path/to/mysql-connector-c++

# Hoặc chỉ định trực tiếp trong CMake
cmake -DMYSQLCPPCONN_DIR=/usr/local/opt/mysql-connector-c++ ../source
```

#### Lỗi wxWidgets:
```bash
# Kiểm tra wxWidgets đã được cài đặt
wx-config --version

# Nếu chưa có, cài đặt lại
# macOS: brew reinstall wxwidgets
# Ubuntu: sudo apt reinstall libwxgtk3.0-gtk3-dev
```

### 6. Dừng hệ thống

#### Dừng ứng dụng:
- Đóng cửa sổ GUI hoặc nhấn Ctrl+C trong terminal

#### Dừng cơ sở dữ liệu:
```bash
# Dừng container MySQL
docker-compose down

# Dừng và xóa volumes (cảnh báo: sẽ mất dữ liệu)
docker-compose down -v
```

### 7. Cấu trúc dữ liệu

Hệ thống đã được tải với dữ liệu mẫu bao gồm:
- **5 máy bay** với các loại ghế khác nhau
- **12 chuyến bay** với các tuyến đường trong nước và quốc tế
- **10 hành khách** với thông tin đầy đủ
- **17 vé máy bay** với các trạng thái khác nhau

#### Các chuyến bay mẫu:
- **VN100/101**: SGN ↔ HAN (Boeing 787-9)
- **VN200/201**: SGN/HAN → DAD (Airbus A321neo)
- **VN300/301**: SGN/HAN → PQC
- **VN400**: SGN → CXR
- **VN500**: SGN → ICN (Seoul)
- **VN600**: HAN → NRT (Tokyo)

### 8. Tính năng chính

Chương trình hỗ trợ các chức năng:
- Quản lý chuyến bay và lịch trình
- Đặt vé và quản lý hành khách
- Quản lý máy bay và sơ đồ ghế ngồi
- Theo dõi trạng thái vé và chuyến bay
- Báo cáo và thống kê