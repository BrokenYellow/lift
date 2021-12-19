#ifndef CONTROLBLOCK_H
#define CONTROLBLOCK_H

#include <QObject>
#include <queue>
#include <memory>
#include "user.h"
#include "doors.h"
#include "cabin.h"

class ControlBlock: public QObject
{
    Q_OBJECT
    enum cb_state_t{
        WAITING_OPENED,
        OPENING_DOORS,
        CLOSING_DOORS,
        MOVING_CABIN,
    };

public:
    ControlBlock();
    ~ControlBlock() = default;

    void connectToUser(const User *user);
    void connectToDoors();
    void connectToCabin();
    void disconnectFromUser(const User *user);
    void disconnectFromDoors();
    void disconnectFromCabin();

    std::shared_ptr<Doors> get_doors() { return doors; }
    std::shared_ptr<Cabin> get_cabin() { return cabin; }

public slots:
    void onCloseDoors(int floor);
    void onMoveCabin();
    void onOpenDoors();
    void onWait();

signals:
    void ElevatorArrived();
    void OpenDoors();
    void CloseDoors();
    void MoveCabin(int next_floor);

    void StateChanged(int state);

private:
    cb_state_t state;
    int floor;
    std::shared_ptr<Doors> doors;
    std::shared_ptr<Cabin> cabin;
};

#endif // CONTROLBLOCK_H
