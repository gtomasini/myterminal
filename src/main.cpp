#include <iostream>
#include "GenEndpoint.h"
#include "Color.h"

int A_2_A_call (GenEndpoint &ep);
int A_2_B_call (GenEndpoint &ep);
int B_available (GenEndpoint &ep);
int C_available (GenEndpoint &ep);
int registration_tst();

enum class Opcion{A2A, A2B, B_core, C_available, D_available, Register};

bool Clr::_print = true;

static void help(const char *s){
    std::cerr<<"write: "<<s<<"xx"<<std::endl;
    std::cerr<<"\twhere xx: A2A (A calls A)"<<std::endl;
    std::cerr<<"\twhere xx: A2B (A calls B)"<<std::endl;
    std::cerr<<"\twhere xx: B (B core available for incomming calls)"<<std::endl;
    std::cerr<<"\twhere xx: C (C available for incomming calls)"<<std::endl;
    std::cerr<<"\twhere xx: D (D available for incomming calls)"<<std::endl;
    std::cerr<<"\twhere xx: R (registration)"<<std::endl;
}

int main(int argc, char *argv[]){
    auto option=Opcion::A2A;
    if (argc != 2){
        help(argv[0]);
        return -1;
    }

    if (strcmp (argv[1], "A2A")==0){
        std::cout<<Clr::cy<<"+++++++++ A 2 A Call test  +++++++++"<<Clr::def<<std::endl;
        option=Opcion::A2A;
    }
    else if (strcmp (argv[1], "A2B")==0){
        std::cout<<Clr::cy<<"+++++++++ A 2 B Call test +++++++++"<<Clr::def<<std::endl;
        option=Opcion::A2B;
    }
    else if (strcmp (argv[1], "B")==0){
        std::cout<<Clr::cy<<"+++++++++ B core available for in Calls test +++++++++"<<Clr::def<<std::endl;
        option=Opcion::B_core;
    }
    else if (strcmp (argv[1], "C")==0){
        std::cout<<Clr::cy<<"+++++++++ C available for in Calls test +++++++++"<<Clr::def<<std::endl;
        option=Opcion::C_available;
    }
    else if (strcmp (argv[1], "R")==0){
        std::cout<<Clr::cy<<"+++++++++ registration test +++++++++"<<Clr::def<<std::endl;
        option=Opcion::Register;
    }
    else{
        std::cerr<<"Option not found error: "<<argv[1]<<"!"<<std::endl;
        help(argv[0]);
        return -1;
    }


    try {
        switch (option)
        {
            case Opcion::A2A:
                {
                    GenEndpoint ep;
                    ep.libCreate();
                    A_2_A_call (ep);
                    ep.libDestroy();
                }
                break;

            case Opcion::A2B:
                {
                    GenEndpoint ep;
                    ep.libCreate();
                    A_2_B_call (ep);
                    ep.libDestroy();
                }
                break;

            case Opcion::B_core:
                {
                    GenEndpoint ep;
                    ep.libCreate();
                    B_available (ep);
                    ep.libDestroy();
                }
                break;

            case Opcion::C_available:
                {
                    GenEndpoint ep;
                    ep.libCreate();
                    C_available (ep);
                    ep.libDestroy();
                }
                break;

            case Opcion::Register:
                registration_tst();
                break;

            default:
                std::cerr<<"Option error!"<<std::endl;
                return -1;
        }
    } catch (pj::Error & err) {
        std::cerr << "error (main), exception: " << err.info() << std::endl;
        return -1;
    }

    return 0;
}