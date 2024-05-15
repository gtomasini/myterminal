#pragma once

struct Uri {
    //acc_cfg.idUri = "sip:test1@pjsip.org";
    static const constexpr char* A_Full="sip:A@localhost:5060";
    static const constexpr char* A="<sip:A@localhost>";
    static const unsigned PortA=5060;

    static const constexpr char* B_Full="sip:B@b_host:5061";
    static const constexpr char* B="<sip:B@b_host>";
    static const unsigned PortB=5061;

    static const constexpr char* C_Full="sip:C@localhost:5062";
    static const constexpr char* C="<sip:C@localhost>";
    static const unsigned PortC=5062;

    static const constexpr char* D_Full="sip:D@localhost:5063";
    static const constexpr char* D="<sip:D@localhost>";
    static const unsigned PortD=5063;
};
