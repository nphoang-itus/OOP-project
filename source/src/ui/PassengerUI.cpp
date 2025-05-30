#include "PassengerUI.h"
#include "services/PassengerService.h"
#include "services/AircraftService.h"
#include "services/FlightService.h"
#include "services/TicketService.h"
#include "core/entities/Passenger.h"
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

wxBEGIN_EVENT_TABLE(PassengerWindow, wxFrame)
    EVT_BUTTON(1001, PassengerWindow::OnBack)
        EVT_BUTTON(1002, PassengerWindow::OnShowPassenger)
            EVT_BUTTON(1003, PassengerWindow::OnAddPassenger)
                EVT_BUTTON(1004, PassengerWindow::OnEditPassenger)
                    EVT_BUTTON(1005, PassengerWindow::OnDeletePassenger)
                        EVT_BUTTON(1006, PassengerWindow::OnSearchById)
                            EVT_BUTTON(1007, PassengerWindow::OnSearchByPassport)
                                EVT_BUTTON(1008, PassengerWindow::OnCheckBookings)
                                    EVT_BUTTON(1009, PassengerWindow::OnViewStats)
                                        EVT_LIST_ITEM_SELECTED(wxID_ANY, PassengerWindow::OnListItemSelected)
                                            wxEND_EVENT_TABLE()

                                                PassengerWindow::PassengerWindow(const wxString &title, std::shared_ptr<PassengerService> passengerService)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1300, 700)), passengerService(passengerService)
{
    CreateUI();
    RefreshPassengerList();
}

void PassengerWindow::setServices(std::shared_ptr<AircraftService> aircraftService,
                                  std::shared_ptr<FlightService> flightService,
                                  std::shared_ptr<PassengerService> passengerService,
                                  std::shared_ptr<TicketService> ticketService)
{
    this->aircraftService = aircraftService;
    this->flightService = flightService;
    this->passengerService = passengerService;
    this->ticketService = ticketService;
}

void PassengerWindow::CreateUI()
{
    panel = new wxPanel(this);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);

    // Create back button
    backButton = new wxButton(panel, 1001, wxT("Quay lại"), wxDefaultPosition, wxSize(100, 30));
    buttonSizer->Add(backButton, 0, wxALL, 10);

    // Create main function buttons
    showButton = new wxButton(panel, 1002, wxT("Hiển thị"), wxDefaultPosition, wxSize(200, 50));
    addButton = new wxButton(panel, 1003, wxT("Thêm"), wxDefaultPosition, wxSize(200, 50));
    editButton = new wxButton(panel, 1004, wxT("Sửa"), wxDefaultPosition, wxSize(200, 50));
    deleteButton = new wxButton(panel, 1005, wxT("Xóa"), wxDefaultPosition, wxSize(200, 50));
    searchByIdButton = new wxButton(panel, 1006, wxT("Tìm theo ID"), wxDefaultPosition, wxSize(200, 50));
    searchByPassportButton = new wxButton(panel, 1007, wxT("Tìm theo hộ chiếu"), wxDefaultPosition, wxSize(200, 50));
    checkBookingsButton = new wxButton(panel, 1008, wxT("Kiểm tra đặt chỗ"), wxDefaultPosition, wxSize(200, 50));
    viewStatsButton = new wxButton(panel, 1009, wxT("Thống kê"), wxDefaultPosition, wxSize(200, 50));

    // Create passenger list
    passengerList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(1200, 350), wxLC_REPORT | wxLC_SINGLE_SEL);
    passengerList->AppendColumn(wxT("ID"), wxLIST_FORMAT_LEFT, 80);
    passengerList->AppendColumn(wxT("Họ tên"), wxLIST_FORMAT_LEFT, 220);
    passengerList->AppendColumn(wxT("Hộ chiếu"), wxLIST_FORMAT_LEFT, 160);
    passengerList->AppendColumn(wxT("Email"), wxLIST_FORMAT_LEFT, 220);
    passengerList->AppendColumn(wxT("Điện thoại"), wxLIST_FORMAT_LEFT, 160);
    passengerList->AppendColumn(wxT("Địa chỉ"), wxLIST_FORMAT_LEFT, 350);

    // Add buttons to content sizer (3 rows)
    wxBoxSizer *row1 = new wxBoxSizer(wxHORIZONTAL);
    row1->Add(showButton, 0, wxALL, 10);
    row1->Add(addButton, 0, wxALL, 10);
    row1->Add(editButton, 0, wxALL, 10);
    row1->Add(deleteButton, 0, wxALL, 10);

    wxBoxSizer *row2 = new wxBoxSizer(wxHORIZONTAL);
    row2->Add(searchByIdButton, 0, wxALL, 10);
    row2->Add(searchByPassportButton, 0, wxALL, 10);

    wxBoxSizer *row3 = new wxBoxSizer(wxHORIZONTAL);
    row3->Add(checkBookingsButton, 0, wxALL, 10);
    row3->Add(viewStatsButton, 0, wxALL, 10);

    contentSizer->Add(row1, 0, wxALIGN_CENTER);
    contentSizer->Add(row2, 0, wxALIGN_CENTER);
    contentSizer->Add(row3, 0, wxALIGN_CENTER);
    contentSizer->Add(passengerList, 1, wxALL | wxEXPAND, 10);

    // Add sizers to main sizer
    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(contentSizer, 1, wxEXPAND);

    panel->SetSizer(mainSizer);
    Centre();
}

