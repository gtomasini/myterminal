#include <condition_variable>
#include <mutex>
#include <atomic>

#include "B_acc.h"
#include "GenAcc.h"
#include "GenCall.h"
#include "GenEndpoint.h"
#include "Uri.h"

std::condition_variable cv;
std::mutex mtx;
std::atomic_bool  call_from_A_established(false);

void available_for_in_call (GenAcc *acc){
    GenCall *call2C = nullptr;
    unsigned delay=1;

    for(;;){
        //if (acc -> getIsConnected_2_A() == false) {
        //    pj_thread_sleep(10);
        //    continue;
        //}

        // here B is connected 2 A
        //GenCall *callFromA=acc->getCallFromA();
        //std::cout<<Clr::og<<"#### getCallFromAWithWait() call ..."<<Clr::def<<std::endl;
        GenCall *callFromA = acc->getCallFromAsync();
        std::cout<<Clr::og<<"#### call from A !"<<Clr::def<<std::endl;
    
        // call to C subscriber...
        if (call2C == nullptr)  {
            call2C = new GenCall(*acc);
            pj::CallOpParam prm(true);
            prm.opt.audioCount = 1;
            prm.opt.videoCount = 0;
            std::cout<<Clr::og<<"#### B calling to C... #####"<<Clr::def<<std::endl;
            call2C->makeCall (Uri::C_Full, prm);
        }
        else{
            pj_thread_sleep (++delay*500);
            //auto const ci = call2C->getInfo();// OJO Con esto!!!! hay que chequearlo en el PAS porque si la llamada se cayo da excepcion
            auto const ci = call2C->getCallInfo(); 
            std::cout<<Clr::og<<"### already oncourse outgoing C call...["<<ci.stateText<<"]"<<Clr::def<<std::endl;
            std::cout.setf( std::ios_base::unitbuf );
            if (ci.state == PJSIP_INV_STATE_DISCONNECTED){
                std::cout<<Clr::og<<"### C is disconnected, so hangup to A!!!"<<Clr::def<<std::endl;
                pj::CallOpParam prm(true);
                prm.opt.audioCount = 1;
                prm.opt.videoCount = 0;

                callFromA->hangup(prm);
                acc->resetCallFromA();
                delete(call2C);
                call2C=nullptr;
                continue;
            }
        }

        pj_thread_sleep(1000);

        if (acc -> getCallFromA ()){
            pj::CallInfo ciFromA = acc -> getCallFromA () -> getCallInfo();

            if (ciFromA.state == PJSIP_INV_STATE_DISCONNECTED){
                std::cout<<Clr::og<<"### A HANG UP the Call, so we hangup outgoing call to C!!! "<<std::endl;

                acc -> resetCallFromA ();
                //PAUsHangupAndDelete ();
                pj::CallOpParam prm(true);
                prm.opt.audioCount = 1;
                prm.opt.videoCount = 0;
                call2C->hangup(prm);
                delete(call2C);
                call2C=nullptr;
            }
            else if (ciFromA.state == PJSIP_INV_STATE_CONFIRMED){
                ;
            }
            else {
                ;//error?
            }
        }
        else{
            // TODO: analize this particular scenario if could be possible....
            std::cerr<<Clr::re<<"ERROR: callFromA is null !!!"<<Clr::def<<std::endl;
            continue;
        }

        std::cout<<Clr::og<<"..........................."<<Clr::def<<std::endl;
    }
}

void inf_loop (GenAcc *acc){
    GenCall *call2C=nullptr;

    for(;;){
        std::cout<<Clr::cy<<"===== (B) press c to call C, h to hongout, q to quit ... ====="<<Clr::def<<std::endl;
        char c = getchar();
        if (c == 'c'){
            //call to C subscriber...
            if (call2C==nullptr)
            {
                call2C = new GenCall(*acc);
                pj::CallOpParam prm(true);
                prm.opt.audioCount = 1;
                prm.opt.videoCount = 0;
                std::cout<<Clr::og<<"### B calling C... ====="<<Clr::def<<std::endl;
                call2C->makeCall (Uri::C_Full, prm);
            }
            else
                std::cout<<Clr::og<<"### hangout outgoing C call!!!! ===="<<Clr::def<<std::endl;
        }
        else if (c == 'h'){
            if (call2C != nullptr){
                std::cout<<Clr::og<<"### hangout call... ====="<<Clr::def<<std::endl;
                pj::CallOpParam prm(true);
                prm.opt.audioCount = 1;
                prm.opt.videoCount = 0;

                call2C->hangup(prm);
                delete(call2C);
            }
        }
        else if (c == 'h')
            break;
    }
}

//B is just waiting for incomming calls...and eventually it could call C
int B_available (GenEndpoint &ep){
    __PRETTY__
    call_from_A_established=false;

    pj::EpConfig ep_cfg;
    ep_cfg.logConfig.level = 4;
    ep.libInit( ep_cfg );

    pj::TransportConfig tcfg;
    tcfg.port = Uri::PortB;
    ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

    ep.libStart();
    std::cout << Clr::og<<"### PJSUA2 lib STARTED" << Clr::def<<std::endl;

    // Add account
    pj::AccountConfig acc_cfg;
    acc_cfg.idUri = Uri::B_Full;
    //acc_cfg.regConfig.registrarUri = "sip:sip.pjsip.org";//this is for registration

#if PJSIP_HAS_DIGEST_AKA_AUTH
    pj::AuthCredInfo aci("Digest", "*", "test", PJSIP_CRED_DATA_EXT_AKA | PJSIP_CRED_DATA_PLAIN_PASSWD, "passwd");
    aci.akaK = "passwd";
#else
    pj::AuthCredInfo aci("digest", "*", "test1", 0, "test1");//not used in this test
#endif
    //not use credentials in this call...
    //acc_cfg.sipConfig.authCreds.push_back(aci);
    GenAcc *acc(new GenAcc);
    try {
        acc->create(acc_cfg);
    } catch (...) {
        std::cerr << "ERROR: Adding account failed!" << std::endl;
    }
    
    pj_thread_sleep(2000);

    available_for_in_call (acc);

    pj_thread_sleep(1000);
    ep.hangupAllCalls();

    //std::cout<<Color::cyan<<"===== about to sleep... ====="<<Color::Default<<std::endl;
    //pj_thread_sleep(4000);
    
    std::cout << Clr::og<<"### PJSUA2 lib SHUTTING DOWN" << std::endl;
    delete acc; // Will delete all calls too 
    return 0;
}
