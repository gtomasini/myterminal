#include <pjsua2.hpp>
#include <iostream>
#include <algorithm>
#include <atomic>

#include "C_acc.h"
#include "CinCall.h"
#include "GenEndpoint.h"
#include "Uri.h"


void C_acc::onRegState(pj::OnRegStateParam &prm) {
    __PRETTY__ 
    pj::AccountInfo ai = getInfo();
    std::cout << Clr::gr <<(ai.regIsActive? "======= Register:" : "=== Unregister:")
                << " code=" << prm.code << Clr::def<<std::endl;
}

C_acc::~C_acc(){
    __PRETTY__ 
    std::cout << Clr::gr << "=== account and calls are being deleted: calls num="
                        << _calls_list.size() << Clr::def<<std::endl;
    for (auto call:_calls_list)
        delete(call);

    _calls_list.clear();

}

void C_acc::removeCall (const CinCall *call){
    __PRETTY__ 

    const auto found=std::find(_calls_list.begin(), _calls_list.end(), call);
    if (found != _calls_list.end()){
        delete(call);
        _calls_list.erase(found);
    }
}

void C_acc::onIncomingCall (pj::OnIncomingCallParam &iprm){
    __PRETTY__ 

    CinCall *call = new CinCall(*this, iprm.callId);
    const pj::CallInfo ci = call->getInfo();

    pj::CallOpParam prm;
    
    std::cout<< Clr::gr<<"=== INCOMING CALL: " <<  ci.remoteUri
        <<", loc uri:"<< ci.localUri
        <<" [" << ci.stateText << "]" << Clr::def<<std::endl;
    
    _calls_list.push_back(call);
    prm.statusCode = PJSIP_SC_OK;;

    //TODO: conditional answer?
    call->answer(prm);
}