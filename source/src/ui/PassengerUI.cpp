#include "PassengerUI.h"

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
    showButton = new wxButton(panel, ID_SHOW, "Hiển thị thông tin hành khách", wxDefaultPosition, wxSize(250, 50));
    addButton = new wxButton(panel, ID_ADD, "Thêm hành khách", wxDefaultPosition, wxSize(250, 50));
    editButton = new wxButton(panel, ID_EDIT, "Sửa thông tin hành khách", wxDefaultPosition, wxSize(250, 50));
    deleteButton = new wxButton(panel, ID_DELETE, "Xóa hành khách", wxDefaultPosition, wxSize(250, 50));

    // Create passenger list
    passengerList = new wxListCtrl(panel, ID_PASSENGER_LIST, wxDefaultPosition, wxSize(700, 300),
                                   wxLC_REPORT | wxLC_SINGLE_SEL);
    passengerList->InsertColumn(0, "Mã hành khách");
    passengerList->InsertColumn(1, "Tên");
    passengerList->InsertColumn(2, "Số điện thoại");
    passengerList->InsertColumn(3, "Email");

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
    Passenger p;
    p.setName("Nguyen Van A");
    p.setPhone("0123456789");
    p.setPassport("B1234567");
    p.setAddress("HCM");
    bool ok = passengerService.addPassenger(p);
    if (ok)
        infoLabel->SetLabel("Đã thêm hành khách mới!");
    else
        infoLabel->SetLabel("Thêm hành khách thất bại!");
    RefreshPassengerList();
}

void PassengerWindow::OnEditPassenger(wxCommandEvent &event)
{
    long selectedIndex = passengerList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn hành khách cần sửa!", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }
    std::string idStr = passengerList->GetItemText(selectedIndex, 0).ToStdString();
    int id = 0;
    try
    {
        id = std::stoi(idStr);
    }
    catch (...)
    {
        infoLabel->SetLabel("ID hành khách không hợp lệ!");
        return;
    }
    auto passengerOpt = passengerService.getPassengerById(id);
    if (passengerOpt)
    {
        Passenger p = *passengerOpt;
        p.setName("Nguyen Van B");
        bool ok = passengerService.updatePassenger(p);
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
    std::string idStr = passengerList->GetItemText(selectedIndex, 0).ToStdString();
    int id = 0;
    try
    {
        id = std::stoi(idStr);
    }
    catch (...)
    {
        infoLabel->SetLabel("ID hành khách không hợp lệ!");
        return;
    }
    bool ok = passengerService.deletePassenger(id);
    if (ok)
        infoLabel->SetLabel("Đã xóa hành khách!");
    else
        infoLabel->SetLabel("Xóa hành khách thất bại!");
    RefreshPassengerList();
}

void PassengerWindow::OnListItemSelected(wxListEvent &event)
{
    RefreshPassengerList();
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
        index++;
    }
    infoLabel->SetLabel("Danh sách hành khách đã được cập nhật");
}