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

| Tên sheet | Nội dung chính|
|:-:|-|
| **M1** | Phân tích yêu cầu & thiết kế hệ thống |
| **M2** | Thiết kế cơ sở dữ liệu và kết nối dữ liệu với chức năng và cài đặt các chức năng (CRUD) |
| **M3** | Kiểm thử và sửa lỗi |
| **M4** | Báo cáo và hoàn thiện hệ thống |
| **OOP** | Danh sách các mục áp dụng SOLID, Design Pattern, Dependency Injection trong đồ án |
| **ĐÁNH GIÁ** | Thống kê số task mỗi thành viên nhận và hoàn thành (tính tỷ lệ đóng góp) |

### Tỉ lệ điểm của các thành viên:
*(Dựa vào tỉ lệ đóng góp)*

---
## III. Cách thức đánh giá
### 1. Teamwork (15%):
#### 1.1. Giao tiếp:
- Trao đổi thông tin qua và họp qua Discord.
- Nhóm tổ chức họp định kỳ mỗi tuần một lần (trừ các tuần dính lịch thi học kỳ).
- Thư mục lưu trữ biên bản họp: [BIÊN BẢN HỌP](https://drive.google.com/drive/folders/1H37agCGiqLQLTfkh5w6cz3WedyLYk4zb?usp=drive_link)

#### 1.2. Theo dõi tiến độ:
- Thực hiện phân công theo mô hình phân chia [milestone](https://fastwork.vn/milestone-la-gi/).
- Lập kế hoạch: Chia nhỏ công việc theo các Milestone (1-2 tuần), theo dõi tiến độ bằng Google Sheet.
- Kế hoạch dự án: [KẾ HOẠCH DỰ ÁN](https://docs.google.com/spreadsheets/d/17MIbYC_3728fK3oDs7YA8lyq8wD4hYn5dRZaQUcmwqk/edit?usp=sharing)
- Phân công công việc: [PHÂN CÔNG CÔNG VIỆC](https://docs.google.com/spreadsheets/d/10Fg3uumLuMZ6uxxLJZAJpDxn2EyuMy1-oPprR4_YdoU/edit?usp=sharing)
- Workspace: [ĐỒ ÁN OOP](https://drive.google.com/drive/folders/1MYWCPn5Pc7lfP3YCtnfkBv9NWD09XwXE?usp=drive_link) và Discord

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

- **Hệ thống cũ (Từ mã nguồn trên):**
  - **Reverse Seat**: Cho phép hành khách đặt chỗ trên chuyến bay, hủy chỗ và quản lý thông tin hành khách (Tên, Số điện thoại, Email, Passport, ...).
  - **User Ticket**: Cho phép tìm kiếm thông tin vé theo hành khách và hiển thị thông tin vé máy bay đã đặt của hành khách, bao gồm thông tin chuyến bay, ghế ngồi, thời gian bay, và trạng thái vé (đã xác nhận, đã hủy).
  - **Flight Schedule**: Hiển thị lịch trình chuyến bay (gồm số hiệu, điểm đến, giờ khởi hành, giờ dự kiến đến, ...).
  - **Display Passenger**: Hiển thị danh sách của các hành khách đã đặt vé trên chuyến bay cụ thể.
  - **Add New Flight**: Thêm một chuyến bay mới.
  - **Edit Flight**: Chỉnh sửa thông tin chuyến bay.
  - **Delete Flight**: Xóa chuyến bay khỏi hệ thống.
  - **Edlit Flight Leave and Arrive**: Sửa thông tin chuyến bay (thời gian khởi hành và thời gian đến).

- **Hệ thống mới (Bổ sung thêm vào dự án):**
  1.  **Quản lý Máy bay (Aircraft Management):**
      *   **Thêm máy bay mới:** Tạo thông tin máy bay mới với số hiệu (`AircraftSerial`), mẫu mã (`AircraftModel`), và sơ đồ ghế (`SeatClassMap`).
      *   **Xem thông tin máy bay:**
          *   Xem danh sách tất cả máy bay.
          *   Xem chi tiết một máy bay cụ thể (theo ID hoặc số hiệu).
          *   Kiểm tra số lượng ghế theo từng hạng ghế.
          *   Kiểm tra tính hợp lệ của một số ghế cụ thể trên máy bay.
      *   **Cập nhật thông tin máy bay:** Thay đổi mẫu mã, sơ đồ ghế của máy bay.
      *   **Xóa máy bay:** Loại bỏ máy bay khỏi hệ thống (có thể có các ràng buộc nếu máy bay đang được sử dụng cho chuyến bay).
      *   **Kiểm tra sự tồn tại của máy bay:** Dựa trên ID hoặc số hiệu.
      *   **Đếm số lượng máy bay.**

  2.  **Quản lý Chuyến bay (Flight Management):**
      *   **Tạo lịch trình chuyến bay mới:**
          *   Xác định số hiệu chuyến bay (`FlightNumber`).
          *   Xác định lộ trình (`Route` - điểm đi, điểm đến).
          *   Lên lịch trình (`Schedule` - thời gian khởi hành, thời gian đến).
          *   Gán một máy bay cụ thể (`Aircraft`) cho chuyến bay.
          *   Khởi tạo tình trạng ghế trống trên chuyến bay dựa trên sơ đồ ghế của máy bay được gán.
      *   **Xem thông tin chuyến bay:**
          *   Xem danh sách tất cả các chuyến bay.
          *   Xem chi tiết một chuyến bay cụ thể (theo ID hoặc số hiệu chuyến bay).
          *   Xem lộ trình, lịch trình, máy bay sử dụng, tình trạng chuyến bay (`FlightStatus`).
          *   Kiểm tra tình trạng ghế trống (theo số ghế cụ thể).
      *   **Cập nhật thông tin chuyến bay:**
          *   Thay đổi lịch trình (thời gian khởi hành/đến).
          *   Thay đổi máy bay (có thể phức tạp, cần cập nhật lại tình trạng ghế).
          *   Cập nhật trạng thái chuyến bay (ví dụ: SCHEDULED, BOARDING, DEPARTED, CANCELLED).
      *   **Hủy chuyến bay:** Đánh dấu chuyến bay là `CANCELLED`.
      *   **Quản lý ghế trên chuyến bay:**
          *   Đặt (reserve) một ghế cụ thể.
          *   Hủy đặt (release) một ghế cụ thể.
      *   **Kiểm tra sự tồn tại của chuyến bay:** Dựa trên ID hoặc số hiệu chuyến bay.
      *   **Đếm số lượng chuyến bay.**

  3.  **Quản lý Hành khách (Passenger Management):**
      *   **Đăng ký/Thêm hành khách mới:**
          *   Ghi nhận tên (`Name`).
          *   Lưu trữ thông tin liên lạc (`ContactInfo` - email, điện thoại, địa chỉ).
          *   Ghi nhận số hộ chiếu (`PassportNumber`).
      *   **Xem thông tin hành khách:**
          *   Xem danh sách tất cả hành khách.
          *   Xem chi tiết một hành khách cụ thể (theo ID hoặc số hộ chiếu).
      *   **Cập nhật thông tin hành khách:** Thay đổi tên, thông tin liên lạc.
      *   **Xóa hành khách:** Loại bỏ hành khách khỏi hệ thống (có thể có ràng buộc nếu hành khách đang có vé).
      *   **Kiểm tra sự tồn tại của hành khách:** Dựa trên ID hoặc số hộ chiếu.
      *   **Đếm số lượng hành khách.**

  4.  **Quản lý Vé (Ticket Management):**
      *   **Đặt vé/Tạo vé mới:**
          *   Liên kết vé với một hành khách (`Passenger`) và một chuyến bay (`Flight`).
          *   Gán một số ghế (`SeatNumber`) cụ thể trên chuyến bay đó cho vé.
          *   Xác định giá vé (`Price`).
          *   Gán số vé (`TicketNumber`) duy nhất.
          *   Đặt trạng thái ban đầu cho vé (ví dụ: `PENDING`).
      *   **Xem thông tin vé:**
          *   Xem danh sách tất cả các vé.
          *   Xem chi tiết một vé cụ thể (theo ID hoặc số vé).
          *   Xem thông tin hành khách, chuyến bay, số ghế, giá vé, trạng thái vé (`TicketStatus`).
      *   **Cập nhật trạng thái vé:**
          *   Xác nhận vé (`CONFIRMED`).
          *   Làm thủ tục (check-in) cho vé (`CHECKED_IN`).
          *   Lên máy bay (`BOARDED`).
          *   Hoàn thành chuyến đi (`COMPLETED`).
          *   Hủy vé (`CANCELLED`).
          *   Hoàn tiền (`REFUNDED`).
          *   Logic thay đổi trạng thái vé sẽ phụ thuộc vào trạng thái hiện tại của vé và trạng thái của chuyến bay liên quan (ví dụ: chỉ có thể check-in khi vé đã `CONFIRMED` và chuyến bay đang `BOARDING`).
      *   **Cập nhật thông tin vé:** Có thể cho phép thay đổi giá (ví dụ: nâng hạng).
      *   **Hủy vé:** Thay đổi trạng thái vé thành `CANCELLED` và giải phóng ghế trên chuyến bay.
      *   **Tìm kiếm vé:**
          *   Theo ID hành khách.
          *   Theo số hiệu máy bay (liên quan đến các chuyến bay sử dụng máy bay đó).
      *   **Kiểm tra sự tồn tại của vé:** Dựa trên ID hoặc số vé.
      *   **Đếm số lượng vé.**

  5.  **Chức năng Phụ trợ và Xác thực:**
      *   **Xác thực dữ liệu đầu vào:** Đảm bảo số hiệu máy bay, số hiệu chuyến bay, thông tin liên lạc, số hộ chiếu, sơ đồ ghế, v.v., đều hợp lệ trước khi tạo hoặc cập nhật đối tượng.
      *   **Quản lý lỗi:** Hệ thống sử dụng `Result<T>` để quản lý và trả về các lỗi một cách có cấu trúc.
      *   **Ghi log (Logging):** Hệ thống có cơ chế ghi log các hoạt động và lỗi (thông qua `Logger`).
      *   **Kết nối và tương tác với cơ sở dữ liệu:** Được quản lý bởi `IDatabaseConnection` và các `Repository`.

#### 2.2. Cài đặt lại chức năng của mã nguồn mẫu theo hướng đối tượng (3%)
Sử dụng mạnh mẽ các khái niệm OOP. Có các lớp `Entity` (Aircraft, Flight, Passenger, Ticket) với hành vi và thuộc tính riêng, `Value Object` (AircraftSerial, FlightNumber, ContactInfo, SeatClassMap, Price, v.v.) để đảm bảo tính toàn vẹn dữ liệu, và `Repository` để trừu tượng hóa việc truy cập dữ liệu.
***(NOTE: Bổ sung thêm)***

#### 2.3. Các chức năng thêm và cải tiến (14%)
  1.  **Quản lý Máy bay Chi tiết hơn:**
      *   Phiên bản có `Aircraft` với `SeatClassMap`, cho phép quản lý chi tiết số lượng ghế theo từng hạng (Economy, Business, First). Phiên bản tham khảo không đề cập đến việc quản lý chi tiết này.
      *   Khả năng kiểm tra tính hợp lệ của số ghế (`isValidSeatNumber`).

  2.  **Quản lý Chuyến bay Phức tạp hơn:**
      *   **Phiên bản mới:**
          *   `Flight` liên kết với một `Aircraft` cụ thể (thông qua `std::shared_ptr<Aircraft>`), cho phép truy cập thông tin máy bay và sơ đồ ghế.
          *   Quản lý tình trạng ghế trống trên chuyến bay (`_seatAvailability` trong `Flight`).
          *   Quản lý trạng thái chuyến bay (`FlightStatus` - SCHEDULED, BOARDING, CANCELLED, etc.).
          *   Sử dụng `Route` và `Schedule` làm Value Object, đảm bảo tính nhất quán.
      *   **Phiên bản tham khảo:** "Flight Details" có vẻ đơn giản hơn, "Flight Leave And Arrive" có thể là cập nhật trạng thái hoặc thời gian thực tế. Thông tin ghế không được quản lý chi tiết trên từng chuyến bay.

  3.  **Quản lý Hành khách và Thông tin Liên hệ Bài bản:**
      *   **Phiên bản mới:**
          *   `Passenger` entity.
          *   `ContactInfo` là một Value Object với email, phone, address và có trình xác thực riêng (`ContactInfoValidator`).
          *   `PassportNumber` là một Value Object với trình xác thực riêng.
      *   **Phiên bản tham khảo:** Thông tin hành khách được nhập trực tiếp và lưu vào bảng `userreservation_tb` mà không có đối tượng `Passenger` hay `ContactInfo` riêng biệt với logic xác thực mạnh mẽ.

  4.  **Quản lý Vé Chuyên sâu:**
      *   **Phiên bản mới:**
          *   `Ticket` là một Entity trung tâm, liên kết `Passenger` và `Flight`.
          *   Quản lý `SeatNumber` cụ thể cho vé.
          *   Quản lý `Price` (với amount và currency) như một Value Object.
          *   Quản lý trạng thái vé (`TicketStatus` - PENDING, CONFIRMED, CANCELLED, etc.) với các logic nghiệp vụ như `canBeCancelled()`, `canBeCheckedIn()`.
      *   **Phiên bản tham khảo:** Chức năng "Reserve Seat" và "User Ticket" có vẻ đơn giản là ghi/đọc thông tin đặt chỗ. Khái niệm "Ticket" không phải là một đối tượng phức tạp với các trạng thái và logic riêng.

  5.  **An toàn hơn:**
      *   **Phiên bản mới:** Việc sử dụng prepared statements (mặc dù `MySQLXConnection` hiện tại mô phỏng nó bằng cách xây dựng chuỗi query) và các Value Object giúp giảm nguy cơ SQL Injection so với việc ghép chuỗi trực tiếp như phiên bản tham khảo.
      *   **Phiên bản tham khảo:** Dễ bị SQL Injection do ghép chuỗi query trực tiếp.

  6.  **Quản lý Lỗi Tốt hơn:**
      *   **Phiên bản mới:**
          * Sử dụng `Result<T>` (dựa trên `std::expected`) để xử lý lỗi một cách tường minh và có cấu trúc, tránh việc chương trình bị dừng đột ngột và cho phép client xử lý lỗi một cách linh hoạt.
          * Ghi log (Logging): Hệ thống có cơ chế ghi log các hoạt động và lỗi (thông qua `Logger`) để phục vụ việc DEBUG chương trình phức tạp và giúp dễ dàng hơn trong việc xử lý các lỗi chương trình.
      *   **Phiên bản tham khảo:** Kiểm tra lỗi thủ công sau mỗi lệnh `mysql_query` và chỉ hiển thị mã lỗi.

  7.  **Tính Toàn vẹn và Xác thực Dữ liệu Mạnh mẽ:**
      *   **Phiên bản mới:**
          *   **Value Objects:** Đảm bảo các giá trị như số hiệu máy bay, số hiệu chuyến bay, thông tin liên hệ, v.v., luôn hợp lệ ngay từ khi tạo.
          *   **Validators:** Các lớp Validator riêng biệt (ví dụ `AircraftSerialValidator`) chứa logic xác thực phức tạp.
          *   Factory methods (`create()`) trong Entity và Value Object thường đi kèm xác thực.
      *   **Phiên bản tham khảo:** Không có cơ chế xác thực dữ liệu đầu vào rõ ràng trước khi đưa vào CSDL.

### 2.4. Giao diện bố trí hợp lý (3%)
***(CƯỜNG BỔ SUNG VÀO)***

### 3. Kiến trúc phần mềm + design pattern (20%)
#### 3.1. Mô tả kiến trúc phần mềm
Chương trình của chúng em có tham khảo kiến trúc Layered Achievement và có sử dụng Domain Driven Design:

#### **Domain Layer (Lớp Miền/Nghiệp vụ):**
  - **Entities (Thực thể):** `Aircraft`, `Flight`, `Passenger`, `Ticket`. Đây là các đối tượng cốt lõi của nghiệp vụ, có định danh (ID) và vòng đời riêng. Các thực thể này kế thừa từ `IEntity` chứa trạng thái và các phương thức thể hiện hành vi nghiệp vụ cốt lõi (ví dụ: `Flight::reserveSeat`, `Ticket::canBeCancelled`).
  - **Value Objects (Đối tượng Giá trị):** `AircraftSerial`, `SeatClassMap`, `FlightNumber`, `Route`, `Schedule`, `SeatNumber`, `PassportNumber`, `ContactInfo`, `Price`, `TicketNumber`, `FlightStatus`, `TicketStatus`. Các đối tượng này mô tả các thuộc tính của thực thể, không có định danh riêng, và thường là bất biến (immutable). Chúng được tạo ra, xác thực và sử dụng để đảm bảo tính nhất quán dữ liệu.
  - **Validators (Trình xác thực):** `AircraftSerialValidator`, `FlightNumberValidator`, `ContactInfoValidator`,... Các lớp này chịu trách nhiệm xác thực dữ liệu đầu vào cho các Value Object hoặc các thuộc tính của Entity, đảm bảo tính hợp lệ trước khi đối tượng được tạo hoặc cập nhật.

#### **Data Access Layer (DAL) / Repository Layer (Lớp Truy cập Dữ liệu / Kho lưu trữ):**
  - **Repository Interfaces:** `IRepository<T, IdType>`. Định nghĩa các hoạt động CRUD (Create, Read, Update, Delete) cơ bản cho các Entity.
  - **Concrete Repositories:** `AircraftRepository`, `FlightRepository`, `PassengerRepository`, `TicketRepository`. Các lớp này triển khai `IRepository`, chịu trách nhiệm tương tác với cơ sở dữ liệu để lưu trữ và truy xuất các Entity. Chúng che giấu chi tiết về cách dữ liệu được lưu trữ (ví dụ: SQL queries, ORM).
  - **Database Abstraction:** `InterfaceDatabaseConnection` và `MySQLXConnection`. `InterfaceDatabaseConnection` định nghĩa một giao diện chung cho việc kết nối và tương tác với cơ sở dữ liệu. `MySQLXConnection` là một triển khai cụ thể cho MySQL sử dụng X DevAPI. Điều này cho phép thay đổi cơ sở dữ liệu backend mà ít ảnh hưởng đến các Repository. `MySQLXResult` đóng gói kết quả truy vấn.

#### **Application Layer (Lớp Ứng dụng):**
  - Lớp này sẽ điều phối các hoạt động, sử dụng các Repository để lấy/lưu trữ Entity và gọi các phương thức nghiệp vụ trên Entity. Ví dụ, một "BookingService" có thể sử dụng `FlightRepository`, `PassengerRepository`, `TicketRepository` để thực hiện việc đặt vé.
  ***(CƯỜNG BỔ SUNG THÊM)***

#### **Presentation Layer (Lớp Trình diễn):**
  - Giao diện người dùng sẽ tương tác với Application Layer.
  ***(CƯỜNG BỔ SUNG THÊM)***

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
***(NOTE: BỔ SUNG GIẢI THÍCH ĐẦY ĐỦ)***
#### 3.2. Nguyên tắc và nguyên lý OOP
##### **Tính đóng gói (Encapsulation)**:
  - Dữ liệu (thuộc tính) của các lớp (`_serial`, `_model` trong `Aircraft`) thường được khai báo là `protected` hoặc `private`.
  - Truy cập và thay đổi dữ liệu được thực hiện thông qua các phương thức public (getters, setters, và các phương thức nghiệp vụ).
  - Logic tạo đối tượng phức tạp được đóng gói trong các factory method (`create`).
  - Value Objects đóng gói logic xác thực của chúng (ví dụ `AircraftSerial::create` gọi `AircraftSerialValidator`).

##### **Tính kế thừa (Inheritance)**:
  - Các lớp Entity (`Aircraft`, `Flight`, `Passenger`, `Ticket`) kế thừa từ `IEntity` để có các hành vi chung như `getId()`, `toString()`, `equals()`, `clone()`.
  - `MySQLXConnection` kế thừa (triển khai) `IDatabaseConnection`.

##### **Tính đa hình (Polymorphism)**:
  - Sử dụng các phương thức `virtual` trong `IEntity` (ví dụ: `toString()`, `clone()`) cho phép các đối tượng Entity cụ thể có triển khai riêng.
  - Có thể lưu trữ một tập hợp các `EntityPtr` (là `std::unique_ptr<IEntity>`) và gọi các phương thức virtual trên chúng mà không cần biết kiểu cụ thể tại thời điểm biên dịch.
  - Các Repository có thể được sử dụng thông qua interface `IRepository`, cho phép thay thế các triển khai Repository (ví dụ: một InMemoryRepository để test).

##### **Tính trừu tượng (Abstraction)**:
  - **Interfaces:** `IEntity`, `IRepository`, `IDatabaseConnection`, `IDatabaseResult`. Các interface này định nghĩa "hợp đồng" mà các lớp cụ thể phải tuân theo, che giấu chi tiết triển khai. Ví dụ, lớp Application sẽ làm việc với `IRepository<Aircraft>` thay vì `AircraftRepository` cụ thể.
  - Các Repository trừu tượng hóa việc truy cập dữ liệu, người dùng không cần biết về SQL hay cách kết nối CSDL.

***(CƯỜNG BỔ SUNG THÊM PHẦN SERVICES VÀ UI)***

#### 3.3. Nguyên tắc SOLID
*   **S - Single Responsibility Principle (Nguyên tắc Đơn trách nhiệm):**
    *   Mỗi Entity chịu trách nhiệm cho trạng thái và quy tắc nghiệp vụ của riêng nó.
    *   Mỗi Repository chịu trách nhiệm cho việc lưu trữ và truy xuất một loại Entity cụ thể.
    *   Mỗi Validator chỉ chịu trách nhiệm xác thực một loại dữ liệu cụ thể.
    *   `ContactInfoFormatter`, `ContactInfoParser` có trách nhiệm rõ ràng.
*   **O - Open/Closed Principle (Nguyên tắc Đóng/Mở):**
    *   Sử dụng interfaces (`IRepository`, `IDatabaseConnection`) cho phép mở rộng hệ thống (ví dụ, thêm một loại CSDL mới bằng cách triển khai `IDatabaseConnection`) mà không cần sửa đổi code hiện có đang sử dụng các interface đó.
    *   Các factory method cho phép thay đổi logic tạo đối tượng mà không ảnh hưởng đến client.
*   **L - Liskov Substitution Principle (Nguyên tắc Thay thế Liskov):**
    *   Các lớp con (ví dụ `Aircraft`) có thể thay thế cho lớp cha (`IEntity`) mà không làm thay đổi tính đúng đắn của chương trình.
    *   `MySQLXConnection` phải có khả năng thay thế hoàn toàn cho `IDatabaseConnection`.
*   **I - Interface Segregation Principle (Nguyên tắc Phân tách Interface):**
    *   `IEntity`, `IRepository` là các interface khá nhỏ gọn và tập trung. `IDatabaseConnection` có vẻ lớn hơn một chút nhưng các phương thức của nó khá gắn kết với nhau cho việc thao tác CSDL.
*   **D - Dependency Inversion Principle (Nguyên tắc Đảo ngược Phụ thuộc):**
    *   Các module cấp cao (ví dụ Application Layer) phụ thuộc vào các abstraction (`IRepository`, `IDatabaseConnection`) chứ không phải các triển khai cụ thể (`AircraftRepository`, `MySQLXConnection`).
    *   Các Repository phụ thuộc vào `IDatabaseConnection` (được inject vào constructor hoặc set).
***(CƯỜNG BỔ SUNG THÊM PHẦN SERVICES VÀ UI)***

#### 3.4. Design Patterns

Một số Design Pattern được sử dụng:

1.  **Repository Pattern:** Rất rõ ràng với `AircraftRepository`, `FlightRepository`, etc. và interface `IRepository`. Pattern này tách biệt logic nghiệp vụ khỏi chi tiết truy cập dữ liệu.
2.  **Factory Method Pattern:** Các phương thức static `create()` trong các lớp Entity (`Aircraft::create`, `Flight::create`) và Value Object (`AircraftSerial::create`, `ContactInfo::create`). Chúng đóng gói logic khởi tạo đối tượng, bao gồm cả việc xác thực đầu vào và trả về `Result<T>` để xử lý lỗi.
3.  **Value Object Pattern:** `AircraftSerial`, `FlightNumber`, `ContactInfo`, v.v. Các đối tượng này đại diện cho các giá trị, thường là immutable, và logic xác thực được thực hiện khi tạo.
4.  **Singleton Pattern:** `MySQLXConnection::getInstance()` và `Logger::getInstance()` (được đề cập trong code). Được sử dụng để đảm bảo chỉ có một instance của kết nối CSDL hoặc logger trong toàn bộ ứng dụng.
5.  **Result/Optional (Monadic Error Handling):** Lớp `Result<T>` (sử dụng `std::expected`) là một cách tiếp cận hiện đại để xử lý lỗi mà không cần dùng exception cho các lỗi có thể phục hồi. Nó làm cho việc xử lý lỗi trở nên tường minh hơn.
6.  **Prototype Pattern:** Phương thức `IEntity::clone()` cho phép tạo một bản sao của một Entity.
7.  **Facade Pattern (Tiềm năng):** `IDatabaseConnection` có thể được xem như một Facade, cung cấp một giao diện đơn giản hơn cho các hoạt động phức tạp của thư viện CSDL bên dưới (ví dụ: MySQL X DevAPI).
8.  **Strategy Pattern (hoặc Template Method trong `ContactInfo`):**
    *   Trong `ContactInfo::createInternal`, việc sử dụng `if constexpr` để lựa chọn cách xử lý dựa trên `InputType` (tuple hay string) có hơi hướng của Strategy hoặc Template Method, nơi các bước cụ thể (parse hoặc dùng trực tiếp) được chọn dựa trên loại đầu vào.
    *   Tương tự trong `ContactInfoValidator::validateInternal` và `validateSpecific`.
***(CƯỜNG BỔ SUNG THÊM PHẦN SERVICES VÀ UI)***

### 4. Đảm bảo chất lượng (20%)
#### 4.1. Unit Test
#### A. Test Plan
##### 1. Giới thiệu

###### 1.1 Mục đích
Kiểm thử chi tiết cho các thành phần đơn vị (unit components) của Hệ thống Quản lý Đặt vé Máy bay. Mục tiêu chính là xác minh tính đúng đắn, độ tin cậy và hiệu suất của từng module riêng lẻ, bao gồm các Core Entities (Thực thể lõi), Value Objects (Đối tượng giá trị), Repositories (Kho lưu trữ dữ liệu), Services (Các nghiệp vụ chức năng), cũng như lớp kết nối cơ sở dữ liệu.

###### 1.2 Phạm vi

**Trong phạm vi (In-scope):**

- **Core Entities**:
  - Aircraft: Kiểm tra logic tạo, các thuộc tính, và các phương thức cơ bản.
  - Flight: Kiểm tra logic tạo, quản lý ghế, các thuộc tính, và liên kết với Aircraft.
  - Passenger: Kiểm tra logic tạo và các thuộc tính.
  - Ticket: Kiểm tra logic tạo, quản lý trạng thái (check-in), và liên kết với Flight, Passenger.

- **Value Objects**: Kiểm tra tính hợp lệ, định dạng, các quy tắc nghiệp vụ và các phép toán của: AircraftSerial, ContactInfo, FlightNumber, PassportNumber, Price, Route, Schedule, SeatClassMap, SeatNumber, TicketNumber

- **Repositories**: Kiểm tra các hoạt động CRUD (Create, Read, Update, Delete) và các phương thức truy vấn cụ thể đối với cơ sở dữ liệu MySQL. Kiểm tra khả năng xử lý lỗi và tính nhất quán dữ liệu: AircraftRepository, FlightRepository, PassengerRepository, TicketRepository.

- **Database Utilities**:
  - MySQLXConnection: Kiểm tra khả năng kết nối, ngắt kết nối, thực thi câu lệnh SQL, prepared statements, quản lý transaction (nếu có), và xử lý lỗi kết nối.
***(CƯỜNG BỔ SUNG THÊM PHẦN SERVICES)***

**Ngoài phạm vi (Out-of-scope):**
- Kiểm thử tích hợp hệ thống toàn diện (System Integration Testing).
- Kiểm thử giao diện người dùng (UI Testing).
***(CƯỜNG BỔ SUNG THÊM PHẦN UI)***

##### 2. Chiến lược kiểm thử

###### 2.1 Loại kiểm thử
- **Kiểm thử đơn vị (Unit Testing)**: Tập trung vào việc kiểm tra từng thành phần nhỏ nhất của mã nguồn (hàm, lớp) một cách độc lập. Đối với Entities và Value Objects, các phụ thuộc sẽ được giả lập (mock) nếu cần, hoặc sử dụng các đối tượng thật nếu chúng đơn giản và không có tác dụng phụ.

- **Kiểm thử tích hợp (Integration Testing) ở mức độ thấp**: Đối với các lớp Repository, việc kiểm thử sẽ bao gồm tương tác thực tế với cơ sở dữ liệu MySQL để đảm bảo các câu lệnh SQL và logic ánh xạ đối tượng-quan hệ (ORM) hoạt động chính xác.

###### 2.2 Phương pháp kiểm thử
- **Kiểm thử hộp trắng (White-box testing)**: Các ca kiểm thử được thiết kế dựa trên kiến thức về cấu trúc nội tại của mã nguồn.
- **Tự động hóa kiểm thử**: Sử dụng Google Test (GTest) framework để viết và thực thi các ca kiểm thử một cách tự động.
- **Data-Driven Testing**: Sử dụng các bộ dữ liệu đầu vào hợp lệ và không hợp lệ được định nghĩa sẵn để kiểm tra toàn diện các quy tắc xác thực.
- **Test Fixtures**: Sử dụng các lớp Test Fixture trong GTest để thiết lập môi trường kiểm thử chung và dọn dẹp sau kiểm thử.

###### 2.3 Tiêu chí hoàn thành/chấp nhận

**Tiêu chí bắt đầu (Entry Criteria):**
- Mã nguồn của các module cần kiểm thử đã được hoàn thành và commit vào hệ thống quản lý phiên bản.
- Môi trường kiểm thử đã được thiết lập và sẵn sàng.
- Các Test Case đã được thiết kế và phê duyệt.

**Tiêu chí hoàn thành/chấp nhận (Exit/Acceptance Criteria):**
- 100% các Test Case đã được định nghĩa trong phạm vi phải được thực thi.
- Tỷ lệ Test Case thành công (Pass) đạt tối thiểu 98% (ưu tiên 100% cho unit test).
- Tất cả các lỗi có mức độ nghiêm trọng "Critical" hoặc "High" phải được sửa lỗi và kiểm tra lại.
- Độ bao phủ mã nguồn đạt ngưỡng yêu cầu (80% cho các module quan trọng).
- Báo cáo kiểm thử được hoàn thành và phê duyệt.

##### 3. Môi trường kiểm thử
- **Hệ điều hành**: macOS Sequoia 15.4
- **Trình biên dịch**: g++ 11.3.0, MSVC 19.37, Clang 15.0.0
- **Framework kiểm thử**: Google Test 1.12.1
- **Cơ sở dữ liệu**: MySQL Server 8.0.32
- **Schema**: airlines_db
- **IDE**: Visual Studio Code
- **Hệ thống quản lý phiên bản**: Git
- **Công cụ build**: CMake

##### 4. Tài nguyên kiểm thử
- Bản cài đặt Google Test library
- Trình biên dịch C++ và các thư viện phụ thuộc
- MySQL Server và MySQL Connector/C++
- Môi trường phát triển tích hợp và các plugin liên quan
- Git client

##### 5. Lịch trình kiểm thử

| Giai đoạn | Công việc | Bắt đầu | Kết thúc | Người thực hiện |
|-|-|-|-|-|
| **Chuẩn bị** | Rà soát và hoàn thiện Test Plan | TBD | TBD | Nhóm |
| | Chuẩn bị môi trường kiểm thử | TBD | TBD | Nhóm |
| | Hoàn thiện Test Cases | TBD | TBD | Nhóm |
| **Thực thi** | Unit Test cho Value Objects | TBD | TBD | Nhóm |
| | Unit Test cho Entities | TBD | TBD | Nhóm |
| | Integration Test cho Repositories | TBD | TBD | Nhóm |
| | Phân tích kết quả, ghi nhận lỗi | TBD | TBD | Nhóm |
| **Báo cáo** | Sửa lỗi và regression testing | TBD | TBD | Nhóm |
| | Hoàn thiện Test Report | TBD | TBD | Nhóm |
| | Review và phê duyệt | TBD | TBD | Giảng viên |

***(NOTE: SỬA LẠI THỜI GIAN; CƯỜNG BỔ SUNG THÊM SERVICES)***

##### 6. Sản phẩm kiểm thử (Deliverables)
- **Test Plan**: Kế hoạch kiểm thử
- **Test Cases**: Tài liệu mô tả chi tiết từng ca kiểm thử
- **Test Scripts**: Các file *.cpp chứa mã nguồn Google Test
- **Test Data**: Dữ liệu được sử dụng trong SetUp và test cases
- **Test Execution Logs**: Output từ việc chạy GTest
- **Test Report**: Tài liệu tổng kết kết quả kiểm thử

##### 7. Rủi ro và Giải pháp

| Rủi ro | Mức độ ảnh hưởng | Khả năng xảy ra | Giải pháp |
|-|-|-|-|
| **R1**: Môi trường cơ sở dữ liệu không ổn định | Cao | Trung bình | - Đảm bảo MySQL server được cấu hình đúng<br>- Sử dụng cơ sở dữ liệu riêng cho test<br>- TearDown phải dọn dẹp dữ liệu cẩn thận |
| **R2**: Dữ liệu test không nhất quán | Cao | Trung bình | - Test case tự quản lý dữ liệu của mình<br>- Tránh phụ thuộc vào dữ liệu cố định |
| **R3**: Thay đổi mã nguồn làm hỏng unit test | Cao | Cao | - Chạy unit test thường xuyên<br>- Tích hợp vào CI/CD<br>- Cập nhật unit test song song với code |
#### B. Test Case

**Quy ước cột:**
- **ID**: Mã định danh Test Case
- **Test Suite**: Tên lớp Test Fixture từ GTest
- **Test Case Name**: Tên của hàm test trong GTest hoặc mô tả case cụ thể
- **Mô tả/Mục tiêu**: Mục đích chính của test case
- **Kết quả mong đợi chính**: Kết quả cốt lõi kỳ vọng

| ID | Test Suite | Test Case Name | Mô tả/Mục tiêu | Kết quả mong đợi chính |
|-|-|-|-|-|
| **AircraftTest** |||||
| AC_001 | AircraftTest | CreateAircraft | Tạo Aircraft hợp lệ, getter trả về đúng giá trị | Tạo thành công, giá trị getter khớp |
| AC_002 | AircraftTest | CreateAircraftWithInvalidParameters (model rỗng) | Tạo Aircraft với model rỗng | Tạo thất bại, lỗi "INVALID_MODEL" |
| AC_003 | AircraftTest | CreateAircraftWithInvalidParameters (serial rỗng) | Tạo Aircraft với serial rỗng (chuỗi) | Tạo thất bại, lỗi "INVALID_SERIAL" |
| AC_004 | AircraftTest | CreateAircraftWithInvalidParameters (layout rỗng) | Tạo Aircraft với seat layout rỗng (chuỗi) | Tạo thất bại, lỗi "INVALID_SEAT_LAYOUT" |
| AC_005 | AircraftTest | ValueObjectOperations | Kiểm tra equals, clone, toString | equals đúng, clone hợp lệ, toString đúng định dạng |
| AC_006 | AircraftTest | StringBasedCreation | Tạo Aircraft từ các tham số chuỗi | Tạo thành công, getter khớp giá trị chuỗi |
| AC_007 | AircraftTest | DifferentAircraftModels | Tạo Aircraft với các model khác nhau | Tạo thành công, getModel() trả về đúng model |
| AC_008 | AircraftTest | DifferentSeatLayouts | Tạo Aircraft với seat layout khác | Tạo thành công, getSeatLayout() trả về đúng layout mới |
| **AircraftRepositoryTest** |||||
| ACR_001 | AircraftRepositoryTest | CreateAircraft | Lưu Aircraft mới vào DB | Lưu thành công, có ID > 0, dữ liệu khớp |
| ACR_002 | AircraftRepositoryTest | FindAircraftById | Tìm Aircraft bằng ID | Tìm thấy Aircraft, dữ liệu khớp |
| ACR_003 | AircraftRepositoryTest | FindAllAircraft | Lấy tất cả Aircraft từ DB | Danh sách không rỗng, chứa Aircraft test |
| ACR_004 | AircraftRepositoryTest | ExistsAircraft | Kiểm tra sự tồn tại của Aircraft bằng ID | True cho ID tồn tại, False cho ID không tồn tại |
| ACR_005 | AircraftRepositoryTest | CountAircraft | Đếm số lượng Aircraft | Số lượng tăng 1 sau khi thêm |
| ACR_006 | AircraftRepositoryTest | UpdateAircraft | Cập nhật Aircraft đã có | Cập nhật thành công, dữ liệu mới được lưu |
| ACR_007 | AircraftRepositoryTest | DeleteAircraftById | Xóa Aircraft bằng ID | Xóa thành công, Aircraft không còn tồn tại |
| ACR_008 | AircraftRepositoryTest | ErrorHandling | Xử lý lỗi khi tìm ID không hợp lệ, update/delete ID không tồn tại | Thao tác thất bại, trả về lỗi phù hợp |
| **AircraftSerialTest** |||||
| ASR_V01 | AircraftSerialTest | ValidAircraftSerials - V1 ("VN123") | Tạo AircraftSerial hợp lệ (2-letter prefix) | Tạo thành công, giá trị "VN123" |
| ASR_V02 | AircraftSerialTest | ValidAircraftSerials - V2 ("VNA123") | Tạo AircraftSerial hợp lệ (3-letter prefix) | Tạo thành công, giá trị "VNA123" |
| ASR_V03 | AircraftSerialTest | ValidAircraftSerials - V3 ("VN1") | Tạo AircraftSerial hợp lệ (min length) | Tạo thành công, giá trị "VN1" |
| ASR_V04 | AircraftSerialTest | ValidAircraftSerials - V4 ("VN123") | Tạo AircraftSerial hợp lệ (typical length) | Tạo thành công, giá trị "VN123" |
| ASR_V05 | AircraftSerialTest | ValidAircraftSerials - V5 ("VNA1234567") | Tạo AircraftSerial hợp lệ (max length) | Tạo thành công, giá trị "VNA1234567" |
| ASR_E01 | AircraftSerialTest | ValidAircraftSerials - E1 ("VN1234567") | Tạo AircraftSerial hợp lệ (min prefix, max digits) | Tạo thành công, giá trị "VN1234567" |
| ASR_E02 | AircraftSerialTest | ValidAircraftSerials - E2 ("VNA1") | Tạo AircraftSerial hợp lệ (max prefix, min digits) | Tạo thành công, giá trị "VNA1" |
| ASR_E03 | AircraftSerialTest | ValidAircraftSerials - E3 ("VN1111111") | Tạo AircraftSerial hợp lệ (all same digits) | Tạo thành công, giá trị "VN1111111" |
| ASR_E04 | AircraftSerialTest | ValidAircraftSerials - E4 ("VVV111") | Tạo AircraftSerial hợp lệ (all same letters) | Tạo thành công, giá trị "VVV111" |
| ASR_I01 | AircraftSerialTest | InvalidAircraftSerials - I1 ("") | Tạo AircraftSerial với chuỗi rỗng | Tạo thất bại, lỗi "EMPTY_AIRCRAFT_SERIAL" |
| ASR_I02 | AircraftSerialTest | InvalidAircraftSerials - I2 ("V1") | Tạo AircraftSerial quá ngắn | Tạo thất bại, lỗi "INVALID_LENGTH" |
| ASR_I03 | AircraftSerialTest | InvalidAircraftSerials - I3 ("VNA12345678") | Tạo AircraftSerial quá dài | Tạo thất bại, lỗi "INVALID_LENGTH" |
| ASR_I04 | AircraftSerialTest | InvalidAircraftSerials - I4 (" ") | Tạo AircraftSerial chỉ có khoảng trắng | Tạo thất bại, lỗi "INVALID_FORMAT" |
| ASR_I05 | AircraftSerialTest | InvalidAircraftSerials - I5 ("1VN23") | Tạo AircraftSerial bắt đầu bằng số | Tạo thất bại, lỗi "INVALID_FORMAT" |
| ASR_I06 | AircraftSerialTest | InvalidAircraftSerials - I6 ("VNAA123") | Tạo AircraftSerial có hơn 3 chữ cái | Tạo thất bại, lỗi "INVALID_FORMAT" |
| ASR_I07 | AircraftSerialTest | InvalidAircraftSerials - I7 ("VN12345678") | Tạo AircraftSerial có hơn 7 chữ số | Tạo thất bại, lỗi "INVALID_FORMAT" |
| ASR_I08 | AircraftSerialTest | InvalidAircraftSerials - I8 ("VN-123") | Tạo AircraftSerial chứa ký tự đặc biệt | Tạo thất bại, lỗi "INVALID_FORMAT" |
| ASR_I09 | AircraftSerialTest | InvalidAircraftSerials - I9 ("vn123") | Tạo AircraftSerial chữ thường | Tạo thất bại, lỗi "INVALID_FORMAT" |
| ASR_I10 | AircraftSerialTest | InvalidAircraftSerials - I10 ("VN 123") | Tạo AircraftSerial chứa khoảng trắng | Tạo thất bại, lỗi "INVALID_FORMAT" |
| ASR_I11 | AircraftSerialTest | InvalidAircraftSerials - I11 ("Vn123") | Tạo AircraftSerial chữ hoa/thường lẫn lộn | Tạo thất bại, lỗi "INVALID_FORMAT" |
| **FlightNumberTest** |||||
| FN_V01 | FlightNumberTest | ValidFlightNumbers - V1 ("VN123") | Tạo FlightNumber hợp lệ (2-letter prefix) | Tạo thành công, giá trị "VN123" |
| FN_V02 | FlightNumberTest | ValidFlightNumbers - V2 ("VN123") | Tạo FlightNumber hợp lệ (3-letter prefix) | Tạo thành công, giá trị "VN123" |
| FN_V03 | FlightNumberTest | ValidFlightNumbers - V3 ("VN1") | Tạo FlightNumber hợp lệ (min length) | Tạo thành công, giá trị "VN1" |
| FN_V04 | FlightNumberTest | ValidFlightNumbers - V4 ("VN123") | Tạo FlightNumber hợp lệ (typical length) | Tạo thành công, giá trị "VN123" |
| FN_V05 | FlightNumberTest | ValidFlightNumbers - V5 ("VN1234") | Tạo FlightNumber hợp lệ (max length) | Tạo thành công, giá trị "VN1234" |
| FN_E01 | FlightNumberTest | ValidFlightNumbers - E1 ("VN1234") | Tạo FlightNumber hợp lệ (max digits) | Tạo thành công, giá trị "VN1234" |
| FN_E02 | FlightNumberTest | ValidFlightNumbers - E2 ("VN1") | Tạo FlightNumber hợp lệ (min digits) | Tạo thành công, giá trị "VN1" |
| FN_E03 | FlightNumberTest | ValidFlightNumbers - E3 ("VN1111") | Tạo FlightNumber hợp lệ (all same digits) | Tạo thành công, giá trị "VN1111" |
| FN_E04 | FlightNumberTest | ValidFlightNumbers - E4 ("VV111") | Tạo FlightNumber hợp lệ (all same letters) | Tạo thành công, giá trị "VV111" |
| FN_I01 | FlightNumberTest | InvalidFlightNumbers - I1 ("") | Tạo FlightNumber với chuỗi rỗng | Tạo thất bại, lỗi "EMPTY_FLIGHT_NUMBER" |
| FN_I02 | FlightNumberTest | InvalidFlightNumbers - I2 ("V1") | Tạo FlightNumber quá ngắn | Tạo thất bại, lỗi "INVALID_LENGTH" |
| FN_I03 | FlightNumberTest | InvalidFlightNumbers - I3 ("VNA12345") | Tạo FlightNumber quá dài (tổng thể) | Tạo thất bại, lỗi "INVALID_LENGTH" |
| FN_I04 | FlightNumberTest | InvalidFlightNumbers - I6 ("VNAA123") | Tạo FlightNumber có hơn 2 chữ cái | Tạo thất bại, lỗi "INVALID_LENGTH" |
| FN_I05 | FlightNumberTest | InvalidFlightNumbers - I7 ("VN12345") | Tạo FlightNumber có hơn 4 chữ số | Tạo thất bại, lỗi "INVALID_LENGTH" |
| FN_I06 | FlightNumberTest | InvalidFlightNumbers - I4 (" ") | Tạo FlightNumber chỉ có khoảng trắng | Tạo thất bại, lỗi "INVALID_FORMAT" |
| FN_I07 | FlightNumberTest | InvalidFlightNumbers - I5 ("1VN23") | Tạo FlightNumber bắt đầu bằng số | Tạo thất bại, lỗi "INVALID_FORMAT" |
| FN_I08 | FlightNumberTest | InvalidFlightNumbers - I8 ("VN-123") | Tạo FlightNumber chứa ký tự đặc biệt | Tạo thất bại, lỗi "INVALID_FORMAT" |
| FN_I09 | FlightNumberTest | InvalidFlightNumbers - I9 ("vn123") | Tạo FlightNumber chữ thường | Tạo thất bại, lỗi "INVALID_FORMAT" |
| FN_I10 | FlightNumberTest | InvalidFlightNumbers - I10 ("VN 123") | Tạo FlightNumber chứa khoảng trắng | Tạo thất bại, lỗi "INVALID_FORMAT" |
| FN_I11 | FlightNumberTest | InvalidFlightNumbers - I11 ("Vn123") | Tạo FlightNumber chữ hoa/thường lẫn lộn | Tạo thất bại, lỗi "INVALID_FORMAT" |
| FN_OP01 | FlightNumberTest | ValueObjectOperations | Kiểm tra equals, not equals, toString | equals đúng, not equals đúng, toString đúng |
| **FlightRepositoryTest** |||||
| FR_001 | FlightRepositoryTest | CreateFlight | Lưu Flight mới vào DB | Lưu thành công, có ID > 0, dữ liệu khớp |
| FR_002 | FlightRepositoryTest | FindFlightById | Tìm Flight bằng ID | Tìm thấy Flight, dữ liệu khớp |
| FR_003 | FlightRepositoryTest | FindAllFlights | Lấy tất cả Flight từ DB | Danh sách không rỗng, chứa Flight test |
| FR_004 | FlightRepositoryTest | ExistsFlight | Kiểm tra sự tồn tại của Flight bằng ID | True cho ID tồn tại, False cho ID không tồn tại |
| FR_005 | FlightRepositoryTest | CountFlights | Đếm số lượng Flight | Số lượng tăng 1 sau khi thêm |
| FR_006 | FlightRepositoryTest | UpdateFlight | Cập nhật Flight đã có | Cập nhật thành công, dữ liệu mới được lưu |
| FR_007 | FlightRepositoryTest | DeleteFlightById | Xóa Flight bằng ID | Xóa thành công, Flight không còn tồn tại |
| FR_008 | FlightRepositoryTest | ErrorHandling | Xử lý lỗi khi tìm ID không hợp lệ, update/delete ID không tồn tại | Thao tác thất bại, trả về lỗi phù hợp |
| **ContactInfoTest** |||||
| CI_V01 | ContactInfoTest | ValidContactInfo - V1 (full) | Tạo ContactInfo hợp lệ (email, phone, address) | Tạo thành công, getter trả về đúng |
| CI_V02 | ContactInfoTest | ValidContactInfo - V2 (no address) | Tạo ContactInfo hợp lệ (không có address) | Tạo thành công, getter trả về đúng |
| CI_V03-V05 | ContactInfoTest | ValidContactInfo - V3-V5 (email variations) | Tạo ContactInfo với các biến thể email hợp lệ | Tạo thành công, getter trả về đúng |
| CI_V06-V07 | ContactInfoTest | ValidContactInfo - V6-V07 (phone variations) | Tạo ContactInfo với các biến thể phone hợp lệ | Tạo thành công, getter trả về đúng |
| CI_V08-V10 | ContactInfoTest | ValidContactInfo - V8-V10 (address variations) | Tạo ContactInfo với các biến thể address hợp lệ | Tạo thành công, getter trả về đúng |
| CI_I01 | ContactInfoTest | InvalidContactInfo - I3 (invalid email format) | Tạo ContactInfo với email không hợp lệ | Tạo thất bại, lỗi "INVALID_EMAIL_FORMAT" |
| CI_I02 | ContactInfoTest | InvalidContactInfo - I4 (incomplete email) | Tạo ContactInfo với email không đầy đủ | Tạo thất bại, lỗi "INVALID_EMAIL_FORMAT" |
| CI_I03 | ContactInfoTest | InvalidContactInfo - I5 (missing local part) | Tạo ContactInfo email thiếu local part | Tạo thất bại, lỗi "INVALID_EMAIL_FORMAT" |
| CI_I04 | ContactInfoTest | InvalidContactInfo - I6 (missing domain) | Tạo ContactInfo email thiếu domain | Tạo thất bại, lỗi "INVALID_EMAIL_FORMAT" |
| CI_I05 | ContactInfoTest | InvalidContactInfo - I7 (incomplete domain) | Tạo ContactInfo email domain không đầy đủ | Tạo thất bại, lỗi "INVALID_EMAIL_FORMAT" |
| CI_I06 | ContactInfoTest | InvalidContactInfo - I8 (email too long) | Tạo ContactInfo email quá dài | Tạo thất bại, lỗi "EMAIL_TOO_LONG" |
| CI_I07 | ContactInfoTest | InvalidContactInfo - I9 (non-numeric phone) | Tạo ContactInfo phone chứa ký tự | Tạo thất bại, lỗi "INVALID_PHONE_FORMAT" |
| CI_I08 | ContactInfoTest | InvalidContactInfo - I11 (phone too long) | Tạo ContactInfo phone quá dài | Tạo thất bại, lỗi "PHONE_TOO_LONG" |
| CI_I09 | ContactInfoTest | InvalidContactInfo - I10 (phone too short) | Tạo ContactInfo phone quá ngắn | Tạo thất bại, lỗi "INVALID_PHONE_FORMAT" |
| CI_I10 | ContactInfoTest | InvalidContactInfo - I12 (phone + too long) | Tạo ContactInfo phone (có "+") quá dài | Tạo thất bại, lỗi "PHONE_TOO_LONG" |
| CI_I11 | ContactInfoTest | InvalidContactInfo - I13 (invalid address chars) | Tạo ContactInfo address chứa ký tự không hợp lệ | Tạo thất bại, lỗi "INVALID_ADDRESS_FORMAT" |
| CI_I12 | ContactInfoTest | InvalidContactInfo - I14 (address too long) | Tạo ContactInfo address quá dài | Tạo thất bại, lỗi "ADDRESS_TOO_LONG" |
| CI_I13 | ContactInfoTest | InvalidContactInfo - I15 (multiple invalid) | Tạo ContactInfo với nhiều trường không hợp lệ | Tạo thất bại, nhiều mã lỗi được kết hợp |
| CI_P01 | ContactInfoTest | CreateWithIndividualParams (valid) | Tạo ContactInfo từ tham số riêng hợp lệ | Tạo thành công, getter trả về đúng |
| CI_P02 | ContactInfoTest | CreateWithIndividualParams (empty email) | Tạo ContactInfo từ tham số riêng, email rỗng | Tạo thất bại, lỗi "EMPTY_EMAIL" |
| CI_P03 | ContactInfoTest | CreateWithIndividualParams (empty phone) | Tạo ContactInfo từ tham số riêng, phone rỗng | Tạo thất bại, lỗi "EMPTY_PHONE" |
| CI_P04 | ContactInfoTest | CreateWithIndividualParams (invalid email) | Tạo ContactInfo từ tham số riêng, email không hợp lệ | Tạo thất bại, lỗi "INVALID_EMAIL_FORMAT" |
| CI_P05 | ContactInfoTest | CreateWithIndividualParams (email too long) | Tạo ContactInfo từ tham số riêng, email quá dài | Tạo thất bại, lỗi "EMAIL_TOO_LONG" |
| CI_P06 | ContactInfoTest | CreateWithIndividualParams (invalid phone) | Tạo ContactInfo từ tham số riêng, phone không hợp lệ | Tạo thất bại, lỗi "INVALID_PHONE_FORMAT" |
| CI_P07 | ContactInfoTest | CreateWithIndividualParams (phone too long) | Tạo ContactInfo từ tham số riêng, phone quá dài | Tạo thất bại, lỗi "PHONE_TOO_LONG" |
| CI_P08 | ContactInfoTest | CreateWithIndividualParams (invalid address) | Tạo ContactInfo từ tham số riêng, address không hợp lệ | Tạo thất bại, lỗi "INVALID_ADDRESS_FORMAT" |
| CI_P09 | ContactInfoTest | CreateWithIndividualParams (address too long) | Tạo ContactInfo từ tham số riêng, address quá dài | Tạo thất bại, lỗi "ADDRESS_TOO_LONG" |
| CI_F01 | ContactInfoTest | FormattingVariations | Kiểm tra toString() với các định dạng khác nhau | toString() trả về chuỗi đúng định dạng |
| CI_OP01 | ContactInfoTest | ValueObjectOperations | Kiểm tra equals, not equals | equals đúng, not equals đúng |
| **FlightTest** |||||
| FLT_001 | FlightTest | CreateFlight | Tạo Flight hợp lệ, getter trả về đúng | Tạo thành công, getter khớp |
| FLT_002 | FlightTest | CreateFlightWithInvalidParameters (null aircraft) | Tạo Flight với aircraft null | Tạo thất bại, lỗi "INVALID_AIRCRAFT" |
| FLT_003 | FlightTest | CreateFlightWithInvalidParameters (invalid flight number) | Tạo Flight với flight number không hợp lệ | Tạo thất bại, lỗi "INVALID_FLIGHT_NUMBER" |
| FLT_004 | FlightTest | CreateFlightWithInvalidParameters (invalid route) | Tạo Flight với route không hợp lệ | Tạo thất bại, lỗi "INVALID_ROUTE" |
| FLT_005 | FlightTest | CreateFlightWithInvalidParameters (invalid schedule) | Tạo Flight với schedule không hợp lệ | Tạo thất bại, lỗi "INVALID_SCHEDULE" |
| FLT_006 | FlightTest | SeatManagement | Kiểm tra isSeatAvailable, reserveSeat, releaseSeat | Các hàm hoạt động đúng logic, trạng thái ghế thay đổi chính xác |
| FLT_007 | FlightTest | ValueObjectOperations | Kiểm tra equals, clone, toString | equals đúng, clone hợp lệ, toString đúng định dạng |
| FLT_008 | FlightTest | SeatAvailabilityMap | Kiểm tra map ghế trống ban đầu | Tất cả ghế đều trống, số lượng ghế khớp layout |
| FLT_009 | FlightTest | DifferentSeatLayouts | Tạo Flight với aircraft có seat layout khác | Tạo thành công, quản lý ghế theo layout mới |
| **PassengerRepositoryTest** |||||
| PR_001 | PassengerRepositoryTest | CreatePassenger | Lưu Passenger mới vào DB | Lưu thành công, có ID > 0, dữ liệu khớp |
| PR_002 | PassengerRepositoryTest | FindPassengerById | Tìm Passenger bằng ID | Tìm thấy Passenger, dữ liệu khớp |
| PR_003 | PassengerRepositoryTest | FindAllPassengers | Lấy tất cả Passenger từ DB | Danh sách không rỗng, chứa Passenger test |
| PR_004 | PassengerRepositoryTest | ExistsPassenger | Kiểm tra sự tồn tại của Passenger bằng ID | True cho ID tồn tại, False cho ID không tồn tại |
| PR_005 | PassengerRepositoryTest | CountPassengers | Đếm số lượng Passenger | Số lượng tăng 1 sau khi thêm |
| PR_006 | PassengerRepositoryTest | UpdatePassenger | Cập nhật Passenger đã có | Cập nhật thành công, dữ liệu mới được lưu |
| PR_007 | PassengerRepositoryTest | DeletePassengerById | Xóa Passenger bằng ID | Xóa thành công, Passenger không còn tồn tại |
| PR_008 | PassengerRepositoryTest | FindPassengerByPassportNumber | Tìm Passenger bằng PassportNumber | Tìm thấy Passenger, dữ liệu khớp |
| PR_009 | PassengerRepositoryTest | ExistsPassport | Kiểm tra sự tồn tại của PassportNumber trong DB | True cho passport tồn tại, False cho passport không tồn tại |
| PR_010 | PassengerRepositoryTest | ErrorHandling | Xử lý lỗi khi tìm ID/passport không hợp lệ | Thao tác thất bại, trả về lỗi phù hợp |
| **PassportNumberTest** |||||
| PN_V01-V03 | PassportNumberTest | ValidPassportNumbers - V1-V3 (country codes upper) | Tạo PassportNumber hợp lệ với các mã quốc gia (hoa) | Tạo thành công, giá trị chuẩn hóa (hoa) |
| PN_V04-V06 | PassportNumberTest | ValidPassportNumbers - V4-V6 (country codes lower) | Tạo PassportNumber hợp lệ với các mã quốc gia (thường) | Tạo thành công, giá trị chuẩn hóa (hoa) |
| PN_V07-V10 | PassportNumberTest | ValidPassportNumbers - V7-V10 (number lengths) | Tạo PassportNumber hợp lệ với các độ dài số khác nhau | Tạo thành công, giá trị đúng |
| PN_E01-E03 | PassportNumberTest | ValidPassportNumbers - E1-E3 (edge cases) | Tạo PassportNumber hợp lệ với các trường hợp biên | Tạo thành công, giá trị đúng |
| PN_I01 | PassportNumberTest | InvalidPassportNumbers - I1 ("") | Tạo PassportNumber với chuỗi rỗng | Tạo thất bại, lỗi "EMPTY_PASSPORT_NUMBER" |
| PN_I02-I06 | PassportNumberTest | InvalidPassportNumbers - I2-I6, I13-I15 (invalid format) | Tạo PassportNumber với định dạng không hợp lệ | Tạo thất bại, lỗi "INVALID_FORMAT" |
| PN_I07-I09 | PassportNumberTest | InvalidPassportNumbers - I8-I9 (invalid country) | Tạo PassportNumber với mã quốc gia không hợp lệ | Tạo thất bại, lỗi "INVALID_ISSUING_COUNTRY" |
| PN_I10-I11 | PassportNumberTest | InvalidPassportNumbers - I11-I12 (invalid number length) | Tạo PassportNumber với độ dài số không hợp lệ | Tạo thất bại, lỗi "INVALID_NUMBER_LENGTH" |
| PN_P01 | PassportNumberTest | CreateWithIndividualParams (valid) | Tạo PassportNumber từ tham số riêng hợp lệ | Tạo thành công, getter trả về đúng |
| PN_P02 | PassportNumberTest | CreateWithIndividualParams (empty country/number) | Tạo PassportNumber từ tham số riêng, country/number rỗng | Tạo thất bại, lỗi "EMPTY_PASSPORT_NUMBER" |
| PN_P03 | PassportNumberTest | CreateWithIndividualParams (invalid number length) | Tạo PassportNumber từ tham số riêng, độ dài số không hợp lệ | Tạo thất bại, lỗi "INVALID_NUMBER_LENGTH" |
| PN_F01 | PassportNumberTest | FormattingVariations | Kiểm tra chuẩn hóa mã quốc gia (hoa/thường) | toString() trả về mã quốc gia viết hoa |
| PN_OP01 | PassportNumberTest | ValueObjectOperations | Kiểm tra equals (case insensitive), not equals | equals đúng, not equals đúng |
| **PassengerTest** |||||
| PS_001 | PassengerTest | CreatePassenger | Tạo Passenger hợp lệ, getter trả về đúng | Tạo thành công, getter khớp |
| PS_002 | PassengerTest | CreatePassengerWithInvalidParameters (empty name) | Tạo Passenger với tên rỗng | Tạo thất bại, lỗi "INVALID_NAME" |
| PS_003 | PassengerTest | CreatePassengerWithInvalidParameters (invalid contact) | Tạo Passenger với contact info không hợp lệ | Tạo thất bại, lỗi "INVALID_CONTACT_INFO" |
| PS_004 | PassengerTest | CreatePassengerWithInvalidParameters (invalid passport) | Tạo Passenger với passport không hợp lệ | Tạo thất bại, lỗi "INVALID_PASSPORT" |
| PS_005 | PassengerTest | ValueObjectOperations | Kiểm tra equals, clone, toString | equals đúng, clone hợp lệ, toString đúng định dạng |
| PS_006 | PassengerTest | StringBasedCreation | Tạo Passenger từ các tham số chuỗi | Tạo thành công, getter khớp giá trị chuỗi |
| PS_007 | PassengerTest | DifferentContactInfoFormats | Tạo Passenger với các contact info hợp lệ khác nhau | Tạo thành công, getContactInfo() trả về đúng |
| PS_008 | PassengerTest | DifferentPassportFormats | Tạo Passenger với các passport hợp lệ khác nhau | Tạo thành công, getPassport() trả về đúng |
| **PriceTest** |||||
| PRCE_V01-V02 | PriceTest | ValidPrices - V1-V2 (no decimal) | Tạo Price hợp lệ (không có phần thập phân) | Tạo thành công, chuẩn hóa thành "XXX.00 CUR" |
| PRCE_V03-V07 | PriceTest | ValidPrices - V3-V7 (dot decimal) | Tạo Price hợp lệ (dấu "." thập phân) | Tạo thành công, giá trị đúng |
| PRCE_V08-V12 | PriceTest | ValidPrices - V8-V12 (comma decimal) | Tạo Price hợp lệ (dấu "," thập phân) | Tạo thành công, chuẩn hóa thành "." thập phân |
| PRCE_V13-V14 | PriceTest | ValidPrices - V13-V14 (lowercase currency) | Tạo Price hợp lệ (tiền tệ chữ thường) | Tạo thành công, tiền tệ chuẩn hóa thành chữ hoa |
| PRCE_I01 | PriceTest | InvalidPrices - I1 ("") | Tạo Price với chuỗi rỗng | Tạo thất bại, lỗi "EMPTY_PRICE" |
| PRCE_I02-I08 | PriceTest | InvalidPrices - I2-I8, I12-I13 (invalid format) | Tạo Price với định dạng không hợp lệ | Tạo thất bại, lỗi "INVALID_FORMAT" |
| PRCE_I09-I11 | PriceTest | InvalidPrices - I9-I11 (invalid currency) | Tạo Price với mã tiền tệ không hợp lệ | Tạo thất bại, lỗi "INVALID_CURRENCY" |
| PRCE_I12-I15 | PriceTest | InvalidPrices - I14-I17 (negative amount) | Tạo Price với số tiền âm | Tạo thất bại, lỗi "NEGATIVE_AMOUNT" |
| PRCE_OP01 | PriceTest | ValueObjectOperations | Kiểm tra equals (khác biệt decimal sep), not equals | equals đúng, not equals đúng |
| PRCE_G01 | PriceTest | Getters | Kiểm tra getAmount, getCurrency, getCurrencyName | Trả về giá trị/tên đúng |
| PRCE_P01 | PriceTest | CreateWithSeparateParams | Tạo Price từ tham số (double amount, string currency) hợp lệ | Tạo thành công, toString() đúng định dạng |
| PRCE_F01 | PriceTest | Formatting | Kiểm tra toString() chuẩn hóa | toString() trả về chuỗi đã chuẩn hóa |
| **RouteTest** |||||
| RT_V01-V02 | RouteTest | ValidRoutes - V1-V2 (basic valid) | Tạo Route hợp lệ (HN-HCM, DN-HP) | Tạo thành công, getter trả về đúng |
| RT_V03-V05 | RouteTest | ValidRoutes - V3-V5 (different city names) | Tạo Route hợp lệ với các tên thành phố quốc tế | Tạo thành công, getter trả về đúng |
| RT_E01-E03 | RouteTest | ValidRoutes - E1-E3 (edge cases) | Tạo Route hợp lệ với tên TP ngắn/dài/có số | Tạo thành công, getter trả về đúng |
| RT_I01 | RouteTest | InvalidRoutes - I1 ("") | Tạo Route với chuỗi rỗng | Tạo thất bại, lỗi "EMPTY_ROUTE" |
| RT_I02-I06 | RouteTest | InvalidRoutes - I2-I6 (invalid format) | Tạo Route với định dạng không hợp lệ | Tạo thất bại, lỗi "INVALID_FORMAT" |
| RT_I07-I09 | RouteTest | InvalidRoutes - I7-I9 (invalid origin code) | Tạo Route với mã sân bay đi không hợp lệ | Tạo thất bại, lỗi "INVALID_ORIGIN_CODE" |
| RT_I10-I12 | RouteTest | InvalidRoutes - I10-I12 (invalid dest code) | Tạo Route với mã sân bay đến không hợp lệ | Tạo thất bại, lỗi "INVALID_DESTINATION_CODE" |
| RT_I13-I14 | RouteTest | InvalidRoutes - I13-I14 (same origin/dest) | Tạo Route với điểm đi và điểm đến giống nhau | Tạo thất bại, lỗi "SAME_ORIGIN_DESTINATION" |
| RT_P01 | RouteTest | CreateWithIndividualParams (valid) | Tạo Route từ tham số riêng hợp lệ | Tạo thành công, getter trả về đúng |
| RT_P02 | RouteTest | CreateWithIndividualParams (empty fields) | Tạo Route từ tham số riêng, trường rỗng | Tạo thất bại, lỗi "EMPTY_ROUTE" |
| RT_P03 | RouteTest | CreateWithIndividualParams (invalid codes) | Tạo Route từ tham số riêng, mã sân bay không hợp lệ | Tạo thất bại, lỗi code tương ứng |
| RT_P04 | RouteTest | CreateWithIndividualParams (same origin/dest) | Tạo Route từ tham số riêng, điểm đi/đến trùng | Tạo thất bại, lỗi "SAME_ORIGIN_DESTINATION" |
| RT_F01 | RouteTest | FormattingVariations | Kiểm tra toString() và các getter | toString() đúng định dạng, getter trả về đúng |
| RT_OP01 | RouteTest | ValueObjectOperations | Kiểm tra equals, not equals | equals đúng, not equals đúng |
| **SeatClassMapTest** |||||
| SCM_V01-V03 | SeatClassMapTest | ValidSeatClassMaps - V1-V3 (single class) | Tạo SeatClassMap hợp lệ (1 hạng ghế) | Tạo thành công, toString() đúng |
| SCM_V04-V05 | SeatClassMapTest | ValidSeatClassMaps - V4-V5 (multiple classes) | Tạo SeatClassMap hợp lệ (nhiều hạng ghế) | Tạo thành công, toString() đúng |
| SCM_V06 | SeatClassMapTest | ValidSeatClassMaps - V6 (lowercase name) | Tạo SeatClassMap hợp lệ (tên hạng ghế chữ thường) | Tạo thành công, chuẩn hóa thành chữ hoa |
| SCM_E01-E03 | SeatClassMapTest | ValidSeatClassMaps - E1-E3 (edge cases) | Tạo SeatClassMap hợp lệ (số ghế min/max) | Tạo thành công, toString() đúng |
| SCM_I01 | SeatClassMapTest | InvalidSeatClassMaps - I1 ("") | Tạo SeatClassMap với chuỗi rỗng | Tạo thất bại, lỗi "EMPTY_SEAT_CLASS_MAP" |
| SCM_I02-I06 | SeatClassMapTest | InvalidSeatClassMaps - I2-I6, I11, I15 (invalid format) | Tạo SeatClassMap với định dạng không hợp lệ | Tạo thất bại, lỗi "INVALID_FORMAT" |
| SCM_I07-I10 | SeatClassMapTest | InvalidSeatClassMaps - I7-I10 (invalid class name) | Tạo SeatClassMap với tên hạng ghế không hợp lệ | Tạo thất bại, lỗi "INVALID_SEAT_TYPE" |
| SCM_I11-I12 | SeatClassMapTest | InvalidSeatClassMaps - I12-I13 (invalid seat count) | Tạo SeatClassMap với số ghế không hợp lệ | Tạo thất bại, lỗi "INVALID_SEAT_COUNT" |
| **ScheduleTest** |||||
| SCH_V01-V02 | ScheduleTest | ValidSchedules - V1-V2 (basic valid) | Tạo Schedule hợp lệ (cùng ngày, qua ngày) | Tạo thành công, toString() đúng |
| SCH_V03-V05 | ScheduleTest | ValidSchedules - V3-V5 (time formats) | Tạo Schedule hợp lệ với các định dạng giờ khác nhau | Tạo thành công, toString() đúng |
| SCH_E01-E03 | ScheduleTest | ValidSchedules - E1-E3 (edge cases) | Tạo Schedule hợp lệ (năm nhuận, qua năm, 1 phút) | Tạo thành công, toString() đúng |
| SCH_I01 | ScheduleTest | InvalidSchedules - I1 ("") | Tạo Schedule với chuỗi rỗng | Tạo thất bại, lỗi "EMPTY_SCHEDULE" |
| SCH_I02-I04 | ScheduleTest | InvalidSchedules - I2-I3 (invalid format) | Tạo Schedule với định dạng chuỗi không hợp lệ | Tạo thất bại, lỗi "INVALID_FORMAT" |
| SCH_I05-I12 | ScheduleTest | InvalidSchedules - I7-I10, I5 (invalid departure time) | Tạo Schedule với thời gian đi không hợp lệ | Tạo thất bại, lỗi "INVALID_DEPARTURE_TIME" |
| SCH_I13-I16 | ScheduleTest | InvalidSchedules - I11-I14, I4 (invalid arrival time) | Tạo Schedule với thời gian đến không hợp lệ | Tạo thất bại, lỗi "INVALID_ARRIVAL_TIME" |
| SCH_I17-I19 | ScheduleTest | InvalidSchedules - I15-I17 (arrival before departure) | Tạo Schedule với thời gian đến trước hoặc bằng thời gian đi | Tạo thất bại, lỗi "ARRIVAL_BEFORE_DEPARTURE" |
| SCH_P01 | ScheduleTest | CreateWithIndividualParams (valid) | Tạo Schedule từ 2 tm struct hợp lệ | Tạo thành công, getter trả về đúng |
| SCH_P02 | ScheduleTest | CreateWithIndividualParams (arrival before/equal) | Tạo Schedule từ 2 tm struct, đến trước/bằng đi | Tạo thất bại, lỗi "ARRIVAL_BEFORE_DEPARTURE" |
| SCH_S01 | ScheduleTest | CreateWithTwoStringParams (valid) | Tạo Schedule từ 2 chuỗi datetime hợp lệ | Tạo thành công, toString() đúng |
| SCH_S02 | ScheduleTest | CreateWithTwoStringParams (empty/invalid strings) | Tạo Schedule từ 2 chuỗi, có chuỗi rỗng/không hợp lệ | Tạo thất bại, lỗi tương ứng |
| SCH_S03 | ScheduleTest | CreateWithTwoStringParams (arrival before/equal) | Tạo Schedule từ 2 chuỗi, đến trước/bằng đi | Tạo thất bại, lỗi "ARRIVAL_BEFORE_DEPARTURE" |
| **SeatNumberTest** |||||
| SN_V01-V03 | SeatNumberTest | ValidSeatNumbers - V1-V3 (Economy) | Tạo SeatNumber hợp lệ cho hạng Economy | Tạo thành công, toString() đúng, getter đúng |
| SN_V04-V06 | SeatNumberTest | ValidSeatNumbers - V4-V6 (Business) | Tạo SeatNumber hợp lệ cho hạng Business | Tạo thành công, toString() đúng, getter đúng |
| SN_V07-V09 | SeatNumberTest | ValidSeatNumbers - V7-V9 (First) | Tạo SeatNumber hợp lệ cho hạng First | Tạo thành công, toString() đúng, getter đúng |
| SN_I01 | SeatNumberTest | InvalidSeatNumbers - I1 ("") | Tạo SeatNumber với chuỗi rỗng | Tạo thất bại, lỗi "EMPTY_SEAT_NUMBER" |
| SN_I02-I06 | SeatNumberTest | InvalidSeatNumbers - I2-I6, I8-I9, I14 (invalid format) | Tạo SeatNumber với định dạng không hợp lệ | Tạo thất bại, lỗi "INVALID_FORMAT" |
| SN_I07 | SeatNumberTest | InvalidSeatNumbers - I7 ("X001") | Tạo SeatNumber với mã hạng ghế không tồn tại | Tạo thất bại, lỗi "INVALID_SEAT_CLASS" |
| SN_I08-I11 | SeatNumberTest | InvalidSeatNumbers - I10-I13 (invalid sequence) | Tạo SeatNumber với số thứ tự không hợp lệ | Tạo thất bại, lỗi "INVALID_SEQUENCE_NUMBER" |
| SN_G01 | SeatNumberTest | Getters | Kiểm tra getClassCode, getSequenceNumber | Trả về mã hạng và số thứ tự đúng |
| SN_OP01 | SeatNumberTest | ValueObjectOperations | Kiểm tra equals, not equals | equals đúng, not equals đúng |
| **MySQLXConnectionTest** |||||
| MYC_001 | MySQLXConnectionTest | ConnectionTest | Kết nối, ngắt kết nối, kết nối lại DB | Các thao tác thành công, isConnected trả về đúng |
| MYC_002 | MySQLXConnectionTest | BasicQueryTest | Thực thi INSERT, SELECT, DELETE cơ bản | Các câu lệnh thực thi thành công, dữ liệu được thay đổi |
| MYC_003 | MySQLXConnectionTest | PreparedStatementTest | Chuẩn bị, set tham số, thực thi, giải phóng prepared statement | Các thao tác thành công, dữ liệu được INSERT đúng |
| MYC_004 | MySQLXConnectionTest | TransactionTest | Bắt đầu, rollback, commit transaction | Rollback hủy thay đổi, commit lưu thay đổi |
| MYC_005 | MySQLXConnectionTest | ErrorHandlingTest | Xử lý lỗi khi truy vấn/statement/tham số không hợp lệ | Thao tác thất bại, trả về Result không có giá trị |
| MYC_006 | MySQLXConnectionTest | DataTypeTest | Kiểm tra đọc/ghi các kiểu dữ liệu | Đọc/ghi thành công, giá trị và kiểu dữ liệu khớp |
| MYC_007 | MySQLXConnectionTest | ConcurrentAccessTest | Thực thi truy vấn đồng thời từ nhiều thread | Tất cả thread thực thi thành công |
| **TicketNumberTest** |||||
| TN_V01-V04 | TicketNumberTest | ValidTicketNumbers - V1-V4 (prefix digits) | Tạo TicketNumber hợp lệ với số chữ số khác nhau sau "MC" | Tạo thành công, toString() đúng |
| TN_V05-V07 | TicketNumberTest | ValidTicketNumbers - V5-V7 (sequence) | Tạo TicketNumber hợp lệ với các sequence khác nhau | Tạo thành công, toString() đúng |
| TN_I01 | TicketNumberTest | InvalidTicketNumbers - I1 ("") | Tạo TicketNumber với chuỗi rỗng | Tạo thất bại, lỗi "EMPTY_TICKET_NUMBER" |
| TN_I02-I15 | TicketNumberTest | InvalidTicketNumbers - I2-I15 (invalid format) | Tạo TicketNumber với định dạng không hợp lệ | Tạo thất bại, lỗi "INVALID_FORMAT" |
| TN_OP01 | TicketNumberTest | ValueObjectOperations | Kiểm tra equals, not equals | equals đúng, not equals đúng |
| **TicketRepositoryTest** |||||
| TR_001 | TicketRepositoryTest | CreateTicket | Lưu Ticket mới vào DB | Lưu thành công, có ID > 0, dữ liệu khớp |
| TR_002 | TicketRepositoryTest | FindTicketById | Tìm Ticket bằng ID | Tìm thấy Ticket, dữ liệu khớp |
| TR_003 | TicketRepositoryTest | FindAllTickets | Lấy tất cả Ticket từ DB | Danh sách không rỗng, chứa Ticket test |
| TR_004 | TicketRepositoryTest | ExistsTicket (by ID) | Kiểm tra sự tồn tại của Ticket bằng ID | True cho ID tồn tại, False cho ID không tồn tại |
| TR_005 | TicketRepositoryTest | CountTickets | Đếm số lượng Ticket | Số lượng tăng 1 sau khi thêm |
| TR_006 | TicketRepositoryTest | UpdateTicket | Cập nhật Ticket đã có (price, status) | Cập nhật thành công, dữ liệu mới được lưu |
| TR_007 | TicketRepositoryTest | DeleteTicketById | Xóa Ticket bằng ID | Xóa thành công, Ticket không còn tồn tại |
| TR_008 | TicketRepositoryTest | FindByTicketNumber | Tìm Ticket bằng TicketNumber | Tìm thấy Ticket, dữ liệu khớp |
| TR_009 | TicketRepositoryTest | ExistsTicketNumber | Kiểm tra sự tồn tại của Ticket bằng TicketNumber | True cho TicketNumber tồn tại, False cho TicketNumber không tồn tại |
| TR_010 | TicketRepositoryTest | FindTicketsByPassengerId | Tìm các Ticket của một Passenger | Danh sách chứa Ticket test của Passenger đó |
| TR_011 | TicketRepositoryTest | FindTicketsBySerialNumber (Aircraft) | Tìm các Ticket liên quan đến một Aircraft | Danh sách chứa Ticket test của Aircraft đó |
| TR_012 | TicketRepositoryTest | ErrorHandling | Xử lý lỗi khi tìm ID/TicketNumber không hợp lệ | Thao tác thất bại, trả về lỗi phù hợp |
| **TicketTest** |||||
| TKT_001 | TicketTest | CreateTicket | Tạo Ticket hợp lệ, getter trả về đúng | Tạo thành công, getter khớp, isCheckedIn là false |
| TKT_002 | TicketTest | CreateTicketWithInvalidParameters (null flight) | Tạo Ticket với flight null | Tạo thất bại, lỗi "INVALID_FLIGHT" |
| TKT_003 | TicketTest | CreateTicketWithInvalidParameters (null passenger) | Tạo Ticket với passenger null | Tạo thất bại, lỗi "INVALID_PASSENGER" |
| TKT_004 | TicketTest | CheckInOperations | Kiểm tra checkIn, cancelCheckIn, và các trường hợp ném lỗi | Trạng thái isCheckedIn thay đổi đúng, ném CoreError khi thao tác không hợp lệ |
| TKT_005 | TicketTest | ValueObjectOperations | Kiểm tra equals, clone, toString | equals đúng, clone hợp lệ, toString đúng định dạng |
| TKT_006 | TicketTest | DifferentSeatNumbers | Tạo Ticket với các SeatNumber hợp lệ khác nhau | Tạo thành công, getSeatNumber() trả về đúng |
| TKT_007 | TicketTest | DifferentPrices | Tạo Ticket với các Price hợp lệ khác nhau | Tạo thành công, getPrice() trả về đúng |
| TKT_008 | TicketTest | DifferentTicketNumbers | Tạo Ticket với các TicketNumber hợp lệ khác nhau | Tạo thành công, getTicketNumber() trả về đúng |

***(CƯỜNG BỔ SUNG THÊM PHẦN SERVICES)***

#### C. Test Report

##### Tóm tắt tổng thể

| Thống kê | Số lượng | Tỷ lệ |
|-|-|-|
| **Tổng số Test Suite đã thực thi** | 19 | 100% |
| **Tổng số Test Case dự kiến/thực thi** | 360 | 100% |
| **Số Test Case Pass** | 360 | 100% |
| **Số Test Case Fail** | 0 | 0% |
| **Số Test Case Bỏ qua (Skipped)** | 0 | 0% |

***(NOTE: CẬP NHẬT LẠI)***

**Biểu đồ trạng thái Test Case:**
```
PASS:    [████████████████████████████████████████] 100% (360/360)
FAIL:    [                                        ] 0%   (0/360)
SKIPPED: [                                        ] 0%   (0/360)
```

***(NOTE: CẬP NHẬT LẠI)***

##### Chi tiết kết quả theo Test Suite

| ID | Test Suite | Tổng TC | TC Pass | TC Fail | Tỷ lệ Pass | Thời gian (s) | Ghi chú |
|-|-|-|-|-|-|-|-|
| 1 | AircraftTest | 8 | 8 | 0 | 100% | TBD | |
| 2 | AircraftRepositoryTest | 8 | 8 | 0 | 100% | TBD | |
| 3 | AircraftSerialTest | 24 | 24 | 0 | 100% | TBD | |
| 4 | FlightNumberTest | 21 | 21 | 0 | 100% | TBD | |
| 5 | FlightRepositoryTest | 8 | 8 | 0 | 100% | TBD | |
| 6 | ContactInfoTest | 35 | 35 | 0 | 100% | TBD | |
| 7 | FlightTest | 9 | 9 | 0 | 100% | TBD | |
| 8 | PassengerRepositoryTest | 10 | 10 | 0 | 100% | TBD | |
| 9 | PassportNumberTest | 30 | 30 | 0 | 100% | TBD | |
| 10 | PassengerTest | 8 | 8 | 0 | 100% | TBD | |
| 11 | PriceTest | 34 | 34 | 0 | 100% | TBD | |
| 12 | RouteTest | 25 | 25 | 0 | 100% | TBD | |
| 13 | SeatClassMapTest | 24 | 24 | 0 | 100% | TBD | |
| 14 | ScheduleTest | 35 | 35 | 0 | 100% | TBD | |
| 15 | SeatNumberTest | 26 | 26 | 0 | 100% | TBD | |
| 16 | MySQLXConnectionTest | 7 | 7 | 0 | 100% | TBD | |
| 17 | TicketNumberTest | 23 | 23 | 0 | 100% | TBD | |
| 18 | TicketRepositoryTest | 12 | 12 | 0 | 100% | TBD | |
| 19 | TicketTest | 8 | 8 | 0 | 100% | TBD | |
| **Tổng cộng** | **360** | **360** | **0** | **100%** | **TBD** | |

***(NOTE: CƯỜNG BỔ SUNG SERVICES)***

**Biểu đồ kết quả theo Test Suite:**
```
AircraftTest:           [██████████] 100%
AircraftRepositoryTest: [██████████] 100%
AircraftSerialTest:     [██████████] 100%
FlightNumberTest:       [██████████] 100%
FlightRepositoryTest:   [██████████] 100%
ContactInfoTest:        [██████████] 100%
FlightTest:             [██████████] 100%
PassengerRepositoryTest:[██████████] 100%
PassportNumberTest:     [██████████] 100%
PassengerTest:          [██████████] 100%
PriceTest:              [██████████] 100%
RouteTest:              [██████████] 100%
SeatClassMapTest:       [██████████] 100%
ScheduleTest:           [██████████] 100%
SeatNumberTest:         [██████████] 100%
MySQLXConnectionTest:   [██████████] 100%
TicketNumberTest:       [██████████] 100%
TicketRepositoryTest:   [██████████] 100%
TicketTest:             [██████████] 100%
```
***(NOTE: CƯỜNG BỔ SUNG SERVICES)***

##### Danh sách lỗi (Defects)

*(Không có lỗi nào được phát hiện trong quá trình thực thi kiểm thử đơn vị này. Nếu có sẽ bổ sung thêm vào bảng bên dưới)*

| ID Lỗi | Mức độ ưu tiên | Mức độ nghiêm trọng | Mô tả lỗi | Test Case ID | Module Ảnh hưởng | Trạng thái | Người sửa | Ghi chú |
|-|-|-|-|-|-|-|-|-|
| - | - | - | - | - | - | - | - | - |

##### Phân tích và Đánh giá

Kết quả 100% test case pass cho thấy các module được kiểm thử hoạt động ổn định và đúng đắn theo các kịch bản đã định nghĩa. Chất lượng của các thành phần đơn vị là tốt, không phát hiện lỗi nghiêm trọng nào. Điều này tạo nền tảng vững chắc cho các giai đoạn kiểm thử tích hợp và hệ thống tiếp theo.

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

**Comment Code**:
- Sử dụng comment Doxygen cho tất cả các lớp, phương thức, và thuộc tính.
- Mỗi file bắt đầu với một comment Doxygen mô tả file.
- Mỗi lớp bắt đầu với một comment Doxygen mô tả lớp.
- Mỗi phương thức bắt đầu với một comment Doxygen mô tả phương thức, tham số, và giá trị trả về.

**Xử lý lỗi**:
* Sử dụng `Result<T>` (dựa trên `std::expected`) để xử lý lỗi một cách tường minh và có cấu trúc, tránh việc chương trình bị dừng đột ngột và cho phép client xử lý lỗi một cách linh hoạt.
* Ghi log (Logging): Hệ thống có cơ chế ghi log các hoạt động và lỗi (thông qua `Logger`) để phục vụ việc DEBUG chương trình phức tạp và giúp dễ dàng hơn trong việc xử lý các lỗi chương trình.

**Quản lý bộ nhớ**:
- Sử dụng smart pointers (`std::unique_ptr`, `std::shared_ptr`) thay vì raw pointers
- Tuân thủ [Rule of Three/Five/Zero](https://en.cppreference.com/w/cpp/language/rule_of_three).
- Tránh rò rỉ bộ nhớ bằng cách giải phóng tài nguyên trong destructor (Ví dụ như hàm `disconnect` để ngắt kết nối database, tránh việc vẫn còn kết nối khi chương trình kết thúc hoặc trong các trường hợp sử dụng raw pointer).

#### 4.3. Quản lý mã nguồn với Git
Sử dụng "Feature Branch Workflow" để quản lý mã nguồn với Git trên nền tảng Github:
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


### 5. Tài liệu mô tả kiến trúc phần mềm và các module (10%)
#### 5.1. Tài liệu sinh từ Doxygen:
- Bổ sung comment code theo tài liệu Doxygen trong mã nguồn để sinh tài liệu mô tả mã nguồn.
***(Bổ sung sau)***

#### 5.2. Mô hình hoá bằng mermaid
##### a. Class Diagram
***(NOTE: Nhật Cường bổ sung, chia nhỏ các class diagram ra theo từng tầng)***

##### b. Lược đồ ER
***(NOTE: Phúc Hoàng bổ sung)***

### 6. Các chủ đề nâng cao (15%)
***(NOTE: Bổ sung sau)***

---
## IV. Hướng dẫn chạy chương trình
***(NOTE: Bổ sung sau)***

---
## V. Link video demo
***(NOTE: Bổ sung sau)***