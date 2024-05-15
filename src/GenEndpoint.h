#pragma once
#include <iostream>
#include <pjsua2.hpp>
#include "Color.h"


class GenEndpoint : public pj::Endpoint{
public:
    GenEndpoint() : Endpoint() {
        __PRETTY__ 
    };

    virtual pj_status_t onCredAuth(pj::OnCredAuthParam &prm) {
        __PRETTY__ 
        /* Return PJ_ENOTSUP to use
         * pjsip_auth_create_aka_response()/<b>libmilenage</b> (default),
         * if PJSIP_HAS_DIGEST_AKA_AUTH is defined.
         */
        return PJ_ENOTSUP;
    }
};