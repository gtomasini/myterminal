#include "CinCall.h"
#include "C_acc.h"
#include "GenEndpoint.h"
#include "Color.h"
#include "Uri.h"

void CinCall::onCallState (pj::OnCallStateParam &prm){
    __PRETTY__ 
    PJ_UNUSED_ARG(prm);

    _ci = getInfo();//call info
    std::cout << Clr::gr << "==== CALL Info, RemURI:" <<  _ci.remoteUri <<
        ", LocURI:"<< _ci.localUri<<
        " [" << _ci.stateText << "]" << Clr::def<<std::endl;
    
    if (_ci.state == PJSIP_INV_STATE_CONFIRMED) {
        //TODO: something
        _connected_2_B = true;
    }
    else if (_ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        //_myAcc->removeCall(this);
        //delete this;// delete the call?
    }
}

/*
 *see https://docs.pjsip.org/en/2.10/pjsua2/call.html
 */
void CinCall::onCallMediaState(pj::OnCallMediaStateParam &prm){
    __PRETTY__ 
    PJ_UNUSED_ARG(prm);

    _ci = getInfo();//call info
    std::cout << Clr::gr << "==== CI on onCallMediaState: Rem Uri:" <<  _ci.remoteUri <<
        ", Loc Uri:"<< _ci.localUri<<" [" << _ci.stateText << "]" << Clr::def<<std::endl;

    //pj::AudioMedia aud_med;
    //try {
    //    aud_med = getAudioMedia(-1);//get the first audio media
    //} catch(...) {
    //    std::cerr << "ERROR: failed to get audio media!!!" << std::endl;
    //    _error = true;
    //    return;
    //}

    {
        //std::cout<<Clr::bl<<"### we are "<<_ci.localUri<<" subscriber called by "<<
        //                _ci.remoteUri<<Clr::def<<std::endl;
        pj::AudioMedia& speaker_media = pj::Endpoint::instance().audDevManager().getPlaybackDevMedia();
        pj::AudioMedia& mic_media = pj::Endpoint::instance().audDevManager().getCaptureDevMedia();
        // audio loop
        mic_media.startTransmit(speaker_media);
    }

}

void CinCall::onCallTransferRequest(pj::OnCallTransferRequestParam &prm){
    __PRETTY__ 
    prm.newCall = new CinCall(*_myAcc);    // Create new Call for call transfer 
}

void CinCall::onCallReplaceRequest(pj::OnCallReplaceRequestParam &prm){
    __PRETTY__ 
    prm.newCall = new CinCall(*_myAcc);// Create new Call for call replace 
}


