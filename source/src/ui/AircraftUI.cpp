#include "AircraftUI.h"
#include <iomanip>
#include <sstream>

enum
{
    ID_BACK = 1,
    ID_SHOW = 2,
    ID_ADD = 3,
    ID_EDIT = 4,
    ID_DELETE = 5,
    ID_AIRCRAFT_LIST = 6,
    ID_SEARCH_ID = 7,
    ID_SEARCH_REGISTRATION = 8
};

BEGIN_EVENT_TABLE(AircraftWindow, wxFrame)
EVT_BUTTON(ID_BACK, AircraftWindow::OnBack)
EVT_BUTTON(ID_SHOW, AircraftWindow::OnShowAircraft)
EVT_BUTTON(ID_ADD, AircraftWindow::OnAddAircraft)
EVT_BUTTON(ID_EDIT, AircraftWindow::OnEditAircraft)
EVT_BUTTON(ID_DELETE, AircraftWindow::OnDeleteAircraft)
EVT_BUTTON(ID_SEARCH_ID, AircraftWindow::OnSearchById)
EVT_BUTTON(ID_SEARCH_REGISTRATION, AircraftWindow::OnSearchByRegistration)
EVT_LIST_ITEM_SELECTED(ID_AIRCRAFT_LIST, AircraftWindow::OnListItemSelected)
END_EVENT_TABLE()

AircraftWindow::AircraftWindow(const wxString &title, std::shared_ptr<AircraftService> aircraftService)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 600)), aircraftService(aircraftService)
{
    panel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);

    // Create back button
    backButton = new wxButton(panel, ID_BACK, "Back", wxDefaultPosition, wxSize(100, 30));
    buttonSizer->Add(backButton, 0, wxALL, 10);

    // Create main function buttons
    showButton = new wxButton(panel, ID_SHOW, "Hiển thị thông tin máy bay", wxDefaultPosition, wxSize(250, 50));
    addButton = new wxButton(panel, ID_ADD, "Thêm máy bay", wxDefaultPosition, wxSize(250, 50));
    editButton = new wxButton(panel, ID_EDIT, "Sửa thông tin máy bay", wxDefaultPosition, wxSize(250, 50));
    deleteButton = new wxButton(panel, ID_DELETE, "Xóa máy bay", wxDefaultPosition, wxSize(250, 50));
    searchByIdButton = new wxButton(panel, ID_SEARCH_ID, "Tìm theo ID", wxDefaultPosition, wxSize(250, 50));
    searchByRegistrationButton = new wxButton(panel, ID_SEARCH_REGISTRATION, "Tìm theo số đăng ký", wxDefaultPosition, wxSize(250, 50));

    // Create aircraft list
    aircraftList = new wxListCtrl(panel, ID_AIRCRAFT_LIST, wxDefaultPosition, wxSize(900, 300),
                                  wxLC_REPORT | wxLC_SINGLE_SEL);
    aircraftList->InsertColumn(0, "ID");
    aircraftList->InsertColumn(1, "Số đăng ký");
    aircraftList->InsertColumn(2, "Loại máy bay");
    aircraftList->InsertColumn(3, "Ghế thường");
    aircraftList->InsertColumn(4, "Ghế thương gia");
    aircraftList->InsertColumn(5, "Ghế hạng nhất");
    aircraftList->InsertColumn(6, "Tổng ghế");
    aircraftList->InsertColumn(7, "Trạng thái");

    for (int i = 0; i < 8; ++i)
    {
        aircraftList->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
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
    row2->Add(searchByRegistrationButton, 0, wxALL, 10);

    contentSizer->Add(row1, 0, wxALIGN_CENTER);
    contentSizer->Add(row2, 0, wxALIGN_CENTER);
    contentSizer->Add(aircraftList, 1, wxALL | wxEXPAND, 10);
    contentSizer->Add(infoLabel, 0, wxALL | wxALIGN_CENTER, 10);

    // Add sizers to main sizer
    mainSizer->Add(buttonSizer, 0, wxEXPAND);
    mainSizer->Add(contentSizer, 1, wxEXPAND);

    panel->SetSizer(mainSizer);
    Centre();
}

