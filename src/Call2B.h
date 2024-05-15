#pragma once
#include <iostream>
#include <atomic>
#include <pjsua2.hpp>
#include "Color.h"
#include "GenAcc.h"

//class MyAccount;
   
class Call2B : public pj::Call{
public:
    Call2B (pj::Account &acc, int call_id = PJSUA_INVALID_ID)
            :_myAcc((GenAcc *)&acc), Call(acc, call_id){
        __PRETTY__ 
        _ci.state = PJSIP_INV_STATE_NULL;
        _connected_2_B = false;
    }
    
    virtual ~Call2B() {
        __PRETTY__ 
    }

    const pj::CallInfo & getCallInfo() const{
        return _ci;
    }

    bool getConnected_2_B() const {
        return _connected_2_B.load();
    }
    
    virtual void onCallState(pj::OnCallStateParam &prm) override;
    virtual void onCallTransferRequest(pj::OnCallTransferRequestParam &prm) override;
    virtual void onCallReplaceRequest(pj::OnCallReplaceRequestParam &prm) override;
    virtual void onCallMediaState(pj::OnCallMediaStateParam &prm) override;

private:
    GenAcc *_myAcc=nullptr;
    pj::CallInfo _ci;
    std::atomic_bool _connected_2_B;//after call confirmed
    bool _error=false;

};