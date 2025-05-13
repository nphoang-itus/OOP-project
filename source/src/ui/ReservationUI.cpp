#include "ReservationUI.h"
#include <iomanip>
#include <sstream>

enum
{
    ID_BACK = 1,
    ID_SHOW = 2,
    ID_ADD = 3,
    ID_EDIT = 4,
    ID_DELETE = 5,
    ID_RESERVATION_LIST = 6,
    ID_SEARCH_ID = 7,
    ID_SEARCH_TICKET = 8,
    ID_SEARCH_PASSPORT = 9,
    ID_SEARCH_FLIGHT = 10
};

BEGIN_EVENT_TABLE(ReservationWindow, wxFrame)
EVT_BUTTON(ID_BACK, ReservationWindow::OnBack)
EVT_BUTTON(ID_SHOW, ReservationWindow::OnShowReservations)
EVT_BUTTON(ID_ADD, ReservationWindow::OnAddReservation)
EVT_BUTTON(ID_EDIT, ReservationWindow::OnEditReservation)
EVT_BUTTON(ID_DELETE, ReservationWindow::OnDeleteReservation)
EVT_BUTTON(ID_SEARCH_ID, ReservationWindow::OnSearchById)
EVT_BUTTON(ID_SEARCH_TICKET, ReservationWindow::OnSearchByTicketNo)
EVT_BUTTON(ID_SEARCH_PASSPORT, ReservationWindow::OnSearchByPassport)
EVT_BUTTON(ID_SEARCH_FLIGHT, ReservationWindow::OnSearchByFlightNo)
EVT_LIST_ITEM_SELECTED(ID_RESERVATION_LIST, ReservationWindow::OnListItemSelected)
END_EVENT_TABLE()

ReservationWindow::ReservationWindow(const wxString &title, std::shared_ptr<ReservationService> reservationService, std::shared_ptr<FlightService> flightService, std::shared_ptr<PassengerService> passengerService, wxWindow *parent)
    : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 600)), reservationService(reservationService), flightService(flightService), passengerService(passengerService)
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
    searchByIdButton = new wxButton(panel, ID_SEARCH_ID, "Tìm theo ID", wxDefaultPosition, wxSize(250, 50));
    searchByTicketNoButton = new wxButton(panel, ID_SEARCH_TICKET, "Tìm theo mã vé", wxDefaultPosition, wxSize(250, 50));
    searchByPassportButton = new wxButton(panel, ID_SEARCH_PASSPORT, "Tìm theo hộ chiếu", wxDefaultPosition, wxSize(250, 50));
    searchByFlightNoButton = new wxButton(panel, ID_SEARCH_FLIGHT, "Tìm theo chuyến bay", wxDefaultPosition, wxSize(250, 50));

    // Create reservation list
    reservationList = new wxListCtrl(panel, ID_RESERVATION_LIST, wxDefaultPosition, wxSize(700, 300),
                                     wxLC_REPORT | wxLC_SINGLE_SEL);
    reservationList->InsertColumn(0, "ID");
    reservationList->InsertColumn(1, "Mã vé");
    reservationList->InsertColumn(2, "Mã chuyến bay");
    reservationList->InsertColumn(3, "Tên hành khách");
    reservationList->InsertColumn(4, "Số hộ chiếu");

    for (int i = 0; i < 5; ++i)
    {
        reservationList->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    }

    // Create info label
    infoLabel = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxSize(600, 50));
    infoLabel->Wrap(600);

    // Add buttons to content sizer (2 hàng ngang)
    wxBoxSizer *row1 = new wxBoxSizer(wxHORIZONTAL);
    row1->Add(showButton, 0, wxALL, 10);
    row1->Add(addButton, 0, wxALL, 10);
    row1->Add(editButton, 0, wxALL, 10);
    row1->Add(deleteButton, 0, wxALL, 10);

    wxBoxSizer *row2 = new wxBoxSizer(wxHORIZONTAL);
    row2->Add(searchByIdButton, 0, wxALL, 10);
    row2->Add(searchByTicketNoButton, 0, wxALL, 10);
    row2->Add(searchByPassportButton, 0, wxALL, 10);
    row2->Add(searchByFlightNoButton, 0, wxALL, 10);

    contentSizer->Add(row1, 0, wxALIGN_CENTER);
    contentSizer->Add(row2, 0, wxALIGN_CENTER);
    contentSizer->Add(reservationList, 1, wxALL | wxEXPAND, 10);
    contentSizer->Add(infoLabel, 0, wxALL | wxALIGN_CENTER, 10);

    // Add sizers to main sizer
    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(contentSizer, 1, wxEXPAND);

    panel->SetSizer(mainSizer);
    Centre();
}

