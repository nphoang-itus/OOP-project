#ifndef AIRCRAFTVIEW_H
#define AIRCRAFTVIEW_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <memory>
#include "../../services/AircraftService.h"

class AircraftView : public QWidget
{
    Q_OBJECT

public:
    explicit AircraftView(std::shared_ptr<AircraftService> aircraftService, QWidget *parent = nullptr);
    ~AircraftView();

private slots:
    void onAddAircraft();
    void onEditAircraft();
    void onDeleteAircraft();
    void onSearchAircraft();
    void onRefreshAircraft();
    void onViewSeats();

private:
    void setupUI();
    void setupConnections();
    void loadAircraftData();
    void updateStatusBar(const QString& message);

    std::shared_ptr<AircraftService> _aircraftService;
    QTableView* _aircraftTable;
    QLineEdit* _searchBox;
    QPushButton* _addButton;
    QPushButton* _editButton;
    QPushButton* _deleteButton;
    QPushButton* _refreshButton;
    QPushButton* _viewSeatsButton;
    QLabel* _statusLabel;
};

#endif // AIRCRAFTVIEW_H 