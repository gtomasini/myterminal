#include <iostream>
#include "GenAcc.h"
#include "GenCall.h"
//#include "Call2B.h"
#include "GenEndpoint.h"
#include "Uri.h"
#include "C_acc.h"

//
//full test, call to myself!
//
int A_2_A_call (GenEndpoint &ep){
    __PRETTY__

    pj::EpConfig ep_cfg;
    ep_cfg.logConfig.level = 4;
    ep.libInit( ep_cfg );    // Init library

    pj::TransportConfig tcfg;
    tcfg.port = Uri::PortA;
    ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

    ep.libStart();
    std::cout << Clr::og<<"=== PJSUA2 lib STARTED ===" << Clr::def<<std::endl;

    // add account
    pj::AccountConfig acc_cfg;
    acc_cfg.idUri = Uri::A_Full;
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
    //make a call to myself
    std::cout<<Clr::og<<"===== making outgoing call A to A... ====="<<Clr::def<<std::endl;
    pj::Call *call_myself = new GenCall(*acc);
    //acc->_calls_list.push_back(call);
    pj::CallOpParam prm(true);
    prm.opt.audioCount = 1;
    prm.opt.videoCount = 0;
    //call->makeCall("sip:test1@pjsip.org", prm);
    call_myself->makeCall(Uri::A_Full, prm);
    std::cout<<Clr::og<<"===== after made outgoing call... ====="<<Clr::def<<std::endl;

    pj_thread_sleep(5000);
    std::cout<<Clr::og<<"===== about to hangup all calls... ====="<<Clr::def<<std::endl;
    ep.hangupAllCalls();

    //std::cout<<Color::cyan<<"===== about to sleep... ====="<<Color::Default<<std::endl;
    pj_thread_sleep(4000);
    delete(call_myself);    
    std::cout << Clr::og<<"=== PJSUA2 lib SHUTTING DOWN ===" << std::endl;
    delete acc; // Will delete all calls too 
    return 0;
}


// C is just available for incomming calls from B....
int C_available (GenEndpoint &ep){
    __PRETTY__

    pj::EpConfig ep_cfg;
    ep_cfg.logConfig.level = 4;
    ep.libInit( ep_cfg );

    pj::TransportConfig tcfg;
    tcfg.port = Uri::PortC;
    ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

    ep.libStart();
    std::cout << Clr::og<<"=== PJSUA2 lib STARTED ===" << Clr::def<<std::endl;

    pj::AccountConfig acc_cfg;
    acc_cfg.idUri = Uri::C_Full;
    //acc_cfg.regConfig.registrarUri = "sip:sip.pjsip.org";//this is for registration

#if PJSIP_HAS_DIGEST_AKA_AUTH
    pj::AuthCredInfo aci("Digest", "*", "test", PJSIP_CRED_DATA_EXT_AKA | PJSIP_CRED_DATA_PLAIN_PASSWD, "passwd");
    aci.akaK = "passwd";
#else
    pj::AuthCredInfo aci("digest", "*", "test1", 0, "test1");//not used in this test
#endif
    //not use credentials in this call...
    //acc_cfg.sipConfig.authCreds.push_back(aci);
    C_acc *acc(new C_acc);
    try {
        acc->create(acc_cfg);
    } catch (...) {
        std::cerr << "ERROR: Adding account failed!" << std::endl;
    }
    
    std::cout<<"sleeping 60 secs..\n";
    pj_thread_sleep(60*1000);
  
    std::cout << Clr::og<<"=== PJSUA2 lib SHUTTING DOWN ===" << Clr::def<<std::endl;
    ep.hangupAllCalls();
    delete acc; // Will delete all calls too 

    return 0;
}

