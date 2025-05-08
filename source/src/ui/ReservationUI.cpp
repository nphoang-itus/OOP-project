#include "ReservationUI.h"
#include <memory>

enum
{
    ID_BACK = 1,
    ID_SHOW = 2,
    ID_ADD = 3,
    ID_EDIT = 4,
    ID_DELETE = 5,
    ID_RESERVATION_LIST = 6
};

BEGIN_EVENT_TABLE(ReservationWindow, wxFrame)
EVT_BUTTON(ID_BACK, ReservationWindow::OnBack)
EVT_BUTTON(ID_SHOW, ReservationWindow::OnShowReservations)
EVT_BUTTON(ID_ADD, ReservationWindow::OnAddReservation)
EVT_BUTTON(ID_EDIT, ReservationWindow::OnEditReservation)
EVT_BUTTON(ID_DELETE, ReservationWindow::OnDeleteReservation)
EVT_LIST_ITEM_SELECTED(ID_RESERVATION_LIST, ReservationWindow::OnListItemSelected)
END_EVENT_TABLE()

ReservationWindow::ReservationWindow(const wxString &title)
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
    showButton = new wxButton(panel, ID_SHOW, "Hiển thị thông tin đặt chỗ", wxDefaultPosition, wxSize(250, 50));
    addButton = new wxButton(panel, ID_ADD, "Thêm đặt chỗ", wxDefaultPosition, wxSize(250, 50));
    editButton = new wxButton(panel, ID_EDIT, "Sửa thông tin đặt chỗ", wxDefaultPosition, wxSize(250, 50));
    deleteButton = new wxButton(panel, ID_DELETE, "Xóa đặt chỗ", wxDefaultPosition, wxSize(250, 50));

    // Create reservation list
    reservationList = new wxListCtrl(panel, ID_RESERVATION_LIST, wxDefaultPosition, wxSize(700, 300),
                                     wxLC_REPORT | wxLC_SINGLE_SEL);
    reservationList->InsertColumn(0, "Mã đặt chỗ");
    reservationList->InsertColumn(1, "Mã chuyến bay");
    reservationList->InsertColumn(2, "Mã hành khách");
    reservationList->InsertColumn(3, "Số ghế");
    reservationList->InsertColumn(4, "Trạng thái");

    // Create info label
    infoLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(600, 50));
    infoLabel->Wrap(600);

    // Add buttons to content sizer
    contentSizer->Add(showButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(addButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(editButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(deleteButton, 0, wxALL | wxALIGN_CENTER, 10);
    contentSizer->Add(reservationList, 0, wxALL | wxEXPAND, 10);
    contentSizer->Add(infoLabel, 0, wxALL | wxALIGN_CENTER, 10);

    // Add sizers to main sizer
    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(contentSizer, 1, wxEXPAND);

    panel->SetSizer(mainSizer);
    Centre();
}

void ReservationWindow::OnBack(wxCommandEvent &event)
{
    this->Close();
    MainWindow *mainWindow = new MainWindow("Menu chính");
    mainWindow->Show();
}

void ReservationWindow::OnShowReservations(wxCommandEvent &event)
{
    RefreshReservationList();
}

void ReservationWindow::OnAddReservation(wxCommandEvent &event)
{
    auto flight = std::make_shared<Flight>();
    flight->setNo("VN123");
    auto passenger = std::make_shared<Passenger>();
    passenger->setName("Nguyen Van A");

    Reservation r;
    r.setTicketNo("TICKET001");
    r.setFlight(flight);
    r.setPassenger(passenger);

    // Tùy service bạn có thể dùng updateReservation hoặc createReservationWithNewPassenger
    bool ok = reservationService.updateReservation(r);
    if (ok)
        infoLabel->SetLabel("Đã thêm đặt chỗ mới!");
    else
        infoLabel->SetLabel("Thêm đặt chỗ thất bại!");
    RefreshReservationList();
}

void ReservationWindow::OnEditReservation(wxCommandEvent &event)
{
    long selectedIndex = reservationList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn đặt chỗ cần sửa!", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }
    std::string ticketNo = reservationList->GetItemText(selectedIndex, 0).ToStdString();
    auto reservationOpt = reservationService.getReservationByTicketNo(ticketNo);
    if (reservationOpt)
    {
        Reservation r = *reservationOpt;
        // Sửa thông tin (ví dụ sửa lại tên hành khách)
        if (r.getPassenger())
            r.getPassenger()->setName("Nguyen Van B");
        bool ok = reservationService.updateReservation(r);
        if (ok)
            infoLabel->SetLabel("Đã cập nhật đặt chỗ!");
        else
            infoLabel->SetLabel("Cập nhật đặt chỗ thất bại!");
        RefreshReservationList();
    }
}

void ReservationWindow::OnDeleteReservation(wxCommandEvent &event)
{
    long selectedIndex = reservationList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn đặt chỗ cần xóa!", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }
    std::string ticketNo = reservationList->GetItemText(selectedIndex, 0).ToStdString();
    auto reservationOpt = reservationService.getReservationByTicketNo(ticketNo);
    if (reservationOpt)
    {
        int id = reservationOpt->getId();
        bool ok = reservationService.cancelReservation(id);
        if (ok)
            infoLabel->SetLabel("Đã xóa đặt chỗ!");
        else
            infoLabel->SetLabel("Xóa đặt chỗ thất bại!");
        RefreshReservationList();
    }
}

void ReservationWindow::OnListItemSelected(wxListEvent &event)
{
    RefreshReservationList();
}

void ReservationWindow::RefreshReservationList()
{
    reservationList->DeleteAllItems();
    std::vector<Reservation> reservations = reservationService.getAllReservations();
    long index = 0;
    for (const auto &r : reservations)
    {
        reservationList->InsertItem(index, wxString::FromUTF8(r.getTicketNo().c_str()));
        reservationList->SetItem(index, 1, wxString::FromUTF8(r.getFlight() ? r.getFlight()->getNo().c_str() : ""));
        reservationList->SetItem(index, 2, wxString::FromUTF8(r.getPassenger() ? r.getPassenger()->getName().c_str() : ""));
        // Có thể thêm các cột khác nếu cần
        index++;
    }
    infoLabel->SetLabel("Danh sách đặt chỗ đã được cập nhật");
}