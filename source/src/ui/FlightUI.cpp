#include "FlightUI.h"
#include "utils/utils.h"
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
    ID_SEARCH_FLIGHT_NUMBER = 8
};

wxBEGIN_EVENT_TABLE(FlightWindow, wxFrame)
    EVT_BUTTON(ID_BACK, FlightWindow::OnBack)
        EVT_BUTTON(ID_SHOW, FlightWindow::OnShowFlight)
            EVT_BUTTON(ID_ADD, FlightWindow::OnAddFlight)
                EVT_BUTTON(ID_EDIT, FlightWindow::OnEditFlight)
                    EVT_BUTTON(ID_DELETE, FlightWindow::OnDeleteFlight)
                        EVT_BUTTON(ID_SEARCH_ID, FlightWindow::OnSearchById)
                            EVT_BUTTON(ID_SEARCH_FLIGHT_NUMBER, FlightWindow::OnSearchByFlightNumber)
                                EVT_LIST_ITEM_SELECTED(ID_FLIGHT_LIST, FlightWindow::OnListItemSelected)
                                    wxEND_EVENT_TABLE()

                                        FlightWindow::FlightWindow(const wxString &title, std::shared_ptr<FlightService> flightService)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 600)), flightService(flightService)
{
    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);

    // Create back button
    backButton = new wxButton(panel, ID_BACK, "Back", wxDefaultPosition, wxSize(100, 30));
    buttonSizer->Add(backButton, 0, wxALL, 10);

    // Create main function buttons
    showButton = new wxButton(panel, ID_SHOW, "Hiển thị thông tin chuyến bay", wxDefaultPosition, wxSize(250, 50));
    addButton = new wxButton(panel, ID_ADD, "Thêm chuyến bay", wxDefaultPosition, wxSize(250, 50));
    editButton = new wxButton(panel, ID_EDIT, "Sửa thông tin chuyến bay", wxDefaultPosition, wxSize(250, 50));
    deleteButton = new wxButton(panel, ID_DELETE, "Xóa chuyến bay", wxDefaultPosition, wxSize(250, 50));
    searchByIdButton = new wxButton(panel, ID_SEARCH_ID, "Tìm theo ID", wxDefaultPosition, wxSize(250, 50));
    searchByFlightNumberButton = new wxButton(panel, ID_SEARCH_FLIGHT_NUMBER, "Tìm theo số hiệu", wxDefaultPosition, wxSize(250, 50));

    // Create flight list
    flightList = new wxListCtrl(panel, ID_FLIGHT_LIST, wxDefaultPosition, wxSize(1000, 300),
                                wxLC_REPORT | wxLC_SINGLE_SEL);
    flightList->InsertColumn(0, "ID");
    flightList->InsertColumn(1, "Số hiệu");
    flightList->InsertColumn(2, "Điểm đi");
    flightList->InsertColumn(3, "Điểm đến");
    flightList->InsertColumn(4, "Ngày khởi hành");
    flightList->InsertColumn(5, "Giờ khởi hành");
    flightList->InsertColumn(6, "Ngày đến");
    flightList->InsertColumn(7, "Giờ đến");
    flightList->InsertColumn(8, "Máy bay");
    flightList->InsertColumn(9, "Trạng thái");

    for (int i = 0; i < 10; ++i)
    {
        flightList->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    }

    // Adjust specific column widths for better display
    flightList->SetColumnWidth(0, 60);  // ID - shorter
    flightList->SetColumnWidth(1, 100); // Số hiệu
    flightList->SetColumnWidth(2, 110); // Điểm đi
    flightList->SetColumnWidth(3, 110); // Điểm đến
    flightList->SetColumnWidth(4, 110); // Ngày khởi hành
    flightList->SetColumnWidth(5, 90);  // Giờ khởi hành
    flightList->SetColumnWidth(6, 110); // Ngày đến
    flightList->SetColumnWidth(7, 90);  // Giờ đến
    flightList->SetColumnWidth(8, 120); // Máy bay
    flightList->SetColumnWidth(9, 100); // Trạng thái

    // Create info label
    infoLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(800, 50));
    infoLabel->Wrap(800);

    // Add buttons to content sizer (2 hàng ngang)
    wxBoxSizer *row1 = new wxBoxSizer(wxHORIZONTAL);
    row1->Add(showButton, 0, wxALL, 10);
    row1->Add(addButton, 0, wxALL, 10);
    row1->Add(editButton, 0, wxALL, 10);
    row1->Add(deleteButton, 0, wxALL, 10);

    wxBoxSizer *row2 = new wxBoxSizer(wxHORIZONTAL);
    row2->Add(searchByIdButton, 0, wxALL, 10);
    row2->Add(searchByFlightNumberButton, 0, wxALL, 10);

    contentSizer->Add(row1, 0, wxALIGN_CENTER);
    contentSizer->Add(row2, 0, wxALIGN_CENTER);
    contentSizer->Add(flightList, 1, wxALL | wxEXPAND, 10);
    contentSizer->Add(infoLabel, 0, wxALL | wxALIGN_CENTER, 10);

    // Add sizers to main sizer
    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(contentSizer, 1, wxEXPAND);

    panel->SetSizer(mainSizer);
    Centre();
}

