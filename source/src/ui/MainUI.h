#ifndef MAIN_UI_H
#define MAIN_UI_H

#include <wx/wx.h>
#include <memory>
#include "services/AircraftService.h"
#include "services/FlightService.h"
#include "services/PassengerService.h"
#include "services/TicketService.h"

// Forward declarations
class AircraftWindow;
class FlightWindow;
class PassengerWindow;
class TicketWindow;

/**
 * @brief Enum định nghĩa các loại UI window
 */
enum class UIType
{
    AIRCRAFT,   ///< Cửa sổ quản lý máy bay
    FLIGHT,     ///< Cửa sổ quản lý chuyến bay
    PASSENGER,  ///< Cửa sổ quản lý hành khách
    TICKET      ///< Cửa sổ quản lý vé máy bay
};

/**
 * @brief Cửa sổ chính của ứng dụng quản lý hàng không
 * 
 * Lớp này cung cấp giao diện chính để điều hướng đến các module quản lý
 * khác nhau như máy bay, chuyến bay, hành khách và vé máy bay.
 */
class MainWindow : public wxFrame
{
private:
    /**
     * @brief Factory class để tạo các cửa sổ UI
     * 
     * Lớp này sử dụng Factory Pattern để tạo ra các cửa sổ UI khác nhau
     * dựa trên loại UI được yêu cầu.
     */
    class UIFactory
    {
    private:
        /// Service quản lý máy bay
        std::shared_ptr<AircraftService> aircraftService;
        /// Service quản lý chuyến bay
        std::shared_ptr<FlightService> flightService;
        /// Service quản lý hành khách
        std::shared_ptr<PassengerService> passengerService;
        /// Service quản lý vé máy bay
        std::shared_ptr<TicketService> ticketService;

    public:
        /**
         * @brief Constructor cho UIFactory
         * @param aircraft Service quản lý máy bay
         * @param flight Service quản lý chuyến bay
         * @param passenger Service quản lý hành khách
         * @param ticket Service quản lý vé máy bay
         */
        UIFactory(std::shared_ptr<AircraftService> aircraft,
                  std::shared_ptr<FlightService> flight,
                  std::shared_ptr<PassengerService> passenger,
                  std::shared_ptr<TicketService> ticket)
            : aircraftService(aircraft), flightService(flight),
              passengerService(passenger), ticketService(ticket) {}

        /**
         * @brief Tạo cửa sổ UI theo loại yêu cầu
         * @param type Loại UI cần tạo
         * @param title Tiêu đề của cửa sổ
         * @return Pointer đến cửa sổ được tạo
         */
        wxFrame *createWindow(UIType type, const wxString &title);
    };

public:
    /**
     * @brief Constructor cho cửa sổ chính
     * @param title Tiêu đề của cửa sổ
     * @param aircraftService Service quản lý máy bay
     * @param flightService Service quản lý chuyến bay
     * @param passengerService Service quản lý hành khách
     * @param ticketService Service quản lý vé máy bay
     */
    MainWindow(const wxString &title,
               std::shared_ptr<AircraftService> aircraftService,
               std::shared_ptr<FlightService> flightService,
               std::shared_ptr<PassengerService> passengerService,
               std::shared_ptr<TicketService> ticketService);

    /**
     * @brief Lấy service quản lý máy bay
     * @return Shared pointer đến AircraftService
     */
    std::shared_ptr<AircraftService> getAircraftService() const { return aircraftService; }

    /**
     * @brief Lấy service quản lý chuyến bay
     * @return Shared pointer đến FlightService
     */
    std::shared_ptr<FlightService> getFlightService() const { return flightService; }

    /**
     * @brief Lấy service quản lý hành khách
     * @return Shared pointer đến PassengerService
     */
    std::shared_ptr<PassengerService> getPassengerService() const { return passengerService; }

    /**
     * @brief Lấy service quản lý vé máy bay
     * @return Shared pointer đến TicketService
     */
    std::shared_ptr<TicketService> getTicketService() const { return ticketService; }

private:
    /**
     * @brief Xử lý sự kiện mở cửa sổ quản lý máy bay
     * @param event Sự kiện nút bấm
     */
    void OnAircraftService(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện mở cửa sổ quản lý chuyến bay
     * @param event Sự kiện nút bấm
     */
    void OnFlightService(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện mở cửa sổ quản lý hành khách
     * @param event Sự kiện nút bấm
     */
    void OnPassengerService(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện mở cửa sổ quản lý vé máy bay
     * @param event Sự kiện nút bấm
     */
    void OnTicketService(wxCommandEvent &event);

    /**
     * @brief Xử lý sự kiện thoát ứng dụng
     * @param event Sự kiện nút bấm
     */
    void OnExit(wxCommandEvent &event);

    /// Panel chính chứa các thành phần giao diện
    wxPanel *panel;
    /// Sizer chính để quản lý layout
    wxBoxSizer *mainSizer;
    /// Nút mở cửa sổ quản lý máy bay
    wxButton *aircraftButton;
    /// Nút mở cửa sổ quản lý chuyến bay
    wxButton *flightButton;
    /// Nút mở cửa sổ quản lý hành khách
    wxButton *passengerButton;
    /// Nút mở cửa sổ quản lý vé máy bay
    wxButton *ticketButton;
    /// Nút thoát ứng dụng
    wxButton *exitButton;

    /// Service quản lý máy bay
    std::shared_ptr<AircraftService> aircraftService;
    /// Service quản lý chuyến bay
    std::shared_ptr<FlightService> flightService;
    /// Service quản lý hành khách
    std::shared_ptr<PassengerService> passengerService;
    /// Service quản lý vé máy bay
    std::shared_ptr<TicketService> ticketService;

    /// Factory để tạo các cửa sổ UI
    std::unique_ptr<UIFactory> uiFactory;

    DECLARE_EVENT_TABLE()
};

#endif // MAIN_UI_H