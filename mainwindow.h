#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include "holder.h"
#include "user.h"
#include "controlblock.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void user_call(int floor);
    void user_goto(int floor);
    void connectToControlBlock(shared_ptr<ControlBlock> cb);
    void connectToUser(shared_ptr<User> user);
    void disconnectFromControlBlock();
    void disconnectFromUser();

private slots:
    void onStateChanged_C(int state);
    void onStateChanged_D(int state);
    void onStateChanged_CB(int state);
    void onFloorChanged_C(int floor);
    void onNextFloorChanged_C(int next_floor);
    void onFloorChanged_U(int floor);
    void onStateChanged_U(int state);

    void on_btnGoto1_clicked();
    void on_btnGoto2_clicked();
    void on_btnGoto3_clicked();
    void on_btnGoto4_clicked();
    void on_btnGoto5_clicked();
    void on_btnGoto6_clicked();
    void on_btnCall1_clicked();
    void on_btnCall2_clicked();
    void on_btnCall3_clicked();
    void on_btnCall4_clicked();
    void on_btnCall5_clicked();
    void on_btnCall6_clicked();

signals:
    void CallElevator();
    void ChooseFloor(int floor);

private:
    Ui::MainWindow *ui;
    shared_ptr<Holder> holder;
};

#endif // MAINWINDOW_H
