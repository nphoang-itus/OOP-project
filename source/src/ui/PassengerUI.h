#ifndef PASSENGER_WINDOW_H
#define PASSENGER_WINDOW_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/Passenger.h"
#include "services/PassengerService.h"

class PassengerWindow : public wxFrame
{
public:
    PassengerWindow(const wxString &title);

private:
    void OnBack(wxCommandEvent &event);
    void OnShowPassengers(wxCommandEvent &event);
    void OnAddPassenger(wxCommandEvent &event);
    void OnEditPassenger(wxCommandEvent &event);
    void OnDeletePassenger(wxCommandEvent &event);
    void OnListItemSelected(wxListEvent &event);
    void RefreshPassengerList();

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *backButton;
    wxButton *showButton;
    wxButton *addButton;
    wxButton *editButton;
    wxButton *deleteButton;
    wxListCtrl *passengerList;
    wxStaticText *infoLabel;

    PassengerService passengerService;

    DECLARE_EVENT_TABLE()
};

#endif // PASSENGER_WINDOW_H