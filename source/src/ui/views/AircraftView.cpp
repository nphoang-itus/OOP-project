#include "AircraftView.h"
#include "../dialogs/AircraftDialog.h"
#include "../dialogs/SeatViewDialog.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

AircraftView::AircraftView(std::shared_ptr<AircraftService> aircraftService, QWidget *parent)
    : QWidget(parent)
    , _aircraftService(aircraftService)
{
    setupUI();
    setupConnections();
    loadAircraftData();
}

AircraftView::~AircraftView()
{
}

void AircraftView::setupUI()
{
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Create toolbar
    QHBoxLayout* toolbarLayout = new QHBoxLayout();
    
    _searchBox = new QLineEdit(this);
    _searchBox->setPlaceholderText("Search aircraft...");
    toolbarLayout->addWidget(_searchBox);

    _addButton = new QPushButton("Add", this);
    _editButton = new QPushButton("Edit", this);
    _deleteButton = new QPushButton("Delete", this);
    _refreshButton = new QPushButton("Refresh", this);
    _viewSeatsButton = new QPushButton("View Seats", this);

    toolbarLayout->addWidget(_addButton);
    toolbarLayout->addWidget(_editButton);
    toolbarLayout->addWidget(_deleteButton);
    toolbarLayout->addWidget(_refreshButton);
    toolbarLayout->addWidget(_viewSeatsButton);

    mainLayout->addLayout(toolbarLayout);

    // Create table
    _aircraftTable = new QTableView(this);
    _aircraftTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    _aircraftTable->setSelectionMode(QAbstractItemView::SingleSelection);
    _aircraftTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _aircraftTable->horizontalHeader()->setStretchLastSection(true);
    _aircraftTable->verticalHeader()->setVisible(false);
    mainLayout->addWidget(_aircraftTable);

    // Create status bar
    _statusLabel = new QLabel(this);
    mainLayout->addWidget(_statusLabel);

    setLayout(mainLayout);
}

void AircraftView::setupConnections()
{
    connect(_addButton, &QPushButton::clicked, this, &AircraftView::onAddAircraft);
    connect(_editButton, &QPushButton::clicked, this, &AircraftView::onEditAircraft);
    connect(_deleteButton, &QPushButton::clicked, this, &AircraftView::onDeleteAircraft);
    connect(_refreshButton, &QPushButton::clicked, this, &AircraftView::onRefreshAircraft);
    connect(_viewSeatsButton, &QPushButton::clicked, this, &AircraftView::onViewSeats);
    connect(_searchBox, &QLineEdit::textChanged, this, &AircraftView::onSearchAircraft);
}

void AircraftView::loadAircraftData()
{
    auto result = _aircraftService->getAllAircraft();
    if (!result) {
        updateStatusBar("Error loading aircraft data: " + QString::fromStdString(result.error().message));
        return;
    }

    // Create model
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"Serial", "Model", "Manufacturer", "Total Seats", "Status"});

    // Populate model
    for (const auto& aircraft : result.value()) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::fromStdString(aircraft.getSerial().toString()))
            << new QStandardItem(QString::fromStdString(aircraft.getModel()))
            << new QStandardItem(QString::fromStdString(aircraft.getModel()))
            << new QStandardItem(QString::number(aircraft.getSeatLayout().getSeatCounts().size()))
            << new QStandardItem("Active");
        model->appendRow(row);
    }

    // Set model to table
    _aircraftTable->setModel(model);
    _aircraftTable->resizeColumnsToContents();

    updateStatusBar(QString("Loaded ") + QString::number(result.value().size()) + " aircraft");
}

void AircraftView::updateStatusBar(const QString& message)
{
    _statusLabel->setText(message);
}

void AircraftView::onAddAircraft()
{
    AircraftDialog dialog(_aircraftService, this);
    if (dialog.exec() == QDialog::Accepted) {
        loadAircraftData();
    }
}

void AircraftView::onEditAircraft()
{
    QModelIndex currentIndex = _aircraftTable->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Please select an aircraft to edit.");
        return;
    }

    QString serial = _aircraftTable->model()->data(_aircraftTable->model()->index(currentIndex.row(), 0)).toString();
    auto result = _aircraftService->getAircraft(AircraftSerial::create(serial.toStdString()).value());
    if (!result) {
        QMessageBox::critical(this, "Error", QString::fromStdString(result.error().message));
        return;
    }

    AircraftDialog dialog(_aircraftService, result.value(), this);
    if (dialog.exec() == QDialog::Accepted) {
        loadAircraftData();
    }
}

void AircraftView::onDeleteAircraft()
{
    QModelIndex currentIndex = _aircraftTable->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Please select an aircraft to delete.");
        return;
    }

    QString serial = _aircraftTable->model()->data(_aircraftTable->model()->index(currentIndex.row(), 0)).toString();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete",
        "Are you sure you want to delete aircraft " + serial + "?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        auto result = _aircraftService->deleteAircraft(AircraftSerial::create(serial.toStdString()).value());
        if (!result) {
            QMessageBox::critical(this, "Error", QString::fromStdString(result.error().message));
            return;
        }
        loadAircraftData();
    }
}

void AircraftView::onSearchAircraft()
{
    QString searchText = _searchBox->text().toLower();
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(_aircraftTable->model());
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1); // Search in all columns
    proxyModel->setFilterFixedString(searchText);
    _aircraftTable->setModel(proxyModel);
}

void AircraftView::onRefreshAircraft()
{
    loadAircraftData();
}

void AircraftView::onViewSeats()
{
    QModelIndex currentIndex = _aircraftTable->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Please select an aircraft to view seats.");
        return;
    }

    QString serial = _aircraftTable->model()->data(_aircraftTable->model()->index(currentIndex.row(), 0)).toString();
    auto result = _aircraftService->getAircraft(AircraftSerial::create(serial.toStdString()).value());
    if (!result) {
        QMessageBox::critical(this, "Error", QString::fromStdString(result.error().message));
        return;
    }

    SeatViewDialog dialog(_aircraftService, result.value(), this);
    dialog.exec();
} 