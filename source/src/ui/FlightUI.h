#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/entities/Flight.h"
#include "services/FlightService.h"

/**
 * @brief Cửa sổ quản lý chuyến bay
 * 
 * Lớp này cung cấp giao diện người dùng để quản lý chuyến bay
 * bao gồm hiển thị, thêm, sửa, xóa, tìm kiếm và quản lý ghế ngồi.
 */
class FlightWindow : public wxFrame
{
public:
    /**
     * @brief Constructor cho cửa sổ quản lý chuyến bay
     * @param title Tiêu đề của cửa sổ
     * @param flightService Service quản lý chuyến bay
     */
    FlightWindow(const wxString &title, std::shared_ptr<FlightService> flightService);

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
    /// Nút hiển thị danh sách chuyến bay
    wxButton *showButton;
    /// Nút thêm chuyến bay mới
    wxButton *addButton;
    /// Nút chỉnh sửa thông tin chuyến bay
    wxButton *editButton;
    /// Nút xóa chuyến bay
    wxButton *deleteButton;
    /// Nút tìm kiếm theo ID
    wxButton *searchByIdButton;
    /// Nút tìm kiếm theo số hiệu chuyến bay
    wxButton *searchByFlightNumberButton;
    /// Nút xem ghế trống
    wxButton *viewAvailableSeatsButton;
    /// Nút kiểm tra tình trạng ghế
    wxButton *checkSeatAvailabilityButton;
    /// Danh sách hiển thị thông tin chuyến bay
    wxListCtrl *flightList;
    /// Label hiển thị thông tin bổ sung
    wxStaticText *infoLabel;

    /// Service quản lý chuyến bay
    std::shared_ptr<FlightService> flightService;
    /// Service quản lý máy bay
    std::shared_ptr<AircraftService> aircraftService;
    /// Service quản lý hành khách
    std::shared_ptr<PassengerService> passengerService;
    /// Service quản lý vé máy bay
    std::shared_ptr<TicketService> ticketService;

    /**
     * @brief Xử lý sự kiện quay lại menu chính
     * @param event Sự kiện nút bấm
     */
    void OnBack(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện hiển thị danh sách chuyến bay
     * @param event Sự kiện nút bấm
     */
    void OnShowFlight(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện thêm chuyến bay mới
     * @param event Sự kiện nút bấm
     */
    void OnAddFlight(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện chỉnh sửa chuyến bay
     * @param event Sự kiện nút bấm
     */
    void OnEditFlight(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện xóa chuyến bay
     * @param event Sự kiện nút bấm
     */
    void OnDeleteFlight(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện tìm kiếm chuyến bay theo ID
     * @param event Sự kiện nút bấm
     */
    void OnSearchById(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện tìm kiếm chuyến bay theo số hiệu
     * @param event Sự kiện nút bấm
     */
    void OnSearchByFlightNumber(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện xem ghế trống
     * @param event Sự kiện nút bấm
     */
    void OnViewAvailableSeats(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện kiểm tra tình trạng ghế
     * @param event Sự kiện nút bấm
     */
    void OnCheckSeatAvailability(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện chọn item trong danh sách
     * @param event Sự kiện chọn item
     */
    void OnListItemSelected(wxListEvent &event);

    /**
     * @brief Làm mới danh sách chuyến bay
     */
    void RefreshFlightList();

    /**
     * @brief Lấy thông tin ghế của chuyến bay
     * @param flight Chuyến bay cần lấy thông tin ghế
     * @return Chuỗi mô tả thông tin ghế
     */
    std::string getSeatInfo(const Flight &flight);

    DECLARE_EVENT_TABLE()
};