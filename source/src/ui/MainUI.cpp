#include "MainUI.h"
#include "AircraftUI.h"
#include "FlightUI.h"
#include "PassengerUI.h"
#include "TicketUI.h"

enum
{
    ID_AIRCRAFT = 1,
    ID_FLIGHT = 2,
    ID_PASSENGER = 3,
    ID_TICKET = 4,
    ID_EXIT = 5
};

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_BUTTON(ID_AIRCRAFT, MainWindow::OnAircraftService)
EVT_BUTTON(ID_FLIGHT, MainWindow::OnFlightService)
EVT_BUTTON(ID_PASSENGER, MainWindow::OnPassengerService)
EVT_BUTTON(ID_TICKET, MainWindow::OnTicketService)
EVT_BUTTON(ID_EXIT, MainWindow::OnExit)
END_EVENT_TABLE()

MainWindow::MainWindow(const wxString &title,
                       std::shared_ptr<AircraftService> aircraftService,
                       std::shared_ptr<FlightService> flightService,
                       std::shared_ptr<PassengerService> passengerService,
                       std::shared_ptr<TicketService> ticketService)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 600)),
      aircraftService(aircraftService),
      flightService(flightService),
      passengerService(passengerService),
      ticketService(ticketService)
{
    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create buttons
    aircraftButton = new wxButton(panel, ID_AIRCRAFT, "Dịch vụ máy bay", wxDefaultPosition, wxSize(200, 50));
    flightButton = new wxButton(panel, ID_FLIGHT, "Dịch vụ chuyến bay", wxDefaultPosition, wxSize(200, 50));
    passengerButton = new wxButton(panel, ID_PASSENGER, "Dịch vụ hành khách", wxDefaultPosition, wxSize(200, 50));
    ticketButton = new wxButton(panel, ID_TICKET, "Dịch vụ vé máy bay", wxDefaultPosition, wxSize(200, 50));
    exitButton = new wxButton(panel, ID_EXIT, "Thoát", wxDefaultPosition, wxSize(200, 50));

    // Add buttons to sizer with spacing
    mainSizer->AddStretchSpacer();
    mainSizer->Add(aircraftButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(flightButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(passengerButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(ticketButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->Add(exitButton, 0, wxALL | wxALIGN_CENTER, 10);
    mainSizer->AddStretchSpacer();

    panel->SetSizer(mainSizer);
    Centre();

    // Initialize UIFactory
    uiFactory = std::make_unique<UIFactory>(aircraftService, flightService, passengerService, ticketService);
}

// UIFactory implementation
wxFrame *MainWindow::UIFactory::createWindow(UIType type, const wxString &title)
{
    wxFrame *window = nullptr;

    switch (type)
    {
    case UIType::AIRCRAFT:
    {
        AircraftWindow *aircraftWindow = new AircraftWindow(title, aircraftService);
        aircraftWindow->setServices(aircraftService, flightService, passengerService, ticketService);
        window = aircraftWindow;
        break;
    }
    case UIType::FLIGHT:
    {
        FlightWindow *flightWindow = new FlightWindow(title, flightService);
        flightWindow->setServices(aircraftService, flightService, passengerService, ticketService);
        window = flightWindow;
        break;
    }
    case UIType::PASSENGER:
    {
        PassengerWindow *passengerWindow = new PassengerWindow(title, passengerService);
        passengerWindow->setServices(aircraftService, flightService, passengerService, ticketService);
        window = passengerWindow;
        break;
    }
    case UIType::TICKET:
    {
        TicketWindow *ticketWindow = new TicketWindow(title, ticketService);
        ticketWindow->setServices(aircraftService, flightService, passengerService, ticketService);
        window = ticketWindow;
        break;
    }
    }

    return window;
}

void MainWindow::OnAircraftService(wxCommandEvent &event)
{
    wxFrame *window = uiFactory->createWindow(UIType::AIRCRAFT, "Dịch vụ máy bay");
    if (window)
    {
        window->Show();
        this->Hide();
    }
}

void MainWindow::OnFlightService(wxCommandEvent &event)
{
    wxFrame *window = uiFactory->createWindow(UIType::FLIGHT, "Dịch vụ chuyến bay");
    if (window)
    {
        window->Show();
        this->Hide();
    }
}

void MainWindow::OnPassengerService(wxCommandEvent &event)
{
    wxFrame *window = uiFactory->createWindow(UIType::PASSENGER, "Dịch vụ hành khách");
    if (window)
    {
        window->Show();
        this->Hide();
    }
}

void MainWindow::OnTicketService(wxCommandEvent &event)
{
    wxFrame *window = uiFactory->createWindow(UIType::TICKET, "Dịch vụ vé máy bay");
    if (window)
    {
        window->Show();
        this->Hide();
    }
}

void MainWindow::OnExit(wxCommandEvent &event)
{
    Close(true);
}