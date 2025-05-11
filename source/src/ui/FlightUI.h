#ifndef FLIGHT_UI_H
#define FLIGHT_UI_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/Flight.h"
#include "services/FlightService.h"

class FlightWindow : public wxFrame
{
public:
    FlightWindow(const wxString &title);

private:
    void OnBack(wxCommandEvent &event);
    void OnShowFlights(wxCommandEvent &event);
    void OnAddFlight(wxCommandEvent &event);
    void OnEditFlight(wxCommandEvent &event);
    void OnDeleteFlight(wxCommandEvent &event);
    void OnSearchById(wxCommandEvent &event);
    void OnSearchByRoute(wxCommandEvent &event);
    void OnListItemSelected(wxListEvent &event);
    void RefreshFlightList();

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *backButton;
    wxButton *showButton;
    wxButton *addButton;
    wxButton *editButton;
    wxButton *deleteButton;
    wxButton *searchByIdButton;
    wxButton *searchByRouteButton;
    wxListCtrl *flightList;
    wxStaticText *infoLabel;

    FlightService flightService;

    DECLARE_EVENT_TABLE()
};

#endif // FLIGHT_UI_H