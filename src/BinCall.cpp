#include "BinCall.h"
#include "GenEndpoint.h"
#include "Color.h"
#include "Uri.h"
#include "B_acc.h"

void BinCall::onCallState (pj::OnCallStateParam &prm){
    __PRETTY__ 
    PJ_UNUSED_ARG(prm);

    _ci = getInfo();//call info
    std::cout << Clr::gr << "==== CALL Info, RemURI:" <<  _ci.remoteUri <<
        ", LocURI:"<< _ci.localUri<<
        " [" << _ci.stateText << "]" << Clr::def<<std::endl;
    
    if (_ci.state == PJSIP_INV_STATE_CONFIRMED) {
        //TODO: something
        _connected_2_A = true;
    }
    else if (_ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        //_myAcc->removeCall(this);
        //delete this;// delete the call?
    }
}

/*
 *see https://docs.pjsip.org/en/2.10/pjsua2/call.html
 */
void BinCall::onCallMediaState(pj::OnCallMediaStateParam &prm){
    __PRETTY__ 
    PJ_UNUSED_ARG(prm);

    pj::AudioMedia aud_med;
    try {
        aud_med = getAudioMedia(-1);//get the first audio media
    } catch(...) {
        std::cerr << "ERROR: failed to get audio media!" << std::endl;
        _error = true;
        return;
    }


}


void BinCall::onCallTransferRequest(pj::OnCallTransferRequestParam &prm){
    __PRETTY__ 
    prm.newCall = new BinCall(*_myAcc);// Create new Call for call transfer 
}

void BinCall::onCallReplaceRequest(pj::OnCallReplaceRequestParam &prm){
    __PRETTY__ 
    prm.newCall = new BinCall(*_myAcc);// Create new Call for call replace 
}