void PassengerWindow::RefreshPassengerList()
{
    if (!passengerService)
        return;

    passengerList->DeleteAllItems();

    auto passengersResult = passengerService->getAllPassengers();
    if (!passengersResult)
    {
        wxMessageBox(wxString::Format(wxT("Lỗi tải danh sách hành khách: %s"),
                                      passengersResult.error().message.c_str()),
                     wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    const auto &passengers = *passengersResult;
    for (size_t i = 0; i < passengers.size(); ++i)
    {
        const auto &passenger = passengers[i];
        long index = passengerList->InsertItem(i, wxString::Format(wxT("%d"), passenger.getId()));
        passengerList->SetItem(index, 1, wxString(passenger.getName().c_str(), wxConvUTF8));
        passengerList->SetItem(index, 2, wxString(passenger.getPassport().toString().c_str(), wxConvUTF8));
        passengerList->SetItem(index, 3, wxString(passenger.getContactInfo().getEmail().c_str(), wxConvUTF8));
        passengerList->SetItem(index, 4, wxString(passenger.getContactInfo().getPhone().c_str(), wxConvUTF8));
        passengerList->SetItem(index, 5, wxString(passenger.getContactInfo().getAddress().c_str(), wxConvUTF8));
    }
}

void PassengerWindow::OnBack(wxCommandEvent &event)
{
    if (event.GetId() == 1001)
    {
        // Create and show main window
        auto mainWindow = new MainWindow(wxT("Hệ thống quản lý hàng không"), aircraftService, flightService, passengerService, ticketService);
        mainWindow->Show();

        // Close this window
        Close();
    }
}

void PassengerWindow::OnShowPassenger(wxCommandEvent &event)
{
    if (event.GetId() == 1002)
    {
        RefreshPassengerList();
    }
}

void PassengerWindow::OnAddPassenger(wxCommandEvent &event)
{
    if (event.GetId() != 1003)
        return;

    // Create a custom dialog with all fields
    wxDialog dialog(this, wxID_ANY, wxT("Thêm hành khách"), wxDefaultPosition, wxSize(450, 400));
    dialog.Centre();

    wxPanel *panel = new wxPanel(&dialog);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add title
    wxStaticText *titleText = new wxStaticText(panel, wxID_ANY, wxT("THÔNG TIN HÀNH KHÁCH"),
                                               wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(12);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL | wxALIGN_CENTER, 15);

    // Create form sizer with proper alignment
    wxFlexGridSizer *formSizer = new wxFlexGridSizer(5, 2, 10, 10);
    formSizer->AddGrowableCol(1, 1);

    // Name field
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Họ tên:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *nameCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
                                          wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(nameCtrl, 1, wxEXPAND, 0);

    // Email field
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Email:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *emailCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
                                           wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(emailCtrl, 1, wxEXPAND, 0);

    // Phone field
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Điện thoại:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *phoneCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
                                           wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(phoneCtrl, 1, wxEXPAND, 0);

    // Address field
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Địa chỉ:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *addressCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
                                             wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(addressCtrl, 1, wxEXPAND, 0);

    // Passport field
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Hộ chiếu:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *passportCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString,
                                              wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(passportCtrl, 1, wxEXPAND, 0);

    mainSizer->Add(formSizer, 0, wxALL | wxEXPAND, 20);

    // Add some space instead of separator line
    mainSizer->Add(0, 10, 0, wxEXPAND);

    // Buttons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okButton = new wxButton(panel, wxID_OK, wxT("Thêm"), wxDefaultPosition, wxSize(100, 35));
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Hủy"), wxDefaultPosition, wxSize(100, 35));

    buttonSizer->AddStretchSpacer();
    buttonSizer->Add(okButton, 0, wxRIGHT, 10);
    buttonSizer->Add(cancelButton, 0, 0, 0);
    buttonSizer->AddStretchSpacer();

    mainSizer->Add(buttonSizer, 0, wxALL | wxEXPAND, 20);

    panel->SetSizer(mainSizer);

    if (dialog.ShowModal() != wxID_OK)
        return;

    // Validate input
    if (nameCtrl->GetValue().IsEmpty() || emailCtrl->GetValue().IsEmpty() ||
        phoneCtrl->GetValue().IsEmpty() || passportCtrl->GetValue().IsEmpty())
    {
        wxMessageBox(wxT("Vui lòng điền đầy đủ thông tin!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    // Create contact info string in format: email|phone|address
    std::string contactInfo = emailCtrl->GetValue().ToStdString() + "|" +
                              phoneCtrl->GetValue().ToStdString() + "|" +
                              addressCtrl->GetValue().ToStdString();

    auto passengerResult = Passenger::create(
        nameCtrl->GetValue().ToStdString(),
        contactInfo,
        passportCtrl->GetValue().ToStdString());

    if (!passengerResult)
    {
        wxMessageBox(wxString::Format(wxT("Lỗi tạo hành khách: %s"),
                                      passengerResult.error().message.c_str()),
                     wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    auto createResult = passengerService->createPassenger(*passengerResult);
    if (!createResult)
    {
        wxMessageBox(wxString::Format(wxT("Lỗi thêm hành khách: %s"),
                                      createResult.error().message.c_str()),
                     wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    wxMessageBox(wxT("Thêm hành khách thành công!"), wxT("Thành công"), wxOK | wxICON_INFORMATION);
    RefreshPassengerList();
}

void PassengerWindow::OnEditPassenger(wxCommandEvent &event)
{
    if (event.GetId() != 1004)
        return;

    long selectedItem = passengerList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedItem == -1)
    {
        wxMessageBox(wxT("Vui lòng chọn hành khách để sửa!"), wxT("Thông báo"), wxOK | wxICON_WARNING);
        return;
    }

    wxString passportStr = passengerList->GetItemText(selectedItem, 2);

    // Get current passenger using getPassenger (not getPassengerByPassport)
    auto passportResult = PassportNumber::create(passportStr.ToStdString());
    if (!passportResult)
    {
        wxMessageBox(wxT("Lỗi passport number!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    auto passengerResult = passengerService->getPassenger(*passportResult);
    if (!passengerResult)
    {
        wxMessageBox(wxT("Không tìm thấy hành khách!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    const auto &currentPassenger = *passengerResult;

    // Create a custom dialog with all fields pre-filled
    wxDialog dialog(this, wxID_ANY, wxT("Sửa hành khách"), wxDefaultPosition, wxSize(450, 400));
    dialog.Centre();

    wxPanel *panel = new wxPanel(&dialog);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add title
    wxStaticText *titleText = new wxStaticText(panel, wxID_ANY, wxT("CHỈNH SỬA THÔNG TIN HÀNH KHÁCH"),
                                               wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(12);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL | wxALIGN_CENTER, 15);

    // Create form sizer with proper alignment
    wxFlexGridSizer *formSizer = new wxFlexGridSizer(5, 2, 10, 10);
    formSizer->AddGrowableCol(1, 1);

    // Name field
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Họ tên:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *nameCtrl = new wxTextCtrl(panel, wxID_ANY,
                                          wxString(currentPassenger.getName().c_str(), wxConvUTF8),
                                          wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(nameCtrl, 1, wxEXPAND, 0);

    // Email field
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Email:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *emailCtrl = new wxTextCtrl(panel, wxID_ANY,
                                           wxString(currentPassenger.getContactInfo().getEmail().c_str(), wxConvUTF8),
                                           wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(emailCtrl, 1, wxEXPAND, 0);

    // Phone field
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Điện thoại:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *phoneCtrl = new wxTextCtrl(panel, wxID_ANY,
                                           wxString(currentPassenger.getContactInfo().getPhone().c_str(), wxConvUTF8),
                                           wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(phoneCtrl, 1, wxEXPAND, 0);

    // Address field
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Địa chỉ:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *addressCtrl = new wxTextCtrl(panel, wxID_ANY,
                                             wxString(currentPassenger.getContactInfo().getAddress().c_str(), wxConvUTF8),
                                             wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(addressCtrl, 1, wxEXPAND, 0);

    // Passport field (editable for editing)
    formSizer->Add(new wxStaticText(panel, wxID_ANY, wxT("Hộ chiếu:")),
                   0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 0);
    wxTextCtrl *passportCtrl = new wxTextCtrl(panel, wxID_ANY,
                                              wxString(currentPassenger.getPassport().toString().c_str(), wxConvUTF8),
                                              wxDefaultPosition, wxSize(250, 25));
    formSizer->Add(passportCtrl, 1, wxEXPAND, 0);

    mainSizer->Add(formSizer, 0, wxALL | wxEXPAND, 20);

    // Add some space instead of separator line
    mainSizer->Add(0, 10, 0, wxEXPAND);

    // Buttons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okButton = new wxButton(panel, wxID_OK, wxT("Cập nhật"), wxDefaultPosition, wxSize(100, 35));
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Hủy"), wxDefaultPosition, wxSize(100, 35));

    buttonSizer->AddStretchSpacer();
    buttonSizer->Add(okButton, 0, wxRIGHT, 10);
    buttonSizer->Add(cancelButton, 0, 0, 0);
    buttonSizer->AddStretchSpacer();

    mainSizer->Add(buttonSizer, 0, wxALL | wxEXPAND, 20);

    panel->SetSizer(mainSizer);

    if (dialog.ShowModal() != wxID_OK)
        return;

    // Validate input
    if (nameCtrl->GetValue().IsEmpty() || emailCtrl->GetValue().IsEmpty() ||
        phoneCtrl->GetValue().IsEmpty() || passportCtrl->GetValue().IsEmpty())
    {
        wxMessageBox(wxT("Vui lòng điền đầy đủ thông tin!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    // Create contact info string in format: email|phone|address
    std::string contactInfo = emailCtrl->GetValue().ToStdString() + "|" +
                              phoneCtrl->GetValue().ToStdString() + "|" +
                              addressCtrl->GetValue().ToStdString();

    auto updatedPassengerResult = Passenger::create(
        nameCtrl->GetValue().ToStdString(),
        contactInfo,
        passportCtrl->GetValue().ToStdString());

    if (!updatedPassengerResult)
    {
        wxMessageBox(wxString::Format(wxT("Lỗi tạo hành khách: %s"),
                                      updatedPassengerResult.error().message.c_str()),
                     wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    // Preserve the original passenger ID for update
    updatedPassengerResult->setId(currentPassenger.getId());

    auto updateResult = passengerService->updatePassenger(*updatedPassengerResult);
    if (!updateResult)
    {
        wxMessageBox(wxString::Format(wxT("Lỗi cập nhật hành khách: %s"),
                                      updateResult.error().message.c_str()),
                     wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    wxMessageBox(wxT("Cập nhật hành khách thành công!"), wxT("Thành công"), wxOK | wxICON_INFORMATION);
    RefreshPassengerList();
}

void PassengerWindow::OnDeletePassenger(wxCommandEvent &event)
{
    if (event.GetId() != 1005)
        return;

    long selectedItem = passengerList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedItem == -1)
    {
        wxMessageBox(wxT("Vui lòng chọn hành khách để xóa!"), wxT("Thông báo"), wxOK | wxICON_WARNING);
        return;
    }

    wxString name = passengerList->GetItemText(selectedItem, 1);
    wxString passportStr = passengerList->GetItemText(selectedItem, 2);

    int result = wxMessageBox(
        wxString::Format(wxT("Bạn có chắc chắn muốn xóa hành khách '%s'?"), name),
        wxT("Xác nhận xóa"),
        wxYES_NO | wxICON_QUESTION);

    if (result == wxYES)
    {
        auto passportResult = PassportNumber::create(passportStr.ToStdString());
        if (!passportResult)
        {
            wxMessageBox(wxT("Lỗi passport number!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
            return;
        }

        // Check if passenger can be deleted
        auto canDeleteResult = passengerService->canDeletePassenger(*passportResult);
        if (!canDeleteResult)
        {
            wxMessageBox(wxString::Format(wxT("Lỗi kiểm tra khả năng xóa: %s"),
                                          canDeleteResult.error().message.c_str()),
                         wxT("Lỗi"), wxOK | wxICON_ERROR);
            return;
        }

        if (!*canDeleteResult)
        {
            wxMessageBox(wxT("Không thể xóa hành khách này vì còn có đặt chỗ đang hoạt động!"),
                         wxT("Không thể xóa"), wxOK | wxICON_WARNING);
            return;
        }

        auto deleteResult = passengerService->deletePassenger(*passportResult);
        if (!deleteResult)
        {
            wxMessageBox(wxString::Format(wxT("Lỗi xóa hành khách: %s"),
                                          deleteResult.error().message.c_str()),
                         wxT("Lỗi"), wxOK | wxICON_ERROR);
            return;
        }

        wxMessageBox(wxT("Xóa hành khách thành công!"), wxT("Thành công"), wxOK | wxICON_INFORMATION);
        RefreshPassengerList();
    }
}

void PassengerWindow::OnSearchById(wxCommandEvent &event)
{
    if (event.GetId() != 1006)
        return;

    // Since getPassengerById is private, we'll search through all passengers
    wxTextEntryDialog idDialog(this, wxT("Nhập ID hành khách:"), wxT("Tìm kiếm"));
    if (idDialog.ShowModal() != wxID_OK)
        return;

    long searchId;
    if (!idDialog.GetValue().ToLong(&searchId))
    {
        wxMessageBox(wxT("ID không hợp lệ!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    auto passengersResult = passengerService->getAllPassengers();
    if (!passengersResult)
    {
        wxMessageBox(wxT("Lỗi tải danh sách hành khách!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    // Search for passenger with matching ID
    const auto &passengers = *passengersResult;
    bool found = false;

    passengerList->DeleteAllItems();

    for (const auto &passenger : passengers)
    {
        if (passenger.getId() == searchId)
        {
            long index = passengerList->InsertItem(0, wxString::Format(wxT("%d"), passenger.getId()));
            passengerList->SetItem(index, 1, wxString(passenger.getName().c_str(), wxConvUTF8));
            passengerList->SetItem(index, 2, wxString(passenger.getPassport().toString().c_str(), wxConvUTF8));
            passengerList->SetItem(index, 3, wxString(passenger.getContactInfo().getEmail().c_str(), wxConvUTF8));
            passengerList->SetItem(index, 4, wxString(passenger.getContactInfo().getPhone().c_str(), wxConvUTF8));
            passengerList->SetItem(index, 5, wxString(passenger.getContactInfo().getAddress().c_str(), wxConvUTF8));
            found = true;
            break;
        }
    }

    if (found)
    {
        wxMessageBox(wxT("Tìm thấy hành khách có ID: "), wxT("Thông báo"), wxOK | wxICON_INFORMATION);
    }
    else
    {
        wxMessageBox(wxT("Không tìm thấy hành khách!"), wxT("Thông báo"), wxOK | wxICON_INFORMATION);
    }
}

void PassengerWindow::OnSearchByPassport(wxCommandEvent &event)
{
    if (event.GetId() != 1007)
        return;

    wxTextEntryDialog passportDialog(this, wxT("Nhập số hộ chiếu:"), wxT("Tìm kiếm"));
    if (passportDialog.ShowModal() != wxID_OK)
        return;

    auto passportResult = PassportNumber::create(passportDialog.GetValue().ToStdString());
    if (!passportResult)
    {
        wxMessageBox(wxT("Số hộ chiếu không hợp lệ!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    auto passengerResult = passengerService->getPassenger(*passportResult);
    if (!passengerResult)
    {
        wxMessageBox(wxT("Không tìm thấy hành khách!"), wxT("Thông báo"), wxOK | wxICON_INFORMATION);
        return;
    }

    // Clear list and show only found passenger
    passengerList->DeleteAllItems();
    const auto &passenger = *passengerResult;

    long index = passengerList->InsertItem(0, wxString::Format(wxT("%d"), passenger.getId()));
    passengerList->SetItem(index, 1, wxString(passenger.getName().c_str(), wxConvUTF8));
    passengerList->SetItem(index, 2, wxString(passenger.getPassport().toString().c_str(), wxConvUTF8));
    passengerList->SetItem(index, 3, wxString(passenger.getContactInfo().getEmail().c_str(), wxConvUTF8));
    passengerList->SetItem(index, 4, wxString(passenger.getContactInfo().getPhone().c_str(), wxConvUTF8));
    passengerList->SetItem(index, 5, wxString(passenger.getContactInfo().getAddress().c_str(), wxConvUTF8));

    wxMessageBox(wxT("Tìm thấy hành khách: "), wxT("Thông báo"), wxOK | wxICON_INFORMATION);
}

void PassengerWindow::OnListItemSelected(wxListEvent &event)
{
    // Handle list item selection if needed
    event.Skip();
}

void PassengerWindow::OnCheckBookings(wxCommandEvent &event)
{
    if (event.GetId() != 1008)
        return;

    long selectedItem = passengerList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedItem == -1)
    {
        wxMessageBox(wxT("Vui lòng chọn hành khách để kiểm tra đặt chỗ!"), wxT("Thông báo"), wxOK | wxICON_WARNING);
        return;
    }

    wxString passportStr = passengerList->GetItemText(selectedItem, 2);
    wxString name = passengerList->GetItemText(selectedItem, 1);

    auto passportResult = PassportNumber::create(passportStr.ToStdString());
    if (!passportResult)
    {
        wxMessageBox(wxT("Lỗi passport number!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    // Check if passenger has active bookings
    auto hasBookingsResult = passengerService->hasActiveBookings(*passportResult);
    if (!hasBookingsResult)
    {
        wxMessageBox(wxString::Format(wxT("Lỗi kiểm tra đặt chỗ: %s"),
                                      hasBookingsResult.error().message.c_str()),
                     wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    bool hasActiveBookings = *hasBookingsResult;

    // Get total flight count
    auto flightCountResult = passengerService->getTotalFlightCount(*passportResult);
    int totalFlights = 0;
    if (flightCountResult)
    {
        totalFlights = *flightCountResult;
    }

    wxString message = wxString::Format(
        wxT("Thông tin đặt chỗ của hành khách: %s\n\n")
            wxT("Trạng thái đặt chỗ hiện tại: %s\n")
                wxT("Tổng số chuyến bay đã đặt: %d"),
        name,
        hasActiveBookings ? wxT("Có đặt chỗ đang hoạt động") : wxT("Không có đặt chỗ đang hoạt động"),
        totalFlights);

    wxMessageBox(message, wxT("Thông tin đặt chỗ"), wxOK | wxICON_INFORMATION);
}

void PassengerWindow::OnViewStats(wxCommandEvent &event)
{
    if (event.GetId() != 1009)
        return;

    auto passengersResult = passengerService->getAllPassengers();
    if (!passengersResult)
    {
        wxMessageBox(wxT("Lỗi tải danh sách hành khách!"), wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

    const auto &passengers = *passengersResult;
    int totalPassengers = passengers.size();
    int passengersWithBookings = 0;
    int totalFlights = 0;

    // Calculate statistics
    for (const auto &passenger : passengers)
    {
        auto hasBookingsResult = passengerService->hasActiveBookings(passenger.getPassport());
        if (hasBookingsResult && *hasBookingsResult)
        {
            passengersWithBookings++;
        }

        auto flightCountResult = passengerService->getTotalFlightCount(passenger.getPassport());
        if (flightCountResult)
        {
            totalFlights += *flightCountResult;
        }
    }

    double averageFlightsPerPassenger = totalPassengers > 0 ? (double)totalFlights / totalPassengers : 0.0;
    double activeBookingPercentage = totalPassengers > 0 ? (double)passengersWithBookings * 100.0 / totalPassengers : 0.0;

    wxString statsMessage = wxString::Format(
        wxT("THỐNG KÊ HÀNH KHÁCH\n\n")
            wxT("Tổng số hành khách: %d\n")
                wxT("Hành khách có đặt chỗ đang hoạt động: %d (%.1f%%)\n")
                    wxT("Tổng số chuyến bay đã đặt: %d\n")
                        wxT("Trung bình chuyến bay/hành khách: %.1f"),
        totalPassengers,
        passengersWithBookings,
        activeBookingPercentage,
        totalFlights,
        averageFlightsPerPassenger);

    wxMessageBox(statsMessage, wxT("Thống kê hành khách"), wxOK | wxICON_INFORMATION);
}