#include "SeatViewDialog.h"
#include <QMessageBox>

SeatViewDialog::SeatViewDialog(std::shared_ptr<AircraftService> aircraftService, const Aircraft& aircraft, QWidget *parent)
    : QDialog(parent)
    , _aircraftService(aircraftService)
    , _aircraft(aircraft)
{
    setupUI();
    setupConnections();
    loadSeatClasses();
}

SeatViewDialog::~SeatViewDialog()
{
}

void SeatViewDialog::setupUI()
{
    setWindowTitle("View Seats - " + QString::fromStdString(_aircraft.getSerial().toString()));
    resize(400, 500);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Seat class selection
    _seatClassCombo = new QComboBox(this);
    layout->addWidget(new QLabel("Select Seat Class:", this));
    layout->addWidget(_seatClassCombo);

    // Seat list
    _seatList = new QListWidget(this);
    layout->addWidget(new QLabel("Available Seats:", this));
    layout->addWidget(_seatList);

    // Status label
    _statusLabel = new QLabel(this);
    layout->addWidget(_statusLabel);

    setLayout(layout);
}

void SeatViewDialog::setupConnections()
{
    connect(_seatClassCombo, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            this, &SeatViewDialog::onSeatClassChanged);
    connect(_seatList, &QListWidget::itemClicked,
            [this](QListWidgetItem* item) { onSeatSelected(item->text()); });
}

void SeatViewDialog::loadSeatClasses()
{
    auto result = _aircraftService->getAvailableSeatClasses(_aircraft.getSerial());
    if (!result) {
        QMessageBox::critical(this, "Error", QString::fromStdString(result.error().message));
        return;
    }

    _seatClassCombo->clear();
    for (const auto& seatClass : result.value()) {
        _seatClassCombo->addItem(QString::fromStdString(seatClass));
    }

    if (!result.value().empty()) {
        onSeatClassChanged(_seatClassCombo->currentText());
    }
}

void SeatViewDialog::loadSeats(const QString& seatClass)
{
    auto result = _aircraftService->getAvailableSeats(_aircraft.getSerial(), seatClass.toStdString());
    if (!result) {
        QMessageBox::critical(this, "Error", QString::fromStdString(result.error().message));
        return;
    }

    _seatList->clear();
    for (const auto& seat : result.value()) {
        _seatList->addItem(QString::fromStdString(seat));
    }

    _statusLabel->setText(QString("Found ") + QString::number(result.value().size()) + " available seats");
}

void SeatViewDialog::onSeatClassChanged(const QString& seatClass)
{
    if (!seatClass.isEmpty()) {
        loadSeats(seatClass);
    }
}

void SeatViewDialog::onSeatSelected(const QString& seatNumber)
{
    auto result = _aircraftService->isSeatAvailable(_aircraft.getSerial(), seatNumber.toStdString());
    if (!result) {
        QMessageBox::critical(this, "Error", QString::fromStdString(result.error().message));
        return;
    }

    _statusLabel->setText(QString("Seat %1 is %2")
        .arg(seatNumber)
        .arg(result.value() ? "available" : "occupied"));
} 