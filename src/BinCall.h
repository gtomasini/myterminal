#pragma once
#include <iostream>
#include <atomic>
#include <pjsua2.hpp>
#include "Color.h"

class B_acc;

class BinCall : public pj::Call{
public:
    BinCall (pj::Account &acc, int call_id = PJSUA_INVALID_ID)
    :_myAcc((B_acc *)&acc), Call(acc, call_id){
        __PRETTY__ 
        _ci.state = PJSIP_INV_STATE_NULL;
        _connected_2_A = false;
        _wavPlayer = new pj::AudioMediaPlayer();
    }
    
    ~BinCall() {
        __PRETTY__ 
        if (_wavPlayer)  delete _wavPlayer;
    }

    const pj::CallInfo & getCallInfo() const{
        return _ci;
    }

    bool getConnected_2_A() const {
        return _connected_2_A.load();
    }
    
    virtual void onCallState(pj::OnCallStateParam &prm) override;
    virtual void onCallTransferRequest(pj::OnCallTransferRequestParam &prm) override;
    virtual void onCallReplaceRequest(pj::OnCallReplaceRequestParam &prm) override;
    virtual void onCallMediaState(pj::OnCallMediaStateParam &prm) override;

private:
    B_acc *_myAcc=nullptr;
    pj::AudioMediaPlayer *_wavPlayer=nullptr;
    pj::CallInfo _ci;
    std::atomic_bool _connected_2_A;//after call confirmed
    bool _error=false;
};
