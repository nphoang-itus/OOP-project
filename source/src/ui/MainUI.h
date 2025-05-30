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

enum class UIType
{
    AIRCRAFT,
    FLIGHT,
    PASSENGER,
    TICKET
};

class MainWindow : public wxFrame
{
private:
    // Nested Factory class for creating UI windows
    class UIFactory
    {
    private:
        std::shared_ptr<AircraftService> aircraftService;
        std::shared_ptr<FlightService> flightService;
        std::shared_ptr<PassengerService> passengerService;
        std::shared_ptr<TicketService> ticketService;

    public:
        UIFactory(std::shared_ptr<AircraftService> aircraft,
                  std::shared_ptr<FlightService> flight,
                  std::shared_ptr<PassengerService> passenger,
                  std::shared_ptr<TicketService> ticket)
            : aircraftService(aircraft), flightService(flight),
              passengerService(passenger), ticketService(ticket) {}

        wxFrame *createWindow(UIType type, const wxString &title);
    };

public:
    MainWindow(const wxString &title,
               std::shared_ptr<AircraftService> aircraftService,
               std::shared_ptr<FlightService> flightService,
               std::shared_ptr<PassengerService> passengerService,
               std::shared_ptr<TicketService> ticketService);

    // Getter methods for services
    std::shared_ptr<AircraftService> getAircraftService() const { return aircraftService; }
    std::shared_ptr<FlightService> getFlightService() const { return flightService; }
    std::shared_ptr<PassengerService> getPassengerService() const { return passengerService; }
    std::shared_ptr<TicketService> getTicketService() const { return ticketService; }

private:
    void OnAircraftService(wxCommandEvent &event);
    void OnFlightService(wxCommandEvent &event);
    void OnPassengerService(wxCommandEvent &event);
    void OnTicketService(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *aircraftButton;
    wxButton *flightButton;
    wxButton *passengerButton;
    wxButton *ticketButton;
    wxButton *exitButton;

    std::shared_ptr<AircraftService> aircraftService;
    std::shared_ptr<FlightService> flightService;
    std::shared_ptr<PassengerService> passengerService;
    std::shared_ptr<TicketService> ticketService;

    // Factory for creating UI windows
    std::unique_ptr<UIFactory> uiFactory;

    DECLARE_EVENT_TABLE()
};

#endif // MAIN_UI_H