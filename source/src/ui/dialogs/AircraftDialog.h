#ifndef AIRCRAFTDIALOG_H
#define AIRCRAFTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <memory>
#include "../../services/AircraftService.h"
#include "../../core/entities/Aircraft.h"

class AircraftDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AircraftDialog(std::shared_ptr<AircraftService> aircraftService, QWidget *parent = nullptr);
    explicit AircraftDialog(std::shared_ptr<AircraftService> aircraftService, const Aircraft& aircraft, QWidget *parent = nullptr);
    ~AircraftDialog();

    Aircraft getAircraft() const;

private slots:
    void onAccept();
    void validateInput();

private:
    void setupUI();
    void setupConnections();
    void loadAircraftData(const Aircraft& aircraft);

    std::shared_ptr<AircraftService> _aircraftService;
    QLineEdit* _serialEdit;
    QLineEdit* _modelEdit;
    QLineEdit* _manufacturerEdit;
    QLineEdit* _seatLayoutEdit;
    QPushButton* _okButton;
    QPushButton* _cancelButton;
    bool _isEditMode;
    int _aircraftId;
};

#endif // AIRCRAFTDIALOG_H 