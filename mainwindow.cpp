#include <QObject>
#include <memory>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "user.h"
#include "controlblock.h"
#include "errors.h"

std::string C_States[] = { "Standing", "Moving" };
std::string D_States[] = { "Opened", "Closed", "Opening", "Closing" };
std::string U_States[] = { "On Floor", "Waiting For Elevator", "In Elevator", "Moving" };
std::string CB_States[] = { "Waiting Opened", "Opening Doors", "Closing Doors", "Moving Cabin" };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    holder = std::make_shared<Holder>();
    if (!holder)
        throw MemoryError();
    auto cb = std::make_shared<ControlBlock>();
    if (!cb)
        throw MemoryError();
    connectToControlBlock(cb);

    ui->lblCState->setText(C_States[0].c_str());
    ui->lblDState->setText(D_States[0].c_str());
    ui->lblCBState->setText(CB_States[0].c_str());
    ui->lblUState->setText(U_States[0].c_str());
    ui->lblUFloor->setText("-");
    ui->lblCFloor->setNum(1);
    ui->lblCNextFloor->setText("-");
}

MainWindow::~MainWindow()
{
    delete ui;
    disconnectFromUser();
    disconnectFromControlBlock();
}

void MainWindow::user_call(int floor)
{
    auto user = std::make_shared<User>(floor);
    ui->lblUFloor->setNum(floor);

    shared_ptr<ControlBlock> cb = holder->get_cb();
    user->connectToControlBlock(cb.get());
    cb->connectToUser(user.get());
    connectToUser(user);
    emit CallElevator();
}

void MainWindow::user_goto(int floor)
{
    if (!holder->user_empty())
        emit ChooseFloor(floor);
}

void MainWindow::on_btnCall1_clicked()
{
    user_call(1);
}

void MainWindow::on_btnCall2_clicked()
{
    user_call(2);
}

void MainWindow::on_btnCall3_clicked()
{
    user_call(3);
}

void MainWindow::on_btnCall4_clicked()
{
    user_call(4);
}

void MainWindow::on_btnCall5_clicked()
{
    user_call(5);
}

void MainWindow::on_btnCall6_clicked()
{
    user_call(6);
}

void MainWindow::on_btnGoto1_clicked()
{
    user_goto(1);
}

void MainWindow::on_btnGoto2_clicked()
{
    user_goto(2);
}

void MainWindow::on_btnGoto3_clicked()
{
    user_goto(3);
}

void MainWindow::on_btnGoto4_clicked()
{
    user_goto(4);
}

void MainWindow::on_btnGoto5_clicked()
{
    user_goto(5);
}

void MainWindow::on_btnGoto6_clicked()
{
    user_goto(6);
}

void MainWindow::onStateChanged_C(int state)
{
    ui->lblCState->setText(C_States[state].c_str());
}

void MainWindow::onStateChanged_D(int state)
{
    ui->lblDState->setText(D_States[state].c_str());
}

void MainWindow::onStateChanged_CB(int state)
{
    ui->lblCBState->setText(CB_States[state].c_str());
}

void MainWindow::onFloorChanged_C(int floor)
{
    ui->lblCFloor->setNum(floor);
}

void MainWindow::onNextFloorChanged_C(int next_floor)
{
    ui->lblCNextFloor->setNum(next_floor);
}

void MainWindow::onStateChanged_U(int state)
{
    ui->lblUState->setText(U_States[state].c_str());
    if (state == 0){
        holder->get_user()->disconnectFromControlBlock(holder->get_cb().get());
        holder->reset_user();
    }
}

void MainWindow::onFloorChanged_U(int floor)
{
    ui->lblUFloor->setNum(floor);
}

void MainWindow::connectToControlBlock(shared_ptr<ControlBlock> cb)
{
    holder->set_cb(cb);
    QObject::connect(cb->get_cabin().get(), SIGNAL(StateChanged(int)), this, SLOT(onStateChanged_C(int)));
    QObject::connect(cb->get_cabin().get(), SIGNAL(FloorChanged(int)), this, SLOT(onFloorChanged_C(int)));
    QObject::connect(cb->get_cabin().get(), SIGNAL(NextFloorChanged(int)), this, SLOT(onNextFloorChanged_C(int)));
    QObject::connect(cb->get_doors().get(), SIGNAL(StateChanged(int)), this, SLOT(onStateChanged_D(int)));
    QObject::connect(cb.get(), SIGNAL(StateChanged(int)), this, SLOT(onStateChanged_CB(int)));
}

void MainWindow::connectToUser(shared_ptr<User> user)
{
    holder->set_user(user);
    QObject::connect(user.get(), SIGNAL(StateChanged(int)), this, SLOT(onStateChanged_U(int)));
    QObject::connect(user.get(), SIGNAL(FloorChanged(int)), this, SLOT(onFloorChanged_U(int)));
    QObject::connect(this, SIGNAL(CallElevator()), user.get(), SLOT(onCallElevator()));
    QObject::connect(this, SIGNAL(ChooseFloor(int)), user.get(), SLOT(onChooseFloor(int)));
}

void MainWindow::disconnectFromControlBlock()
{
    shared_ptr<ControlBlock> cb = holder->get_cb();
    QObject::disconnect(cb->get_cabin().get(), SIGNAL(StateChanged(int)), this, SLOT(onStateChanged_C(int)));
    QObject::disconnect(cb->get_cabin().get(), SIGNAL(FloorChanged(int)), this, SLOT(onFloorChanged_C(int)));
    QObject::disconnect(cb->get_cabin().get(), SIGNAL(NextFloorChanged(int)), this, SLOT(onNextFloorChanged_C(int)));
    QObject::disconnect(cb->get_doors().get(), SIGNAL(StateChanged(int)), this, SLOT(onStateChanged_D(int)));
    QObject::disconnect(cb.get(), SIGNAL(StateChanged(int)), this, SLOT(onStateChanged_CB(int)));
    holder->reset_cb();
}

void MainWindow::disconnectFromUser()
{
    shared_ptr<User> user = holder->get_user();
    QObject::disconnect(user.get(), SIGNAL(StateChanged(int)), this, SLOT(onStateChanged_U(int)));
    QObject::disconnect(user.get(), SIGNAL(FloorChanged(int)), this, SLOT(onFloorChanged_U(int)));
    QObject::disconnect(this, SIGNAL(CallElevator()), user.get(), SLOT(onCallElevator()));
    QObject::disconnect(this, SIGNAL(ChooseFloor(int)), user.get(), SLOT(onChooseFloor(int)));
    holder->reset_user();
}
