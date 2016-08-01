// User hook for helicity correction

#include "helicity_user_hook_Z.h"
#include "Pythia8/Pythia.h"
using namespace Pythia8;

//==========================================================================

// Write own derived UserHooks class.
// Here, Z bosons are initially decayed isotropically by Pythia
// Now, angular distribution is corrected for each Z based on input double polarization
// polarization: 1.0, -1.0 for transverse; 0.0 for longitudinal; 9.0 for scalar
// correction is made by generating angle following spherical harmonic PDFs:
// (1 -+ cos(theta))^2 for +-1, sin^2(theta) for 0, uniform for 9
// branching ratios retained; decay product four momenta updated according to generated decay angle


    // Constructor can set helicity definition. Destructor does nothing.
    MyUserHooks::MyUserHooks(Info* infoPtrIn, bool inputOption, double polarization)
    : infoPtr(infoPtrIn), helicityDefinedByMother(inputOption), polarization(polarization) {}
    MyUserHooks::~MyUserHooks() {}

  //Allow a veto for the process level, to gain access to decays.  
  bool MyUserHooks::canVetoProcessLevel() {return true;}

  // Access the event after resonance decays.
  bool MyUserHooks::doVetoProcessLevel(Event& process) {

    // Identify decayed Z bosons for study.
    for (int i = 0; i < process.size(); ++i) {
    
        int i1 = process[i].daughter1();

        if (process[i].idAbs() == 23 && process[i].status() == -22 
            && process[i1].idAbs() == 13
            ) 
        {

        // Pick decay angles according to desired distribution
        // based on polarization and particle/antiparticle.
        double cosThe = selectAngle( polarization, process[i].id() );
        //printf("cosThe = %e\n", cosThe);
        double sinThe = sqrt(1.0 - pow2(cosThe));
        double phi    = 2.0 * M_PI * rndmPtr->flat();   // uniform in azimuthal angle

        // Identify Z daughters, properly ordered.
        int idV = process[i].id();
        int i1  = process[i].daughter1();
        int i2  = process[i].daughter2();

        // convention to define according to mu-
        if (process[i1].id() * idV < 0) swap( i1, i2);

        // Set up decay in rest frame of Z
        double mV = process[i].m();
        double m1 = process[i1].m();
        double m2 = process[i2].m();

        // Energy and absolute momentum of first decay product in Z rest frame.
        double e1 = 0.5* (pow2(mV) + pow2(m1) - pow2(m2))/mV;
        double pA = sqrt(pow2(e1) - pow2(m1));

        // Four-vectors for the two decay products.
        Vec4 p1( pA * sinThe * cos(phi), pA *sinThe * sin(phi),
          pA * cosThe, e1);
        Vec4 p2   = Vec4(0,0,0,mV) - p1;

        // Reference four-vector for helicity definition.
        Vec4 pM;

        // Helicity is defined in the mother frame.
        if( helicityDefinedByMother ) 
        {
          pM = process[process[i].mother1()].p();
        } 
        else 
        {
          pM = Vec4( 0., 0., 0., infoPtr->mHat());
        }

        // Angular reference axis defined as opposite the mother
        // direction in Z rest frame.
        pM.bstback( process[i].p() );
        pM.flip3();
        RotBstMatrix Mrotbst;
        Mrotbst.rot( pM);

        // Rotate and boost Z decay products.
        Mrotbst.bst( process[i].p() );
        p1.rotbst(Mrotbst);
        p2.rotbst(Mrotbst);
        process[i1].p( p1 );
        process[i2].p( p2 );


        // For transverse polarization, flip between +/- 1
        if(abs(polarization) == 1.0)
        {
            polarization *= -1;
        }

      }

      // End of loop over Z's. Do not veto any events.
    }
    return false;
  }

    // Select polar angle for the Z decay.
    double MyUserHooks::selectAngle( double inputSpin, double inputId ) {

    // Set up initial angles.
    double rdNow = rndmPtr->flat(); // uniform random variable on [0,1]
    double cosThe;
    // Small number to distinguish -1, 1, and 0 with round-off.
    double eps = 1e-10;

    if (inputSpin > 2.5)
    {
        //printf("scalar case\n");
        if (inputId < 0) inputSpin *= -1;
        cosThe = 1.0 - 2.0*rdNow;    // uniform weighting on the interval [-1, 1]
    }
    else
    {

        if (inputId < 0) inputSpin *= -1;   // accounting for charge

        // 3/8 * (1 - cos(theta))^2  ++
        if (inputSpin > eps) {
          cosThe = max( 1.0 - 2.0 * pow(rdNow, 1./3.), -1.0);
        }

        // 3/8 * (1 + cos(theta))^2  --
        else if (inputSpin < -eps) {
          cosThe = min( 2.0 * pow(rdNow, 1./3.) - 1.0,  1.0);
        }

        // 3/4 * sin(theta)^2        00
        // Solution of cubic equation that yields the correct result.
        else {
          double theA = (acos(1.0 - 2.0 * rdNow) + 4.0 * M_PI) / 3.0;
          cosThe = 2.0 * cos(theA);
        }
    }

    // Return the selected cos(theta) value.
    //printf("cosThe = %e\n", cosThe);
    return cosThe;
  }

//==========================================================================
