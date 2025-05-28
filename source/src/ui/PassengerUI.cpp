#include "PassengerUI.h"
#include "services/PassengerService.h"
#include "services/AircraftService.h"
#include "services/FlightService.h"
#include "core/entities/Passenger.h"
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

wxBEGIN_EVENT_TABLE(PassengerWindow, wxFrame)
    EVT_BUTTON(wxID_ANY, PassengerWindow::OnBack)
        EVT_BUTTON(wxID_ANY, PassengerWindow::OnShowPassenger)
            EVT_BUTTON(wxID_ANY, PassengerWindow::OnAddPassenger)
                EVT_BUTTON(wxID_ANY, PassengerWindow::OnEditPassenger)
                    EVT_BUTTON(wxID_ANY, PassengerWindow::OnDeletePassenger)
                        EVT_BUTTON(wxID_ANY, PassengerWindow::OnSearchById)
                            EVT_BUTTON(wxID_ANY, PassengerWindow::OnSearchByPassport)
                                EVT_LIST_ITEM_SELECTED(wxID_ANY, PassengerWindow::OnListItemSelected)
                                    wxEND_EVENT_TABLE()

                                        PassengerWindow::PassengerWindow(const wxString &title, std::shared_ptr<PassengerService> passengerService)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)), passengerService(passengerService)
{
    CreateUI();
    RefreshPassengerList();
}

void PassengerWindow::setServices(std::shared_ptr<AircraftService> aircraftService,
                                  std::shared_ptr<FlightService> flightService,
                                  std::shared_ptr<PassengerService> passengerService)
{
    this->aircraftService = aircraftService;
    this->flightService = flightService;
    this->passengerService = passengerService;
}

void PassengerWindow::CreateUI()
{
    panel = new wxPanel(this);

    // Main sizer
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // Title
    wxStaticText *titleText = new wxStaticText(panel, wxID_ANY, wxT("Quản lý Hành khách"));
    wxFont titleFont = titleText->GetFont();
    titleFont.SetPointSize(16);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleText->SetFont(titleFont);
    mainSizer->Add(titleText, 0, wxALL | wxCENTER, 10);

    // Info label
    infoLabel = new wxStaticText(panel, wxID_ANY, wxT("Danh sách hành khách"));
    mainSizer->Add(infoLabel, 0, wxALL | wxEXPAND, 5);

    // Passenger list
    passengerList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    passengerList->AppendColumn(wxT("ID"), wxLIST_FORMAT_LEFT, 80);
    passengerList->AppendColumn(wxT("Họ tên"), wxLIST_FORMAT_LEFT, 200);
    passengerList->AppendColumn(wxT("Liên lạc"), wxLIST_FORMAT_LEFT, 250);
    passengerList->AppendColumn(wxT("Hộ chiếu"), wxLIST_FORMAT_LEFT, 150);
    mainSizer->Add(passengerList, 1, wxALL | wxEXPAND, 10);

    // Buttons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    backButton = new wxButton(panel, 1001, wxT("Quay lại"));
    showButton = new wxButton(panel, 1002, wxT("Hiển thị"));
    addButton = new wxButton(panel, 1003, wxT("Thêm"));
    editButton = new wxButton(panel, 1004, wxT("Sửa"));
    deleteButton = new wxButton(panel, 1005, wxT("Xóa"));
    searchByIdButton = new wxButton(panel, 1006, wxT("Tìm theo ID"));
    searchByPassportButton = new wxButton(panel, 1007, wxT("Tìm theo hộ chiếu"));

    buttonSizer->Add(backButton, 0, wxALL, 5);
    buttonSizer->Add(showButton, 0, wxALL, 5);
    buttonSizer->Add(addButton, 0, wxALL, 5);
    buttonSizer->Add(editButton, 0, wxALL, 5);
    buttonSizer->Add(deleteButton, 0, wxALL, 5);
    buttonSizer->Add(searchByIdButton, 0, wxALL, 5);
    buttonSizer->Add(searchByPassportButton, 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALL | wxCENTER, 10);

    panel->SetSizer(mainSizer);
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
        passengerList->SetItem(index, 2, wxString(passenger.getContactInfo().toString().c_str(), wxConvUTF8));
        passengerList->SetItem(index, 3, wxString(passenger.getPassport().toString().c_str(), wxConvUTF8));
    }
}

void PassengerWindow::OnBack(wxCommandEvent &event)
{
    if (event.GetId() == 1001)
    {
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

    wxTextEntryDialog nameDialog(this, wxT("Nhập họ tên:"), wxT("Thêm hành khách"));
    if (nameDialog.ShowModal() != wxID_OK)
        return;

    wxTextEntryDialog contactDialog(this, wxT("Nhập thông tin liên lạc (email, phone, address):"), wxT("Thêm hành khách"));
    if (contactDialog.ShowModal() != wxID_OK)
        return;

    wxTextEntryDialog passportDialog(this, wxT("Nhập số hộ chiếu:"), wxT("Thêm hành khách"));
    if (passportDialog.ShowModal() != wxID_OK)
        return;

    auto passengerResult = Passenger::create(
        nameDialog.GetValue().ToStdString(),
        contactDialog.GetValue().ToStdString(),
        passportDialog.GetValue().ToStdString());

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

    wxString passportStr = passengerList->GetItemText(selectedItem, 3);

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

    // Edit dialogs with current values
    wxTextEntryDialog nameDialog(this, wxT("Sửa họ tên:"), wxT("Sửa hành khách"),
                                 wxString(currentPassenger.getName().c_str(), wxConvUTF8));
    if (nameDialog.ShowModal() != wxID_OK)
        return;

    wxTextEntryDialog contactDialog(this, wxT("Sửa thông tin liên lạc:"), wxT("Sửa hành khách"),
                                    wxString(currentPassenger.getContactInfo().toString().c_str(), wxConvUTF8));
    if (contactDialog.ShowModal() != wxID_OK)
        return;

    auto updatedPassengerResult = Passenger::create(
        nameDialog.GetValue().ToStdString(),
        contactDialog.GetValue().ToStdString(),
        currentPassenger.getPassport().toString());

    if (!updatedPassengerResult)
    {
        wxMessageBox(wxString::Format(wxT("Lỗi tạo hành khách: %s"),
                                      updatedPassengerResult.error().message.c_str()),
                     wxT("Lỗi"), wxOK | wxICON_ERROR);
        return;
    }

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
    wxString passportStr = passengerList->GetItemText(selectedItem, 3);

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
            passengerList->SetItem(index, 2, wxString(passenger.getContactInfo().toString().c_str(), wxConvUTF8));
            passengerList->SetItem(index, 3, wxString(passenger.getPassport().toString().c_str(), wxConvUTF8));
            found = true;
            break;
        }
    }

    if (!found)
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
    passengerList->SetItem(index, 2, wxString(passenger.getContactInfo().toString().c_str(), wxConvUTF8));
    passengerList->SetItem(index, 3, wxString(passenger.getPassport().toString().c_str(), wxConvUTF8));
}

void PassengerWindow::OnListItemSelected(wxListEvent &event)
{
    // Handle list item selection if needed
    event.Skip();
}