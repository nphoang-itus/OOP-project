#include "FlightUI.h"
#include <iomanip>
#include <sstream>

enum
{
    ID_BACK = 1,
    ID_SHOW = 2,
    ID_ADD = 3,
    ID_EDIT = 4,
    ID_DELETE = 5,
    ID_FLIGHT_LIST = 6
};

BEGIN_EVENT_TABLE(FlightWindow, wxFrame)
EVT_BUTTON(ID_BACK, FlightWindow::OnBack)
EVT_BUTTON(ID_SHOW, FlightWindow::OnShowFlights)
EVT_BUTTON(ID_ADD, FlightWindow::OnAddFlight)
EVT_BUTTON(ID_EDIT, FlightWindow::OnEditFlight)
EVT_BUTTON(ID_DELETE, FlightWindow::OnDeleteFlight)
EVT_LIST_ITEM_SELECTED(ID_FLIGHT_LIST, FlightWindow::OnListItemSelected)
END_EVENT_TABLE()

// Hàm chuyển std::tm thành string (đơn giản)
static std::string formatTime(const std::tm &t)
{
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &t);
    return std::string(buf);
}

FlightWindow::FlightWindow(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);

    // Create back button
    backButton = new wxButton(panel, ID_BACK, "Back", wxDefaultPosition, wxSize(100, 30));
    buttonSizer->Add(backButton, 0, wxALL, 10);

    // Create main function buttons
    showButton = new wxButton(panel, ID_SHOW, "Hiển thị thông tin các chuyến bay", wxDefaultPosition, wxSize(250, 50));
    addButton = new wxButton(panel, ID_ADD, "Thêm chuyến bay", wxDefaultPosition, wxSize(250, 50));
    editButton = new wxButton(panel, ID_EDIT, "Sửa chuyến bay", wxDefaultPosition, wxSize(250, 50));
    deleteButton = new wxButton(panel, ID_DELETE, "Xóa chuyến bay", wxDefaultPosition, wxSize(250, 50));

    // Create flight list
    flightList = new wxListCtrl(panel, ID_FLIGHT_LIST, wxDefaultPosition, wxSize(700, 300),
                                wxLC_REPORT | wxLC_SINGLE_SEL);
    flightList->InsertColumn(0, "Mã chuyến bay");
    flightList->InsertColumn(1, "Điểm khởi hành");
    flightList->InsertColumn(2, "Điểm đến");
    flightList->InsertColumn(3, "Giờ khởi hành");
    flightList->InsertColumn(4, "Giờ đến");
    flightList->InsertColumn(5, "Tổng số ghế");
    flightList->InsertColumn(6, "Số ghế còn trống");

    // Create info label
    infoLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(600, 50));
    infoLabel->Wrap(600);

    // Add buttons to content sizer
    contentSizer->Add(showButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(addButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(editButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(deleteButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(flightList, 0, wxALL | wxEXPAND, 10);
    contentSizer->Add(infoLabel, 0, wxALL | wxALIGN_CENTER, 10);

    // Add sizers to main sizer
    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(contentSizer, 1, wxEXPAND);

    panel->SetSizer(mainSizer);
    Centre();
}

void FlightWindow::OnBack(wxCommandEvent &event)
{
    this->Close();
    MainWindow *mainWindow = new MainWindow("Menu chính");
    mainWindow->Show();
}

void FlightWindow::OnShowFlights(wxCommandEvent &event)
{
    RefreshFlightList();
}

void FlightWindow::OnAddFlight(wxCommandEvent &event)
{
    Flight flight;
    flight.setNo("VN123");
    flight.setName("Vietnam Airlines");
    flight.setFrom("SGN");
    flight.setDestination("HAN");
    std::tm now = {};
    flight.setDepartureTime(now);
    flight.setLeaveTime(now);
    flight.setArrivalTime(now);
    flight.setAmount(1500000);
    flight.setAvailability('A');
    bool ok = flightService.addFlight(flight);
    if (ok)
        infoLabel->SetLabel("Đã thêm chuyến bay mới!");
    else
        infoLabel->SetLabel("Thêm chuyến bay thất bại!");
    RefreshFlightList();
}

void FlightWindow::OnEditFlight(wxCommandEvent &event)
{
    long selectedIndex = flightList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn chuyến bay cần sửa!", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }
    std::string flightNo = flightList->GetItemText(selectedIndex, 0).ToStdString();
    auto flightOpt = flightService.getFlightByFlightNo(flightNo);
    if (flightOpt)
    {
        Flight flight = *flightOpt;
        // Sửa thông tin (ví dụ sửa giá vé)
        flight.setAmount(2000000);
        bool ok = flightService.updateFlight(flight);
        if (ok)
            infoLabel->SetLabel("Đã cập nhật chuyến bay!");
        else
            infoLabel->SetLabel("Cập nhật chuyến bay thất bại!");
        RefreshFlightList();
    }
}

void FlightWindow::OnDeleteFlight(wxCommandEvent &event)
{
    long selectedIndex = flightList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn chuyến bay cần xóa!", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }
    std::string flightNo = flightList->GetItemText(selectedIndex, 0).ToStdString();
    auto flightOpt = flightService.getFlightByFlightNo(flightNo);
    if (flightOpt)
    {
        int id = flightOpt->getId();
        bool ok = flightService.deleteFlight(id);
        if (ok)
            infoLabel->SetLabel("Đã xóa chuyến bay!");
        else
            infoLabel->SetLabel("Xóa chuyến bay thất bại!");
        RefreshFlightList();
    }
}

void FlightWindow::OnListItemSelected(wxListEvent &event)
{
    RefreshFlightList();
}

void FlightWindow::RefreshFlightList()
{
    flightList->DeleteAllItems();
    std::vector<Flight> flights = flightService.getAllFlights();
    long index = 0;
    for (const auto &flight : flights)
    {
        flightList->InsertItem(index, wxString::FromUTF8(flight.getNo().c_str()));
        flightList->SetItem(index, 1, wxString::FromUTF8(flight.getFrom().c_str()));
        flightList->SetItem(index, 2, wxString::FromUTF8(flight.getDestination().c_str()));
        flightList->SetItem(index, 3, wxString::FromUTF8(formatTime(flight.getDepartureTime()).c_str()));
        flightList->SetItem(index, 4, wxString::FromUTF8(formatTime(flight.getArrivalTime()).c_str()));
        flightList->SetItem(index, 5, std::to_string(static_cast<int>(flight.getAmount())));
        flightList->SetItem(index, 6, wxString::FromUTF8(std::string(1, flight.getAvailability())));
        index++;
    }
    infoLabel->SetLabel("Danh sách chuyến bay đã được cập nhật");
}