#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <memory>
#include "../services/AircraftService.h"
#include "views/AircraftView.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::shared_ptr<AircraftService> aircraftService, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onActionExit();
    void onActionAircraft();
    void onActionFlights();
    void onActionPassengers();
    void onActionTickets();

private:
    void setupUI();
    void setupConnections();
    void switchToView(int index);

    QStackedWidget* _stackedWidget;
    std::shared_ptr<AircraftService> _aircraftService;
    AircraftView* _aircraftView;
    // TODO: Add other views (FlightView, PassengerView, TicketView)
};

#endif // MAINWINDOW_H 