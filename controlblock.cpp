#include <QObject>
#include <assert.h>
#include <iostream>
#include "controlblock.h"
#include "doors.h"
#include "cabin.h"
#include "errors.h"

ControlBlock::ControlBlock()
    : state(WAITING_OPENED), floor(1)
{
    doors = std::make_shared<Doors>();
    if (!doors)
        throw MemoryError();
    connectToDoors();
    doors->connectToControlBlock(this);
    cabin = std::make_shared<Cabin>();
    if (!cabin)
        throw MemoryError();
    connectToCabin();
    cabin->connectToControlBlock(this);
}

void ControlBlock::connectToUser(const User *user)
{
    QObject::connect(user, SIGNAL(MoveElevator(int)), this, SLOT(onCloseDoors(int)));
}

void ControlBlock::disconnectFromUser(const User *user)
{
    QObject::disconnect(user, SIGNAL(MoveElevator(int)), this, SLOT(onCloseDoors(int)));
}

void ControlBlock::connectToDoors()
{
    QObject::connect(doors.get(), SIGNAL(DoorsOpened()), this, SLOT(onWait()));
    QObject::connect(doors.get(), SIGNAL(DoorsClosed()), this, SLOT(onMoveCabin()));
}

void ControlBlock::disconnectFromDoors()
{
    QObject::disconnect(doors.get(), SIGNAL(DoorsOpened()), this, SLOT(onWait()));
    QObject::disconnect(doors.get(), SIGNAL(DoorsClosed()), this, SLOT(onMoveCabin()));
}

void ControlBlock::connectToCabin()
{
    QObject::connect(cabin.get(), SIGNAL(CabinArrived()), this, SLOT(onOpenDoors()));
}

void ControlBlock::disconnectFromCabin()
{
    QObject::disconnect(cabin.get(), SIGNAL(CabinArrived()), this, SLOT(onOpenDoors()));
}

void ControlBlock::onCloseDoors(int floor)
{
    if (state == WAITING_OPENED){
        if (this->floor == floor){
            emit ElevatorArrived();
        }
        else{
            state = CLOSING_DOORS;
            emit StateChanged(state);
            this->floor = floor;
            emit CloseDoors();
        }
    }
}

void ControlBlock::onMoveCabin()
{
    if (state == CLOSING_DOORS){
        state = MOVING_CABIN;
        emit StateChanged(state);
        emit MoveCabin(floor);
    }
}

void ControlBlock::onOpenDoors()
{
    if (state == MOVING_CABIN){
        state = OPENING_DOORS;
        emit StateChanged(state);
        emit OpenDoors();
    }
}

void ControlBlock::onWait()
{
    if (state == OPENING_DOORS){
        state = WAITING_OPENED;
        emit StateChanged(state);
        emit ElevatorArrived();
    }
}
