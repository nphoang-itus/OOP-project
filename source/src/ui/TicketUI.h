#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/entities/Ticket.h"
#include "services/TicketService.h"

class TicketWindow : public wxFrame
{
public:
    TicketWindow(const wxString &title, std::shared_ptr<TicketService> ticketService);

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
    wxButton *searchButton;
    wxButton *refreshButton;
    wxListCtrl *ticketList;
    wxStaticText *infoLabel;

    std::shared_ptr<TicketService> ticketService;
    std::shared_ptr<AircraftService> aircraftService;
    std::shared_ptr<FlightService> flightService;
    std::shared_ptr<PassengerService> passengerService;

    void CreateUI();
    void RefreshTicketList();
    void ShowTicketDetails(const Ticket &ticket);
    void AddTicketDialog();
    void EditTicketDialog(const Ticket &ticket);
    void DeleteTicketDialog(const Ticket &ticket);
    void SearchTicketDialog();

    void OnBack(wxCommandEvent &event);
    void OnShowTicket(wxCommandEvent &event);
    void OnAddTicket(wxCommandEvent &event);
    void OnEditTicket(wxCommandEvent &event);
    void OnDeleteTicket(wxCommandEvent &event);
    void OnSearchTicket(wxCommandEvent &event);
    void OnRefresh(wxCommandEvent &event);
    void OnListItemSelected(wxListEvent &event);

    DECLARE_EVENT_TABLE()
};