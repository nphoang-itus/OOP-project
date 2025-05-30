#include "TicketUI.h"
#include "services/TicketService.h"
#include "services/AircraftService.h"
#include "services/FlightService.h"
#include "services/PassengerService.h"
#include "core/entities/Ticket.h"
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/numdlg.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <sstream>
#include <iomanip>

enum
{
    ID_ADD = 1,
    ID_EDIT = 2,
    ID_DELETE = 3,
    ID_SHOW = 4,
    ID_SEARCH = 5,
    ID_BACK = 6,
    ID_REFRESH = 7
};

BEGIN_EVENT_TABLE(TicketWindow, wxFrame)
EVT_BUTTON(ID_ADD, TicketWindow::OnAddTicket)
EVT_BUTTON(ID_EDIT, TicketWindow::OnEditTicket)
EVT_BUTTON(ID_DELETE, TicketWindow::OnDeleteTicket)
EVT_BUTTON(ID_SHOW, TicketWindow::OnShowTicket)
EVT_BUTTON(ID_SEARCH, TicketWindow::OnSearchTicket)
EVT_BUTTON(ID_BACK, TicketWindow::OnBack)
EVT_BUTTON(ID_REFRESH, TicketWindow::OnRefresh)
END_EVENT_TABLE()

TicketWindow::TicketWindow(const wxString &title, std::shared_ptr<TicketService> ticketService)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)),
      ticketService(ticketService)
{
    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create list control
    ticketList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxLC_REPORT | wxLC_SINGLE_SEL);
    ticketList->InsertColumn(0, "Số vé", wxLIST_FORMAT_LEFT, 120);
    ticketList->InsertColumn(1, "Hành khách", wxLIST_FORMAT_LEFT, 100);
    ticketList->InsertColumn(2, "Chuyến bay", wxLIST_FORMAT_LEFT, 100);
    ticketList->InsertColumn(3, "Ghế", wxLIST_FORMAT_LEFT, 60);
    ticketList->InsertColumn(4, "Giá (VNĐ)", wxLIST_FORMAT_RIGHT, 100);
    ticketList->InsertColumn(5, "Trạng thái", wxLIST_FORMAT_LEFT, 100);

    // Create buttons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    addButton = new wxButton(panel, ID_ADD, "Thêm vé", wxDefaultPosition, wxSize(80, 30));
    editButton = new wxButton(panel, ID_EDIT, "Sửa vé", wxDefaultPosition, wxSize(80, 30));
    deleteButton = new wxButton(panel, ID_DELETE, "Xóa vé", wxDefaultPosition, wxSize(80, 30));
    showButton = new wxButton(panel, ID_SHOW, "Chi tiết", wxDefaultPosition, wxSize(80, 30));
    searchButton = new wxButton(panel, ID_SEARCH, "Tìm kiếm", wxDefaultPosition, wxSize(80, 30));
    backButton = new wxButton(panel, ID_BACK, "Quay lại", wxDefaultPosition, wxSize(80, 30));
    refreshButton = new wxButton(panel, ID_REFRESH, "Làm mới", wxDefaultPosition, wxSize(80, 30));

    buttonSizer->Add(addButton, 0, wxALL, 5);
    buttonSizer->Add(editButton, 0, wxALL, 5);
    buttonSizer->Add(deleteButton, 0, wxALL, 5);
    buttonSizer->Add(showButton, 0, wxALL, 5);
    buttonSizer->Add(searchButton, 0, wxALL, 5);
    buttonSizer->Add(backButton, 0, wxALL, 5);
    buttonSizer->Add(refreshButton, 0, wxALL, 5);

    mainSizer->Add(ticketList, 1, wxEXPAND | wxALL, 10);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    panel->SetSizer(mainSizer);
    Centre();

    RefreshTicketList();
}

void TicketWindow::setServices(
    std::shared_ptr<AircraftService> aircraftService,
    std::shared_ptr<FlightService> flightService,
    std::shared_ptr<PassengerService> passengerService,
    std::shared_ptr<TicketService> ticketService)
{
    this->aircraftService = aircraftService;
    this->flightService = flightService;
    this->passengerService = passengerService;
    this->ticketService = ticketService;
}

