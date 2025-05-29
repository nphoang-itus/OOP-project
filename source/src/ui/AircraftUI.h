#ifndef AIRCRAFT_UI_H
#define AIRCRAFT_UI_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/entities/Aircraft.h"
#include "services/AircraftService.h"

class AircraftWindow : public wxFrame
{
public:
    AircraftWindow(const wxString &title, std::shared_ptr<AircraftService> aircraftService);

    // Store services for navigation back to main window
    void setServices(std::shared_ptr<AircraftService> aircraft,
                     std::shared_ptr<FlightService> flight,
                     std::shared_ptr<PassengerService> passenger);

private:
    void OnBack(wxCommandEvent &event);
    void OnShowAircraft(wxCommandEvent &event);
    void OnAddAircraft(wxCommandEvent &event);
    void OnEditAircraft(wxCommandEvent &event);
    void OnDeleteAircraft(wxCommandEvent &event);
    void OnSearchById(wxCommandEvent &event);
    void OnSearchByRegistration(wxCommandEvent &event);
    void OnViewSeatClasses(wxCommandEvent &event);
    void OnViewAvailableSeats(wxCommandEvent &event);
    void OnCheckSeatAvailability(wxCommandEvent &event);
    void OnListItemSelected(wxListEvent &event);
    void RefreshAircraftList();

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *backButton;
    wxButton *showButton;
    wxButton *addButton;
    wxButton *editButton;
    wxButton *deleteButton;
    wxButton *searchByIdButton;
    wxButton *searchByRegistrationButton;
    wxButton *viewSeatClassesButton;
    wxButton *viewAvailableSeatsButton;
    wxButton *checkSeatAvailabilityButton;
    wxListCtrl *aircraftList;
    wxStaticText *infoLabel;

    std::shared_ptr<AircraftService> aircraftService;
    std::shared_ptr<FlightService> flightService;
    std::shared_ptr<PassengerService> passengerService;

    DECLARE_EVENT_TABLE()
};

#endif // AIRCRAFT_UI_H