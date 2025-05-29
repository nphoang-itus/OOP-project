#include "MainWindow.h"
#include <QMenu>
#include <QAction>
#include <QMessageBox>

MainWindow::MainWindow(std::shared_ptr<AircraftService> aircraftService, QWidget *parent)
    : QMainWindow(parent)
    , _aircraftService(aircraftService)
{
    setupUI();
    setupConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Set window properties
    setWindowTitle("Airlines Management System");
    resize(1024, 768);

    // Create stacked widget for different views
    _stackedWidget = new QStackedWidget(this);
    setCentralWidget(_stackedWidget);

    // Initialize views
    _aircraftView = new AircraftView(_aircraftService, this);
    _stackedWidget->addWidget(_aircraftView);

    // Create menu bar
    QMenu* fileMenu = menuBar()->addMenu("&File");
    QAction* exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);

    QMenu* viewMenu = menuBar()->addMenu("&View");
    QAction* aircraftAction = viewMenu->addAction("&Aircraft");
    QAction* flightsAction = viewMenu->addAction("&Flights");
    QAction* passengersAction = viewMenu->addAction("&Passengers");
    QAction* ticketsAction = viewMenu->addAction("&Tickets");

    // Create toolbar
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->addAction(aircraftAction);
    toolbar->addAction(flightsAction);
    toolbar->addAction(passengersAction);
    toolbar->addAction(ticketsAction);

    // Set initial view
    switchToView(0);
}

void MainWindow::setupConnections()
{
    // Connect menu actions
    connect(findChild<QAction*>("E&xit"), &QAction::triggered, this, &MainWindow::onActionExit);
    connect(findChild<QAction*>("&Aircraft"), &QAction::triggered, this, &MainWindow::onActionAircraft);
    connect(findChild<QAction*>("&Flights"), &QAction::triggered, this, &MainWindow::onActionFlights);
    connect(findChild<QAction*>("&Passengers"), &QAction::triggered, this, &MainWindow::onActionPassengers);
    connect(findChild<QAction*>("&Tickets"), &QAction::triggered, this, &MainWindow::onActionTickets);
}

void MainWindow::switchToView(int index)
{
    _stackedWidget->setCurrentIndex(index);
}

void MainWindow::onActionExit()
{
    close();
}

void MainWindow::onActionAircraft()
{
    switchToView(0);
}

void MainWindow::onActionFlights()
{
    // TODO: Implement flights view
    QMessageBox::information(this, "Not Implemented", "Flights view is not implemented yet.");
}

void MainWindow::onActionPassengers()
{
    // TODO: Implement passengers view
    QMessageBox::information(this, "Not Implemented", "Passengers view is not implemented yet.");
}

void MainWindow::onActionTickets()
{
    // TODO: Implement tickets view
    QMessageBox::information(this, "Not Implemented", "Tickets view is not implemented yet.");
} 