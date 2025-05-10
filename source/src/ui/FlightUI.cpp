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
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 600))
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

    flightList->InsertColumn(0, "ID");
    flightList->InsertColumn(1, "Mã chuyến bay");
    flightList->InsertColumn(2, "Tên hãng bay");
    flightList->InsertColumn(3, "Điểm khởi hành");
    flightList->InsertColumn(4, "Điểm đến");
    flightList->InsertColumn(5, "Thời gian khởi hành");
    flightList->InsertColumn(6, "Thời gian cất cánh");
    flightList->InsertColumn(7, "Thời gian hạ cánh");
    flightList->InsertColumn(8, "Giá vé");
    flightList->InsertColumn(9, "Trạng thái");
    for (int i = 0; i < 10; ++i)
    {
        flightList->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    }

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
    wxDialog dialog(this, wxID_ANY, "Thêm chuyến bay mới", wxDefaultPosition, wxSize(400, 500));

    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    int labelWidth = 140;

    // Tạo các trường nhập liệu
    wxBoxSizer *noSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *noLabel = new wxStaticText(panel, wxID_ANY, "Mã chuyến bay:");
    noLabel->SetMinSize(wxSize(labelWidth, -1));
    noSizer->Add(noLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *noCtrl = new wxTextCtrl(panel, wxID_ANY);
    noSizer->Add(noCtrl, 1, wxALL, 5);
    sizer->Add(noSizer, 0, wxEXPAND);

    wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *nameLabel = new wxStaticText(panel, wxID_ANY, "Tên hãng bay:");
    nameLabel->SetMinSize(wxSize(labelWidth, -1));
    nameSizer->Add(nameLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *nameCtrl = new wxTextCtrl(panel, wxID_ANY);
    nameSizer->Add(nameCtrl, 1, wxALL, 5);
    sizer->Add(nameSizer, 0, wxEXPAND);

    wxBoxSizer *fromSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *fromLabel = new wxStaticText(panel, wxID_ANY, "Điểm khởi hành:");
    fromLabel->SetMinSize(wxSize(labelWidth, -1));
    fromSizer->Add(fromLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *fromCtrl = new wxTextCtrl(panel, wxID_ANY);
    fromSizer->Add(fromCtrl, 1, wxALL, 5);
    sizer->Add(fromSizer, 0, wxEXPAND);

    wxBoxSizer *destSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *destLabel = new wxStaticText(panel, wxID_ANY, "Điểm đến:");
    destLabel->SetMinSize(wxSize(labelWidth, -1));
    destSizer->Add(destLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *destCtrl = new wxTextCtrl(panel, wxID_ANY);
    destSizer->Add(destCtrl, 1, wxALL, 5);
    sizer->Add(destSizer, 0, wxEXPAND);

    wxBoxSizer *departSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *departLabel = new wxStaticText(panel, wxID_ANY, "Giờ khởi hành:");
    departLabel->SetMinSize(wxSize(labelWidth, -1));
    departSizer->Add(departLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *departCtrl = new wxTextCtrl(panel, wxID_ANY, "HH:MM DD/MM/YYYY");
    departSizer->Add(departCtrl, 1, wxALL, 5);
    sizer->Add(departSizer, 0, wxEXPAND);

    wxBoxSizer *leaveSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *leaveLabel = new wxStaticText(panel, wxID_ANY, "Giờ cất cánh:");
    leaveLabel->SetMinSize(wxSize(labelWidth, -1));
    leaveSizer->Add(leaveLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *leaveCtrl = new wxTextCtrl(panel, wxID_ANY, "HH:MM DD/MM/YYYY");
    leaveSizer->Add(leaveCtrl, 1, wxALL, 5);
    sizer->Add(leaveSizer, 0, wxEXPAND);

    wxBoxSizer *arrivalSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *arrivalLabel = new wxStaticText(panel, wxID_ANY, "Giờ đến:");
    arrivalLabel->SetMinSize(wxSize(labelWidth, -1));
    arrivalSizer->Add(arrivalLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *arrivalCtrl = new wxTextCtrl(panel, wxID_ANY, "HH:MM DD/MM/YYYY");
    arrivalSizer->Add(arrivalCtrl, 1, wxALL, 5);
    sizer->Add(arrivalSizer, 0, wxEXPAND);

    wxBoxSizer *amountSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *amountLabel = new wxStaticText(panel, wxID_ANY, "Giá vé:");
    amountLabel->SetMinSize(wxSize(labelWidth, -1));
    amountSizer->Add(amountLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *amountCtrl = new wxTextCtrl(panel, wxID_ANY);
    amountSizer->Add(amountCtrl, 1, wxALL, 5);
    sizer->Add(amountSizer, 0, wxEXPAND);

    wxBoxSizer *availSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *availLabel = new wxStaticText(panel, wxID_ANY, "Trạng thái (A/U):");
    availLabel->SetMinSize(wxSize(labelWidth, -1));
    availSizer->Add(availLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *availCtrl = new wxTextCtrl(panel, wxID_ANY, "A");
    availSizer->Add(availCtrl, 1, wxALL, 5);
    sizer->Add(availSizer, 0, wxEXPAND);

    // Nút xác nhận và hủy
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okButton = new wxButton(panel, wxID_OK, "Xác nhận");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Hủy");
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 15);

    panel->SetSizer(sizer);

    // Xử lý sự kiện khi nhấn nút OK
    okButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent &evt)
                   {
        // Kiểm tra các trường bắt buộc không được để trống
        if (noCtrl->GetValue().IsEmpty() || 
            nameCtrl->GetValue().IsEmpty() || 
            fromCtrl->GetValue().IsEmpty() || 
            destCtrl->GetValue().IsEmpty() || 
            departCtrl->GetValue().IsEmpty() || 
            leaveCtrl->GetValue().IsEmpty() || 
            arrivalCtrl->GetValue().IsEmpty() || 
            amountCtrl->GetValue().IsEmpty()) {
            
            wxMessageBox("Vui lòng điền đầy đủ thông tin!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }
        
        dialog.EndModal(wxID_OK); });

    if (dialog.ShowModal() == wxID_OK)
    {
        Flight flight;
        flight.setNo(noCtrl->GetValue().ToStdString());
        flight.setName(nameCtrl->GetValue().ToStdString());
        flight.setFrom(fromCtrl->GetValue().ToStdString());
        flight.setDestination(destCtrl->GetValue().ToStdString());

        // Parse time from user input
        auto parseTime = [](const std::string &str) -> std::tm
        {
            std::tm tm = {};
            strptime(str.c_str(), "%H:%M %d/%m/%Y", &tm);
            return tm;
        };
        flight.setDepartureTime(parseTime(departCtrl->GetValue().ToStdString()));
        flight.setLeaveTime(parseTime(leaveCtrl->GetValue().ToStdString()));
        flight.setArrivalTime(parseTime(arrivalCtrl->GetValue().ToStdString()));

        // Xử lý giá vé
        long amount;
        if (amountCtrl->GetValue().ToLong(&amount))
        {
            flight.setAmount(amount);
        }
        else
        {
            flight.setAmount(0);
        }

        // Xử lý trạng thái
        wxString availStr = availCtrl->GetValue();
        char avail = availStr.IsEmpty() ? 'A' : availStr[0];
        flight.setAvailability(avail);

        bool ok = flightService.addFlight(flight);
        if (ok)
            infoLabel->SetLabel("Đã thêm chuyến bay mới!");
        else
            infoLabel->SetLabel("Thêm chuyến bay thất bại!");

        RefreshFlightList();
    }
}

void FlightWindow::OnEditFlight(wxCommandEvent &event)
{
    long selectedIndex = flightList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn chuyến bay cần sửa!", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }
    int flightId = std::stoi(flightList->GetItemText(selectedIndex, 0).ToStdString());
    auto flightOpt = flightService.getFlightById(flightId);
    if (!flightOpt)
    {
        wxMessageBox("Không tìm thấy chuyến bay!", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    Flight flight = *flightOpt;

    wxDialog dialog(this, wxID_ANY, "Sửa thông tin chuyến bay", wxDefaultPosition, wxSize(400, 500));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    int labelWidth = 140;

    wxBoxSizer *noSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *noLabel = new wxStaticText(panel, wxID_ANY, "Mã chuyến bay:");
    noLabel->SetMinSize(wxSize(labelWidth, -1));
    noSizer->Add(noLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *noCtrl = new wxTextCtrl(panel, wxID_ANY, flight.getNo());
    noSizer->Add(noCtrl, 1, wxALL, 5);
    sizer->Add(noSizer, 0, wxEXPAND);

    wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *nameLabel = new wxStaticText(panel, wxID_ANY, "Tên hãng bay:");
    nameLabel->SetMinSize(wxSize(labelWidth, -1));
    nameSizer->Add(nameLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *nameCtrl = new wxTextCtrl(panel, wxID_ANY, flight.getName());
    nameSizer->Add(nameCtrl, 1, wxALL, 5);
    sizer->Add(nameSizer, 0, wxEXPAND);

    wxBoxSizer *fromSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *fromLabel = new wxStaticText(panel, wxID_ANY, "Điểm khởi hành:");
    fromLabel->SetMinSize(wxSize(labelWidth, -1));
    fromSizer->Add(fromLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *fromCtrl = new wxTextCtrl(panel, wxID_ANY, flight.getFrom());
    fromSizer->Add(fromCtrl, 1, wxALL, 5);
    sizer->Add(fromSizer, 0, wxEXPAND);

    wxBoxSizer *destSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *destLabel = new wxStaticText(panel, wxID_ANY, "Điểm đến:");
    destLabel->SetMinSize(wxSize(labelWidth, -1));
    destSizer->Add(destLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *destCtrl = new wxTextCtrl(panel, wxID_ANY, flight.getDestination());
    destSizer->Add(destCtrl, 1, wxALL, 5);
    sizer->Add(destSizer, 0, wxEXPAND);

    auto formatInputTime = [](const std::tm &t) -> wxString
    {
        char buf[20];
        std::strftime(buf, sizeof(buf), "%H:%M %d/%m/%Y", &t);
        return wxString::FromUTF8(buf);
    };

    wxBoxSizer *departSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *departLabel = new wxStaticText(panel, wxID_ANY, "Giờ khởi hành:");
    departLabel->SetMinSize(wxSize(labelWidth, -1));
    departSizer->Add(departLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *departCtrl = new wxTextCtrl(panel, wxID_ANY, formatInputTime(flight.getDepartureTime()));
    departSizer->Add(departCtrl, 1, wxALL, 5);
    sizer->Add(departSizer, 0, wxEXPAND);

    wxBoxSizer *leaveSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *leaveLabel = new wxStaticText(panel, wxID_ANY, "Giờ cất cánh:");
    leaveLabel->SetMinSize(wxSize(labelWidth, -1));
    leaveSizer->Add(leaveLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *leaveCtrl = new wxTextCtrl(panel, wxID_ANY, formatInputTime(flight.getLeaveTime()));
    leaveSizer->Add(leaveCtrl, 1, wxALL, 5);
    sizer->Add(leaveSizer, 0, wxEXPAND);

    wxBoxSizer *arrivalSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *arrivalLabel = new wxStaticText(panel, wxID_ANY, "Giờ đến:");
    arrivalLabel->SetMinSize(wxSize(labelWidth, -1));
    arrivalSizer->Add(arrivalLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *arrivalCtrl = new wxTextCtrl(panel, wxID_ANY, formatInputTime(flight.getArrivalTime()));
    arrivalSizer->Add(arrivalCtrl, 1, wxALL, 5);
    sizer->Add(arrivalSizer, 0, wxEXPAND);

    wxBoxSizer *amountSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *amountLabel = new wxStaticText(panel, wxID_ANY, "Giá vé:");
    amountLabel->SetMinSize(wxSize(labelWidth, -1));
    amountSizer->Add(amountLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *amountCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%d", static_cast<int>(flight.getAmount())));
    amountSizer->Add(amountCtrl, 1, wxALL, 5);
    sizer->Add(amountSizer, 0, wxEXPAND);

    wxBoxSizer *availSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *availLabel = new wxStaticText(panel, wxID_ANY, "Trạng thái (A/U):");
    availLabel->SetMinSize(wxSize(labelWidth, -1));
    availSizer->Add(availLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *availCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%c", flight.getAvailability()));
    availSizer->Add(availCtrl, 1, wxALL, 5);
    sizer->Add(availSizer, 0, wxEXPAND);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okButton = new wxButton(panel, wxID_OK, "Xác nhận");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Hủy");
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 15);

    panel->SetSizer(sizer);

    // Validate required fields
    okButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent &evt)
                   {
        if (noCtrl->GetValue().IsEmpty() || 
            nameCtrl->GetValue().IsEmpty() || 
            fromCtrl->GetValue().IsEmpty() || 
            destCtrl->GetValue().IsEmpty() || 
            departCtrl->GetValue().IsEmpty() || 
            leaveCtrl->GetValue().IsEmpty() || 
            arrivalCtrl->GetValue().IsEmpty() || 
            amountCtrl->GetValue().IsEmpty()) {
            wxMessageBox("Vui lòng điền đầy đủ thông tin!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }
        dialog.EndModal(wxID_OK); });

    if (dialog.ShowModal() == wxID_OK)
    {
        flight.setNo(noCtrl->GetValue().ToStdString());
        flight.setName(nameCtrl->GetValue().ToStdString());
        flight.setFrom(fromCtrl->GetValue().ToStdString());
        flight.setDestination(destCtrl->GetValue().ToStdString());
        auto parseTime = [](const std::string &str) -> std::tm
        {
            std::tm tm = {};
            strptime(str.c_str(), "%H:%M %d/%m/%Y", &tm);
            return tm;
        };
        flight.setDepartureTime(parseTime(departCtrl->GetValue().ToStdString()));
        flight.setLeaveTime(parseTime(leaveCtrl->GetValue().ToStdString()));
        flight.setArrivalTime(parseTime(arrivalCtrl->GetValue().ToStdString()));
        long amount;
        if (amountCtrl->GetValue().ToLong(&amount))
            flight.setAmount(amount);
        else
            flight.setAmount(0);
        wxString availStr = availCtrl->GetValue();
        char avail = availStr.IsEmpty() ? 'A' : availStr[0];
        flight.setAvailability(avail);
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
    int flightId = std::stoi(flightList->GetItemText(selectedIndex, 0).ToStdString());
    auto flightOpt = flightService.getFlightById(flightId);
    if (!flightOpt)
    {
        wxMessageBox("Không tìm thấy chuyến bay!", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    Flight flight = *flightOpt;
    wxString msg = wxString::Format("Bạn có chắc chắn muốn xóa chuyến bay %s không?", flight.getNo());
    int confirm = wxMessageBox(msg, "Xác nhận xóa", wxYES_NO | wxICON_QUESTION);
    if (confirm != wxYES)
        return;
    bool ok = flightService.deleteFlight(flightId);
    if (ok)
        infoLabel->SetLabel("Đã xóa chuyến bay!");
    else
        infoLabel->SetLabel("Xóa chuyến bay thất bại!");
    RefreshFlightList();
}

void FlightWindow::OnListItemSelected(wxListEvent &event)
{
    // Do nothing for now
}

void FlightWindow::RefreshFlightList()
{
    flightList->DeleteAllItems();
    std::vector<Flight> flights = flightService.getAllFlights();
    long index = 0;
    for (const auto &flight : flights)
    {
        flightList->InsertItem(index, std::to_string(flight.getId()));
        flightList->SetItem(index, 1, wxString::FromUTF8(flight.getNo().c_str()));
        flightList->SetItem(index, 2, wxString::FromUTF8(flight.getName().c_str()));
        flightList->SetItem(index, 3, wxString::FromUTF8(flight.getFrom().c_str()));
        flightList->SetItem(index, 4, wxString::FromUTF8(flight.getDestination().c_str()));
        flightList->SetItem(index, 5, wxString::FromUTF8(formatTime(flight.getDepartureTime()).c_str()));
        flightList->SetItem(index, 6, wxString::FromUTF8(formatTime(flight.getLeaveTime()).c_str()));
        flightList->SetItem(index, 7, wxString::FromUTF8(formatTime(flight.getArrivalTime()).c_str()));
        flightList->SetItem(index, 8, std::to_string(static_cast<int>(flight.getAmount())));
        flightList->SetItem(index, 9, wxString::FromUTF8(std::string(1, flight.getAvailability())));
        index++;
    }
    infoLabel->SetLabel("Danh sách chuyến bay đã được cập nhật");
}