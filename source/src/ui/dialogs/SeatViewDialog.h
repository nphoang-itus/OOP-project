#ifndef SEATVIEWDIALOG_H
#define SEATVIEWDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <memory>
#include "../../services/AircraftService.h"

class SeatViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SeatViewDialog(std::shared_ptr<AircraftService> aircraftService, const Aircraft& aircraft, QWidget *parent = nullptr);
    ~SeatViewDialog();

private slots:
    void onSeatClassChanged(const QString& seatClass);
    void onSeatSelected(const QString& seatNumber);

private:
    void setupUI();
    void setupConnections();
    void loadSeatClasses();
    void loadSeats(const QString& seatClass);
    void checkSeatAvailability(const QString& seatNumber);

    std::shared_ptr<AircraftService> _aircraftService;
    Aircraft _aircraft;
    QComboBox* _seatClassCombo;
    QListWidget* _seatList;
    QLabel* _statusLabel;
};

#endif // SEATVIEWDIALOG_H 