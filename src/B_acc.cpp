#include "B_acc.h"

#include <pjsua2.hpp>
#include <iostream>
#include <algorithm>
#include <atomic>
#include "BinCall.h"
#include "GenEndpoint.h"
#include "Uri.h"

//using namespace pj;
extern std::atomic_bool  call_from_A_established;

void B_acc::onRegState(pj::OnRegStateParam &prm) {
    __PRETTY__ 
    pj::AccountInfo ai = getInfo();
    std::cout << Clr::gr <<(ai.regIsActive? "======= Register:" : "=== Unregister:")
                << " code=" << prm.code << Clr::def<<std::endl;
}

B_acc::~B_acc(){
    __PRETTY__ 
    if (_call_from_A){
        delete _call_from_A;
        _call_from_A=nullptr;
    }
}

void B_acc::onIncomingCall (pj::OnIncomingCallParam &iprm){
    __PRETTY__ 

    BinCall *call = new BinCall(*this, iprm.callId);
    const pj::CallInfo ci = call->getInfo();

    pj::CallOpParam prm;
    
    std::cout<< Clr::gr<<"=== INCOMING CALL: " <<  ci.remoteUri
        <<", loc uri:"<< ci.localUri
        <<" [" << ci.stateText << "]" << Clr::def<<std::endl;

    //TODO: check if there another call ongoing!!!    
    _connected_2_A = true;
    _call_from_A = call;

    prm.statusCode = PJSIP_SC_OK;;

    call->answer(prm);//TODO: could be busy?
}