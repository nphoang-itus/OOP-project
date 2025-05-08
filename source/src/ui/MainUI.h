#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <wx/wx.h>
#include "FlightUI.h"
#include "PassengerUI.h"
#include "ReservationUI.h"

class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString &title);

private:
    void OnFlightService(wxCommandEvent &event);
    void OnPassengerService(wxCommandEvent &event);
    void OnReservationService(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *flightButton;
    wxButton *passengerButton;
    wxButton *reservationButton;
    wxButton *exitButton;

    DECLARE_EVENT_TABLE()
};

#endif // MAIN_WINDOW_H