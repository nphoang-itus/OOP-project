#include "AircraftDialog.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include "../../core/value_objects/seat_class_map/SeatClassMap.h"

AircraftDialog::AircraftDialog(std::shared_ptr<AircraftService> aircraftService, QWidget *parent)
    : QDialog(parent)
    , _aircraftService(aircraftService)
    , _isEditMode(false)
{
    setupUI();
    setupConnections();
}

AircraftDialog::AircraftDialog(std::shared_ptr<AircraftService> aircraftService, const Aircraft& aircraft, QWidget *parent)
    : QDialog(parent)
    , _aircraftService(aircraftService)
    , _isEditMode(true)
    , _aircraftId(aircraft.getId())
{
    setupUI();
    setupConnections();
    loadAircraftData(aircraft);
}

AircraftDialog::~AircraftDialog()
{
}

void AircraftDialog::setupUI()
{
    setWindowTitle(_isEditMode ? "Edit Aircraft" : "Add Aircraft");
    resize(400, 200);

    QFormLayout* layout = new QFormLayout(this);

    _serialEdit = new QLineEdit(this);
    _modelEdit = new QLineEdit(this);
    _seatLayoutEdit = new QLineEdit(this);

    layout->addRow("Serial Number:", _serialEdit);
    layout->addRow("Model:", _modelEdit);
    layout->addRow("Seat Layout (e.g., E:30,B:20,F:10):", _seatLayoutEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    _okButton = new QPushButton("OK", this);
    _cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addWidget(_okButton);
    buttonLayout->addWidget(_cancelButton);
    layout->addRow(buttonLayout);

    setLayout(layout);
}

void AircraftDialog::setupConnections()
{
    connect(_okButton, &QPushButton::clicked, this, &AircraftDialog::onAccept);
    connect(_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(_serialEdit, &QLineEdit::textChanged, this, &AircraftDialog::validateInput);
    connect(_modelEdit, &QLineEdit::textChanged, this, &AircraftDialog::validateInput);
    connect(_seatLayoutEdit, &QLineEdit::textChanged, this, &AircraftDialog::validateInput);
}

void AircraftDialog::loadAircraftData(const Aircraft& aircraft)
{
    _serialEdit->setText(QString::fromStdString(aircraft.getSerial().toString()));
    _serialEdit->setReadOnly(true);
    _modelEdit->setText(QString::fromStdString(aircraft.getModel()));
    _seatLayoutEdit->setText(QString::fromStdString(aircraft.getSeatLayout().toString()));
}

Aircraft AircraftDialog::getAircraft() const
{
    // Create seat layout
    auto seatLayoutResult = SeatClassMap::create(_seatLayoutEdit->text().toStdString());
    if (!seatLayoutResult) {
        throw std::runtime_error("Invalid seat layout format");
    }

    // Create aircraft
    auto aircraft = Aircraft::create(
        AircraftSerial::create(_serialEdit->text().toStdString()).value(),
        _modelEdit->text().toStdString(),
        seatLayoutResult.value()
    ).value();

    // Set the ID if in edit mode
    if (_isEditMode) {
        aircraft.setId(_aircraftId);
    }

    return aircraft;
}

void AircraftDialog::onAccept()
{
    try {
        auto aircraft = getAircraft();
        if (_isEditMode) {
            auto result = _aircraftService->updateAircraft(aircraft);
            if (!result) {
                QMessageBox::critical(this, "Error", QString::fromStdString(result.error().message));
                return;
            }
        } else {
            auto result = _aircraftService->createAircraft(aircraft);
            if (!result) {
                QMessageBox::critical(this, "Error", QString::fromStdString(result.error().message));
                return;
            }
        }
        accept();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void AircraftDialog::validateInput()
{
    bool isValid = !_serialEdit->text().isEmpty() &&
                  !_modelEdit->text().isEmpty() &&
                  !_seatLayoutEdit->text().isEmpty();
    _okButton->setEnabled(isValid);
} 