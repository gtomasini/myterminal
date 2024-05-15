#include <pjsua2.hpp>
#include <iostream>
#include <algorithm>
#include <atomic>
#include "GenAcc.h"
#include "GenCall.h"
#include "GenEndpoint.h"
#include "Uri.h"

//using namespace pj;
extern std::atomic_bool  call_from_A_established;

void GenAcc::onRegState(pj::OnRegStateParam &prm) {
    __PRETTY__ 
    pj::AccountInfo ai = getInfo();
    std::cout << Clr::gr <<(ai.regIsActive? "======= Register:" : "=== Unregister:")
                << " code=" << prm.code << Clr::def<<std::endl;
}


void GenAcc::resetCallFromA(){
    __PRETTY__ 
    _connected_2_A = false;
    //std::lock_guard<std::mutex> guard(_mtx_call_from_A);

    {
        std::unique_lock<std::mutex> lck(_mtx);
        if (_call_from_A)
            delete (_call_from_A);
        _call_from_A = nullptr;
    }
}

GenAcc::~GenAcc(){
    __PRETTY__ 
    std::cout << Clr::gr << "=== account and calls are being deleted: calls num="
                        << _calls_list.size() << Clr::def<<std::endl;
    for (auto call:_calls_list)
        delete(call);

    _calls_list.clear();

    if (_call_from_A){
        delete _call_from_A;
        _call_from_A=nullptr;
    }
}

void GenAcc::removeCall (const pj::Call *call){
    __PRETTY__ 

    const auto found=std::find(_calls_list.begin(), _calls_list.end(), call);
    if (found != _calls_list.end()){
        delete(call);
        _calls_list.erase(found);
    }
}

GenCall *GenAcc::getCallFromAsync() const {
    __PRETTY__
    std::unique_lock<std::mutex> lck(_mtx);
    while (!_call_from_A) _cv.wait(lck);

    return _call_from_A;
}

void GenAcc::onIncomingCall (pj::OnIncomingCallParam &iprm){
    __PRETTY__ 

    pj::Call *call = new GenCall(*this, iprm.callId);
    const pj::CallInfo ci = call->getInfo();

    auto is_A_rem_and_B_loc=[=]() {
        return ci.localUri == Uri::B && ci.remoteUri == Uri::A || 
           ci.localUri == Uri::B_Full && ci.remoteUri == Uri::A_Full;
    };
    pj::CallOpParam prm;
    
    std::cout<< Clr::gr<<"=== INCOMING CALL: " <<  ci.remoteUri
        <<", Loc Uri:"<< ci.localUri <<" [" << ci.stateText << "]" << Clr::def<<std::endl;
    

    if (is_A_rem_and_B_loc()){
        std::cout<< Clr::bl<<"### subscriber A is calling me B !!!" <<  Clr::def<<std::endl;
        if (_connected_2_A == true ) {
            std::cout<< Clr::bl<<"### I'm already connected to A (busy), rejecting calll !!!" <<  Clr::def<<std::endl;
            prm.statusCode=PJSIP_SC_DECLINE;
        }
        else{
            std::cout<< Clr::bl<<"### Acept incomming calll !!!" <<  Clr::def<<std::endl;
            prm.statusCode = PJSIP_SC_OK;;
            call->answer(prm);
            {
                std::unique_lock<std::mutex> lck(_mtx);
                _call_from_A = dynamic_cast<GenCall*>(call);
                _cv.notify_one();
            }
            _connected_2_A = true;
            return;
        }
    }
    else{
        std::cout<< Clr::bl<<"### don't know caller, rejecting !!!" <<  Clr::def<<std::endl;
        prm.statusCode=PJSIP_SC_DECLINE;
    }
    //_calls_list.push_back(call);

    //prm.statusCode = PJSIP_SC_OK;;
    call->answer(prm);
}