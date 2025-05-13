#ifndef RESERVATION_UI_H
#define RESERVATION_UI_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "MainUI.h"
#include "core/Reservation.h"
#include "services/ReservationService.h"
#include "services/FlightService.h"
#include "services/PassengerService.h"

class ReservationWindow : public wxFrame
{
public:
    ReservationWindow(const wxString &title, std::shared_ptr<ReservationService> reservationService, std::shared_ptr<FlightService> flightService, std::shared_ptr<PassengerService> passengerService, wxWindow *parent = nullptr);

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
    void ShowReservations(const std::vector<Reservation> &reservations);

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
    std::shared_ptr<FlightService> flightService;
    std::shared_ptr<PassengerService> passengerService;

    DECLARE_EVENT_TABLE()
};

#endif // RESERVATION_UI_H