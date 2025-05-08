#ifndef RESERVATION_WINDOW_H
#define RESERVATION_WINDOW_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/Reservation.h"
#include "services/ReservationService.h"

class ReservationWindow : public wxFrame
{
public:
    ReservationWindow(const wxString &title);

private:
    void OnBack(wxCommandEvent &event);
    void OnShowReservations(wxCommandEvent &event);
    void OnAddReservation(wxCommandEvent &event);
    void OnEditReservation(wxCommandEvent &event);
    void OnDeleteReservation(wxCommandEvent &event);
    void OnListItemSelected(wxListEvent &event);
    void RefreshReservationList();

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *backButton;
    wxButton *showButton;
    wxButton *addButton;
    wxButton *editButton;
    wxButton *deleteButton;
    wxListCtrl *reservationList;
    wxStaticText *infoLabel;

    ReservationService reservationService;

    DECLARE_EVENT_TABLE()
};

#endif // RESERVATION_WINDOW_H