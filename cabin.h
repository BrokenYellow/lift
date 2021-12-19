#ifndef CABIN_H
#define CABIN_H

#include <QObject>
#include <memory>
#define M_TIME 3000

class ControlBlock;

class Cabin: public QObject
{
    Q_OBJECT
    enum c_state_t{
        STANDING,
        MOVING,
    };

public:
    Cabin();
    ~Cabin() = default;

    void connectToControlBlock(const ControlBlock *cb);
    void disconnectFromControlBlock(const ControlBlock *cb);

public slots:
    void onMove(int next_floor);
    void onStop();

signals:
    void CabinArrived();

    void StateChanged(int state);    
    void FloorChanged(int floor);
    void NextFloorChanged(int next_floor);

private:
    c_state_t state;
    int floor;
    int next_floor;
};

#endif // CABIN_H
