#ifndef DOORS_H
#define DOORS_H

#include <QObject>
#include <memory>
#define OC_TIME 5000

class ControlBlock;

class Doors: public QObject
{
    Q_OBJECT
    enum d_state_t{
        OPENED,
        CLOSED,
        OPENING,
        CLOSING,
    };

public:
    Doors();
    ~Doors() = default;

    void connectToControlBlock(const ControlBlock *cb);
    void disconnectFromControlBlock(const ControlBlock *cb);

public slots:
    void onOpening();
    void onClosing();
    void onOpened();
    void onClosed();

signals:
    void DoorsOpened();
    void DoorsClosed();

    void StateChanged(int state);

private:
    d_state_t state;

};

#endif // DOORS_H
