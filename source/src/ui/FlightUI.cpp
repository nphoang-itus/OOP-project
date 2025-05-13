#include "FlightUI.h"
#include <iomanip>
#include <sstream>

enum
{
    ID_BACK = 1,
    ID_SHOW,
    ID_ADD,
    ID_EDIT,
    ID_DELETE,
    ID_FLIGHT_LIST,
    ID_SEARCH_ID,
    ID_SEARCH_ROUTE
};

BEGIN_EVENT_TABLE(FlightWindow, wxFrame)
EVT_BUTTON(ID_BACK, FlightWindow::OnBack)
EVT_BUTTON(ID_SHOW, FlightWindow::OnShowFlights)
EVT_BUTTON(ID_ADD, FlightWindow::OnAddFlight)
EVT_BUTTON(ID_EDIT, FlightWindow::OnEditFlight)
EVT_BUTTON(ID_DELETE, FlightWindow::OnDeleteFlight)
EVT_BUTTON(ID_SEARCH_ID, FlightWindow::OnSearchById)
EVT_BUTTON(ID_SEARCH_ROUTE, FlightWindow::OnSearchByRoute)
EVT_LIST_ITEM_SELECTED(ID_FLIGHT_LIST, FlightWindow::OnListItemSelected)
END_EVENT_TABLE()

// Hàm chuyển std::tm thành string
static std::string formatTime(const std::tm &t)
{
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &t);
    return std::string(buf);
}

FlightWindow::FlightWindow(const wxString &title, std::shared_ptr<FlightService> flightService, wxWindow *parent)
    : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 600)), flightService(flightService)
{
    // Tạo giao diện
    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);

    // Nút quay lại
    backButton = new wxButton(panel, ID_BACK, "Back", wxDefaultPosition, wxSize(100, 30));
    buttonSizer->Add(backButton, 0, wxALL, 10);

    // Các nút chức năng
    showButton = new wxButton(panel, ID_SHOW, "Hiển thị thông tin các chuyến bay", wxDefaultPosition, wxSize(250, 50));
    addButton = new wxButton(panel, ID_ADD, "Thêm chuyến bay", wxDefaultPosition, wxSize(250, 50));
    editButton = new wxButton(panel, ID_EDIT, "Sửa chuyến bay", wxDefaultPosition, wxSize(250, 50));
    deleteButton = new wxButton(panel, ID_DELETE, "Xóa chuyến bay", wxDefaultPosition, wxSize(250, 50));
    searchByIdButton = new wxButton(panel, ID_SEARCH_ID, "Tìm theo ID", wxDefaultPosition, wxSize(250, 50));
    searchByRouteButton = new wxButton(panel, ID_SEARCH_ROUTE, "Tìm theo tuyến bay", wxDefaultPosition, wxSize(250, 50));

    // Danh sách chuyến bay
    flightList = new wxListCtrl(panel, ID_FLIGHT_LIST, wxDefaultPosition, wxSize(700, 300),
                                wxLC_REPORT | wxLC_SINGLE_SEL);

    // Thêm cột cho danh sách
    const char *columns[] = {"ID", "Mã chuyến bay", "Tên hãng bay", "Điểm khởi hành",
                             "Điểm đến", "Thời gian khởi hành", "Thời gian cất cánh",
                             "Thời gian hạ cánh", "Giá vé", "Trạng thái"};

    for (int i = 0; i < 10; ++i)
    {
        flightList->InsertColumn(i, columns[i]);
        flightList->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    }

    // Nhãn thông báo
    infoLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(600, 50));
    infoLabel->Wrap(600);

    // Sắp xếp các nút thành 2 hàng
    wxBoxSizer *row1 = new wxBoxSizer(wxHORIZONTAL);
    row1->Add(showButton, 0, wxALL, 10);
    row1->Add(addButton, 0, wxALL, 10);
    row1->Add(editButton, 0, wxALL, 10);
    row1->Add(deleteButton, 0, wxALL, 10);

    wxBoxSizer *row2 = new wxBoxSizer(wxHORIZONTAL);
    row2->Add(searchByIdButton, 0, wxALL, 10);
    row2->Add(searchByRouteButton, 0, wxALL, 10);

    // Thêm các thành phần vào sizer
    contentSizer->Add(row1, 0, wxALIGN_CENTER);
    contentSizer->Add(row2, 0, wxALIGN_CENTER);
    contentSizer->Add(flightList, 1, wxALL | wxEXPAND, 10);
    contentSizer->Add(infoLabel, 0, wxALL | wxALIGN_CENTER, 10);

    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(contentSizer, 1, wxEXPAND);

    panel->SetSizer(mainSizer);
    Centre();
}

