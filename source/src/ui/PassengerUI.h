#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/entities/Passenger.h"
#include "services/PassengerService.h"

/**
 * @brief Cửa sổ quản lý hành khách
 * 
 * Lớp này cung cấp giao diện người dùng để quản lý thông tin hành khách
 * bao gồm hiển thị, thêm, sửa, xóa, tìm kiếm và xem thống kê hành khách.
 */
class PassengerWindow : public wxFrame
{
public:
    /**
     * @brief Constructor cho cửa sổ quản lý hành khách
     * @param title Tiêu đề của cửa sổ
     * @param passengerService Service quản lý hành khách
     */
    PassengerWindow(const wxString &title, std::shared_ptr<PassengerService> passengerService);

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
    /// Panel chính chứa các thành phần giao diện
    wxPanel *panel;
    /// Sizer chính để quản lý layout
    wxBoxSizer *mainSizer;
    /// Nút quay lại menu chính
    wxButton *backButton;
    /// Nút hiển thị danh sách hành khách
    wxButton *showButton;
    /// Nút thêm hành khách mới
    wxButton *addButton;
    /// Nút chỉnh sửa thông tin hành khách
    wxButton *editButton;
    /// Nút xóa hành khách
    wxButton *deleteButton;
    /// Nút tìm kiếm theo ID
    wxButton *searchByIdButton;
    /// Nút tìm kiếm theo số hộ chiếu
    wxButton *searchByPassportButton;
    /// Nút kiểm tra thông tin đặt chỗ
    wxButton *checkBookingsButton;
    /// Nút xem thống kê hành khách
    wxButton *viewStatsButton;
    /// Danh sách hiển thị thông tin hành khách
    wxListCtrl *passengerList;
    /// Label hiển thị thông tin bổ sung
    wxStaticText *infoLabel;

    /// Service quản lý hành khách
    std::shared_ptr<PassengerService> passengerService;
    /// Service quản lý máy bay
    std::shared_ptr<AircraftService> aircraftService;
    /// Service quản lý chuyến bay
    std::shared_ptr<FlightService> flightService;
    /// Service quản lý vé máy bay
    std::shared_ptr<TicketService> ticketService;

    /**
     * @brief Khởi tạo giao diện người dùng
     */
    void CreateUI();

    /**
     * @brief Làm mới danh sách hành khách
     */
    void RefreshPassengerList();

    /**
     * @brief Xử lý sự kiện quay lại menu chính
     * @param event Sự kiện nút bấm
     */
    void OnBack(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện hiển thị danh sách hành khách
     * @param event Sự kiện nút bấm
     */
    void OnShowPassenger(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện thêm hành khách mới
     * @param event Sự kiện nút bấm
     */
    void OnAddPassenger(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện chỉnh sửa thông tin hành khách
     * @param event Sự kiện nút bấm
     */
    void OnEditPassenger(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện xóa hành khách
     * @param event Sự kiện nút bấm
     */
    void OnDeletePassenger(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện tìm kiếm hành khách theo ID
     * @param event Sự kiện nút bấm
     */
    void OnSearchById(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện tìm kiếm hành khách theo số hộ chiếu
     * @param event Sự kiện nút bấm
     */
    void OnSearchByPassport(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện kiểm tra thông tin đặt chỗ của hành khách
     * @param event Sự kiện nút bấm
     */
    void OnCheckBookings(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện xem thống kê hành khách
     * @param event Sự kiện nút bấm
     */
    void OnViewStats(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện chọn item trong danh sách
     * @param event Sự kiện chọn item
     */
    void OnListItemSelected(wxListEvent &event);

    DECLARE_EVENT_TABLE()
};