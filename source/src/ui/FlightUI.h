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
    FlightWindow(const wxString &title, std::shared_ptr<FlightService> flightService);

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
    void ShowFlights(const std::vector<Flight> &flights);
    void ShowFlight(const Flight &flight);
    bool ShowInputDialog(const wxString &title, const wxString &label, wxString &outValue);
    bool ShowInputDialog2(const wxString &title, const wxString &label1, wxString &out1, const wxString &label2, wxString &out2);

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

    std::shared_ptr<FlightService> flightService;

    DECLARE_EVENT_TABLE()
};

#endif // FLIGHT_UI_H