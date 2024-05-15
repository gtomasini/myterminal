#include "Call2B.h"
#include "GenEndpoint.h"
#include "Color.h"
#include "Uri.h"

void Call2B::onCallState (pj::OnCallStateParam &prm){
    __PRETTY__ 
    PJ_UNUSED_ARG(prm);

    _ci = getInfo();//call info
    std::cout << Clr::gr << "==== CALL Info, RemURI:" <<  _ci.remoteUri <<
        ", LocURI:"<< _ci.localUri<<
        " [" << _ci.stateText << "]" << Clr::def<<std::endl;
    
    if (_ci.state == PJSIP_INV_STATE_CONFIRMED) {
        //TODO: something
        _connected_2_C = true;
    }
    else if (_ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        //_myAcc->removeCall(this);
        //delete this;// delete the call?
    }
}

/*
 *see https://docs.pjsip.org/en/2.10/pjsua2/call.html
 */
void Call2B::onCallMediaState(pj::OnCallMediaStateParam &prm){
    __PRETTY__ 
    PJ_UNUSED_ARG(prm);

     _ci = getInfo();//call info
    std::cout << Clr::gr << "==== CI on onCallMediaState: RemUri:" <<  _ci.remoteUri <<
        ", LocUri:"<< _ci.localUri<<
        " [" << _ci.stateText << "]" << Clr::def<<std::endl;

    pj::AudioMedia aud_med;
    try {
        aud_med = getAudioMedia(-1);//get the first audio media
    } catch(...) {
        std::cerr << "ERROR: failed to get audio media!" << std::endl;
        _error = true;
        return;
    }

    std::cout<<Clr::bl<<"### we are "<<_ci.localUri<<" subscriber calling to "<<
            _ci.remoteUri<<Clr::def<<std::endl;
    pj::AudioMediaRecorder wav_writer;
    try {
        wav_writer.createRecorder("A_call.wav");
    }
    catch(...) {
        std::cerr << "ERROR: creating the recorder" << std::endl;
        return;
    }
    std::cout<<Clr::bl<<"### connecting call audio media to sound device."<<Clr::def<<std::endl;
    //aud_med.startTransmit(wav_writer);//record
    pj::AudDevManager& aud_dev_mgr = pj::Endpoint::instance().audDevManager();
    aud_med.startTransmit(aud_dev_mgr.getPlaybackDevMedia());
    aud_dev_mgr.getCaptureDevMedia().startTransmit(aud_med);//DevMedia is the mic.
}

void Call2B::onCallTransferRequest(pj::OnCallTransferRequestParam &prm){
    __PRETTY__ 
    prm.newCall = new Call2B(*_myAcc);    // Create new Call for call transfer 
}

void Call2B::onCallReplaceRequest(pj::OnCallReplaceRequestParam &prm){
    __PRETTY__ 
    prm.newCall = new Call2B(*_myAcc);// Create new Call for call replace 
}
