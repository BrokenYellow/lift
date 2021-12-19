#include <QObject>
#include <QTime>
#include <QTimer>
#include "cabin.h"
#include "controlblock.h"

Cabin::Cabin()
    : state(STANDING), floor(1)
{}

void Cabin::connectToControlBlock(const ControlBlock *cb)
{
    QObject::connect(cb, SIGNAL(MoveCabin(int)), this, SLOT(onMove(int)));
}

void Cabin::disconnectFromControlBlock(const ControlBlock *cb)
{
    QObject::connect(cb, SIGNAL(MoveCabin(int)), this, SLOT(onMove(int)));
}

void Cabin::onMove(int next_floor)
{
    if (state == STANDING){
        state = MOVING;
        emit StateChanged(state);
        this->next_floor = next_floor;
        emit NextFloorChanged(next_floor);
        QTimer::singleShot(M_TIME*abs(next_floor-floor), this, SLOT(onStop()));
    }
}

void Cabin::onStop()
{
    if (state == MOVING){
        state = STANDING;
        emit StateChanged(state);
        floor = next_floor;
        emit FloorChanged(floor);
        emit CabinArrived();
    }
}

/*
void Cabin::move_cabin()
{
    if (floor == next_floor){
        state = STANDING;
        emit StateChanged(state);
        emit CabinArrived();
    }
    else
        QTimer::singleShot(M_TIME, this, SLOT(onTimeoutMoving()));
}

void Cabin::onTimeoutMoving()
{
    if (floor < next_floor)
        floor++;
    else
        floor--;
    emit FloorChanged(floor);
    move_cabin();
}
*/

