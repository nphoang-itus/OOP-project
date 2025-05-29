// [DEBUG] Building prepared statement from: DELETE FROM flight WHERE id = ?
// [DEBUG] Built final query: DELETE FROM flight WHERE id = '13'
// [DEBUG] Executing prepared statement: DELETE FROM flight WHERE id = '13'
// [ERROR] MySQL error executing statement: CDK Error: Cannot delete or update a parent row: a foreign key constraint fails (`airlines_db`.`flight_seat_availability`, CONSTRAINT `flight_seat_availability_ibfk_1` FOREIGN KEY (`flight_id`) REFERENCES `flight` (`id`))
// [DEBUG] Freeing prepared statement with ID: 76
// [DEBUG] Statement freed successfully
// [DEBUG] Rolling back database transaction
// [DEBUG] Transaction rolled back successfully
// [ERROR] Failed to execute statement for deleting flight

#include "FlightUI.h"
#include "utils/utils.h"
#include "../core/value_objects/flight_number/FlightNumber.h"
#include "../core/value_objects/route/Route.h"
#include "../core/value_objects/schedule/Schedule.h"
#include "../core/value_objects/aircraft_serial/AircraftSerial.h"
#include "../core/value_objects/flight_status/FlightStatus.h"
#include "../core/entities/Flight.h"
#include "../core/entities/Ticket.h"
#include "services/TicketService.h"
#include <iomanip>
#include <sstream>
#include <ctime>
#include <wx/textdlg.h>
#include <wx/choice.h>

enum
{
    ID_BACK = 1,
    ID_SHOW = 2,
    ID_ADD = 3,
    ID_EDIT = 4,
    ID_DELETE = 5,
    ID_FLIGHT_LIST = 6,
    ID_SEARCH_ID = 7,
    ID_SEARCH_FLIGHT_NUMBER = 8,
    ID_VIEW_AVAILABLE_SEATS = 9,
    ID_CHECK_SEAT_AVAILABILITY = 10
};

