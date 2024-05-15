#pragma once
#include <iostream>
#include <atomic>
#include <pjsua2.hpp>
#include "Color.h"

class BinCall;

// Subclass to extend the Account and get notifications etc.
class B_acc : public pj::Account {
public:
    B_acc(){
        __PRETTY__ 
        _connected_2_A = false;
    }
    virtual void onRegState(pj::OnRegStateParam &prm) override;

    virtual void onIncomingCall(pj::OnIncomingCallParam &iprm) override;

    inline bool getIsConnected_2_A() const {
        return _connected_2_A.load();
    }

    inline BinCall *getAcall()const{
        return _call_from_A;
    }

    void deleteAcall();

    virtual ~B_acc();

private:
    std::atomic_bool _connected_2_A;//after (A)call confirmed
    BinCall *_call_from_A=nullptr;
};