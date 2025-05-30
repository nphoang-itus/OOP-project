#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/entities/Ticket.h"
#include "services/TicketService.h"

/**
 * @brief Cửa sổ quản lý vé máy bay
 * 
 * Lớp này cung cấp giao diện người dùng để quản lý vé máy bay
 * bao gồm đặt vé, hiển thị, chỉnh sửa, xóa và tìm kiếm vé.
 */
class TicketWindow : public wxFrame
{
public:
    /**
     * @brief Constructor cho cửa sổ quản lý vé máy bay
     * @param title Tiêu đề của cửa sổ
     * @param ticketService Service quản lý vé máy bay
     */
    TicketWindow(const wxString &title, std::shared_ptr<TicketService> ticketService);

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
    /// Nút hiển thị danh sách vé
    wxButton *showButton;
    /// Nút đặt vé mới
    wxButton *addButton;
    /// Nút chỉnh sửa thông tin vé
    wxButton *editButton;
    /// Nút hủy/xóa vé
    wxButton *deleteButton;
    /// Nút tìm kiếm vé
    wxButton *searchButton;
    /// Nút làm mới danh sách
    wxButton *refreshButton;
    /// Danh sách hiển thị thông tin vé
    wxListCtrl *ticketList;
    /// Label hiển thị thông tin bổ sung
    wxStaticText *infoLabel;

    /// Service quản lý vé máy bay
    std::shared_ptr<TicketService> ticketService;
    /// Service quản lý máy bay
    std::shared_ptr<AircraftService> aircraftService;
    /// Service quản lý chuyến bay
    std::shared_ptr<FlightService> flightService;
    /// Service quản lý hành khách
    std::shared_ptr<PassengerService> passengerService;

    /**
     * @brief Khởi tạo giao diện người dùng
     */
    void CreateUI();

    /**
     * @brief Làm mới danh sách vé
     */
    void RefreshTicketList();

    /**
     * @brief Hiển thị chi tiết thông tin vé
     * @param ticket Vé cần hiển thị chi tiết
     */
    void ShowTicketDetails(const Ticket &ticket);

    /**
     * @brief Hiển thị dialog đặt vé mới
     */
    void AddTicketDialog();

    /**
     * @brief Hiển thị dialog chỉnh sửa vé
     * @param ticket Vé cần chỉnh sửa
     */
    void EditTicketDialog(const Ticket &ticket);

    /**
     * @brief Hiển thị dialog xóa vé
     * @param ticket Vé cần xóa
     */
    void DeleteTicketDialog(const Ticket &ticket);

    /**
     * @brief Hiển thị dialog tìm kiếm vé
     */
    void SearchTicketDialog();

    /**
     * @brief Xử lý sự kiện quay lại menu chính
     * @param event Sự kiện nút bấm
     */
    void OnBack(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện hiển thị chi tiết vé
     * @param event Sự kiện nút bấm
     */
    void OnShowTicket(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện đặt vé mới
     * @param event Sự kiện nút bấm
     */
    void OnAddTicket(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện chỉnh sửa vé
     * @param event Sự kiện nút bấm
     */
    void OnEditTicket(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện xóa vé
     * @param event Sự kiện nút bấm
     */
    void OnDeleteTicket(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện tìm kiếm vé
     * @param event Sự kiện nút bấm
     */
    void OnSearchTicket(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện làm mới danh sách
     * @param event Sự kiện nút bấm
     */
    void OnRefresh(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện chọn item trong danh sách
     * @param event Sự kiện chọn item
     */
    void OnListItemSelected(wxListEvent &event);

    DECLARE_EVENT_TABLE()
};