void TicketWindow::RefreshTicketList()
{
    ticketList->DeleteAllItems();
    auto tickets = ticketService->getAllTickets();
    if (!tickets)
    {
        wxMessageBox("Lỗi khi lấy danh sách vé", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    int index = 0;
    for (const auto &ticket : tickets.value())
    {
        ticketList->InsertItem(index, ticket.getTicketNumber().toString());
        ticketList->SetItem(index, 1, ticket.getPassenger()->getPassport().toString());
        ticketList->SetItem(index, 2, ticket.getFlight()->getFlightNumber().toString());
        ticketList->SetItem(index, 3, ticket.getSeatNumber().toString());

        // Format price with thousand separators
        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << ticket.getPrice().getAmount();
        std::string priceStr = ss.str();
        // Add thousand separators
        for (int i = priceStr.length() - 3; i > 0; i -= 3)
        {
            priceStr.insert(i, ".");
        }
        ticketList->SetItem(index, 4, priceStr);

        ticketList->SetItem(index, 5, ticket.getStatusVietnamese());
        index++;
    }
}

void TicketWindow::ShowTicketDetails(const Ticket &ticket)
{
    std::stringstream ss;
    ss << "==============================\n";
    ss << "        CHI TIẾT VÉ        \n";
    ss << "==============================\n";
    ss << "Số vé:         " << ticket.getTicketNumber().toString() << "\n";
    ss << "Hành khách:    " << ticket.getPassenger()->getPassport().toString() << "\n";
    ss << "Chuyến bay:    " << ticket.getFlight()->getFlightNumber().toString() << "\n";
    ss << "Ghế:           " << ticket.getSeatNumber().toString() << "\n";

    // Format price with thousand separators
    std::stringstream priceStream;
    priceStream << std::fixed << std::setprecision(0) << ticket.getPrice().getAmount();
    std::string priceStr = priceStream.str();
    for (int i = priceStr.length() - 3; i > 0; i -= 3)
    {
        priceStr.insert(i, ".");
    }
    ss << "Giá:           " << priceStr << " " << ticket.getPrice().getCurrency() << "\n";
    ss << "Trạng thái:    " << ticket.getStatusVietnamese() << "\n";
    ss << "==============================";

    wxMessageBox(ss.str(), "Chi tiết vé", wxOK | wxICON_INFORMATION);
}

void TicketWindow::AddTicketDialog()
{
    // Step 1: Get and validate passport number
    wxString passportStr = wxGetTextFromUser("Nhập số hộ chiếu:", "Thêm vé");
    if (passportStr.IsEmpty())
        return;

    auto passportResult = PassportNumber::create(passportStr.ToStdString());
    if (!passportResult)
    {
        wxMessageBox("Số hộ chiếu không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    // Check if passenger exists
    auto passengerResult = passengerService->getPassenger(passportResult.value());
    if (!passengerResult)
    {
        wxMessageBox("Không tìm thấy hành khách với số hộ chiếu này", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    // Step 2: Get and validate flight number
    wxString flightStr = wxGetTextFromUser("Nhập số hiệu chuyến bay:", "Thêm vé");
    if (flightStr.IsEmpty())
        return;

    auto flightNumberResult = FlightNumber::create(flightStr.ToStdString());
    if (!flightNumberResult)
    {
        wxMessageBox("Số hiệu chuyến bay không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    // Check if flight exists
    auto flightResult = flightService->getFlight(flightNumberResult.value());
    if (!flightResult)
    {
        wxMessageBox("Không tìm thấy chuyến bay với số hiệu này", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    const auto &flight = flightResult.value();
    const auto &aircraft = flight.getAircraft();
    if (!aircraft)
    {
        wxMessageBox("Chuyến bay không có thông tin máy bay", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    // Step 3: Show seat class selection dialog
    wxArrayString choices;
    choices.Add("Hạng thường (Economy)");
    choices.Add("Hạng thương gia (Business)");
    choices.Add("Hạng nhất (First)");

    wxSingleChoiceDialog dialog(this, "Chọn hạng ghế:", "Chọn hạng ghế", choices);
    if (dialog.ShowModal() != wxID_OK)
        return;

    std::string seatClass;
    switch (dialog.GetSelection())
    {
    case 0:
        seatClass = "ECONOMY";
        break; // Economy
    case 1:
        seatClass = "BUSINESS";
        break; // Business
    case 2:
        seatClass = "FIRST";
        break; // First
    default:
        return;
    }

    // Step 4: Get available seats for selected class
    auto availableSeatsResult = aircraftService->getAvailableSeats(aircraft->getSerial(), seatClass);
    if (!availableSeatsResult)
    {
        wxMessageBox("Không thể lấy thông tin ghế trống: " + availableSeatsResult.error().message,
                     "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    const auto &availableSeats = availableSeatsResult.value();
    if (availableSeats.empty())
    {
        wxMessageBox("Không còn ghế trống cho hạng này", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }

    // Step 5: Show seat selection dialog
    wxArrayString seatChoices;
    for (const auto &seat : availableSeats)
    {
        seatChoices.Add(seat);
    }

    wxSingleChoiceDialog seatDialog(this, "Chọn ghế:", "Chọn ghế", seatChoices);
    if (seatDialog.ShowModal() != wxID_OK)
        return;

    std::string selectedSeat = seatChoices[seatDialog.GetSelection()].ToStdString();

    // Step 6: Set price based on seat class
    int price;
    switch (dialog.GetSelection())
    {
    case 0:
        price = 1800000;
        break; // Economy
    case 1:
        price = 3500000;
        break; // Business
    case 2:
        price = 5000000;
        break; // First
    default:
        return;
    }

    // Format price as string with currency
    std::stringstream priceSS;
    priceSS << std::fixed << std::setprecision(0) << price << " VND";
    std::string priceStr = priceSS.str();

    auto priceResult = Price::create(priceStr);
    if (!priceResult)
    {
        wxMessageBox("Lỗi khi tạo giá vé: " + priceResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    // Step 7: Book the ticket
    auto result = ticketService->bookTicket(
        passportResult.value(),
        flightNumberResult.value(),
        selectedSeat,
        priceResult.value());

    if (!result)
    {
        wxMessageBox("Lỗi khi tạo vé: " + result.error().message, "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    wxMessageBox("Tạo vé thành công!", "Thông báo", wxOK | wxICON_INFORMATION);
    RefreshTicketList();
}

void TicketWindow::EditTicketDialog(const Ticket &ticket)
{
    // Get new passport number
    wxString passportStr = wxGetTextFromUser("Nhập số hộ chiếu mới:", "Sửa vé",
                                             ticket.getPassenger()->getPassport().toString());
    if (passportStr.IsEmpty())
        return;

    // Validate passport number format
    auto passportResult = PassportNumber::create(passportStr.ToStdString());
    if (!passportResult)
    {
        wxMessageBox("Số hộ chiếu không hợp lệ: " + passportResult.error().message, "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    // Check if passenger with this passport exists
    auto newPassengerResult = passengerService->getPassenger(passportResult.value());
    if (!newPassengerResult)
    {
        wxMessageBox("Không tìm thấy hành khách với số hộ chiếu này. Vui lòng đảm bảo hành khách đã được đăng ký trong hệ thống.",
                     "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    // Create updated ticket by copying the original ticket and updating the passenger
    auto updatedTicket = ticket; // Copy the original ticket to preserve ID and other properties

    // Update only the passenger information
    auto newTicket = Ticket::create(
        ticket.getTicketNumber(),
        std::make_shared<Passenger>(newPassengerResult.value()),
        ticket.getFlight(),
        ticket.getSeatNumber(),
        ticket.getPrice());

    if (!newTicket)
    {
        wxMessageBox("Lỗi khi tạo vé với hành khách mới: " + newTicket.error().message, "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    // Preserve the original ticket's ID and status but update the passenger
    updatedTicket = newTicket.value();
    updatedTicket.setId(ticket.getId());
    updatedTicket.setStatus(ticket.getStatus());

    // Update ticket
    auto result = ticketService->updateTicket(updatedTicket);
    if (!result)
    {
        wxMessageBox("Lỗi khi cập nhật vé: " + result.error().message, "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    wxMessageBox("Cập nhật vé thành công!", "Thông báo", wxOK | wxICON_INFORMATION);
    RefreshTicketList();
}
void TicketWindow::DeleteTicketDialog(const Ticket &ticket)
{
    int answer = wxMessageBox(
        "Bạn có chắc chắn muốn xóa vé " + ticket.getTicketNumber().toString() + "?",
        "Xác nhận xóa",
        wxYES_NO | wxICON_QUESTION);

    if (answer == wxYES)
    {
        auto result = ticketService->deleteTicket(ticket.getTicketNumber());
        if (!result)
        {
            wxMessageBox("Lỗi khi xóa vé: " + result.error().message, "Lỗi", wxOK | wxICON_ERROR);
            return;
        }

        wxMessageBox("Xóa vé thành công!", "Thông báo", wxOK | wxICON_INFORMATION);
        RefreshTicketList();
    }
}

void TicketWindow::SearchTicketDialog()
{
    wxString choices[] = {"Theo số vé", "Theo hành khách", "Theo chuyến bay", "Theo trạng thái"};
    int choice = wxGetSingleChoiceIndex("Chọn tiêu chí tìm kiếm:", "Tìm kiếm vé", 4, choices);

    if (choice == -1)
        return;

    std::vector<Ticket> results;
    switch (choice)
    {
    case 0:
    { // Theo số vé
        wxString ticketStr = wxGetTextFromUser("Nhập số vé:", "Tìm kiếm");
        if (ticketStr.IsEmpty())
            return;

        auto ticketResult = TicketNumber::create(ticketStr.ToStdString());
        if (!ticketResult)
        {
            wxMessageBox("Số vé không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
            return;
        }

        auto result = ticketService->getTicket(ticketResult.value());
        if (!result)
        {
            wxMessageBox("Không tìm thấy vé", "Thông báo", wxOK | wxICON_INFORMATION);
            return;
        }
        results.push_back(result.value());
        break;
    }
    case 1:
    { // Theo hành khách
        wxString passportStr = wxGetTextFromUser("Nhập số hộ chiếu:", "Tìm kiếm");
        if (passportStr.IsEmpty())
            return;

        auto passportResult = PassportNumber::create(passportStr.ToStdString());
        if (!passportResult)
        {
            wxMessageBox("Số hộ chiếu không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
            return;
        }

        auto result = ticketService->searchByPassenger(passportResult.value());
        if (!result)
        {
            wxMessageBox("Lỗi khi tìm kiếm", "Lỗi", wxOK | wxICON_ERROR);
            return;
        }
        results = result.value();
        break;
    }
    case 2:
    { // Theo chuyến bay
        wxString flightStr = wxGetTextFromUser("Nhập số hiệu chuyến bay:", "Tìm kiếm");
        if (flightStr.IsEmpty())
            return;

        auto flightResult = FlightNumber::create(flightStr.ToStdString());
        if (!flightResult)
        {
            wxMessageBox("Số hiệu chuyến bay không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
            return;
        }

        auto result = ticketService->searchByFlight(flightResult.value());
        if (!result)
        {
            wxMessageBox("Lỗi khi tìm kiếm", "Lỗi", wxOK | wxICON_ERROR);
            return;
        }
        results = result.value();
        break;
    }
    case 3:
    { // Theo trạng thái
        wxString statusChoices[] = {"Đã xác nhận", "Đã hủy", "Đã check-in", "Đã lên máy bay", "Đã hoàn tiền"};
        int statusChoice = wxGetSingleChoiceIndex("Chọn trạng thái:", "Tìm kiếm", 5, statusChoices);
        if (statusChoice == -1)
            return;

        TicketStatus status;
        switch (statusChoice)
        {
        case 0:
            status = TicketStatus::CONFIRMED;
            break;
        case 1:
            status = TicketStatus::CANCELLED;
            break;
        case 2:
            status = TicketStatus::CHECKED_IN;
            break;
        case 3:
            status = TicketStatus::BOARDED;
            break;
        case 4:
            status = TicketStatus::REFUNDED;
            break;
        default:
            return;
        }

        auto result = ticketService->searchByStatus(status);
        if (!result)
        {
            wxMessageBox("Lỗi khi tìm kiếm", "Lỗi", wxOK | wxICON_ERROR);
            return;
        }
        results = result.value();
        break;
    }
    }

    // Display results
    if (results.empty())
    {
        wxMessageBox("Không tìm thấy vé nào", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }

    ticketList->DeleteAllItems();
    int index = 0;
    for (const auto &ticket : results)
    {
        ticketList->InsertItem(index, ticket.getTicketNumber().toString());
        ticketList->SetItem(index, 1, ticket.getPassenger()->getPassport().toString());
        ticketList->SetItem(index, 2, ticket.getFlight()->getFlightNumber().toString());
        ticketList->SetItem(index, 3, ticket.getSeatNumber().toString());
        ticketList->SetItem(index, 4, std::to_string(ticket.getPrice().getAmount()));
        ticketList->SetItem(index, 5, ticket.getStatusVietnamese());
        index++;
    }
}

void TicketWindow::OnAddTicket(wxCommandEvent &event)
{
    AddTicketDialog();
}

void TicketWindow::OnEditTicket(wxCommandEvent &event)
{
    long selectedIndex = ticketList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn vé cần sửa", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }

    wxString ticketNumber = ticketList->GetItemText(selectedIndex, 0);
    auto ticketResult = TicketNumber::create(ticketNumber.ToStdString());
    if (!ticketResult)
    {
        wxMessageBox("Số vé không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    auto result = ticketService->getTicket(ticketResult.value());
    if (!result)
    {
        wxMessageBox("Không tìm thấy vé", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    EditTicketDialog(result.value());
}

void TicketWindow::OnDeleteTicket(wxCommandEvent &event)
{
    long selectedIndex = ticketList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn vé cần xóa", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }

    wxString ticketNumber = ticketList->GetItemText(selectedIndex, 0);
    auto ticketResult = TicketNumber::create(ticketNumber.ToStdString());
    if (!ticketResult)
    {
        wxMessageBox("Số vé không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    auto result = ticketService->getTicket(ticketResult.value());
    if (!result)
    {
        wxMessageBox("Không tìm thấy vé", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    DeleteTicketDialog(result.value());
}

void TicketWindow::OnShowTicket(wxCommandEvent &event)
{
    long selectedIndex = ticketList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1)
    {
        wxMessageBox("Vui lòng chọn vé cần xem", "Thông báo", wxOK | wxICON_INFORMATION);
        return;
    }

    wxString ticketNumber = ticketList->GetItemText(selectedIndex, 0);
    auto ticketResult = TicketNumber::create(ticketNumber.ToStdString());
    if (!ticketResult)
    {
        wxMessageBox("Số vé không hợp lệ", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    auto result = ticketService->getTicket(ticketResult.value());
    if (!result)
    {
        wxMessageBox("Không tìm thấy vé", "Lỗi", wxOK | wxICON_ERROR);
        return;
    }

    ShowTicketDetails(result.value());
}

void TicketWindow::OnSearchTicket(wxCommandEvent &event)
{
    SearchTicketDialog();
}

void TicketWindow::OnBack(wxCommandEvent &event)
{
    // Create and show main window
    auto mainWindow = new MainWindow(wxT("Hệ thống quản lý hàng không"),
                                     aircraftService,
                                     flightService,
                                     passengerService,
                                     ticketService);
    mainWindow->Show();

    // Close this window
    Close();
}

void TicketWindow::OnRefresh(wxCommandEvent &event)
{
    RefreshTicketList();
}