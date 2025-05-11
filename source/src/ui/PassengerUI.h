#ifndef PASSENGER_UI_H
#define PASSENGER_UI_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/Passenger.h"
#include "services/PassengerService.h"

class PassengerWindow : public wxFrame
{
public:
    PassengerWindow(const wxString &title, std::shared_ptr<PassengerService> passengerService);

private:
    void OnBack(wxCommandEvent &event);
    void OnShowPassengers(wxCommandEvent &event);
    void OnAddPassenger(wxCommandEvent &event);
    void OnEditPassenger(wxCommandEvent &event);
    void OnDeletePassenger(wxCommandEvent &event);
    void OnSearchById(wxCommandEvent &event);
    void OnSearchByName(wxCommandEvent &event);
    void OnSearchByPassport(wxCommandEvent &event);
    void OnListItemSelected(wxListEvent &event);
    void RefreshPassengerList();

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *backButton;
    wxButton *showButton;
    wxButton *addButton;
    wxButton *editButton;
    wxButton *deleteButton;
    wxButton *searchByIdButton;
    wxButton *searchByNameButton;
    wxButton *searchByPassportButton;
    wxListCtrl *passengerList;
    wxStaticText *infoLabel;

    std::shared_ptr<PassengerService> passengerService;

    DECLARE_EVENT_TABLE()
};

#endif // PASSENGER_UI_H