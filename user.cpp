#include <QObject>
#include <QTime>
#include <QTimer>
#include "errors.h"
#include "user.h"
#include "controlblock.h"

User::User()
    : state(ON_FLOOR), floor(1)
{}

User::User(int floor)
    : state(ON_FLOOR), floor(floor)
{}

void User::connectToControlBlock(ControlBlock *cb)
{
    QObject::connect(cb, SIGNAL(ElevatorArrived()), this, SLOT(onEnterElevator()));
    QObject::connect(cb, SIGNAL(ElevatorArrived()), this, SLOT(onLeaveElevator()));
}

void User::disconnectFromControlBlock(ControlBlock *cb)
{
    QObject::disconnect(cb, SIGNAL(ElevatorArrived()), this, SLOT(onEnterElevator()));
    QObject::disconnect(cb, SIGNAL(ElevatorArrived()), this, SLOT(onLeaveElevator()));
}

void User::onCallElevator()
{
    if (state == ON_FLOOR){
        state = WAITING_FOR_ELEVATOR;
        emit StateChanged(state);
        emit MoveElevator(floor);
    }
}

void User::onChooseFloor(int next_floor)
{
    if (state == IN_ELEVATOR){
        state = MOVING;
        emit StateChanged(state);
        this->next_floor = next_floor;
        emit MoveElevator(next_floor);
    }
}

void User::onEnterElevator()
{
    if (state == WAITING_FOR_ELEVATOR){
        state = IN_ELEVATOR;
        emit StateChanged(state);
    }
}

void User::onLeaveElevator()
{
    if (state == MOVING){
        state = ON_FLOOR;
        emit StateChanged(state);
        floor = next_floor;
        emit FloorChanged(floor);
    }
}
