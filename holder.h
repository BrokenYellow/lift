#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <memory>
using namespace std;

class ControlBlock;
class User;
class MainWindow;

class Holder
{
private:
    shared_ptr<ControlBlock> cb;
    shared_ptr<User> user;

public:
    Holder() = default;

    void set_cb(shared_ptr<ControlBlock> cb){
        this->cb = cb;
    }

    void set_user(shared_ptr<User> user){
        this->user = user;
    }

    void reset_cb(){
        cb.reset();
    }

    void reset_user(){
        user.reset();
    }

    shared_ptr<ControlBlock> get_cb(){
        return cb;
    }

    shared_ptr<User> get_user(){
        return user;
    }

    bool cb_empty(){
        return !cb;
    }

    bool user_empty(){
        return !user;
    }
};

#endif // MEDIATOR_H
