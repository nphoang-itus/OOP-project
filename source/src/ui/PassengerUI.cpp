#include "PassengerUI.h"
#include <iomanip>
#include <sstream>

enum
{
    ID_BACK = 1,
    ID_SHOW = 2,
    ID_ADD = 3,
    ID_EDIT = 4,
    ID_DELETE = 5,
    ID_PASSENGER_LIST = 6
};

BEGIN_EVENT_TABLE(PassengerWindow, wxFrame)
EVT_BUTTON(ID_BACK, PassengerWindow::OnBack)
EVT_BUTTON(ID_SHOW, PassengerWindow::OnShowPassengers)
EVT_BUTTON(ID_ADD, PassengerWindow::OnAddPassenger)
EVT_BUTTON(ID_EDIT, PassengerWindow::OnEditPassenger)
EVT_BUTTON(ID_DELETE, PassengerWindow::OnDeletePassenger)
EVT_LIST_ITEM_SELECTED(ID_PASSENGER_LIST, PassengerWindow::OnListItemSelected)
END_EVENT_TABLE()

PassengerWindow::PassengerWindow(const wxString &title)
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
    showButton = new wxButton(panel, ID_SHOW, "Hiển thị thông tin hành khách", wxDefaultPosition, wxSize(250, 50));
    addButton = new wxButton(panel, ID_ADD, "Thêm hành khách", wxDefaultPosition, wxSize(250, 50));
    editButton = new wxButton(panel, ID_EDIT, "Sửa thông tin hành khách", wxDefaultPosition, wxSize(250, 50));
    deleteButton = new wxButton(panel, ID_DELETE, "Xóa hành khách", wxDefaultPosition, wxSize(250, 50));

    // Create passenger list
    passengerList = new wxListCtrl(panel, ID_PASSENGER_LIST, wxDefaultPosition, wxSize(700, 300),
                                   wxLC_REPORT | wxLC_SINGLE_SEL);
    passengerList->InsertColumn(0, "ID");
    passengerList->InsertColumn(1, "Tên");
    passengerList->InsertColumn(2, "Số điện thoại");
    passengerList->InsertColumn(3, "Số hộ chiếu");
    passengerList->InsertColumn(4, "Địa chỉ");

    for (int i = 0; i < 5; ++i)
    {
        passengerList->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    }

    // Create info label
    infoLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(600, 50));
    infoLabel->Wrap(600);

    // Add buttons to content sizer
    contentSizer->Add(showButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(addButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(editButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(deleteButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(passengerList, 0, wxALL | wxEXPAND, 10);
    contentSizer->Add(infoLabel, 0, wxALL | wxALIGN_CENTER, 10);

    // Add sizers to main sizer
    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(contentSizer, 1, wxEXPAND);

    panel->SetSizer(mainSizer);
    Centre();
}

void PassengerWindow::OnBack(wxCommandEvent &event)
{
    this->Close();
    MainWindow *mainWindow = new MainWindow("Menu chính");
    mainWindow->Show();
}

void PassengerWindow::OnShowPassengers(wxCommandEvent &event)
{
    RefreshPassengerList();
}

void PassengerWindow::OnAddPassenger(wxCommandEvent &event)
{
    wxDialog dialog(this, wxID_ANY, "Thêm hành khách mới", wxDefaultPosition, wxSize(400, 500));

    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    int labelWidth = 120;

    // Tạo các trường nhập liệu
    wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *nameLabel = new wxStaticText(panel, wxID_ANY, "Tên:");
    nameLabel->SetMinSize(wxSize(labelWidth, -1));
    nameSizer->Add(nameLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *nameCtrl = new wxTextCtrl(panel, wxID_ANY);
    nameSizer->Add(nameCtrl, 1, wxALL, 5);
    sizer->Add(nameSizer, 0, wxEXPAND);

    wxBoxSizer *phoneSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *phoneLabel = new wxStaticText(panel, wxID_ANY, "Số điện thoại:");
    phoneLabel->SetMinSize(wxSize(labelWidth, -1));
    phoneSizer->Add(phoneLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *phoneCtrl = new wxTextCtrl(panel, wxID_ANY);
    phoneSizer->Add(phoneCtrl, 1, wxALL, 5);
    sizer->Add(phoneSizer, 0, wxEXPAND);

    wxBoxSizer *passportSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *passportLabel = new wxStaticText(panel, wxID_ANY, "Số hộ chiếu:");
    passportLabel->SetMinSize(wxSize(labelWidth, -1));
    passportSizer->Add(passportLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *passportCtrl = new wxTextCtrl(panel, wxID_ANY);
    passportSizer->Add(passportCtrl, 1, wxALL, 5);
    sizer->Add(passportSizer, 0, wxEXPAND);

    wxBoxSizer *addressSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *addressLabel = new wxStaticText(panel, wxID_ANY, "Địa chỉ:");
    addressLabel->SetMinSize(wxSize(labelWidth, -1));
    addressSizer->Add(addressLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *addressCtrl = new wxTextCtrl(panel, wxID_ANY);
    addressSizer->Add(addressCtrl, 1, wxALL, 5);
    sizer->Add(addressSizer, 0, wxEXPAND);

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
        if (nameCtrl->GetValue().IsEmpty() || 
            phoneCtrl->GetValue().IsEmpty() || 
            passportCtrl->GetValue().IsEmpty()) {
            
            wxMessageBox("Vui lòng điền đầy đủ thông tin bắt buộc (Tên, Số điện thoại, Số hộ chiếu)!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }
        
        dialog.EndModal(wxID_OK); });

    if (dialog.ShowModal() == wxID_OK)
    {
        Passenger passenger;
        passenger.setName(nameCtrl->GetValue().ToStdString());
        passenger.setPhone(phoneCtrl->GetValue().ToStdString());
        passenger.setPassport(passportCtrl->GetValue().ToStdString());
        passenger.setAddress(addressCtrl->GetValue().ToStdString());

        bool ok = passengerService.addPassenger(passenger);
        if (ok)
            infoLabel->SetLabel("Đã thêm hành khách mới!");
        else
            infoLabel->SetLabel("Thêm hành khách thất bại!");

        RefreshPassengerList();
    }
}

void PassengerWindow::OnEditPassenger(wxCommandEvent &event)
{
    long selectedIndex = passengerList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn hành khách cần sửa!", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }
    int passengerId = std::stoi(passengerList->GetItemText(selectedIndex, 0).ToStdString());
    auto passengerOpt = passengerService.getPassengerById(passengerId);
    if (!passengerOpt)
    {
        wxMessageBox("Không tìm thấy hành khách!", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    Passenger passenger = *passengerOpt;

    wxDialog dialog(this, wxID_ANY, "Sửa thông tin hành khách", wxDefaultPosition, wxSize(400, 500));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    int labelWidth = 120;

    wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *nameLabel = new wxStaticText(panel, wxID_ANY, "Tên:");
    nameLabel->SetMinSize(wxSize(labelWidth, -1));
    nameSizer->Add(nameLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *nameCtrl = new wxTextCtrl(panel, wxID_ANY, passenger.getName());
    nameSizer->Add(nameCtrl, 1, wxALL, 5);
    sizer->Add(nameSizer, 0, wxEXPAND);

    wxBoxSizer *phoneSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *phoneLabel = new wxStaticText(panel, wxID_ANY, "Số điện thoại:");
    phoneLabel->SetMinSize(wxSize(labelWidth, -1));
    phoneSizer->Add(phoneLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *phoneCtrl = new wxTextCtrl(panel, wxID_ANY, passenger.getPhone());
    phoneSizer->Add(phoneCtrl, 1, wxALL, 5);
    sizer->Add(phoneSizer, 0, wxEXPAND);

    wxBoxSizer *passportSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *passportLabel = new wxStaticText(panel, wxID_ANY, "Số hộ chiếu:");
    passportLabel->SetMinSize(wxSize(labelWidth, -1));
    passportSizer->Add(passportLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *passportCtrl = new wxTextCtrl(panel, wxID_ANY, passenger.getPassport());
    passportSizer->Add(passportCtrl, 1, wxALL, 5);
    sizer->Add(passportSizer, 0, wxEXPAND);

    wxBoxSizer *addressSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *addressLabel = new wxStaticText(panel, wxID_ANY, "Địa chỉ:");
    addressLabel->SetMinSize(wxSize(labelWidth, -1));
    addressSizer->Add(addressLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *addressCtrl = new wxTextCtrl(panel, wxID_ANY, passenger.getAddress());
    addressSizer->Add(addressCtrl, 1, wxALL, 5);
    sizer->Add(addressSizer, 0, wxEXPAND);

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
        if (nameCtrl->GetValue().IsEmpty() || 
            phoneCtrl->GetValue().IsEmpty() || 
            passportCtrl->GetValue().IsEmpty()) {
            wxMessageBox("Vui lòng điền đầy đủ thông tin bắt buộc (Tên, Số điện thoại, Số hộ chiếu)!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }
        dialog.EndModal(wxID_OK); });

    if (dialog.ShowModal() == wxID_OK)
    {
        passenger.setName(nameCtrl->GetValue().ToStdString());
        passenger.setPhone(phoneCtrl->GetValue().ToStdString());
        passenger.setPassport(passportCtrl->GetValue().ToStdString());
        passenger.setAddress(addressCtrl->GetValue().ToStdString());

        bool ok = passengerService.updatePassenger(passenger);
        if (ok)
            infoLabel->SetLabel("Đã cập nhật hành khách!");
        else
            infoLabel->SetLabel("Cập nhật hành khách thất bại!");
        RefreshPassengerList();
    }
}

void PassengerWindow::OnDeletePassenger(wxCommandEvent &event)
{
    long selectedIndex = passengerList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn hành khách cần xóa!", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }
    int passengerId = std::stoi(passengerList->GetItemText(selectedIndex, 0).ToStdString());
    auto passengerOpt = passengerService.getPassengerById(passengerId);
    if (!passengerOpt)
    {
        wxMessageBox("Không tìm thấy hành khách!", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    Passenger passenger = *passengerOpt;
    wxString msg = wxString::Format("Bạn có chắc chắn muốn xóa hành khách %s không?", passenger.getName());
    int confirm = wxMessageBox(msg, "Xác nhận xóa", wxYES_NO | wxICON_QUESTION);
    if (confirm != wxYES)
        return;
    bool ok = passengerService.deletePassenger(passengerId);
    if (ok)
        infoLabel->SetLabel("Đã xóa hành khách!");
    else
        infoLabel->SetLabel("Xóa hành khách thất bại!");
    RefreshPassengerList();
}

void PassengerWindow::OnListItemSelected(wxListEvent &event)
{
    // Do nothing for now
}

void PassengerWindow::RefreshPassengerList()
{
    passengerList->DeleteAllItems();
    std::vector<Passenger> passengers = passengerService.getAllPassengers();
    long index = 0;
    for (const auto &p : passengers)
    {
        passengerList->InsertItem(index, std::to_string(p.getId()));
        passengerList->SetItem(index, 1, wxString::FromUTF8(p.getName().c_str()));
        passengerList->SetItem(index, 2, wxString::FromUTF8(p.getPhone().c_str()));
        passengerList->SetItem(index, 3, wxString::FromUTF8(p.getPassport().c_str()));
        passengerList->SetItem(index, 4, wxString::FromUTF8(p.getAddress().c_str()));
        index++;
    }
    infoLabel->SetLabel("Danh sách hành khách đã được cập nhật");
}