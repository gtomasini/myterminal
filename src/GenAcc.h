#pragma once
#include <iostream>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <pjsua2.hpp>
#include "Color.h"

class GenCall;

// Subclass to extend the Account and get notifications etc.
class GenAcc : public pj::Account {
public:
    GenAcc(){
        __PRETTY__ 
        _connected_2_A = false;
    }
    virtual void onRegState(pj::OnRegStateParam &prm) override;
    void removeCall(const pj::Call *call);
    virtual void onIncomingCall(pj::OnIncomingCallParam &iprm) override;

    inline bool getIsConnected_2_A() const {
        return _connected_2_A.load();
    }

    inline GenCall *getCallFromA() const{
        __PRETTY__
        std::unique_lock<std::mutex> lck(_mtx);
        return _call_from_A;
    }

    GenCall *getCallFromAsync() const;

    //pj::CallInfo &getCIfromA() const {
    //    return _call_from_A->getCallInfo();
    //}

    void resetCallFromA();

    virtual ~GenAcc();

private:
    std::vector<pj::Call *> _calls_list;//TODO: change vector for list
    std::atomic_bool _connected_2_A;//after (A)call confirmed
    GenCall *_call_from_A=nullptr;
    mutable std::mutex _mtx;
    mutable std::condition_variable _cv;
};