//*********************************************
//  This is auto generated by G4gen 0.6
//                                  author:Qian

#include "G4RunManager.hh"
#include "G4Electron.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4LogicalVolume.hh"

#include "MyStackAction.hh"
#include "MyEvtAction.hh"

#include "Verbose.hh"
#include "MyDetectorConstruction.hh"

MyStackAction::MyStackAction()
    : G4UserStackingAction(), fKillSecondary(0)
{
    if (verbose)
        G4cout << "====>MyStackAction::MyStackAction()" << G4endl;
}

MyStackAction::~MyStackAction()
{
}

G4ClassificationOfNewTrack MyStackAction::ClassifyNewTrack(const G4Track *aTrack)
{
    if (verbose)
        G4cout << "====>MyStackAction::ClassifyNewTrack(const G4Track* aTrack)" << G4endl;

    G4int pdgID = aTrack->GetDefinition()->GetPDGEncoding();
    G4int trackID = aTrack->GetTrackID();
    G4double energy = aTrack->GetKineticEnergy();
    G4ThreeVector momDir = aTrack->GetMomentumDirection();
    G4int ParentID = aTrack->GetParentID();
	const G4ThreeVector rejectPolarization = aTrack->GetPolarization();
	const G4ThreeVector trackPositionPoint = aTrack->GetPosition();  
    const G4String particleProcessName = aTrack->GetCreatorModelName();

    /*
    if (trackID == 1)
    {
        SimEvent *fEvent = MyAnalysisManager::GetInstance()->GetSimEvent();
        //fEvent->SetPDGID(pdgID);
        //fEvent->SetTrueEnergy(energy);
        fEvent->SetMomentumX(momDir.x());
        fEvent->SetMomentumY(momDir.y());
        fEvent->SetMomentumZ(momDir.z());
		//G4cout<<"===+++===+++ "<<rejectPolarization<<G4endl;
    }

    if (ParentID == 1 && pdgID == 11 && particleProcessName == "pol-phot") //e- PDGID=11
	//if (pdgID == 11 && trackID !=1 ) //并不是单纯的光电效应径迹
    {
        const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

        if (detectorConstruction->InGasDetector(aTrack->GetPosition()))
        {
			
            SimEvent *fEvent = MyAnalysisManager::GetInstance()->GetSimEvent();
            fEvent->SetPhoelecMomentumX(momDir.x());
            fEvent->SetPhoelecMomentumY(momDir.y());
            fEvent->SetPhoelecMomentumZ(momDir.z());
			//G4cout<<"===+++===+++ "<<momDir<<G4endl;
			fEvent->SetTrueEnergy(energy);
			fEvent->SetEleCreatPosX(trackPositionPoint.x());
			fEvent->SetEleCreatPosY(trackPositionPoint.y());
			fEvent->SetEleCreatPosZ(trackPositionPoint.z());
			//G4cout<<"volume======= "<<aTrack->GetVolume()<<G4endl;
       }  
		
    }
    */

    return fUrgent;
}
