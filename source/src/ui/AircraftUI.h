#ifndef AIRCRAFT_UI_H
#define AIRCRAFT_UI_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/entities/Aircraft.h"
#include "services/AircraftService.h"

/**
 * @brief Cửa sổ quản lý máy bay
 * 
 * Lớp này cung cấp giao diện người dùng để quản lý thông tin máy bay
 * bao gồm hiển thị, thêm, sửa, xóa và tìm kiếm máy bay.
 */
class AircraftWindow : public wxFrame
{
public:
    /**
     * @brief Constructor cho cửa sổ quản lý máy bay
     * @param title Tiêu đề của cửa sổ
     * @param aircraftService Service quản lý máy bay
     */
    AircraftWindow(const wxString &title, std::shared_ptr<AircraftService> aircraftService);

    /**
     * @brief Thiết lập các service cần thiết cho việc điều hướng
     * @param aircraft Service quản lý máy bay
     * @param flight Service quản lý chuyến bay
     * @param passenger Service quản lý hành khách
     * @param ticket Service quản lý vé máy bay
     */
    void setServices(std::shared_ptr<AircraftService> aircraft,
                     std::shared_ptr<FlightService> flight,
                     std::shared_ptr<PassengerService> passenger,
                     std::shared_ptr<TicketService> ticket);

private:
    /**
     * @brief Xử lý sự kiện quay lại menu chính
     * @param event Sự kiện nút bấm
     */
    void OnBack(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện hiển thị danh sách máy bay
     * @param event Sự kiện nút bấm
     */
    void OnShowAircraft(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện thêm máy bay mới
     * @param event Sự kiện nút bấm
     */
    void OnAddAircraft(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện chỉnh sửa thông tin máy bay
     * @param event Sự kiện nút bấm
     */
    void OnEditAircraft(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện xóa máy bay
     * @param event Sự kiện nút bấm
     */
    void OnDeleteAircraft(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện tìm kiếm máy bay theo ID
     * @param event Sự kiện nút bấm
     */
    void OnSearchById(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện tìm kiếm máy bay theo số đăng ký
     * @param event Sự kiện nút bấm
     */
    void OnSearchByRegistration(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện kiểm tra sự tồn tại của máy bay
     * @param event Sự kiện nút bấm
     */
    void OnCheckAircraftExists(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện chọn item trong danh sách
     * @param event Sự kiện chọn item
     */
    void OnListItemSelected(wxListEvent &event);

    /**
     * @brief Làm mới danh sách máy bay
     */
    void RefreshAircraftList();

    /// Panel chính chứa các thành phần giao diện
    wxPanel *panel;
    /// Sizer chính để quản lý layout
    wxBoxSizer *mainSizer;
    /// Nút quay lại menu chính
    wxButton *backButton;
    /// Nút hiển thị danh sách máy bay
    wxButton *showButton;
    /// Nút thêm máy bay mới
    wxButton *addButton;
    /// Nút chỉnh sửa thông tin máy bay
    wxButton *editButton;
    /// Nút xóa máy bay
    wxButton *deleteButton;
    /// Nút tìm kiếm theo ID
    wxButton *searchByIdButton;
    /// Nút tìm kiếm theo số đăng ký
    wxButton *searchByRegistrationButton;
    /// Nút kiểm tra sự tồn tại của máy bay
    wxButton *checkAircraftExistsButton;
    /// Danh sách hiển thị thông tin máy bay
    wxListCtrl *aircraftList;
    /// Label hiển thị thông tin bổ sung
    wxStaticText *infoLabel;

    /// Service quản lý máy bay
    std::shared_ptr<AircraftService> aircraftService;
    /// Service quản lý chuyến bay
    std::shared_ptr<FlightService> flightService;
    /// Service quản lý hành khách
    std::shared_ptr<PassengerService> passengerService;
    /// Service quản lý vé máy bay
    std::shared_ptr<TicketService> ticketService;

    DECLARE_EVENT_TABLE()
};

#endif // AIRCRAFT_UI_H