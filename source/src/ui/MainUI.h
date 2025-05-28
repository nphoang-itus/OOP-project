#ifndef MAIN_UI_H
#define MAIN_UI_H

#include <wx/wx.h>
#include <memory>
#include "services/AircraftService.h"
#include "services/FlightService.h"
#include "services/PassengerService.h"

// Forward declarations
class AircraftWindow;
class FlightWindow;
class PassengerWindow;

class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString &title,
               std::shared_ptr<AircraftService> aircraftService,
               std::shared_ptr<FlightService> flightService,
               std::shared_ptr<PassengerService> passengerService);

    // Getter methods for services
    std::shared_ptr<AircraftService> getAircraftService() const { return aircraftService; }
    std::shared_ptr<FlightService> getFlightService() const { return flightService; }
    std::shared_ptr<PassengerService> getPassengerService() const { return passengerService; }

private:
    void OnAircraftService(wxCommandEvent &event);
    void OnFlightService(wxCommandEvent &event);
    void OnPassengerService(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *aircraftButton;
    wxButton *flightButton;
    wxButton *passengerButton;
    wxButton *exitButton;

    std::shared_ptr<AircraftService> aircraftService;
    std::shared_ptr<FlightService> flightService;
    std::shared_ptr<PassengerService> passengerService;

    DECLARE_EVENT_TABLE()
};

#endif // MAIN_UI_H