wxBEGIN_EVENT_TABLE(FlightWindow, wxFrame)
    EVT_BUTTON(ID_BACK, FlightWindow::OnBack)
        EVT_BUTTON(ID_SHOW, FlightWindow::OnShowFlight)
            EVT_BUTTON(ID_ADD, FlightWindow::OnAddFlight)
                EVT_BUTTON(ID_EDIT, FlightWindow::OnEditFlight)
                    EVT_BUTTON(ID_DELETE, FlightWindow::OnDeleteFlight)
                        EVT_BUTTON(ID_SEARCH_ID, FlightWindow::OnSearchById)
                            EVT_BUTTON(ID_SEARCH_FLIGHT_NUMBER, FlightWindow::OnSearchByFlightNumber)
                                EVT_BUTTON(ID_VIEW_AVAILABLE_SEATS, FlightWindow::OnViewAvailableSeats)
                                    EVT_BUTTON(ID_CHECK_SEAT_AVAILABILITY, FlightWindow::OnCheckSeatAvailability)
                                        EVT_LIST_ITEM_SELECTED(ID_FLIGHT_LIST, FlightWindow::OnListItemSelected)
                                            wxEND_EVENT_TABLE()

                                                FlightWindow::FlightWindow(const wxString &title, std::shared_ptr<FlightService> flightService)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1400, 700)), flightService(flightService)
{
    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // Back button row
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    backButton = new wxButton(panel, ID_BACK, "Back", wxDefaultPosition, wxSize(100, 30));
    buttonSizer->Add(backButton, 0, wxALL, 10);
    mainSizer->Add(buttonSizer, 0, wxALIGN_LEFT);

    // Title
    wxStaticText *titleText = new wxStaticText(panel, wxID_ANY, "QUẢN LÝ CHUYẾN BAY", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(18);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    // Main function buttons (2 rows)
    wxBoxSizer *buttonRow1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonRow2 = new wxBoxSizer(wxHORIZONTAL);

    showButton = new wxButton(panel, ID_SHOW, "Hiển thị thông tin", wxDefaultPosition, wxSize(250, 50));
    addButton = new wxButton(panel, ID_ADD, "Thêm chuyến bay", wxDefaultPosition, wxSize(250, 50));
    editButton = new wxButton(panel, ID_EDIT, "Sửa thông tin", wxDefaultPosition, wxSize(250, 50));
    deleteButton = new wxButton(panel, ID_DELETE, "Xóa chuyến bay", wxDefaultPosition, wxSize(250, 50));
    buttonRow1->Add(showButton, 0, wxALL, 10);
    buttonRow1->Add(addButton, 0, wxALL, 10);
    buttonRow1->Add(editButton, 0, wxALL, 10);
    buttonRow1->Add(deleteButton, 0, wxALL, 10);

    searchByIdButton = new wxButton(panel, ID_SEARCH_ID, "Tìm theo ID", wxDefaultPosition, wxSize(250, 50));
    searchByFlightNumberButton = new wxButton(panel, ID_SEARCH_FLIGHT_NUMBER, "Tìm theo số hiệu", wxDefaultPosition, wxSize(250, 50));
    viewAvailableSeatsButton = new wxButton(panel, wxID_ANY, "Xem ghế trống", wxDefaultPosition, wxSize(250, 50));
    checkSeatAvailabilityButton = new wxButton(panel, wxID_ANY, "Kiểm tra ghế", wxDefaultPosition, wxSize(250, 50));
    buttonRow2->Add(searchByIdButton, 0, wxALL, 10);
    buttonRow2->Add(searchByFlightNumberButton, 0, wxALL, 10);
    buttonRow2->Add(viewAvailableSeatsButton, 0, wxALL, 10);
    buttonRow2->Add(checkSeatAvailabilityButton, 0, wxALL, 10);

    mainSizer->Add(buttonRow1, 0, wxALIGN_CENTER);
    mainSizer->Add(buttonRow2, 0, wxALIGN_CENTER);

    // Flight list
    flightList = new wxListCtrl(panel, ID_FLIGHT_LIST, wxDefaultPosition, wxSize(1300, 400),
                                wxLC_REPORT | wxLC_SINGLE_SEL | wxBORDER_SUNKEN);
    flightList->InsertColumn(0, "ID", wxLIST_FORMAT_LEFT, 60);
    flightList->InsertColumn(1, "Số hiệu", wxLIST_FORMAT_LEFT, 120);
    flightList->InsertColumn(2, "Điểm đi", wxLIST_FORMAT_LEFT, 140);
    flightList->InsertColumn(3, "Điểm đến", wxLIST_FORMAT_LEFT, 140);
    flightList->InsertColumn(4, "Ngày khởi hành", wxLIST_FORMAT_LEFT, 120);
    flightList->InsertColumn(5, "Giờ khởi hành", wxLIST_FORMAT_LEFT, 100);
    flightList->InsertColumn(6, "Ngày đến", wxLIST_FORMAT_LEFT, 120);
    flightList->InsertColumn(7, "Giờ đến", wxLIST_FORMAT_LEFT, 100);
    flightList->InsertColumn(8, "Máy bay", wxLIST_FORMAT_LEFT, 140);
    flightList->InsertColumn(9, "Trạng thái", wxLIST_FORMAT_LEFT, 120);
    flightList->InsertColumn(10, "Thông tin ghế", wxLIST_FORMAT_LEFT, 400);
    mainSizer->Add(flightList, 1, wxEXPAND | wxALL, 20);

    panel->SetSizer(mainSizer);
    Centre();

    // Đăng ký sự kiện
    viewAvailableSeatsButton->Bind(wxEVT_BUTTON, &FlightWindow::OnViewAvailableSeats, this);
    checkSeatAvailabilityButton->Bind(wxEVT_BUTTON, &FlightWindow::OnCheckSeatAvailability, this);
}

void FlightWindow::setServices(std::shared_ptr<AircraftService> aircraft,
                               std::shared_ptr<FlightService> flight,
                               std::shared_ptr<PassengerService> passenger,
                               std::shared_ptr<TicketService> ticket)
{
    this->aircraftService = aircraft;
    this->flightService = flight;
    this->passengerService = passenger;
    this->ticketService = ticket;
}

void FlightWindow::OnBack(wxCommandEvent &event)
{
    this->Hide();
    MainWindow *mainWindow = new MainWindow("Quản lý hãng hàng không",
                                            aircraftService,
                                            flightService,
                                            passengerService,
                                            ticketService);
    mainWindow->Show();
    this->Close();
}

void FlightWindow::OnShowFlight(wxCommandEvent &event)
{
    RefreshFlightList();
}

void FlightWindow::OnAddFlight(wxCommandEvent &event)
{
    wxDialog *dialog = new wxDialog(this, wxID_ANY, "Thêm chuyến bay", wxDefaultPosition, wxSize(500, 600));
    wxPanel *panel = new wxPanel(dialog);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *flightNumberLabel = new wxStaticText(panel, wxID_ANY, "Số hiệu chuyến bay:");
    wxTextCtrl *flightNumberCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *originLabel = new wxStaticText(panel, wxID_ANY, "Điểm đi:");
    wxTextCtrl *originCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *originCodeLabel = new wxStaticText(panel, wxID_ANY, "Mã sân bay đi (VD: SGN):");
    wxTextCtrl *originCodeCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *destinationLabel = new wxStaticText(panel, wxID_ANY, "Điểm đến:");
    wxTextCtrl *destinationCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *destinationCodeLabel = new wxStaticText(panel, wxID_ANY, "Mã sân bay đến (VD: HAN):");
    wxTextCtrl *destinationCodeCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *aircraftLabel = new wxStaticText(panel, wxID_ANY, "Số hiệu máy bay:");
    wxTextCtrl *aircraftCtrl = new wxTextCtrl(panel, wxID_ANY);

    wxStaticText *departureDateLabel = new wxStaticText(panel, wxID_ANY, "Ngày khởi hành (YYYY-MM-DD):");
    wxTextCtrl *departureDateCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *departureTimeLabel = new wxStaticText(panel, wxID_ANY, "Giờ khởi hành (HH:MM):");
    wxTextCtrl *departureTimeCtrl = new wxTextCtrl(panel, wxID_ANY);

    wxStaticText *arrivalDateLabel = new wxStaticText(panel, wxID_ANY, "Ngày đến (YYYY-MM-DD):");
    wxTextCtrl *arrivalDateCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *arrivalTimeLabel = new wxStaticText(panel, wxID_ANY, "Giờ đến (HH:MM):");
    wxTextCtrl *arrivalTimeCtrl = new wxTextCtrl(panel, wxID_ANY);

    sizer->Add(flightNumberLabel, 0, wxALL, 5);
    sizer->Add(flightNumberCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(originLabel, 0, wxALL, 5);
    sizer->Add(originCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(originCodeLabel, 0, wxALL, 5);
    sizer->Add(originCodeCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(destinationLabel, 0, wxALL, 5);
    sizer->Add(destinationCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(destinationCodeLabel, 0, wxALL, 5);
    sizer->Add(destinationCodeCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(aircraftLabel, 0, wxALL, 5);
    sizer->Add(aircraftCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(departureDateLabel, 0, wxALL, 5);
    sizer->Add(departureDateCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(departureTimeLabel, 0, wxALL, 5);
    sizer->Add(departureTimeCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(arrivalDateLabel, 0, wxALL, 5);
    sizer->Add(arrivalDateCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(arrivalTimeLabel, 0, wxALL, 5);
    sizer->Add(arrivalTimeCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->AddStretchSpacer(1);
    wxButton *okButton = new wxButton(panel, wxID_OK, "OK");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Cancel");
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 15);
    panel->SetSizer(sizer);
    wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
    dialogSizer->Add(panel, 1, wxEXPAND | wxALL, 10);
    dialog->SetSizer(dialogSizer);
    dialog->SetMinSize(wxSize(500, 600));
    dialog->Fit();

    if (dialog->ShowModal() == wxID_OK)
    {
        std::string flightNumber = flightNumberCtrl->GetValue().ToStdString();
        std::string origin = originCtrl->GetValue().ToStdString();
        std::string originCode = originCodeCtrl->GetValue().ToStdString();
        std::string destination = destinationCtrl->GetValue().ToStdString();
        std::string destinationCode = destinationCodeCtrl->GetValue().ToStdString();
        std::string aircraft = aircraftCtrl->GetValue().ToStdString();
        std::string departureDate = departureDateCtrl->GetValue().ToStdString();
        std::string departureTime = departureTimeCtrl->GetValue().ToStdString();
        std::string arrivalDate = arrivalDateCtrl->GetValue().ToStdString();
        std::string arrivalTime = arrivalTimeCtrl->GetValue().ToStdString();

        if (flightNumber.empty() || origin.empty() || originCode.empty() ||
            destination.empty() || destinationCode.empty() || aircraft.empty() ||
            departureDate.empty() || departureTime.empty() || arrivalDate.empty() ||
            arrivalTime.empty())
        {
            wxMessageBox("Vui lòng điền đầy đủ thông tin", "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        // Tạo các value object
        auto flightNumberResult = FlightNumber::create(flightNumber);
        if (!flightNumberResult)
        {
            wxMessageBox("Số hiệu chuyến bay không hợp lệ: " + flightNumberResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        // Kiểm tra máy bay tồn tại
        auto aircraftSerialResult = AircraftSerial::create(aircraft);
        if (!aircraftSerialResult)
        {
            wxMessageBox("Số hiệu máy bay không hợp lệ: " + aircraftSerialResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        // Lấy máy bay từ service
        if (!aircraftService)
        {
            wxMessageBox("Không tìm thấy dịch vụ máy bay", "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        // Kiểm tra máy bay có tồn tại không
        auto aircraftResult = aircraftService->getAircraft(*aircraftSerialResult);
        if (!aircraftResult)
        {
            wxMessageBox("Không tìm thấy máy bay với số hiệu: " + aircraft, "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }
        auto aircraftPtr = std::make_shared<Aircraft>(aircraftResult.value());

        // Tạo Route với mã sân bay
        auto routeResult = Route::create(origin, originCode, destination, destinationCode);
        if (!routeResult)
        {
            wxMessageBox("Tuyến đường không hợp lệ: " + routeResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        // Tạo Schedule
        std::string departureDateTime = departureDate + " " + departureTime;
        std::string arrivalDateTime = arrivalDate + " " + arrivalTime;
        auto scheduleResult = Schedule::create(departureDateTime, arrivalDateTime);
        if (!scheduleResult)
        {
            wxMessageBox("Lịch trình không hợp lệ: " + scheduleResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        // Tạo Flight
        auto flightResult = Flight::create(*flightNumberResult, *routeResult, *scheduleResult, aircraftPtr);
        if (!flightResult)
        {
            wxMessageBox("Không thể tạo chuyến bay: " + flightResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        // Lưu chuyến bay vào database
        auto createResult = flightService->createFlight(flightResult.value());
        if (!createResult)
        {
            wxMessageBox("Lỗi khi thêm chuyến bay: " + createResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        // Kiểm tra lại danh sách chuyến bay vừa lấy về
        bool found = false;
        auto allFlightsResult = flightService->getAllFlights();
        if (allFlightsResult)
        {
            for (const auto &f : allFlightsResult.value())
            {
                if (f.getFlightNumber().toString() == flightNumber)
                {
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            wxMessageBox("Đã thêm chuyến bay nhưng không thấy trong danh sách! Có thể database hoặc service đang lỗi. Vui lòng kiểm tra lại.", "Cảnh báo", wxOK | wxICON_WARNING);
        }
        else
        {
            wxMessageBox("Thêm chuyến bay thành công!", "Thông báo", wxOK | wxICON_INFORMATION);
        }
        RefreshFlightList();
    }
    dialog->Destroy();
}

void FlightWindow::OnEditFlight(wxCommandEvent &event)
{
    long itemIndex = -1;
    itemIndex = flightList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1)
    {
        wxMessageBox("Vui lòng chọn chuyến bay để sửa", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    // Get current flight ID first
    wxListItem item;
    item.SetId(itemIndex);
    item.SetColumn(0);
    item.SetMask(wxLIST_MASK_TEXT);
    flightList->GetItem(item);
    std::string flightId = item.GetText().ToStdString();

    // Get current flight data from list - reset item id each time
    item.SetId(itemIndex);
    item.SetColumn(1);
    flightList->GetItem(item);
    std::string currentFlightNumber = item.GetText().ToStdString();

    item.SetId(itemIndex);
    item.SetColumn(2);
    flightList->GetItem(item);
    std::string currentOrigin = item.GetText().ToStdString();

    item.SetId(itemIndex);
    item.SetColumn(3);
    flightList->GetItem(item);
    std::string currentDestination = item.GetText().ToStdString();

    item.SetId(itemIndex);
    item.SetColumn(4);
    flightList->GetItem(item);
    std::string currentDepartureDate = item.GetText().ToStdString();

    item.SetId(itemIndex);
    item.SetColumn(5);
    flightList->GetItem(item);
    std::string currentDepartureTime = item.GetText().ToStdString();

    item.SetId(itemIndex);
    item.SetColumn(6);
    flightList->GetItem(item);
    std::string currentArrivalDate = item.GetText().ToStdString();

    item.SetId(itemIndex);
    item.SetColumn(7);
    flightList->GetItem(item);
    std::string currentArrivalTime = item.GetText().ToStdString();

    item.SetId(itemIndex);
    item.SetColumn(8);
    flightList->GetItem(item);
    std::string currentAircraft = item.GetText().ToStdString();

    wxDialog *dialog = new wxDialog(this, wxID_ANY, "Sửa chuyến bay", wxDefaultPosition, wxSize(500, 700));
    wxPanel *panel = new wxPanel(dialog);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *flightNumberLabel = new wxStaticText(panel, wxID_ANY, "Số hiệu chuyến bay:");
    wxTextCtrl *flightNumberCtrl = new wxTextCtrl(panel, wxID_ANY, currentFlightNumber);
    wxStaticText *originLabel = new wxStaticText(panel, wxID_ANY, "Điểm đi:");
    wxTextCtrl *originCtrl = new wxTextCtrl(panel, wxID_ANY, currentOrigin);
    wxStaticText *destinationLabel = new wxStaticText(panel, wxID_ANY, "Điểm đến:");
    wxTextCtrl *destinationCtrl = new wxTextCtrl(panel, wxID_ANY, currentDestination);

    wxStaticText *aircraftLabel = new wxStaticText(panel, wxID_ANY, "Máy bay:");
    wxTextCtrl *aircraftCtrl = new wxTextCtrl(panel, wxID_ANY, currentAircraft);

    // Departure date and time separate inputs
    wxStaticText *departureDateLabel = new wxStaticText(panel, wxID_ANY, "Ngày khởi hành (YYYY-MM-DD):");
    wxTextCtrl *departureDateCtrl = new wxTextCtrl(panel, wxID_ANY, currentDepartureDate);
    wxStaticText *departureTimeLabel = new wxStaticText(panel, wxID_ANY, "Giờ khởi hành (HH:MM):");
    wxTextCtrl *departureTimeCtrl = new wxTextCtrl(panel, wxID_ANY, currentDepartureTime);

    // Arrival date and time separate inputs
    wxStaticText *arrivalDateLabel = new wxStaticText(panel, wxID_ANY, "Ngày đến (YYYY-MM-DD):");
    wxTextCtrl *arrivalDateCtrl = new wxTextCtrl(panel, wxID_ANY, currentArrivalDate);
    wxStaticText *arrivalTimeLabel = new wxStaticText(panel, wxID_ANY, "Giờ đến (HH:MM):");
    wxTextCtrl *arrivalTimeCtrl = new wxTextCtrl(panel, wxID_ANY, currentArrivalTime);

    wxStaticText *statusLabel = new wxStaticText(panel, wxID_ANY, "Trạng thái:");
    wxChoice *statusChoice = new wxChoice(panel, wxID_ANY);
    statusChoice->Append("SCHEDULED");
    statusChoice->Append("DELAYED");
    statusChoice->Append("BOARDING");
    statusChoice->Append("DEPARTED");
    statusChoice->Append("IN_FLIGHT");
    statusChoice->Append("LANDED");
    statusChoice->Append("CANCELLED");
    statusChoice->SetSelection(0);

    sizer->Add(flightNumberLabel, 0, wxALL, 5);
    sizer->Add(flightNumberCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(originLabel, 0, wxALL, 5);
    sizer->Add(originCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(destinationLabel, 0, wxALL, 5);
    sizer->Add(destinationCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(aircraftLabel, 0, wxALL, 5);
    sizer->Add(aircraftCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(departureDateLabel, 0, wxALL, 5);
    sizer->Add(departureDateCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(departureTimeLabel, 0, wxALL, 5);
    sizer->Add(departureTimeCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(arrivalDateLabel, 0, wxALL, 5);
    sizer->Add(arrivalDateCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(arrivalTimeLabel, 0, wxALL, 5);
    sizer->Add(arrivalTimeCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(statusLabel, 0, wxALL, 5);
    sizer->Add(statusChoice, 0, wxEXPAND | wxALL, 5);

    // Add spacer to push buttons to bottom
    sizer->AddStretchSpacer(1);

    wxButton *okButton = new wxButton(panel, wxID_OK, "OK");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Cancel");
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 15);

    panel->SetSizer(sizer);

    // Create dialog sizer and set minimum size
    wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
    dialogSizer->Add(panel, 1, wxEXPAND | wxALL, 10);
    dialog->SetSizer(dialogSizer);
    dialog->SetMinSize(wxSize(500, 700));
    dialog->Fit();

    if (dialog->ShowModal() == wxID_OK)
    {
        std::string flightNumber = flightNumberCtrl->GetValue().ToStdString();
        std::string origin = originCtrl->GetValue().ToStdString();
        std::string destination = destinationCtrl->GetValue().ToStdString();
        std::string aircraft = aircraftCtrl->GetValue().ToStdString();
        std::string departureDate = departureDateCtrl->GetValue().ToStdString();
        std::string departureTime = departureTimeCtrl->GetValue().ToStdString();
        std::string arrivalDate = arrivalDateCtrl->GetValue().ToStdString();
        std::string arrivalTime = arrivalTimeCtrl->GetValue().ToStdString();
        std::string status = statusChoice->GetStringSelection().ToStdString();

        // Validate input
        if (flightNumber.empty() || origin.empty() || destination.empty() ||
            aircraft.empty() || departureDate.empty() || departureTime.empty() ||
            arrivalDate.empty() || arrivalTime.empty())
        {
            wxMessageBox("Vui lòng điền đầy đủ thông tin", "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        try
        {
            // Create FlightNumber value object to check if flight exists
            auto checkFlightNumberResult = FlightNumber::create(flightNumber);
            if (!checkFlightNumberResult)
            {
                wxMessageBox("Số hiệu chuyến bay không hợp lệ: " + checkFlightNumberResult.error().message,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }

            // Check if the flight exists (using the flight number from the form)
            auto existingFlightResult = flightService->getFlight(*checkFlightNumberResult);
            if (!existingFlightResult)
            {
                wxMessageBox("Không thể tìm thấy chuyến bay để cập nhật: " + existingFlightResult.error().message,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }

            // Get the flight ID from the existing flight
            int existingFlightId = existingFlightResult.value().getId();

            // Create FlightNumber value object
            auto flightNumberResult = FlightNumber::create(flightNumber);
            if (!flightNumberResult)
            {
                wxMessageBox("Số hiệu chuyến bay không hợp lệ: " + flightNumberResult.error().message,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }

            // Get the existing route with airport codes from the current flight
            const auto &existingRoute = existingFlightResult.value().getRoute();

            // Create Route value object using the existing airport codes but potentially updated city names
            auto routeResult = Route::create(origin, existingRoute.getOriginCode(), destination, existingRoute.getDestinationCode());
            if (!routeResult)
            {
                wxMessageBox("Tuyến đường không hợp lệ: " + routeResult.error().message,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }

            // Find aircraft by serial number
            auto aircraftSerialResult = AircraftSerial::create(aircraft);
            if (!aircraftSerialResult)
            {
                wxMessageBox("Số hiệu máy bay không hợp lệ: " + aircraftSerialResult.error().message,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }

            auto aircraftResult = aircraftService->getAircraft(*aircraftSerialResult);
            if (!aircraftResult)
            {
                wxMessageBox("Không tìm thấy máy bay với số hiệu: " + aircraft,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }
            auto aircraftPtr = std::make_shared<Aircraft>(aircraftResult.value());

            // Create Schedule value object from separate date and time strings
            std::string departureDateTime = departureDate + " " + departureTime;
            std::string arrivalDateTime = arrivalDate + " " + arrivalTime;

            auto scheduleResult = Schedule::create(departureDateTime, arrivalDateTime);
            if (!scheduleResult)
            {
                wxMessageBox("Lịch trình không hợp lệ: " + scheduleResult.error().message,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }

            // Convert status from Vietnamese to enum
            FlightStatus flightStatus;
            try
            {
                flightStatus = FlightStatusUtil::fromString(status);
            }
            catch (const std::exception &e)
            {
                wxMessageBox("Trạng thái chuyến bay không hợp lệ: " + status,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }

            // Create updated flight object
            auto updatedFlightResult = Flight::create(*flightNumberResult, *routeResult,
                                                      *scheduleResult, aircraftPtr);
            if (!updatedFlightResult)
            {
                wxMessageBox("Không thể tạo chuyến bay: " + updatedFlightResult.error().message,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }

            // Set the ID and status for the updated flight
            auto updatedFlight = updatedFlightResult.value();
            updatedFlight.setId(existingFlightId);
            updatedFlight.setStatus(flightStatus);

            // Update the flight in the database
            auto updateResult = flightService->updateFlight(updatedFlight);
            if (!updateResult)
            {
                wxMessageBox("Lỗi khi cập nhật chuyến bay: " + updateResult.error().message,
                             "Lỗi", wxOK | wxICON_ERROR);
                dialog->Destroy();
                return;
            }

            wxMessageBox("Cập nhật chuyến bay thành công!", "Thông báo", wxOK | wxICON_INFORMATION);
            RefreshFlightList();
        }
        catch (const std::exception &e)
        {
            wxMessageBox("Lỗi: " + std::string(e.what()), "Lỗi", wxOK | wxICON_ERROR);
        }
    }

    dialog->Destroy();
}

void FlightWindow::OnDeleteFlight(wxCommandEvent &event)
{
    long itemIndex = -1;
    itemIndex = flightList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1)
    {
        wxMessageBox("Vui lòng chọn chuyến bay để xóa", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    wxListItem item;
    item.SetId(itemIndex);
    item.SetColumn(1);
    item.SetMask(wxLIST_MASK_TEXT);
    flightList->GetItem(item);
    std::string flightNumber = item.GetText().ToStdString();

    wxString msg = wxString::Format("Bạn có chắc chắn muốn xóa chuyến bay %s?", flightNumber);
    if (wxMessageBox(msg, "Xác nhận", wxYES_NO | wxICON_QUESTION) == wxYES)
    {
        auto flightNumberResult = FlightNumber::create(flightNumber);
        if (!flightNumberResult)
        {
            wxMessageBox("Số hiệu chuyến bay không hợp lệ!", "Lỗi", wxOK | wxICON_ERROR);
            return;
        }
        // Lấy flight để log ID
        auto flightResult = flightService->getFlight(flightNumberResult.value());
        int flightId = -1;
        if (flightResult && flightResult.value().getId() > 0)
        {
            flightId = flightResult.value().getId();
        }
        wxLogMessage("[DEBUG] Đang xóa chuyến bay: %s, ID: %d", flightNumber, flightId);
        auto result = flightService->deleteFlight(flightNumberResult.value());
        if (!result)
        {
            wxString errMsg = result.error().message;
            if (errMsg.Contains("Failed to execute statement"))
            {
                wxMessageBox("Không thể xóa chuyến bay. Có thể dữ liệu chuyến bay bị lỗi hoặc ID không hợp lệ. Hãy kiểm tra lại dữ liệu!", "Lỗi", wxOK | wxICON_ERROR);
            }
            else
            {
                wxMessageBox("Lỗi khi xóa chuyến bay: " + errMsg, "Lỗi", wxOK | wxICON_ERROR);
            }
            return;
        }
        wxMessageBox("Xóa chuyến bay thành công!", "Thông báo", wxOK | wxICON_INFORMATION);
        RefreshFlightList();
    }
}

void FlightWindow::OnSearchById(wxCommandEvent &event)
{
    wxTextEntryDialog dialog(this, "Nhập ID chuyến bay:", "Tìm kiếm theo ID", "");
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString idStr = dialog.GetValue();
        long id;
        if (!idStr.ToLong(&id))
        {
            wxMessageBox("ID không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
            return;
        }

        // Clear current list
        flightList->DeleteAllItems();

        // Search through all flights to find the one with matching ID
        auto result = flightService->getAllFlights();
        if (result.has_value())
        {
            const auto &flights = result.value();
            bool found = false;
            for (const auto &flight : flights)
            {
                if (flight.getId() == id)
                {
                    long index = flightList->InsertItem(0, wxString::Format("%d", flight.getId()));
                    flightList->SetItem(index, 1, flight.getFlightNumber().toString());
                    flightList->SetItem(index, 2, flight.getRoute().getOrigin());
                    flightList->SetItem(index, 3, flight.getRoute().getDestination());

                    // Parse schedule information properly
                    const auto &schedule = flight.getSchedule();
                    const std::tm &departure = schedule.getDeparture();
                    const std::tm &arrival = schedule.getArrival();

                    // Set departure date and time using utils function
                    std::string departureDateTime = convertTimeToString(departure);
                    flightList->SetItem(index, 4, departureDateTime.substr(0, 10)); // Date: YYYY-MM-DD
                    flightList->SetItem(index, 5, departureDateTime.substr(11, 5)); // Time: HH:MM

                    // Set arrival date and time using utils function
                    std::string arrivalDateTime = convertTimeToString(arrival);
                    flightList->SetItem(index, 6, arrivalDateTime.substr(0, 10)); // Date: YYYY-MM-DD
                    flightList->SetItem(index, 7, arrivalDateTime.substr(11, 5)); // Time: HH:MM

                    // Aircraft info
                    if (flight.getAircraft())
                    {
                        flightList->SetItem(index, 8, flight.getAircraft()->getSerial().toString());
                    }
                    else
                    {
                        flightList->SetItem(index, 8, "N/A");
                    }

                    // Status
                    flightList->SetItem(index, 9, "SCHEDULED");
                    found = true;
                    break;
                }
            }

            if (found)
            {
                // infoLabel->SetLabel(wxString::Format("Tìm thấy chuyến bay có ID: %ld", id));
            }
            else
            {
                // infoLabel->SetLabel(wxString::Format("Không tìm thấy chuyến bay có ID: %ld", id));
            }
        }
        else
        {
            wxMessageBox("Không thể tải danh sách chuyến bay", "Lỗi", wxOK | wxICON_ERROR);
        }
    }
}

void FlightWindow::OnSearchByFlightNumber(wxCommandEvent &event)
{
    wxTextEntryDialog dialog(this, "Nhập số hiệu chuyến bay:", "Tìm kiếm theo số hiệu", "");
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString flightNumber = dialog.GetValue();
        if (flightNumber.IsEmpty())
        {
            wxMessageBox("Số hiệu chuyến bay không được để trống", "Lỗi", wxOK | wxICON_ERROR);
            return;
        }

        // Clear current list
        flightList->DeleteAllItems();

        // Create FlightNumber object and search
        auto flightNumberResult = FlightNumber::create(flightNumber.ToStdString());
        if (flightNumberResult.has_value())
        {
            auto result = flightService->getFlight(flightNumberResult.value());
            if (result.has_value())
            {
                const auto &flight = result.value();
                long index = flightList->InsertItem(0, wxString::Format("%d", flight.getId()));
                flightList->SetItem(index, 1, flight.getFlightNumber().toString());
                flightList->SetItem(index, 2, flight.getRoute().getOrigin());
                flightList->SetItem(index, 3, flight.getRoute().getDestination());

                // Parse schedule information properly
                const auto &schedule = flight.getSchedule();
                const std::tm &departure = schedule.getDeparture();
                const std::tm &arrival = schedule.getArrival();

                // Set departure date and time using utils function
                std::string departureDateTime = convertTimeToString(departure);
                flightList->SetItem(index, 4, departureDateTime.substr(0, 10)); // Date: YYYY-MM-DD
                flightList->SetItem(index, 5, departureDateTime.substr(11, 5)); // Time: HH:MM

                // Set arrival date and time using utils function
                std::string arrivalDateTime = convertTimeToString(arrival);
                flightList->SetItem(index, 6, arrivalDateTime.substr(0, 10)); // Date: YYYY-MM-DD
                flightList->SetItem(index, 7, arrivalDateTime.substr(11, 5)); // Time: HH:MM

                // Aircraft info
                if (flight.getAircraft())
                {
                    flightList->SetItem(index, 8, flight.getAircraft()->getSerial().toString());
                }
                else
                {
                    flightList->SetItem(index, 8, "N/A");
                }

                // Status
                flightList->SetItem(index, 9, "SCHEDULED");

                // infoLabel->SetLabel(wxString::Format("Tìm thấy chuyến bay: %s", flightNumber));
            }
            else
            {
                // infoLabel->SetLabel(wxString::Format("Không tìm thấy chuyến bay: %s", flightNumber));
            }
        }
        else
        {
            wxMessageBox("Số hiệu chuyến bay không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
        }
    }
}

void FlightWindow::OnListItemSelected(wxListEvent &event)
{
    long item = event.GetIndex();
    if (item != -1)
    {
        // infoLabel->SetLabel(wxString::Format("Đã chọn chuyến bay ở dòng %ld", item + 1));
    }
}

std::string FlightWindow::getSeatInfo(const Flight &flight)
{
    if (!flight.getAircraft())
    {
        return "N/A";
    }

    const auto &seatLayout = flight.getAircraft()->getSeatLayout();
    std::stringstream ss;

    // Get total seats and available seats
    int totalSeats = seatLayout.getTotalSeatCount();
    int availableSeats = seatLayout.getTotalAvailableSeatCount();

    // Get seats by class
    int economySeats = 0;
    int businessSeats = 0;
    int firstSeats = 0;

    for (const auto &[seatClass, count] : seatLayout.getSeatCounts())
    {
        if (seatClass.getCode() == "E")
            economySeats = seatLayout.getAvailableSeatCount("E");
        else if (seatClass.getCode() == "B")
            businessSeats = seatLayout.getAvailableSeatCount("B");
        else if (seatClass.getCode() == "F")
            firstSeats = seatLayout.getAvailableSeatCount("F");
    }

    // Format seat information
    ss << "Tổng: " << totalSeats << " ghế, Còn trống: " << availableSeats << " ghế (";
    if (economySeats > 0)
        ss << "E:" << economySeats;
    if (businessSeats > 0)
        ss << ", B:" << businessSeats;
    if (firstSeats > 0)
        ss << ", F:" << firstSeats;
    ss << ")";

    return ss.str();
}

// Hàm đồng bộ số ghế đã đặt từ vé vào chuyến bay
void syncBookedSeatsWithTickets(const std::vector<Ticket> &tickets, std::vector<Flight> &flights)
{
    std::map<std::string, Flight *> flightMap;
    for (auto &flight : flights)
    {
        flightMap[flight.getFlightNumber().toString()] = &flight;
        if (flight.getAircraft())
        {
            auto aircraft = flight.getAircraft();
            auto seatLayout = aircraft->getSeatLayout();
            seatLayout.initializeBookedSeats();
            aircraft->setSeatLayout(seatLayout);
        }
    }
    for (const auto &ticket : tickets)
    {
        auto flightNum = ticket.getFlight()->getFlightNumber().toString();
        auto seatNum = ticket.getSeatNumber().toString();
        std::string seatClass = seatNum.substr(0, 1); // E/B/F
        if (flightMap.count(flightNum) && flightMap[flightNum]->getAircraft())
        {
            auto aircraft = flightMap[flightNum]->getAircraft();
            auto seatLayout = aircraft->getSeatLayout();
            seatLayout.bookSeat(seatClass);
            aircraft->setSeatLayout(seatLayout);
        }
    }
}

void FlightWindow::RefreshFlightList()
{
    flightList->DeleteAllItems();

    // Lấy danh sách chuyến bay và vé
    auto result = flightService->getAllFlights();
    std::vector<Flight> flights;
    if (result.has_value())
        flights = result.value();
    else
    {
        // infoLabel->SetLabel("Không thể tải danh sách chuyến bay");
        return;
    }
    std::vector<Ticket> tickets;
    if (ticketService)
    {
        auto ticketResult = ticketService->getAllTickets();
        if (ticketResult.has_value())
            tickets = ticketResult.value();
    }
    // Đồng bộ số ghế đã đặt
    syncBookedSeatsWithTickets(tickets, flights);

    for (size_t i = 0; i < flights.size(); ++i)
    {
        const auto &flight = flights[i];
        try
        {
            // Kiểm tra các trường quan trọng
            if (!flight.getAircraft())
            {
                wxLogError("Flight %s thiếu thông tin máy bay!", flight.getFlightNumber().toString());
                wxMessageBox(wxString::Format("Chuyến bay %s thiếu thông tin máy bay!", flight.getFlightNumber().toString()), "Lỗi dữ liệu", wxOK | wxICON_ERROR);
                continue;
            }
            const auto &route = flight.getRoute();
            if (route.getOrigin().empty() || route.getDestination().empty() || route.getOriginCode().empty() || route.getDestinationCode().empty())
            {
                wxLogError("Flight %s thiếu thông tin route!", flight.getFlightNumber().toString());
                wxMessageBox(wxString::Format("Chuyến bay %s thiếu thông tin tuyến đường!", flight.getFlightNumber().toString()), "Lỗi dữ liệu", wxOK | wxICON_ERROR);
                continue;
            }
            const auto &schedule = flight.getSchedule();
            // Nếu schedule lỗi sẽ throw
            const std::tm &departure = schedule.getDeparture();
            const std::tm &arrival = schedule.getArrival();

            long index = flightList->InsertItem(i, wxString::Format("%d", flight.getId()));
            flightList->SetItem(index, 1, flight.getFlightNumber().toString());
            flightList->SetItem(index, 2, flight.getRoute().getOrigin());
            flightList->SetItem(index, 3, flight.getRoute().getDestination());

            std::string departureDateTime = convertTimeToString(departure);
            flightList->SetItem(index, 4, departureDateTime.substr(0, 10));
            flightList->SetItem(index, 5, departureDateTime.substr(11, 5));

            std::string arrivalDateTime = convertTimeToString(arrival);
            flightList->SetItem(index, 6, arrivalDateTime.substr(0, 10));
            flightList->SetItem(index, 7, arrivalDateTime.substr(11, 5));

            if (flight.getAircraft())
            {
                flightList->SetItem(index, 8, flight.getAircraft()->getSerial().toString());
            }
            else
            {
                flightList->SetItem(index, 8, "N/A");
            }
            flightList->SetItem(index, 9, "SCHEDULED");
            flightList->SetItem(index, 10, getSeatInfo(flight));
        }
        catch (const std::exception &e)
        {
            wxLogError("Lỗi khi load chuyến bay %s: %s", flight.getFlightNumber().toString(), e.what());
            wxMessageBox(wxString::Format("Lỗi khi load chuyến bay %s: %s", flight.getFlightNumber().toString(), e.what()), "Lỗi dữ liệu", wxOK | wxICON_ERROR);
            continue;
        }
    }
    // wxString statusMsg = wxString::Format("Đã tải %zu chuyến bay", flights.size());
    // infoLabel->SetLabel(statusMsg);
}

void FlightWindow::OnViewAvailableSeats(wxCommandEvent &event)
{
    long itemIndex = flightList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1)
    {
        wxMessageBox("Vui lòng chọn một chuyến bay để xem ghế trống", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    wxString serial = flightList->GetItemText(itemIndex, 8); // Cột máy bay
    auto serialResult = AircraftSerial::create(serial.ToStdString());
    if (!serialResult)
    {
        wxMessageBox("Số đăng ký máy bay không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    wxArrayString choices;
    choices.Add("ECONOMY");
    choices.Add("BUSINESS");
    choices.Add("FIRST");
    wxSingleChoiceDialog dialog(this, "Chọn hạng ghế:", "Chọn hạng ghế", choices);
    if (dialog.ShowModal() != wxID_OK)
        return;
    std::string seatClass = dialog.GetStringSelection().ToStdString();
    auto availableSeatsResult = aircraftService->getAvailableSeats(*serialResult, seatClass);
    if (!availableSeatsResult)
    {
        wxMessageBox("Không thể lấy thông tin ghế trống: " + availableSeatsResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    const auto &availableSeats = *availableSeatsResult;
    wxString seatClassName;
    if (seatClass == "ECONOMY")
        seatClassName = "Hạng thường";
    else if (seatClass == "BUSINESS")
        seatClassName = "Hạng thương gia";
    else if (seatClass == "FIRST")
        seatClassName = "Hạng nhất";
    wxString message = wxString::Format("Ghế trống cho máy bay %s - %s:\n\n", serial, seatClassName);
    message += wxString::Format("Tổng số ghế trống: %zu\n\n", availableSeats.size());
    if (availableSeats.empty())
    {
        message += "Không có ghế trống.";
    }
    else
    {
        message += "Danh sách ghế trống:\n";
        int count = 0;
        for (const auto &seat : availableSeats)
        {
            message += seat + " ";
            count++;
            if (count % 10 == 0)
                message += "\n";
        }
    }
    wxMessageBox(message, "Thông tin ghế trống", wxOK | wxICON_INFORMATION);
}

void FlightWindow::OnCheckSeatAvailability(wxCommandEvent &event)
{
    long itemIndex = flightList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1)
    {
        wxMessageBox("Vui lòng chọn một chuyến bay để kiểm tra ghế", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    wxString serial = flightList->GetItemText(itemIndex, 8); // Cột máy bay
    auto serialResult = AircraftSerial::create(serial.ToStdString());
    if (!serialResult)
    {
        wxMessageBox("Số đăng ký máy bay không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    wxString seatNumber = wxGetTextFromUser(
        "Nhập số ghế cần kiểm tra (VD: E001, B001, F001):",
        "Kiểm tra ghế");
    if (seatNumber.IsEmpty())
        return;
    auto availabilityResult = aircraftService->isSeatAvailable(*serialResult, seatNumber.ToStdString());
    if (!availabilityResult)
    {
        wxMessageBox("Không thể kiểm tra ghế: " + availabilityResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    bool isAvailable = *availabilityResult;
    wxString message = wxString::Format(
        "Ghế %s trên máy bay %s:\n\n%s",
        seatNumber,
        serial,
        isAvailable ? "✅ TRỐNG - Có thể đặt" : "❌ ĐÃ ĐƯỢC ĐẶT - Không thể đặt");
    wxMessageBox(message, "Kết quả kiểm tra ghế", wxOK | wxICON_INFORMATION);
}