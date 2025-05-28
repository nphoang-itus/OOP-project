#include "MainUI.h"
#include "AircraftUI.h"
#include "FlightUI.h"
#include "PassengerUI.h"

enum
{
    ID_AIRCRAFT = 1,
    ID_FLIGHT = 2,
    ID_PASSENGER = 3,
    ID_EXIT = 4
};

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_BUTTON(ID_AIRCRAFT, MainWindow::OnAircraftService)
EVT_BUTTON(ID_FLIGHT, MainWindow::OnFlightService)
EVT_BUTTON(ID_PASSENGER, MainWindow::OnPassengerService)
EVT_BUTTON(ID_EXIT, MainWindow::OnExit)
END_EVENT_TABLE()

MainWindow::MainWindow(const wxString &title,
                       std::shared_ptr<AircraftService> aircraftService,
                       std::shared_ptr<FlightService> flightService,
                       std::shared_ptr<PassengerService> passengerService)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 600)),
      aircraftService(aircraftService),
      flightService(flightService),
      passengerService(passengerService)
{
    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create buttons
    aircraftButton = new wxButton(panel, ID_AIRCRAFT, "Dịch vụ máy bay", wxDefaultPosition, wxSize(200, 50));
    flightButton = new wxButton(panel, ID_FLIGHT, "Dịch vụ chuyến bay", wxDefaultPosition, wxSize(200, 50));
    passengerButton = new wxButton(panel, ID_PASSENGER, "Dịch vụ hành khách", wxDefaultPosition, wxSize(200, 50));
    exitButton = new wxButton(panel, ID_EXIT, "Thoát", wxDefaultPosition, wxSize(200, 50));

    // Add buttons to sizer with spacing
    mainSizer->AddStretchSpacer();
    mainSizer->Add(aircraftButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(flightButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(passengerButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(exitButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->AddStretchSpacer();

    panel->SetSizer(mainSizer);
    Centre();
}

void MainWindow::OnAircraftService(wxCommandEvent &event)
{
    AircraftWindow *aircraftWindow = new AircraftWindow("Dịch vụ máy bay", aircraftService);
    aircraftWindow->setServices(aircraftService, flightService, passengerService);
    aircraftWindow->Show();
    this->Hide();
}

void MainWindow::OnFlightService(wxCommandEvent &event)
{
    FlightWindow *flightWindow = new FlightWindow("Dịch vụ chuyến bay", flightService);
    flightWindow->setServices(aircraftService, flightService, passengerService);
    flightWindow->Show();
    this->Hide();
}

void MainWindow::OnPassengerService(wxCommandEvent &event)
{
    PassengerWindow *passengerWindow = new PassengerWindow("Dịch vụ hành khách", passengerService);
    passengerWindow->setServices(aircraftService, flightService, passengerService);
    passengerWindow->Show();
    this->Hide();
}

void MainWindow::OnExit(wxCommandEvent &event)
{
    Close(true);
}