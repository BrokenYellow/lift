#ifndef USER_H
#define USER_H

#include <QObject>
#include <QTimer>
#include <memory>

class ControlBlock;

class User: public QObject
{
    Q_OBJECT
    enum u_state_t{
        ON_FLOOR,
        WAITING_FOR_ELEVATOR,
        IN_ELEVATOR,
        MOVING,
    };
public:
    User();
    User(int floor);
    ~User() = default;

    void connectToControlBlock(ControlBlock *cb);
    void disconnectFromControlBlock(ControlBlock *cb);

public slots:
    void onCallElevator();
    void onChooseFloor(int next_floor);
    void onEnterElevator();
    void onLeaveElevator();

signals:
    void MoveElevator(int floor);

    void StateChanged(int state);
    void FloorChanged(int floor);

private:
    u_state_t state;
    int floor;
    int next_floor;
};

#endif // USER_H
