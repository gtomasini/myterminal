#pragma once
#include <iostream>
#include <atomic>
#include <pjsua2.hpp>
#include "Color.h"

class CinCall;

// Subclass to extend the Account and get notifications etc.
class C_acc : public pj::Account {
public:
    C_acc(){
        __PRETTY__ 
    }
    virtual void onRegState(pj::OnRegStateParam &prm) override;
    void removeCall(const CinCall *call);
    virtual void onIncomingCall(pj::OnIncomingCallParam &iprm) override;

    inline std::vector<CinCall *> getCallsList() const{ return _calls_list;}
    virtual ~C_acc();

private:
    std::vector<CinCall *> _calls_list;//TODO: change vector for list
};