void AircraftWindow::OnBack(wxCommandEvent &event)
{
    this->Hide();
    MainWindow *mainWindow = new MainWindow("Quản lý hãng hàng không",
                                            aircraftService,
                                            flightService,
                                            passengerService);
    mainWindow->Show();
    this->Close();
}

void AircraftWindow::setServices(std::shared_ptr<AircraftService> aircraft,
                                 std::shared_ptr<FlightService> flight,
                                 std::shared_ptr<PassengerService> passenger)
{
    this->aircraftService = aircraft;
    this->flightService = flight;
    this->passengerService = passenger;
}

void AircraftWindow::OnShowAircraft(wxCommandEvent &event)
{
    RefreshAircraftList();
}

void AircraftWindow::OnAddAircraft(wxCommandEvent &event)
{
    wxDialog *dialog = new wxDialog(this, wxID_ANY, "Add Aircraft", wxDefaultPosition, wxSize(400, 500));
    wxPanel *panel = new wxPanel(dialog);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *registrationLabel = new wxStaticText(panel, wxID_ANY, "Registration:");
    wxTextCtrl *registrationCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *typeLabel = new wxStaticText(panel, wxID_ANY, "Type:");
    wxTextCtrl *typeCtrl = new wxTextCtrl(panel, wxID_ANY);
    wxStaticText *economySeatsLabel = new wxStaticText(panel, wxID_ANY, "Economy Seats:");
    wxTextCtrl *economySeatsCtrl = new wxTextCtrl(panel, wxID_ANY, "");
    wxStaticText *businessSeatsLabel = new wxStaticText(panel, wxID_ANY, "Business Seats:");
    wxTextCtrl *businessSeatsCtrl = new wxTextCtrl(panel, wxID_ANY, "");
    wxStaticText *firstSeatsLabel = new wxStaticText(panel, wxID_ANY, "First Seats:");
    wxTextCtrl *firstSeatsCtrl = new wxTextCtrl(panel, wxID_ANY, "");
    wxStaticText *statusLabel = new wxStaticText(panel, wxID_ANY, "Status (A/I):");
    wxTextCtrl *statusCtrl = new wxTextCtrl(panel, wxID_ANY, "A");

    sizer->Add(registrationLabel, 0, wxALL, 5);
    sizer->Add(registrationCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(typeLabel, 0, wxALL, 5);
    sizer->Add(typeCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(economySeatsLabel, 0, wxALL, 5);
    sizer->Add(economySeatsCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(businessSeatsLabel, 0, wxALL, 5);
    sizer->Add(businessSeatsCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(firstSeatsLabel, 0, wxALL, 5);
    sizer->Add(firstSeatsCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(statusLabel, 0, wxALL, 5);
    sizer->Add(statusCtrl, 0, wxEXPAND | wxALL, 5);

    wxButton *okButton = new wxButton(panel, wxID_OK, "OK");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Cancel");
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);

    panel->SetSizer(sizer);

    if (dialog->ShowModal() == wxID_OK)
    {
        std::string registration = registrationCtrl->GetValue().ToStdString();
        std::string type = typeCtrl->GetValue().ToStdString();
        
        long economySeats, businessSeats, firstSeats;
        if (!economySeatsCtrl->GetValue().ToLong(&economySeats))
        {
            economySeats = 0;
        }
        if (!businessSeatsCtrl->GetValue().ToLong(&businessSeats))
        {
            businessSeats = 0;
        }
        if (!firstSeatsCtrl->GetValue().ToLong(&firstSeats))
        {
            firstSeats = 0;
        }
        
        std::string status = statusCtrl->GetValue().ToStdString();
        if (status.empty())
        {
            status = "A";
        }

        // Create seat layout string with proper format
        std::string seatLayout = "E:" + std::to_string(economySeats) + 
                               ",B:" + std::to_string(businessSeats) + 
                               ",F:" + std::to_string(firstSeats);

        // Create aircraft using factory method
        auto aircraftResult = Aircraft::create(registration, type, seatLayout);
        if (!aircraftResult)
        {
            wxMessageBox("Failed to create aircraft: " + aircraftResult.error().message, "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Save aircraft using service
        auto saveResult = aircraftService->createAircraft(*aircraftResult);
        if (!saveResult)
        {
            wxMessageBox("Failed to save aircraft: " + saveResult.error().message, "Error", wxOK | wxICON_ERROR);
            return;
        }

        RefreshAircraftList();
    }

    dialog->Destroy();
}

void AircraftWindow::OnEditAircraft(wxCommandEvent &event)
{
    long itemIndex = -1;
    itemIndex = aircraftList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1)
    {
        wxMessageBox("Please select an aircraft to edit", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxListItem item;
    item.SetId(itemIndex);
    item.SetColumn(0);
    item.SetMask(wxLIST_MASK_TEXT);
    aircraftList->GetItem(item);
    int aircraftId = std::stoi(item.GetText().ToStdString());

    auto aircraftResult = aircraftService->getAircraftById(aircraftId);
    if (!aircraftResult)
    {
        wxMessageBox("Failed to get aircraft: " + aircraftResult.error().message, "Error", wxOK | wxICON_ERROR);
        return;
    }

    const Aircraft &aircraft = *aircraftResult;

    wxDialog *dialog = new wxDialog(this, wxID_ANY, "Edit Aircraft", wxDefaultPosition, wxSize(400, 500));
    wxPanel *panel = new wxPanel(dialog);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText *registrationLabel = new wxStaticText(panel, wxID_ANY, "Registration:");
    wxTextCtrl *registrationCtrl = new wxTextCtrl(panel, wxID_ANY, aircraft.getSerial().toString());
    wxStaticText *typeLabel = new wxStaticText(panel, wxID_ANY, "Type:");
    wxTextCtrl *typeCtrl = new wxTextCtrl(panel, wxID_ANY, aircraft.getModel());
    wxStaticText *economySeatsLabel = new wxStaticText(panel, wxID_ANY, "Economy Seats:");
    wxTextCtrl *economySeatsCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%d", aircraft.getSeatCount("E")));
    wxStaticText *businessSeatsLabel = new wxStaticText(panel, wxID_ANY, "Business Seats:");
    wxTextCtrl *businessSeatsCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%d", aircraft.getSeatCount("B")));
    wxStaticText *firstSeatsLabel = new wxStaticText(panel, wxID_ANY, "First Seats:");
    wxTextCtrl *firstSeatsCtrl = new wxTextCtrl(panel, wxID_ANY, wxString::Format("%d", aircraft.getSeatCount("F")));
    wxStaticText *statusLabel = new wxStaticText(panel, wxID_ANY, "Status (A/I):");
    wxTextCtrl *statusCtrl = new wxTextCtrl(panel, wxID_ANY, "A");

    sizer->Add(registrationLabel, 0, wxALL, 5);
    sizer->Add(registrationCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(typeLabel, 0, wxALL, 5);
    sizer->Add(typeCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(economySeatsLabel, 0, wxALL, 5);
    sizer->Add(economySeatsCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(businessSeatsLabel, 0, wxALL, 5);
    sizer->Add(businessSeatsCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(firstSeatsLabel, 0, wxALL, 5);
    sizer->Add(firstSeatsCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(statusLabel, 0, wxALL, 5);
    sizer->Add(statusCtrl, 0, wxEXPAND | wxALL, 5);

    wxButton *okButton = new wxButton(panel, wxID_OK, "OK");
    wxButton *cancelButton = new wxButton(panel, wxID_CANCEL, "Cancel");
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);

    panel->SetSizer(sizer);

    if (dialog->ShowModal() == wxID_OK)
    {
        std::string registration = registrationCtrl->GetValue().ToStdString();
        std::string type = typeCtrl->GetValue().ToStdString();
        
        long economySeats, businessSeats, firstSeats;
        if (!economySeatsCtrl->GetValue().ToLong(&economySeats))
        {
            economySeats = 0;
        }
        if (!businessSeatsCtrl->GetValue().ToLong(&businessSeats))
        {
            businessSeats = 0;
        }
        if (!firstSeatsCtrl->GetValue().ToLong(&firstSeats))
        {
            firstSeats = 0;
        }
        
        std::string status = statusCtrl->GetValue().ToStdString();
        if (status.empty())
        {
            status = "A";
        }

        // Create seat layout string with proper format
        std::string seatLayout = "E:" + std::to_string(economySeats) + 
                               ",B:" + std::to_string(businessSeats) + 
                               ",F:" + std::to_string(firstSeats);

        // Create aircraft using factory method
        auto aircraftResult = Aircraft::create(registration, type, seatLayout);
        if (!aircraftResult)
        {
            wxMessageBox("Failed to create aircraft: " + aircraftResult.error().message, "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Update aircraft using service
        auto updateResult = aircraftService->updateAircraft(*aircraftResult);
        if (!updateResult)
        {
            wxMessageBox("Failed to update aircraft: " + updateResult.error().message, "Error", wxOK | wxICON_ERROR);
            return;
        }

        RefreshAircraftList();
    }

    dialog->Destroy();
}

void AircraftWindow::OnDeleteAircraft(wxCommandEvent &event)
{
    long itemIndex = -1;
    itemIndex = aircraftList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1)
    {
        wxMessageBox("Please select an aircraft to delete", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxListItem item;
    item.SetId(itemIndex);
    item.SetColumn(0);
    item.SetMask(wxLIST_MASK_TEXT);
    aircraftList->GetItem(item);
    int aircraftId = std::stoi(item.GetText().ToStdString());

    auto aircraftResult = aircraftService->getAircraftById(aircraftId);
    if (!aircraftResult)
    {
        wxMessageBox("Failed to get aircraft: " + aircraftResult.error().message, "Error", wxOK | wxICON_ERROR);
        return;
    }

    const Aircraft &aircraft = *aircraftResult;
    wxString msg = wxString::Format("Are you sure you want to delete aircraft %s?", aircraft.getSerial().toString());
    if (wxMessageBox(msg, "Confirm", wxYES_NO | wxICON_QUESTION) == wxYES)
    {
        auto deleteResult = aircraftService->deleteAircraft(aircraft.getSerial());
        if (!deleteResult)
        {
            wxMessageBox("Failed to delete aircraft: " + deleteResult.error().message, "Error", wxOK | wxICON_ERROR);
            return;
        }
        RefreshAircraftList();
    }
}

void AircraftWindow::OnListItemSelected(wxListEvent &event)
{
    // Do nothing for now
}

void AircraftWindow::RefreshAircraftList()
{
    aircraftList->DeleteAllItems();
    auto aircraftsResult = aircraftService->getAllAircraft();
    if (!aircraftsResult)
    {
        wxMessageBox("Failed to get aircraft list: " + aircraftsResult.error().message, "Error", wxOK | wxICON_ERROR);
        return;
    }

    const auto &aircrafts = *aircraftsResult;
    for (const auto &a : aircrafts)
    {
        long index = aircraftList->GetItemCount();
        aircraftList->InsertItem(index, wxString::Format("%d", a.getId()));
        aircraftList->SetItem(index, 1, wxString::FromUTF8(a.getSerial().toString().c_str()));
        aircraftList->SetItem(index, 2, wxString::FromUTF8(a.getModel().c_str()));

        // Display seat counts for each class
        int economySeats = a.getSeatCount("E");
        int businessSeats = a.getSeatCount("B");
        int firstSeats = a.getSeatCount("F");
        int totalSeats = economySeats + businessSeats + firstSeats;

        aircraftList->SetItem(index, 3, wxString::Format("%d", economySeats));
        aircraftList->SetItem(index, 4, wxString::Format("%d", businessSeats));
        aircraftList->SetItem(index, 5, wxString::Format("%d", firstSeats));
        aircraftList->SetItem(index, 6, wxString::Format("%d", totalSeats));
        aircraftList->SetItem(index, 7, "A"); // Default status
    }
}

void AircraftWindow::OnSearchById(wxCommandEvent &event)
{
    wxString idStr = wxGetTextFromUser("Enter aircraft ID:", "Search by ID");
    if (idStr.IsEmpty())
        return;

    long id;
    if (!idStr.ToLong(&id))
    {
        wxMessageBox("Invalid ID format", "Error", wxOK | wxICON_ERROR);
        return;
    }

    auto aircraftResult = aircraftService->getAircraftById(static_cast<int>(id));
    if (!aircraftResult)
    {
        wxMessageBox("Failed to get aircraft: " + aircraftResult.error().message, "Error", wxOK | wxICON_ERROR);
        return;
    }

    const Aircraft &a = *aircraftResult;
    aircraftList->DeleteAllItems();
    aircraftList->InsertItem(0, wxString::Format("%d", a.getId()));
    aircraftList->SetItem(0, 1, wxString::FromUTF8(a.getSerial().toString().c_str()));
    aircraftList->SetItem(0, 2, wxString::FromUTF8(a.getModel().c_str()));

    // Display seat counts for each class
    int economySeats = a.getSeatCount("E");
    int businessSeats = a.getSeatCount("B");
    int firstSeats = a.getSeatCount("F");
    int totalSeats = economySeats + businessSeats + firstSeats;

    aircraftList->SetItem(0, 3, wxString::Format("%d", economySeats));
    aircraftList->SetItem(0, 4, wxString::Format("%d", businessSeats));
    aircraftList->SetItem(0, 5, wxString::Format("%d", firstSeats));
    aircraftList->SetItem(0, 6, wxString::Format("%d", totalSeats));
    aircraftList->SetItem(0, 7, "A"); // Default status
}

void AircraftWindow::OnSearchByRegistration(wxCommandEvent &event)
{
    wxString registration = wxGetTextFromUser("Enter aircraft registration:", "Search by Registration");
    if (registration.IsEmpty())
        return;

    auto aircraftResult = aircraftService->getAircraft(AircraftSerial::create(registration.ToStdString()).value());
    if (!aircraftResult)
    {
        wxMessageBox("Failed to get aircraft: " + aircraftResult.error().message, "Error", wxOK | wxICON_ERROR);
        return;
    }

    const Aircraft &a = *aircraftResult;
    aircraftList->DeleteAllItems();
    aircraftList->InsertItem(0, wxString::Format("%d", a.getId()));
    aircraftList->SetItem(0, 1, wxString::FromUTF8(a.getSerial().toString().c_str()));
    aircraftList->SetItem(0, 2, wxString::FromUTF8(a.getModel().c_str()));

    // Display seat counts for each class
    int economySeats = a.getSeatCount("E");
    int businessSeats = a.getSeatCount("B");
    int firstSeats = a.getSeatCount("F");
    int totalSeats = economySeats + businessSeats + firstSeats;

    aircraftList->SetItem(0, 3, wxString::Format("%d", economySeats));
    aircraftList->SetItem(0, 4, wxString::Format("%d", businessSeats));
    aircraftList->SetItem(0, 5, wxString::Format("%d", firstSeats));
    aircraftList->SetItem(0, 6, wxString::Format("%d", totalSeats));
    aircraftList->SetItem(0, 7, "A"); // Default status
}