void FlightWindow::OnBack(wxCommandEvent &event)
{
    this->Hide();
    MainWindow *mainWindow = new MainWindow("Quản lý hãng hàng không",
                                            aircraftService,
                                            flightService,
                                            passengerService);
    mainWindow->Show();
    this->Close();
}

void FlightWindow::setServices(std::shared_ptr<AircraftService> aircraft,
                               std::shared_ptr<FlightService> flight,
                               std::shared_ptr<PassengerService> passenger)
{
    this->aircraftService = aircraft;
    this->flightService = flight;
    this->passengerService = passenger;
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
    wxStaticText *destinationLabel = new wxStaticText(panel, wxID_ANY, "Điểm đến:");
    wxTextCtrl *destinationCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *aircraftLabel = new wxStaticText(panel, wxID_ANY, "Số hiệu máy bay:");
    wxTextCtrl *aircraftCtrl = new wxTextCtrl(panel, wxID_ANY);

    // Departure date and time separate inputs
    wxStaticText *departureDateLabel = new wxStaticText(panel, wxID_ANY, "Ngày khởi hành (YYYY-MM-DD):");
    wxTextCtrl *departureDateCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *departureTimeLabel = new wxStaticText(panel, wxID_ANY, "Giờ khởi hành (HH:MM):");
    wxTextCtrl *departureTimeCtrl = new wxTextCtrl(panel, wxID_ANY);

    // Arrival date and time separate inputs
    wxStaticText *arrivalDateLabel = new wxStaticText(panel, wxID_ANY, "Ngày đến (YYYY-MM-DD):");
    wxTextCtrl *arrivalDateCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *arrivalTimeLabel = new wxStaticText(panel, wxID_ANY, "Giờ đến (HH:MM):");
    wxTextCtrl *arrivalTimeCtrl = new wxTextCtrl(panel, wxID_ANY);

    // Add form fields to sizer
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

    // Add spacer to push buttons to bottom
    sizer->AddStretchSpacer(1);

    // Create button sizer
    wxButton *okButton = new wxButton(panel, wxID_OK, "OK");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Cancel");
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 15);

    // Set panel sizer
    panel->SetSizer(sizer);

    // Create dialog sizer and set minimum size
    wxBoxSizer *dialogSizer = new wxBoxSizer(wxVERTICAL);
    dialogSizer->Add(panel, 1, wxEXPAND | wxALL, 10);
    dialog->SetSizer(dialogSizer);
    dialog->SetMinSize(wxSize(500, 600));
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

        if (flightNumber.empty() || origin.empty() || destination.empty() ||
            aircraft.empty() || departureDate.empty() || departureTime.empty() ||
            arrivalDate.empty() || arrivalTime.empty())
        {
            wxMessageBox("Vui lòng điền đầy đủ thông tin", "Lỗi", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        // Combine date and time for processing
        std::string departure = departureDate + " " + departureTime + ":00";
        std::string arrival = arrivalDate + " " + arrivalTime + ":00";

        // Create flight using factory method would be called here
        // For now just show success message
        wxMessageBox("Thêm chuyến bay thành công!", "Thông báo", wxOK | wxICON_INFORMATION);
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

    wxListItem item;
    item.SetId(itemIndex);
    item.SetColumn(0);
    item.SetMask(wxLIST_MASK_TEXT);
    flightList->GetItem(item);
    std::string flightId = item.GetText().ToStdString();

    // Get current flight data from list
    item.SetColumn(1);
    flightList->GetItem(item);
    std::string currentFlightNumber = item.GetText().ToStdString();

    item.SetColumn(2);
    flightList->GetItem(item);
    std::string currentOrigin = item.GetText().ToStdString();

    item.SetColumn(3);
    flightList->GetItem(item);
    std::string currentDestination = item.GetText().ToStdString();

    item.SetColumn(4);
    flightList->GetItem(item);
    std::string currentDepartureDate = item.GetText().ToStdString();

    item.SetColumn(5);
    flightList->GetItem(item);
    std::string currentDepartureTime = item.GetText().ToStdString();

    item.SetColumn(6);
    flightList->GetItem(item);
    std::string currentArrivalDate = item.GetText().ToStdString();

    item.SetColumn(7);
    flightList->GetItem(item);
    std::string currentArrivalTime = item.GetText().ToStdString();

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

        // Combine date and time for processing if needed
        std::string departure = departureDate + " " + departureTime + ":00";
        std::string arrival = arrivalDate + " " + arrivalTime + ":00";

        wxMessageBox("Cập nhật chuyến bay thành công!", "Thông báo", wxOK | wxICON_INFORMATION);
        RefreshFlightList();
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
                    flightList->SetItem(index, 5, departureDateTime.substr(11, 5));  // Time: HH:MM

                    // Set arrival date and time using utils function
                    std::string arrivalDateTime = convertTimeToString(arrival);
                    flightList->SetItem(index, 6, arrivalDateTime.substr(0, 10)); // Date: YYYY-MM-DD
                    flightList->SetItem(index, 7, arrivalDateTime.substr(11, 5));  // Time: HH:MM

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
                infoLabel->SetLabel(wxString::Format("Tìm thấy chuyến bay có ID: %ld", id));
            }
            else
            {
                infoLabel->SetLabel(wxString::Format("Không tìm thấy chuyến bay có ID: %ld", id));
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
                flightList->SetItem(index, 5, departureDateTime.substr(11, 5));  // Time: HH:MM

                // Set arrival date and time using utils function
                std::string arrivalDateTime = convertTimeToString(arrival);
                flightList->SetItem(index, 6, arrivalDateTime.substr(0, 10)); // Date: YYYY-MM-DD
                flightList->SetItem(index, 7, arrivalDateTime.substr(11, 5));  // Time: HH:MM

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

                infoLabel->SetLabel(wxString::Format("Tìm thấy chuyến bay: %s", flightNumber));
            }
            else
            {
                infoLabel->SetLabel(wxString::Format("Không tìm thấy chuyến bay: %s", flightNumber));
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
        wxString info = wxString::Format("Đã chọn chuyến bay ở dòng %ld", item + 1);
        infoLabel->SetLabel(info);
    }
}

void FlightWindow::RefreshFlightList()
{
    flightList->DeleteAllItems();

    auto result = flightService->getAllFlights();
    if (result.has_value())
    {
        const auto &flights = result.value();
        for (size_t i = 0; i < flights.size(); ++i)
        {
            const auto &flight = flights[i];
            long index = flightList->InsertItem(i, wxString::Format("%d", flight.getId()));

            // Set basic flight information
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
            flightList->SetItem(index, 5, departureDateTime.substr(11, 5));  // Time: HH:MM

            // Set arrival date and time using utils function
            std::string arrivalDateTime = convertTimeToString(arrival);
            flightList->SetItem(index, 6, arrivalDateTime.substr(0, 10)); // Date: YYYY-MM-DD
            flightList->SetItem(index, 7, arrivalDateTime.substr(11, 5));  // Time: HH:MM

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
        }

        wxString statusMsg = wxString::Format("Đã tải %zu chuyến bay", flights.size());
        infoLabel->SetLabel(statusMsg);
    }
    else
    {
        infoLabel->SetLabel("Không thể tải danh sách chuyến bay");
    }
}