void ReservationWindow::OnBack(wxCommandEvent &event)
{
    if (GetParent())
        GetParent()->Show();
    this->Destroy();
}

void ReservationWindow::OnShowReservations(wxCommandEvent &event)
{
    RefreshReservationList();
}

void ReservationWindow::OnAddReservation(wxCommandEvent &event)
{
    wxDialog dialog(this, wxID_ANY, "Thêm đặt chỗ mới", wxDefaultPosition, wxSize(400, 500));

    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    int labelWidth = 120;

    // Tạo các trường nhập liệu
    wxBoxSizer *flightIdSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *flightIdLabel = new wxStaticText(panel, wxID_ANY, "ID chuyến bay:");
    flightIdLabel->SetMinSize(wxSize(labelWidth, -1));
    flightIdSizer->Add(flightIdLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *flightIdCtrl = new wxTextCtrl(panel, wxID_ANY);
    flightIdSizer->Add(flightIdCtrl, 1, wxALL, 5);
    sizer->Add(flightIdSizer, 0, wxEXPAND);

    wxBoxSizer *passengerIdSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *passengerIdLabel = new wxStaticText(panel, wxID_ANY, "ID hành khách:");
    passengerIdLabel->SetMinSize(wxSize(labelWidth, -1));
    passengerIdSizer->Add(passengerIdLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *passengerIdCtrl = new wxTextCtrl(panel, wxID_ANY);
    passengerIdSizer->Add(passengerIdCtrl, 1, wxALL, 5);
    sizer->Add(passengerIdSizer, 0, wxEXPAND);

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
        if (flightIdCtrl->GetValue().IsEmpty() || 
            passengerIdCtrl->GetValue().IsEmpty()) {
            
            wxMessageBox("Vui lòng điền đầy đủ thông tin bắt buộc!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }
        
        dialog.EndModal(wxID_OK); });

    if (dialog.ShowModal() == wxID_OK)
    {
        int flightId = std::stoi(flightIdCtrl->GetValue().ToStdString());
        int passengerId = std::stoi(passengerIdCtrl->GetValue().ToStdString());

        // Lấy flightNo và Passenger object
        auto flightOpt = flightService->findById(flightId);
        auto passengerOpt = passengerService->findById(passengerId);
        if (flightOpt && passengerOpt)
        {
            auto reservationOpt = reservationService->createReservation(flightOpt->getNo(), *passengerOpt);
            if (reservationOpt)
            {
                infoLabel->SetLabel("Đã thêm đặt chỗ mới!");
                RefreshReservationList();
            }
            else
            {
                infoLabel->SetLabel("Thêm đặt chỗ thất bại!");
            }
        }
        else
        {
            infoLabel->SetLabel("Không tìm thấy chuyến bay hoặc hành khách!");
        }
    }
}

void ReservationWindow::OnEditReservation(wxCommandEvent &event)
{
    long selectedIndex = reservationList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn đặt chỗ cần sửa!", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }
    int reservationId = std::stoi(reservationList->GetItemText(selectedIndex, 0).ToStdString());
    auto reservationOpt = reservationService->findById(reservationId);
    if (!reservationOpt)
    {
        wxMessageBox("Không tìm thấy đặt chỗ!", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    Reservation reservation = *reservationOpt;

    wxDialog dialog(this, wxID_ANY, "Sửa thông tin đặt chỗ", wxDefaultPosition, wxSize(400, 500));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    int labelWidth = 120;

    wxBoxSizer *flightIdSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *flightIdLabel = new wxStaticText(panel, wxID_ANY, "ID chuyến bay:");
    flightIdLabel->SetMinSize(wxSize(labelWidth, -1));
    flightIdSizer->Add(flightIdLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *flightIdCtrl = new wxTextCtrl(panel, wxID_ANY,
                                              wxString::Format("%d", reservation.getFlight() ? reservation.getFlight()->getId() : 0));
    flightIdSizer->Add(flightIdCtrl, 1, wxALL, 5);
    sizer->Add(flightIdSizer, 0, wxEXPAND);

    wxBoxSizer *passengerIdSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *passengerIdLabel = new wxStaticText(panel, wxID_ANY, "ID hành khách:");
    passengerIdLabel->SetMinSize(wxSize(labelWidth, -1));
    passengerIdSizer->Add(passengerIdLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *passengerIdCtrl = new wxTextCtrl(panel, wxID_ANY,
                                                 wxString::Format("%d", reservation.getPassenger() ? reservation.getPassenger()->getId() : 0));
    passengerIdSizer->Add(passengerIdCtrl, 1, wxALL, 5);
    sizer->Add(passengerIdSizer, 0, wxEXPAND);

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
        if (flightIdCtrl->GetValue().IsEmpty() || 
            passengerIdCtrl->GetValue().IsEmpty()) {
            wxMessageBox("Vui lòng điền đầy đủ thông tin bắt buộc!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }
        dialog.EndModal(wxID_OK); });

    if (dialog.ShowModal() == wxID_OK)
    {
        int flightId = std::stoi(flightIdCtrl->GetValue().ToStdString());
        int passengerId = std::stoi(passengerIdCtrl->GetValue().ToStdString());

        // Tạo các đối tượng Flight và Passenger mới
        auto flight = std::make_shared<Flight>();
        flight->setId(flightId);
        auto passenger = std::make_shared<Passenger>();
        passenger->setId(passengerId);

        // Cập nhật thông tin đặt chỗ
        reservation.setFlight(flight);
        reservation.setPassenger(passenger);

        bool ok = reservationService->update(reservation);
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
    int reservationId = std::stoi(reservationList->GetItemText(selectedIndex, 0).ToStdString());
    auto reservationOpt = reservationService->findById(reservationId);
    if (!reservationOpt)
    {
        wxMessageBox("Không tìm thấy đặt chỗ!", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }
    Reservation reservation = *reservationOpt;
    wxString msg = wxString::Format("Bạn có chắc chắn muốn xóa đặt chỗ %s không?", reservation.getTicketNo());
    int confirm = wxMessageBox(msg, "Xác nhận xóa", wxYES_NO | wxICON_QUESTION);
    if (confirm != wxYES)
        return;
    bool ok = reservationService->remove(reservationId);
    if (ok)
        infoLabel->SetLabel("Đã xóa đặt chỗ!");
    else
        infoLabel->SetLabel("Xóa đặt chỗ thất bại!");
    RefreshReservationList();
}

void ReservationWindow::OnListItemSelected(wxListEvent &event)
{
    // Do nothing for now
}

void ReservationWindow::RefreshReservationList()
{
    try
    {
        wxLogMessage("Starting to refresh reservation list...");
        std::vector<Reservation> reservations = reservationService->findAll();
        wxLogMessage("Found %d reservations in database", reservations.size());

        ShowReservations(reservations);
        infoLabel->SetLabel("Danh sách đặt chỗ đã được cập nhật");
        wxLogMessage("Reservation list refresh completed successfully");
    }
    catch (const std::exception &e)
    {
        wxLogError("Error loading reservations: %s", e.what());
        infoLabel->SetLabel("Lỗi khi tải danh sách đặt chỗ!");
    }
}

void ReservationWindow::ShowReservations(const std::vector<Reservation> &reservations)
{
    wxLogMessage("Displaying %d reservations in the list", reservations.size());
    reservationList->DeleteAllItems();
    long index = 0;
    for (const auto &reservation : reservations)
    {
        try
        {
            reservationList->InsertItem(index, std::to_string(reservation.getId()));
            reservationList->SetItem(index, 1, wxString::FromUTF8(reservation.getTicketNo().c_str()));
            reservationList->SetItem(index, 2, wxString::FromUTF8(reservation.getFlight() ? reservation.getFlight()->getNo().c_str() : ""));
            reservationList->SetItem(index, 3, wxString::FromUTF8(reservation.getPassenger() ? reservation.getPassenger()->getName().c_str() : ""));
            reservationList->SetItem(index, 4, wxString::FromUTF8(reservation.getPassenger() ? reservation.getPassenger()->getPassport().c_str() : ""));
            index++;
        }
        catch (const std::exception &e)
        {
            wxLogError("Error displaying reservation at index %d: %s", index, e.what());
        }
    }
    wxLogMessage("Finished displaying reservations in the list");
}

void ReservationWindow::OnSearchById(wxCommandEvent &event)
{
    wxDialog dialog(this, wxID_ANY, "Tìm đặt chỗ theo ID", wxDefaultPosition, wxSize(300, 150));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *idSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *idLabel = new wxStaticText(panel, wxID_ANY, "ID đặt chỗ:");
    idLabel->SetMinSize(wxSize(100, -1));
    idSizer->Add(idLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *idCtrl = new wxTextCtrl(panel, wxID_ANY);
    idSizer->Add(idCtrl, 1, wxALL, 5);
    sizer->Add(idSizer, 0, wxEXPAND);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okButton = new wxButton(panel, wxID_OK, "Tìm kiếm");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Hủy");
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 15);

    panel->SetSizer(sizer);

    if (dialog.ShowModal() == wxID_OK)
    {
        long id;
        if (idCtrl->GetValue().ToLong(&id))
        {
            auto reservationOpt = reservationService->findById(static_cast<int>(id));
            reservationList->DeleteAllItems();
            if (reservationOpt)
            {
                Reservation r = *reservationOpt;
                reservationList->InsertItem(0, std::to_string(r.getId()));
                reservationList->SetItem(0, 1, wxString::FromUTF8(r.getTicketNo().c_str()));
                reservationList->SetItem(0, 2, wxString::FromUTF8(r.getFlight() ? r.getFlight()->getNo().c_str() : ""));
                reservationList->SetItem(0, 3, wxString::FromUTF8(r.getPassenger() ? r.getPassenger()->getName().c_str() : ""));
                reservationList->SetItem(0, 4, wxString::FromUTF8(r.getPassenger() ? r.getPassenger()->getPassport().c_str() : ""));
                infoLabel->SetLabel("Đã tìm thấy đặt chỗ!");
            }
            else
            {
                infoLabel->SetLabel("Không tìm thấy đặt chỗ với ID này!");
            }
        }
        else
        {
            wxMessageBox("Vui lòng nhập ID hợp lệ!", "Lỗi", wxICON_ERROR | wxOK);
        }
    }
}

void ReservationWindow::OnSearchByTicketNo(wxCommandEvent &event)
{
    wxDialog dialog(this, wxID_ANY, "Tìm đặt chỗ theo mã vé", wxDefaultPosition, wxSize(350, 150));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *ticketSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *ticketLabel = new wxStaticText(panel, wxID_ANY, "Mã vé:");
    ticketLabel->SetMinSize(wxSize(120, -1));
    ticketSizer->Add(ticketLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *ticketCtrl = new wxTextCtrl(panel, wxID_ANY);
    ticketSizer->Add(ticketCtrl, 1, wxALL, 5);
    sizer->Add(ticketSizer, 0, wxEXPAND);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okButton = new wxButton(panel, wxID_OK, "Tìm kiếm");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Hủy");
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 15);

    panel->SetSizer(sizer);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString ticket = ticketCtrl->GetValue();
        if (ticket.IsEmpty())
        {
            wxMessageBox("Vui lòng nhập mã vé!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }
        auto reservationOpt = reservationService->findByTicketNo(ticket.ToStdString());
        reservationList->DeleteAllItems();
        if (reservationOpt)
        {
            Reservation r = *reservationOpt;
            reservationList->InsertItem(0, std::to_string(r.getId()));
            reservationList->SetItem(0, 1, wxString::FromUTF8(r.getTicketNo().c_str()));
            reservationList->SetItem(0, 2, wxString::FromUTF8(r.getFlight() ? r.getFlight()->getNo().c_str() : ""));
            reservationList->SetItem(0, 3, wxString::FromUTF8(r.getPassenger() ? r.getPassenger()->getName().c_str() : ""));
            reservationList->SetItem(0, 4, wxString::FromUTF8(r.getPassenger() ? r.getPassenger()->getPassport().c_str() : ""));
            infoLabel->SetLabel("Đã tìm thấy đặt chỗ!");
        }
        else
        {
            infoLabel->SetLabel("Không tìm thấy đặt chỗ với mã vé này!");
        }
    }
}

void ReservationWindow::OnSearchByPassport(wxCommandEvent &event)
{
    wxDialog dialog(this, wxID_ANY, "Tìm đặt chỗ theo hộ chiếu", wxDefaultPosition, wxSize(350, 150));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *passportSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *passportLabel = new wxStaticText(panel, wxID_ANY, "Số hộ chiếu:");
    passportLabel->SetMinSize(wxSize(120, -1));
    passportSizer->Add(passportLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *passportCtrl = new wxTextCtrl(panel, wxID_ANY);
    passportSizer->Add(passportCtrl, 1, wxALL, 5);
    sizer->Add(passportSizer, 0, wxEXPAND);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okButton = new wxButton(panel, wxID_OK, "Tìm kiếm");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Hủy");
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 15);

    panel->SetSizer(sizer);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString passport = passportCtrl->GetValue();
        if (passport.IsEmpty())
        {
            wxMessageBox("Vui lòng nhập số hộ chiếu!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }
        std::vector<Reservation> results = reservationService->findByPassenger(passport.ToStdString());
        reservationList->DeleteAllItems();
        long index = 0;
        for (const auto &r : results)
        {
            reservationList->InsertItem(index, std::to_string(r.getId()));
            reservationList->SetItem(index, 1, wxString::FromUTF8(r.getTicketNo().c_str()));
            reservationList->SetItem(index, 2, wxString::FromUTF8(r.getFlight() ? r.getFlight()->getNo().c_str() : ""));
            reservationList->SetItem(index, 3, wxString::FromUTF8(r.getPassenger() ? r.getPassenger()->getName().c_str() : ""));
            reservationList->SetItem(index, 4, wxString::FromUTF8(r.getPassenger() ? r.getPassenger()->getPassport().c_str() : ""));
            index++;
        }
        if (results.empty())
            infoLabel->SetLabel("Không tìm thấy đặt chỗ nào!");
        else
            infoLabel->SetLabel(wxString::Format("Đã tìm thấy %d đặt chỗ!", (int)results.size()));
    }
}

void ReservationWindow::OnSearchByFlightNo(wxCommandEvent &event)
{
    wxDialog dialog(this, wxID_ANY, "Tìm đặt chỗ theo chuyến bay", wxDefaultPosition, wxSize(350, 150));
    wxPanel *panel = new wxPanel(&dialog, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *flightSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *flightLabel = new wxStaticText(panel, wxID_ANY, "Mã chuyến bay:");
    flightLabel->SetMinSize(wxSize(120, -1));
    flightSizer->Add(flightLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *flightCtrl = new wxTextCtrl(panel, wxID_ANY);
    flightSizer->Add(flightCtrl, 1, wxALL, 5);
    sizer->Add(flightSizer, 0, wxEXPAND);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okButton = new wxButton(panel, wxID_OK, "Tìm kiếm");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Hủy");
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP, 15);

    panel->SetSizer(sizer);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString flightNo = flightCtrl->GetValue();
        if (flightNo.IsEmpty())
        {
            wxMessageBox("Vui lòng nhập mã chuyến bay!", "Lỗi", wxICON_ERROR | wxOK);
            return;
        }
        std::vector<Reservation> results = reservationService->findByFlight(flightNo.ToStdString());
        reservationList->DeleteAllItems();
        long index = 0;
        for (const auto &r : results)
        {
            reservationList->InsertItem(index, std::to_string(r.getId()));
            reservationList->SetItem(index, 1, wxString::FromUTF8(r.getTicketNo().c_str()));
            reservationList->SetItem(index, 2, wxString::FromUTF8(r.getFlight() ? r.getFlight()->getNo().c_str() : ""));
            reservationList->SetItem(index, 3, wxString::FromUTF8(r.getPassenger() ? r.getPassenger()->getName().c_str() : ""));
            reservationList->SetItem(index, 4, wxString::FromUTF8(r.getPassenger() ? r.getPassenger()->getPassport().c_str() : ""));
            index++;
        }
        if (results.empty())
            infoLabel->SetLabel("Không tìm thấy đặt chỗ nào!");
        else
            infoLabel->SetLabel(wxString::Format("Đã tìm thấy %d đặt chỗ!", (int)results.size()));
    }
}