void FlightWindow::OnBack(wxCommandEvent &event)
{
    if (GetParent())
        GetParent()->Show();
    this->Destroy();
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

    // Cấu trúc dữ liệu cho các trường nhập liệu
    struct FieldInfo
    {
        wxString label;
        wxString defaultValue;
        wxTextCtrl *ctrl;
    };

    FieldInfo fields[] = {
        {"Mã chuyến bay:", "", nullptr},
        {"Tên hãng bay:", "", nullptr},
        {"Điểm khởi hành:", "", nullptr},
        {"Điểm đến:", "", nullptr},
        {"Giờ khởi hành:", "HH:MM DD/MM/YYYY", nullptr},
        {"Giờ cất cánh:", "HH:MM DD/MM/YYYY", nullptr},
        {"Giờ đến:", "HH:MM DD/MM/YYYY", nullptr},
        {"Giá vé:", "", nullptr},
        {"Trạng thái (A/U):", "A", nullptr}};

    int labelWidth = 140;

    // Tạo các trường nhập liệu
    for (auto &field : fields)
    {
        wxBoxSizer *fieldSizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText *label = new wxStaticText(panel, wxID_ANY, field.label);
        label->SetMinSize(wxSize(labelWidth, -1));
        field.ctrl = new wxTextCtrl(panel, wxID_ANY, field.defaultValue);

        fieldSizer->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        fieldSizer->Add(field.ctrl, 1, wxALL, 5);
        sizer->Add(fieldSizer, 0, wxEXPAND);
    }

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
        // Kiểm tra các trường bắt buộc
        for (const auto &field : fields) {
            if (field.ctrl->GetValue().IsEmpty()) {
                wxMessageBox("Vui lòng điền đầy đủ thông tin!", "Lỗi", wxICON_ERROR | wxOK);
                return;
            }
        }
        dialog.EndModal(wxID_OK); });

    if (dialog.ShowModal() == wxID_OK)
    {
        Flight flight;
        flight.setNo(fields[0].ctrl->GetValue().ToStdString());
        flight.setName(fields[1].ctrl->GetValue().ToStdString());
        flight.setFrom(fields[2].ctrl->GetValue().ToStdString());
        flight.setDestination(fields[3].ctrl->GetValue().ToStdString());

        // Hàm phân tích thời gian
        auto parseTime = [](const std::string &str) -> std::tm
        {
            std::tm tm = {};
            strptime(str.c_str(), "%H:%M %d/%m/%Y", &tm);
            return tm;
        };

        flight.setDepartureTime(parseTime(fields[4].ctrl->GetValue().ToStdString()));
        flight.setLeaveTime(parseTime(fields[5].ctrl->GetValue().ToStdString()));
        flight.setArrivalTime(parseTime(fields[6].ctrl->GetValue().ToStdString()));

        // Xử lý giá vé
        long amount;
        if (fields[7].ctrl->GetValue().ToLong(&amount))
        {
            flight.setAmount(amount);
        }
        else
        {
            flight.setAmount(0);
        }

        // Xử lý trạng thái
        wxString availStr = fields[8].ctrl->GetValue();
        char avail = availStr.IsEmpty() ? 'A' : availStr[0];
        flight.setAvailability(avail);

        bool ok = flightService->save(flight);
        infoLabel->SetLabel(ok ? "Đã thêm chuyến bay mới!" : "Thêm chuyến bay thất bại!");
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
    auto flightOpt = flightService->findById(flightId);
    if (!flightOpt)
    {
        wxMessageBox("Không tìm thấy chuyến bay!", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    Flight flight = *flightOpt;

    wxDialog dialog(this, wxID_ANY, "Sửa thông tin chuyến bay", wxDefaultPosition, wxSize(400, 500));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    // Định dạng thời gian cho input
    auto formatInputTime = [](const std::tm &t) -> wxString
    {
        char buf[20];
        std::strftime(buf, sizeof(buf), "%H:%M %d/%m/%Y", &t);
        return wxString::FromUTF8(buf);
    };

    // Cấu trúc dữ liệu cho các trường nhập liệu
    struct FieldInfo
    {
        wxString label;
        wxString value;
        wxTextCtrl *ctrl;
    };

    FieldInfo fields[] = {
        {"Mã chuyến bay:", flight.getNo(), nullptr},
        {"Tên hãng bay:", flight.getName(), nullptr},
        {"Điểm khởi hành:", flight.getFrom(), nullptr},
        {"Điểm đến:", flight.getDestination(), nullptr},
        {"Giờ khởi hành:", formatInputTime(flight.getDepartureTime()), nullptr},
        {"Giờ cất cánh:", formatInputTime(flight.getLeaveTime()), nullptr},
        {"Giờ đến:", formatInputTime(flight.getArrivalTime()), nullptr},
        {"Giá vé:", wxString::Format("%d", static_cast<int>(flight.getAmount())), nullptr},
        {"Trạng thái (A/U):", wxString::Format("%c", flight.getAvailability()), nullptr}};

    int labelWidth = 140;

    // Tạo các trường nhập liệu
    for (auto &field : fields)
    {
        wxBoxSizer *fieldSizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText *label = new wxStaticText(panel, wxID_ANY, field.label);
        label->SetMinSize(wxSize(labelWidth, -1));
        field.ctrl = new wxTextCtrl(panel, wxID_ANY, field.value);

        fieldSizer->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        fieldSizer->Add(field.ctrl, 1, wxALL, 5);
        sizer->Add(fieldSizer, 0, wxEXPAND);
    }

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
        // Kiểm tra các trường bắt buộc
        for (const auto &field : fields) {
            if (field.ctrl->GetValue().IsEmpty()) {
                wxMessageBox("Vui lòng điền đầy đủ thông tin!", "Lỗi", wxICON_ERROR | wxOK);
                return;
            }
        }
        dialog.EndModal(wxID_OK); });

    if (dialog.ShowModal() == wxID_OK)
    {
        flight.setNo(fields[0].ctrl->GetValue().ToStdString());
        flight.setName(fields[1].ctrl->GetValue().ToStdString());
        flight.setFrom(fields[2].ctrl->GetValue().ToStdString());
        flight.setDestination(fields[3].ctrl->GetValue().ToStdString());

        // Hàm phân tích thời gian
        auto parseTime = [](const std::string &str) -> std::tm
        {
            std::tm tm = {};
            strptime(str.c_str(), "%H:%M %d/%m/%Y", &tm);
            return tm;
        };

        flight.setDepartureTime(parseTime(fields[4].ctrl->GetValue().ToStdString()));
        flight.setLeaveTime(parseTime(fields[5].ctrl->GetValue().ToStdString()));
        flight.setArrivalTime(parseTime(fields[6].ctrl->GetValue().ToStdString()));

        // Xử lý giá vé
        long amount;
        if (fields[7].ctrl->GetValue().ToLong(&amount))
        {
            flight.setAmount(amount);
        }
        else
        {
            flight.setAmount(0);
        }

        // Xử lý trạng thái
        wxString availStr = fields[8].ctrl->GetValue();
        char avail = availStr.IsEmpty() ? 'A' : availStr[0];
        flight.setAvailability(avail);

        bool ok = flightService->update(flight);
        infoLabel->SetLabel(ok ? "Đã cập nhật chuyến bay!" : "Cập nhật chuyến bay thất bại!");
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
    auto flightOpt = flightService->findById(flightId);
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

    bool ok = flightService->remove(flightId);
    infoLabel->SetLabel(ok ? "Đã xóa chuyến bay!" : "Xóa chuyến bay thất bại!");
    RefreshFlightList();
}

void FlightWindow::OnListItemSelected(wxListEvent &event)
{
    // Để trống - có thể mở rộng chức năng sau
}

void FlightWindow::RefreshFlightList()
{
    try
    {
        wxLogMessage("Starting to refresh flight list...");
        std::vector<Flight> flights = flightService->findAll();
        wxLogMessage("Found %d flights in database", flights.size());

        ShowFlights(flights);
        infoLabel->SetLabel("Danh sách chuyến bay đã được cập nhật");
        wxLogMessage("Flight list refresh completed successfully");
    }
    catch (const std::exception &e)
    {
        wxLogError("Error loading flights: %s", e.what());
        infoLabel->SetLabel("Lỗi khi tải danh sách chuyến bay!");
    }
}

void FlightWindow::ShowFlights(const std::vector<Flight> &flights)
{
    wxLogMessage("Displaying %d flights in the list", flights.size());
    flightList->DeleteAllItems();
    long index = 0;
    for (const auto &flight : flights)
    {
        try
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
        catch (const std::exception &e)
        {
            wxLogError("Error displaying flight at index %d: %s", index, e.what());
        }
    }
    wxLogMessage("Finished displaying flights in the list");
}

void FlightWindow::ShowFlight(const Flight &flight)
{
    ShowFlights({flight});
}

bool FlightWindow::ShowInputDialog(const wxString &title, const wxString &label, wxString &outValue)
{
    wxDialog dialog(this, wxID_ANY, title, wxDefaultPosition, wxSize(300, 150));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    // Input field
    wxBoxSizer *inputSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *inputLabel = new wxStaticText(panel, wxID_ANY, label);
    inputLabel->SetMinSize(wxSize(100, -1));
    wxTextCtrl *inputCtrl = new wxTextCtrl(panel, wxID_ANY);
    inputSizer->Add(inputLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    inputSizer->Add(inputCtrl, 1, wxALL, 5);

    // Buttons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(panel, wxID_OK, "Tìm kiếm"), 0, wxALL, 5);
    buttonSizer->Add(new wxButton(panel, wxID_CANCEL, "Hủy"), 0, wxALL, 5);

    sizer->Add(inputSizer, 0, wxEXPAND);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 15);
    panel->SetSizer(sizer);

    if (dialog.ShowModal() == wxID_OK)
    {
        outValue = inputCtrl->GetValue();
        return true;
    }
    return false;
}

bool FlightWindow::ShowInputDialog2(const wxString &title, const wxString &label1, wxString &out1, const wxString &label2, wxString &out2)
{
    wxDialog dialog(this, wxID_ANY, title, wxDefaultPosition, wxSize(400, 200));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    // First input field
    wxBoxSizer *sizer1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *labelCtrl1 = new wxStaticText(panel, wxID_ANY, label1, wxDefaultPosition, wxSize(120, -1));
    wxTextCtrl *ctrl1 = new wxTextCtrl(panel, wxID_ANY);
    sizer1->Add(labelCtrl1, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    sizer1->Add(ctrl1, 1, wxALL, 5);

    // Second input field
    wxBoxSizer *sizer2 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *labelCtrl2 = new wxStaticText(panel, wxID_ANY, label2, wxDefaultPosition, wxSize(120, -1));
    wxTextCtrl *ctrl2 = new wxTextCtrl(panel, wxID_ANY);
    sizer2->Add(labelCtrl2, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    sizer2->Add(ctrl2, 1, wxALL, 5);

    // Buttons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(panel, wxID_OK, "Tìm kiếm"), 0, wxALL, 5);
    buttonSizer->Add(new wxButton(panel, wxID_CANCEL, "Hủy"), 0, wxALL, 5);

    // Add all to main sizer
    sizer->Add(sizer1, 0, wxEXPAND);
    sizer->Add(sizer2, 0, wxEXPAND);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 15);

    panel->SetSizer(sizer);

    if (dialog.ShowModal() == wxID_OK)
    {
        out1 = ctrl1->GetValue();
        out2 = ctrl2->GetValue();
        return true;
    }
    return false;
}

void FlightWindow::OnSearchById(wxCommandEvent &event)
{
    wxString idStr;
    if (ShowInputDialog("Tìm chuyến bay theo ID", "ID chuyến bay:", idStr))
    {
        long id;
        if (idStr.ToLong(&id))
        {
            auto flightOpt = flightService->findById(static_cast<int>(id));
            if (flightOpt)
            {
                ShowFlight(*flightOpt);
                infoLabel->SetLabel("Đã tìm thấy chuyến bay!");
            }
            else
            {
                infoLabel->SetLabel("Không tìm thấy chuyến bay với ID này!");
            }
        }
        else
        {
            wxMessageBox("Vui lòng nhập ID hợp lệ!", "Lỗi", wxICON_ERROR | wxOK);
        }
    }
}

void FlightWindow::OnSearchByRoute(wxCommandEvent &event)
{
    wxString fromStr, toStr;
    if (ShowInputDialog2("Tìm chuyến bay theo tuyến", "Điểm khởi hành:", fromStr, "Điểm đến:", toStr))
    {
        if (fromStr.IsEmpty() || toStr.IsEmpty())
        {
            wxMessageBox("Vui lòng nhập đầy đủ điểm khởi hành và điểm đến!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }

        std::vector<Flight> flights = flightService->findByRoute(fromStr.ToStdString(), toStr.ToStdString());
        ShowFlights(flights);

        if (flights.empty())
        {
            infoLabel->SetLabel("Không tìm thấy chuyến bay nào phù hợp!");
        }
        else
        {
            infoLabel->SetLabel(wxString::Format("Đã tìm thấy %d chuyến bay!", (int)flights.size()));
        }
    }
}