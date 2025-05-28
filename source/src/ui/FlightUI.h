#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/entities/Flight.h"
#include "services/FlightService.h"

class FlightWindow : public wxFrame
{
public:
    FlightWindow(const wxString &title, std::shared_ptr<FlightService> flightService);

    // Store services for navigation back to main window
    void setServices(std::shared_ptr<AircraftService> aircraft,
                     std::shared_ptr<FlightService> flight,
                     std::shared_ptr<PassengerService> passenger);

private:
    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *backButton;
    wxButton *showButton;
    wxButton *addButton;
    wxButton *editButton;
    wxButton *deleteButton;
    wxButton *searchByIdButton;
    wxButton *searchByFlightNumberButton;
    wxListCtrl *flightList;
    wxStaticText *infoLabel;

    std::shared_ptr<FlightService> flightService;
    std::shared_ptr<AircraftService> aircraftService;
    std::shared_ptr<PassengerService> passengerService;

    void OnBack(wxCommandEvent &event);
    void OnShowFlight(wxCommandEvent &event);
    void OnAddFlight(wxCommandEvent &event);
    void OnEditFlight(wxCommandEvent &event);
    void OnDeleteFlight(wxCommandEvent &event);
    void OnSearchById(wxCommandEvent &event);
    void OnSearchByFlightNumber(wxCommandEvent &event);
    void OnListItemSelected(wxListEvent &event);
    void RefreshFlightList();

    DECLARE_EVENT_TABLE()
};