// **************************************************************************************
//                                  A calls B
// **************************************************************************************
int A_2_B_call (GenEndpoint &ep){
    __PRETTY__

    pj::EpConfig ep_cfg;
    ep_cfg.logConfig.level = 4;
    ep.libInit( ep_cfg );    // Init library

    pj::TransportConfig tcfg;
    tcfg.port = Uri::PortA;
    ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

    // Set codec priority
    std::cout << Clr::cy<<"=== set codecs priority ===" << Clr::def<<std::endl;
    pj_str_t    tmp;
    pj_status_t status;
    status = pjsua_codec_set_priority (pj_cstr (&tmp, "L16/16000/1"), 251);
    if (status != 0)
        std::cerr<<"ERROR: L16/16000/1 set_priority !!!"<<std::endl;
    status = pjsua_codec_set_priority (pj_cstr (&tmp, "L16/16000/2"), 250);
    if (status != 0)
        std::cerr<<"ERROR: L16/16000/2 set_priority !!!"<<std::endl;
    status = pjsua_codec_set_priority (pj_cstr (&tmp, "PCMU/8000/1"), 240);
    if (status != 0)
        std::cerr<<"ERROR: PCMU set_priority !!!"<<std::endl;
    status = pjsua_codec_set_priority (pj_cstr (&tmp, "PCMA/8000/1"), 240);
    if (status != 0)
        std::cerr<<"ERROR: PCMA set_priority !!!"<<std::endl;
    pjsua_codec_set_priority (pj_cstr (&tmp, "pcmu"), 0);
    pjsua_codec_set_priority (pj_cstr (&tmp, "pcma"), 0);
    pjsua_codec_set_priority (pj_cstr (&tmp, "speex/8000"), 0);
    pjsua_codec_set_priority (pj_cstr (&tmp, "ilbc"),0);
    pjsua_codec_set_priority (pj_cstr (&tmp, "speex/16000"), 0);
    pjsua_codec_set_priority (pj_cstr (&tmp, "speex/32000"), 0);
    pjsua_codec_set_priority (pj_cstr (&tmp, "gsm"), 100);

    ep.libStart();
    std::cout << Clr::cy<<"=== PJSUA2 lib STARTED ===" << Clr::def<<std::endl;

    pj::AccountConfig acc_cfg;// add account
    acc_cfg.idUri = Uri::A_Full;
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
    //make a call to B
    std::cout<<Clr::cy<<"===== making outgoing call A to B... ====="<<Clr::def<<std::endl;
    //pj::Call *call = new MyCall(*acc);
    GenCall *call2B = new GenCall(*acc);

    call2B->setCall2Bcase();// this is A 2 B
    pj::CallOpParam prm(true);
    prm.opt.audioCount = 1;
    prm.opt.videoCount = 0;
    call2B->makeCall (Uri::B_Full, prm);
    //call2B->makeCall (Uri::B_Full, prm);

    pj_thread_sleep(10000);
    std::cout<<Clr::cy<<"===== press any key to hangup outgoing call to B... ====="<<Clr::def<<std::endl;
    const auto c=getchar();

    call2B->hangup(prm);
    delete(call2B);

    ep.hangupAllCalls();

    //std::cout<<Color::cyan<<"===== about to sleep... ====="<<Color::Default<<std::endl;
    pj_thread_sleep(4000);
    
    std::cout << Clr::cy<<"=== PJSUA2 lib SHUTTING DOWN ===" << std::endl;
    delete acc;// Will delete all calls too 
    return 0;
}


int registration_tst (){
    __PRETTY__
    pj::Endpoint ep;
    ep.libCreate();

    // Initialize endpoint
    pj::EpConfig ep_cfg;
    ep.libInit( ep_cfg );

    // Create SIP transport. Error handling sample is shown
    pj::TransportConfig tcfg;
    //tcfg.port = 5060;
    tcfg.port = 5061;
    try {
        ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
    } catch (pj::Error &err) {
        std::cout << err.info() << std::endl;
        return 1;
    }

    ep.libStart(); // Start the library (worker threads etc)
    std::cout << Clr::bl<<"*** PJSUA2 lib STARTED ***" << Clr::def<<std::endl;

    // Configure an AccountConfig
    pj::AccountConfig acfg;
    acfg.idUri = "sip:test@pjsip.org";
    //acfg.regConfig.registrarUri = "sip:pjsip.org";
    acfg.regConfig.registrarUri = "sip:localhost";
    pj::AuthCredInfo cred("digest", "*", "test", 0, "secret");
    acfg.sipConfig.authCreds.push_back( cred );

    // Create the account
    GenAcc acc;// = new MyAccount;
    acc.create(acfg);

    std::cout <<  Clr::bl<<"*** end of registration ***" << Clr::def<<std::endl;
    // Here we don't have anything else to do..
    pj_thread_sleep(30000);

    // This will implicitly shutdown the library
    return 0;
}


