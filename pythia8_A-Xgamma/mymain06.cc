// Pythia generator code for A->X+gamma with X-> mu+mu-
// A taken to be Higgs; X taken to be Z
// Angular corrections made using user hooks class
// Includes options for polarization, variable mass, LHE files
// Polarization correction based on pythia example code main62.cc

#include <iostream>
#include "Pythia8/Pythia.h"
#include "stdio.h"
#include "string.h"
#include <cmath>

// ROOT, for histogramming.
#include "TH1.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"

// ROOT, for saving file.
#include "TFile.h"

// Derived UserHooks class
#include "helicity_user_hook_Z.h"

#include "TTree.h"

using namespace Pythia8;

std::string str(int i) {return static_cast<std::ostringstream*>( &(std::ostringstream() << i) )->str();} // int to string converter

int main(int argc, char* argv[]){

    
    // Create the ROOT application environment.
    TApplication theApp("hist", &argc, argv);

    const int num_mass_pts = 4;     // Iterate over mass points
    const int mass_spacing = 20;
    const int num_polar_types = 3;  // and over polarization types, including scalar
    const int start_mass = 30;
    Pythia pythia[num_polar_types][num_mass_pts]; 

    // Define histograms

    TFile *outFile[num_polar_types][num_mass_pts];
    TH1F *cosDist[num_polar_types][num_mass_pts];
    TH1F *ZPt[num_polar_types][num_mass_pts];
    TH1F *photonPt[num_polar_types][num_mass_pts];
    TH1F *leadingMuonPt[num_polar_types][num_mass_pts]; 
    TH1F *trailingMuonPt[num_polar_types][num_mass_pts];
    TH1F *dimuonMass[num_polar_types][num_mass_pts];
    TH1F *leadingMuonEta[num_polar_types][num_mass_pts];
    TH1F *trailingMuonEta[num_polar_types][num_mass_pts];
    TH1F *muonDeltaR[num_polar_types][num_mass_pts]; 
     
    double polarization;

    for (int iPol = 0; iPol < num_polar_types; ++iPol)
    {    
        switch (iPol)
        {
            case 0: polarization = 9.0;     // scalar
            break;
            case 1: polarization = 1.0;    // - 
            break;
            case 2: polarization = 0.0;     // 0
            break;
            case 3: polarization = -1.0;     // +
            break;
        }
    
        for (int iMass = 0; iMass < num_mass_pts; ++iMass)
        {
            int X_mass = start_mass + (iMass * mass_spacing);
            std::string mass_string = str(X_mass);
            std::string polstr = str(polarization);

            // Create file
            outFile[iPol][iMass] = new TFile(("hZgamma_Pol"+polstr+"_mass"+mass_string+".root").c_str(), "RECREATE");
            
            // Create tree
            //TTree *eventTree = new TTree(("Event_tree_"+polstr+"_mass="+mass_string+".root").c_str(), "Event Tree");    
            //eventTree->Branch("event", &pythia[iPol][iMass].event); 

            // Create histograms
            cosDist[iPol][iMass] = new TH1F( "cos(theta)_Z", "cos(theta)_Z", 20, -1.0, 1.0);     // Angular distribution 
            cosDist[iPol][iMass]->SetMinimum(0); 
            ZPt[iPol][iMass] = new TH1F("ZPt", "ZPt", 50, 0., 400.);
            photonPt[iPol][iMass] = new TH1F("photonPt", "photonPt", 50, 0., 200.);
            leadingMuonPt[iPol][iMass] = new TH1F("leadingMuonPt", "leadingMuonPt", 50, 0., 200.);
            trailingMuonPt[iPol][iMass] = new TH1F("trailingMuonPt", "trailingMuonPt", 50, 0., 200.);
            dimuonMass[iPol][iMass] = new TH1F("dimuonMass", "dimuonMass", 50, 0., 100.);
            leadingMuonEta[iPol][iMass] = new TH1F("leadingMuonEta", "leadingMuonEta", 50, -5., 5.);
            trailingMuonEta[iPol][iMass] = new TH1F("trailingMuonEta", "trailingMuonEta", 50, -5., 5.);
            muonDeltaR[iPol][iMass] = new TH1F("muonDeltaR", "muonDeltaR", 20, 0., 5.); 
             
            pythia[iPol][iMass].readFile("mymain06.cmnd");      // Global settings
            pythia[iPol][iMass].readString("HiggsSM:gg2H = on");
            pythia[iPol][iMass].readString("25:m0 = 350");
            pythia[iPol][iMass].readString("23:m0 = "+mass_string); // Change Z mass
            pythia[iPol][iMass].readString("25:onMode = off");   // Switch off top decay modes
            pythia[iPol][iMass].readString("25:onIfMatch = 22 23"); // Force decay
            pythia[iPol][iMass].readString("HadronLevel:all = off");     
            pythia[iPol][iMass].readString("PartonLevel:all = off");     
            pythia[iPol][iMass].readString("23:onMode = off");  // Switch off Z decay modes
            pythia[iPol][iMass].readString("23:onIfMatch = 13 -13");    // Force decay Z -> mu+nu

            int nEvent = pythia[iPol][iMass].mode("Main:numberOfEvents");
            int nAbort = pythia[iPol][iMass].mode("Main:timesAllowErrors");
           
            // Initialize UserHooks class in helicity_user_hook.cc & helicity_user_hook.h 
            MyUserHooks* myUserHooks = new MyUserHooks(&pythia[iPol][iMass].info, true, polarization);  // bool argument to switch on and off process level veto
            pythia[iPol][iMass].setUserHooksPtr(myUserHooks);     // Connect Pythia user hooks to the derived class
            Event& event = pythia[iPol][iMass].process;             // Set to pythia...event for full event tree; pythia...process for only hard process

            
            //// Create an LHAup object that can access relevant information in pythia.
            LHAupFromPYTHIA8 myLHA(&pythia[iPol][iMass].process, &pythia[iPol][iMass].info);    // Again, choose event or process
    
        
            // Open a file on which LHEF events should be stored, and write header.
            myLHA.openLHEF(("mymain06_Pol"+polstr+"_mass"+mass_string+".lhe").c_str());

            pythia[iPol][iMass].init();

            myLHA.setInit();
            myLHA.initLHEF();

            // Tell user the mass and polarization
            cout << "Mass = " << X_mass << endl;
            cout << "Polarization = " << polarization << endl;

            // Begin event loop.
            int iAbort = 0;
            for (int iEvent = 0; iEvent < nEvent; ++iEvent) 
            {

                // Generate events. Quit if many failures.
                if (!pythia[iPol][iMass].next()) 
                {
                    if (++iAbort < nAbort) continue;
                    cout << " Event generation aborted prematurely, owing to error!\n";
                    break;
                }
                
                // Loop over particles.
                for (int i = 0; i < event.size(); ++i)
                {
                    // identify daughters of the Z
                    int idAbs = event[i].idAbs();
                    int i1 = event[i].daughter1();
                    int i2 = event[i].daughter2();  
                    if ( idAbs == 23 && event[i].status() == -22 && event[i1].idAbs() == 13     // Z selection
                         && event[i1].pT() > 5.0 && event[i2].pT() > 5.0                        // muon Pt selection
                         && abs(event[i1].eta()) < 2.4 && abs(event[i2].eta()) < 2.4 )          // muon Eta selection 
                        {

                        // Angular distribution is defined with respect to mu-
                        if (event[i1].id() * event[i].id() < 0) 
                        {
                        i1 = event[i].daughter2();
                        i2 = event[i].daughter1();
                        }
                        
                        // Reconstruct Z decay angle by boosting daughter and mother to Z
                        // rest frame. Z direction in mother rest frame opposite to mother now.

                        RotBstMatrix Mrotbst;
                        Mrotbst.bstback( event[i].p() ); // Assign boost matrix to boost to Z frame
                        Vec4 p1 = event[i1].p();
                        p1.rotbst( Mrotbst );		// boost the daughter to Z frame
                        Vec4 pM = event[event[i].mother1()].p();
                        pM.rotbst( Mrotbst );		// boost the mother to Z frame
                        pM.flip3();
                        double costhe = costheta( p1, pM );

                        // fill histograms
        
                        //printf("costhe = %e\n", costhe);
                        cosDist[iPol][iMass]->Fill( costhe );

                        double Pt_Z = event[i].pT();
                        ZPt[iPol][iMass]->Fill(Pt_Z);
                 
                        double Pt_muon_i1 = event[i1].pT();
                        double Pt_muon_i2 = event[i2].pT(); 
                        double eta_muon_i1 = event[i1].eta();
                        double eta_muon_i2 = event[i2].eta(); 

                        if (Pt_muon_i1 > Pt_muon_i2)
                        {
                            leadingMuonPt[iPol][iMass]->Fill( Pt_muon_i1 );
                            leadingMuonEta[iPol][iMass]->Fill( eta_muon_i1);
                            trailingMuonPt[iPol][iMass]->Fill( Pt_muon_i2 );
                            trailingMuonEta[iPol][iMass]->Fill( eta_muon_i2);

                        }
                        else                         
                        {
                            leadingMuonPt[iPol][iMass]->Fill( Pt_muon_i2 );
                            leadingMuonEta[iPol][iMass]->Fill( eta_muon_i2);
                            trailingMuonPt[iPol][iMass]->Fill( Pt_muon_i1 );
                            trailingMuonEta[iPol][iMass]->Fill( eta_muon_i1);
                        }

                        Vec4 vec4_muon_i1 = event[i1].p();
                        Vec4 vec4_muon_i2 = event[i2].p();
                        Vec4 vec4_2bod = vec4_muon_i1 + vec4_muon_i2;
                        double two_body_mass = vec4_2bod.mCalc();
                        dimuonMass[iPol][iMass]->Fill(two_body_mass);

                        double phi_muon_i1 = event[i1].phi();
                        double phi_muon_i2 = event[i2].phi();

                        double deltaR = sqrt(pow((phi_muon_i1 - phi_muon_i2), 2.0) + pow((eta_muon_i1 - eta_muon_i2), 2.0));
                        muonDeltaR[iPol][iMass]->Fill( deltaR );                            
                       
                        int iPhot = event[i].sisterList()[0];
                        int photon_id = event[iPhot].idAbs();
                        // Check that it's a photon
                        if (photon_id == 22)
                        {
                            double gammaPt = event[iPhot].pT();
                            photonPt[iPol][iMass]->Fill(gammaPt);
                        }
                 
                    }

                }

                    //eventTree->Fill();

                    // Handle LHE
                    myLHA.setEvent();
                    myLHA.eventLHEF();

            }   // end of event loop
            
            // LHE
            myLHA.updateSigma();
            myLHA.closeLHEF(true);
                       
            //eventTree->Print();
            //eventTree->Write();            
 
            cosDist[iPol][iMass]->Write();
            ZPt[iPol][iMass]->Write();
            photonPt[iPol][iMass]->Write();
            leadingMuonPt[iPol][iMass]->Write();
            trailingMuonPt[iPol][iMass]->Write();
            dimuonMass[iPol][iMass]->Write();
            leadingMuonEta[iPol][iMass]->Write();
            trailingMuonEta[iPol][iMass]->Write();
            muonDeltaR[iPol][iMass]->Write();
            

            delete cosDist[iPol][iMass];
            delete leadingMuonPt[iPol][iMass];
            delete trailingMuonPt[iPol][iMass];
            delete ZPt[iPol][iMass];
            delete photonPt[iPol][iMass];
            delete dimuonMass[iPol][iMass];
            delete leadingMuonEta[iPol][iMass];
            delete trailingMuonEta[iPol][iMass];
            delete muonDeltaR[iPol][iMass];
            delete outFile[iPol][iMass];

            pythia[iPol][iMass].stat();

            delete myUserHooks;
        }   // end of mass loop
    }       // end of polar loop

return 0;

}    
