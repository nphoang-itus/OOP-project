#ifndef RESERVATION_UI_H
#define RESERVATION_UI_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/Reservation.h"
#include "services/ReservationService.h"

class ReservationWindow : public wxFrame
{
public:
    ReservationWindow(const wxString &title, std::shared_ptr<ReservationService> reservationService);

private:
    void OnBack(wxCommandEvent &event);
    void OnShowReservations(wxCommandEvent &event);
    void OnAddReservation(wxCommandEvent &event);
    void OnEditReservation(wxCommandEvent &event);
    void OnDeleteReservation(wxCommandEvent &event);
    void OnSearchById(wxCommandEvent &event);
    void OnSearchByTicketNo(wxCommandEvent &event);
    void OnSearchByPassport(wxCommandEvent &event);
    void OnSearchByFlightNo(wxCommandEvent &event);
    void OnListItemSelected(wxListEvent &event);
    void RefreshReservationList();

    wxPanel *panel;
    wxBoxSizer *mainSizer;
    wxButton *backButton;
    wxButton *showButton;
    wxButton *addButton;
    wxButton *editButton;
    wxButton *deleteButton;
    wxButton *searchByIdButton;
    wxButton *searchByTicketNoButton;
    wxButton *searchByPassportButton;
    wxButton *searchByFlightNoButton;
    wxListCtrl *reservationList;
    wxStaticText *infoLabel;

    std::shared_ptr<ReservationService> reservationService;

    DECLARE_EVENT_TABLE()
};

#endif // RESERVATION_UI_H