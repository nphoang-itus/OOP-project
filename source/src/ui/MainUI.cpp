#include "MainUI.h"

enum
{
    ID_FLIGHT = 1,
    ID_PASSENGER = 2,
    ID_RESERVATION = 3,
    ID_EXIT = 4
};

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_BUTTON(ID_FLIGHT, MainWindow::OnFlightService)
EVT_BUTTON(ID_PASSENGER, MainWindow::OnPassengerService)
EVT_BUTTON(ID_RESERVATION, MainWindow::OnReservationService)
EVT_BUTTON(ID_EXIT, MainWindow::OnExit)
END_EVENT_TABLE()

MainWindow::MainWindow(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 600))
{
    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create buttons
    flightButton = new wxButton(panel, ID_FLIGHT, "Dịch vụ chuyến bay", wxDefaultPosition, wxSize(200, 50));
    passengerButton = new wxButton(panel, ID_PASSENGER, "Dịch vụ hành khách", wxDefaultPosition, wxSize(200, 50));
    reservationButton = new wxButton(panel, ID_RESERVATION, "Dịch vụ đặt chỗ", wxDefaultPosition, wxSize(200, 50));
    exitButton = new wxButton(panel, ID_EXIT, "Thoát", wxDefaultPosition, wxSize(200, 50));

    // Add buttons to sizer with spacing
    mainSizer->AddStretchSpacer();
    mainSizer->Add(flightButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(passengerButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(reservationButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(exitButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->AddStretchSpacer();

    panel->SetSizer(mainSizer);
    Centre();
}

void MainWindow::OnFlightService(wxCommandEvent &event)
{
    FlightWindow *flightWindow = new FlightWindow("Dịch vụ chuyến bay");
    flightWindow->Show();
    this->Hide();
}

void MainWindow::OnPassengerService(wxCommandEvent &event)
{
    PassengerWindow *passengerWindow = new PassengerWindow("Dịch vụ hành khách");
    passengerWindow->Show();
    this->Hide();
}

void MainWindow::OnReservationService(wxCommandEvent &event)
{
    ReservationWindow *reservationWindow = new ReservationWindow("Dịch vụ đặt chỗ");
    reservationWindow->Show();
    this->Hide();
}

void MainWindow::OnExit(wxCommandEvent &event)
{
    Close(true);
}