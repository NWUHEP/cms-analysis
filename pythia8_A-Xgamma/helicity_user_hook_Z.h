//helicity_user_hook_Z.h

#ifndef __HELICITY_USER_HOOK_Z_INCLUDED__
#define __HELICITY_USER_HOOK_Z_INCLUDED__

#include "Pythia8/Pythia.h"
using namespace Pythia8;

class MyUserHooks : public UserHooks {

public:

    MyUserHooks(Info* infoPtrIn, bool inputOption, double polarization);
    ~MyUserHooks();

    bool canVetoProcessLevel();
    bool doVetoProcessLevel(Event& process);
    double selectAngle( double inputSpin, double inputId );

private:

    Info* infoPtr;
    // bool to define the frame for helicity.
    bool helicityDefinedByMother;
    double polarization;
};

#endif
