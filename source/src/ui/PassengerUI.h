#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/entities/Passenger.h"
#include "services/PassengerService.h"

class PassengerWindow : public wxFrame
{
public:
    PassengerWindow(const wxString &title, std::shared_ptr<PassengerService> passengerService);

    // Store services for navigation back to main window
    void setServices(std::shared_ptr<AircraftService> aircraft,
                     std::shared_ptr<FlightService> flight,
                     std::shared_ptr<PassengerService> passenger,
                     std::shared_ptr<TicketService> ticket);

private:
    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *backButton;
    wxButton *showButton;
    wxButton *addButton;
    wxButton *editButton;
    wxButton *deleteButton;
    wxButton *searchByIdButton;
    wxButton *searchByPassportButton;
    wxButton *checkBookingsButton;
    wxButton *viewStatsButton;
    wxListCtrl *passengerList;
    wxStaticText *infoLabel;

    std::shared_ptr<PassengerService> passengerService;
    std::shared_ptr<AircraftService> aircraftService;
    std::shared_ptr<FlightService> flightService;
    std::shared_ptr<TicketService> ticketService;

    // Add CreateUI method declaration
    void CreateUI();
    void RefreshPassengerList();

    void OnBack(wxCommandEvent &event);
    void OnShowPassenger(wxCommandEvent &event);
    void OnAddPassenger(wxCommandEvent &event);
    void OnEditPassenger(wxCommandEvent &event);
    void OnDeletePassenger(wxCommandEvent &event);
    void OnSearchById(wxCommandEvent &event);
    void OnSearchByPassport(wxCommandEvent &event);
    void OnCheckBookings(wxCommandEvent &event);
    void OnViewStats(wxCommandEvent &event);
    void OnListItemSelected(wxListEvent &event);

    DECLARE_EVENT_TABLE()
};