#pragma once
#include <iostream>
#include <atomic>
#include <pjsua2.hpp>
#include "Color.h"
#include "GenAcc.h"

//class MyAccount;

class GenCall : public pj::Call{
public:
    GenCall (pj::Account &acc, int call_id = PJSUA_INVALID_ID)
        :_myAcc((GenAcc *)&acc), Call(acc, call_id){
        __PRETTY__ 
        _ci.state = PJSIP_INV_STATE_NULL;
        _connected_2_C = false;
        _wavPlayer = new pj::AudioMediaPlayer();
    }
    
    void setCall2Bcase(){
        call_2_B_case=true;
    }

    ~GenCall() {
        __PRETTY__ 
        if (_wavPlayer)  delete _wavPlayer;
    }

    const pj::CallInfo & getCallInfo() const{
        return _ci;
    }

    bool getConnected_2_C() const {
        return _connected_2_C.load();
    }
    
    virtual void onCallState(pj::OnCallStateParam &prm) override;
    virtual void onCallTransferRequest(pj::OnCallTransferRequestParam &prm) override;
    virtual void onCallReplaceRequest(pj::OnCallReplaceRequestParam &prm) override;
    virtual void onCallMediaState(pj::OnCallMediaStateParam &prm) override;

private:
    GenAcc *_myAcc=nullptr;
    bool    call_2_B_case=false;

    pj::AudioMediaPlayer *_wavPlayer=nullptr;
    pj::CallInfo _ci;
    std::atomic_bool _connected_2_C;//after call confirmed
    bool _error=false;

    static constexpr const char *_wavFile="data/input.16.wav";
};
