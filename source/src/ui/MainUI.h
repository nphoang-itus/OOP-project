#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <wx/wx.h>
#include <memory>
#include "services/FlightService.h"
#include "services/PassengerService.h"
#include "services/ReservationService.h"

// Forward declarations
class FlightWindow;
class PassengerWindow;
class ReservationWindow;

class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString &title,
               std::shared_ptr<FlightService> flightService,
               std::shared_ptr<PassengerService> passengerService,
               std::shared_ptr<ReservationService> reservationService);

    // Getter methods for services
    std::shared_ptr<FlightService> getFlightService() const { return flightService; }
    std::shared_ptr<PassengerService> getPassengerService() const { return passengerService; }
    std::shared_ptr<ReservationService> getReservationService() const { return reservationService; }

private:
    void OnFlightService(wxCommandEvent &event);
    void OnPassengerService(wxCommandEvent &event);
    void OnReservationService(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *flightButton;
    wxButton *passengerButton;
    wxButton *reservationButton;
    wxButton *exitButton;

    std::shared_ptr<FlightService> flightService;
    std::shared_ptr<PassengerService> passengerService;
    std::shared_ptr<ReservationService> reservationService;

    DECLARE_EVENT_TABLE()
};

// Factory pattern for creating UI windows
class UIWindowFactory
{
public:
    static FlightWindow *createFlightWindow(const wxString &title, std::shared_ptr<FlightService> flightService, wxWindow *parent = nullptr);
    static PassengerWindow *createPassengerWindow(const wxString &title, std::shared_ptr<PassengerService> passengerService, wxWindow *parent = nullptr);
    static ReservationWindow *createReservationWindow(const wxString &title, std::shared_ptr<ReservationService> reservationService, std::shared_ptr<FlightService> flightService, std::shared_ptr<PassengerService> passengerService, wxWindow *parent = nullptr);
};

#endif // MAIN_WINDOW_H