#include "GenCall.h"
#include "GenEndpoint.h"
#include "Color.h"
#include "Uri.h"

void GenCall::onCallState (pj::OnCallStateParam &prm)
{
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
void GenCall::onCallMediaState(pj::OnCallMediaStateParam &prm){
    __PRETTY__ 
    PJ_UNUSED_ARG(prm);

    auto is_A_rem_and_B_loc=[=]() {
        return _ci.localUri == Uri::B && _ci.remoteUri == Uri::A || 
        _ci.localUri == Uri::B_Full && _ci.remoteUri == Uri::A_Full;
    };

    auto is_A_loc_and_B_rem=[=]() {
        return _ci.localUri == Uri::A && _ci.remoteUri == Uri::B||
        _ci.localUri == Uri::A_Full && _ci.remoteUri == Uri::B_Full;
    };

    auto is_C_rem_and_B_loc=[=]() {
        return _ci.localUri == Uri::B && _ci.remoteUri == Uri::C || 
        _ci.localUri == Uri::B_Full && _ci.remoteUri == Uri::C_Full;
    };
    
    auto is_C_loc_and_B_rem=[=]() {
        return _ci.localUri == Uri::C && _ci.remoteUri == Uri::B || 
        _ci.localUri == Uri::C_Full && _ci.remoteUri == Uri::B_Full;
    };

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

    /*
    if (is_A_rem_and_B_loc()) {
        std::cout << Clr::bl << "### we are B subscriber, and caller is A..." << Clr::def << std::endl;

        try {
            _wavPlayer->createPlayer (_wavFile, 0);
        } catch (...) {
            std::cerr << "ERROR: failed opening wav file:\""<<_wavFile<<"\" !!" << std::endl;
            delete _wavPlayer;
            _wavPlayer = nullptr;
        }

        // this will connect the wav file to the call audio media
        _wavPlayer->startTransmit(aud_med);

        // this will connect the call audio media to the sound device/speaker
        std::cout<<Clr::bl<<"### playing a tone to caller A..."<<Clr::def<<std::endl;
        pj::AudioMedia& play_dev_med = GenEndpoint::instance().audDevManager().getPlaybackDevMedia();//speaker
        aud_med.startTransmit (play_dev_med);
    }*/
    if (call_2_B_case){
        std::cout<<Clr::bl<<"### CALL A -> B: we are "<<_ci.localUri<<" subscriber calling to "<<
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
    else{
        //this is B subscriber, A incoming call
        std::cout<<Clr::bl<<"### CALL B <- A: we are "<<_ci.localUri<<" subscriber calling to "<<
                        _ci.remoteUri<<Clr::def<<std::endl;
    }
    /*
    if (is_C_loc_and_B_rem()){
        std::cout<<Clr::bl<<"### we are "<<_ci.localUri<<" subscriber called by "<<
                        _ci.remoteUri<<Clr::def<<std::endl;
        pj::AudioMedia& speaker_media = pj::Endpoint::instance().audDevManager().getPlaybackDevMedia();
        pj::AudioMedia& mic_media = pj::Endpoint::instance().audDevManager().getCaptureDevMedia();
        mic_media.startTransmit(speaker_media);
    }
    else
        std::cout<<Clr::gr<<"#### do nothing with the audio..."<<Clr::def<<std::endl;
    */

}

void GenCall::onCallTransferRequest(pj::OnCallTransferRequestParam &prm){
    __PRETTY__ 
    prm.newCall = new GenCall(*_myAcc);    // Create new Call for call transfer 
}

void GenCall::onCallReplaceRequest(pj::OnCallReplaceRequestParam &prm){
    __PRETTY__ 
    prm.newCall = new GenCall(*_myAcc);// Create new Call for call replace 
}


