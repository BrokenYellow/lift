#include <QObject>
#include <QTime>
#include <QTimer>
#include "doors.h"
#include "controlblock.h"

Doors::Doors()
    : state(OPENED)
{}

void Doors::connectToControlBlock(const ControlBlock *cb)
{
    QObject::connect(cb, SIGNAL(OpenDoors()), this, SLOT(onOpening()));
    QObject::connect(cb, SIGNAL(CloseDoors()), this, SLOT(onClosing()));
}

void Doors::disconnectFromControlBlock(const ControlBlock *cb)
{
    QObject::disconnect(cb, SIGNAL(OpenDoors()), this, SLOT(onOpening()));
    QObject::disconnect(cb, SIGNAL(CloseDoors()), this, SLOT(onClosing()));
}

void Doors::onOpening()
{
    if (state == CLOSED){
        state = OPENING;
        emit StateChanged(state);
        QTimer::singleShot(OC_TIME, this, SLOT(onOpened()));
    }
}

void Doors::onClosing()
{
    if (state == OPENED){
        state = CLOSING;
        emit StateChanged(state);
        QTimer::singleShot(OC_TIME, this, SLOT(onClosed()));
    }
}

void Doors::onOpened()
{
    if (state == OPENING){
        state = OPENED;
        emit StateChanged(state);
        emit DoorsOpened();
    }
}

void Doors::onClosed()
{
    if (state == CLOSING){
        state = CLOSED;
        emit StateChanged(state);
        emit DoorsClosed();
    }
}
