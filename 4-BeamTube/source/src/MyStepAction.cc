//*********************************************
//  This is auto generated by G4gen 0.6
//                                  author:Qian

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "MyStepAction.hh"
#include "MyEvtAction.hh"
#include "MyDetectorConstruction.hh"

#include "Verbose.hh"

MyStepAction::MyStepAction()
    : G4UserSteppingAction()
{
    if (verbose)
        G4cout << "====>MyStepAction::MyStepAction()" << G4endl;
}

MyStepAction::~MyStepAction()
{
}

void MyStepAction::UserSteppingAction(const G4Step *aStep)
{
    if (verbose)
        G4cout << "====>MyStepAction::UserSteppingAction()" << G4endl;

/*    if (!fAnticShieldVolume)    反符合等相关储存能量操作
    {
        const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fAnticShieldVolume = detectorConstruction->GetAnticShieldVolume();
    }
    if (!fAnticShieldVolume2)
    {
        const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fAnticShieldVolume2 = detectorConstruction->GetAnticShieldVolume2();
    }
    if (!fBeWindowsVolume)
    {
        const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fBeWindowsVolume = detectorConstruction->GetBeWindowsVolume();
    }
	if (!fGasVolume)
    {
        const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fGasVolume = detectorConstruction->GetGasVolume();
    }


    G4LogicalVolume *presentVolume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();


    if (presentVolume == fAnticShieldVolume)
    {
        G4double edepStep = aStep->GetTotalEnergyDeposit();
        //G4cout<<"+++=====+++++====++++  "<<edepStep<<G4endl;
        SimEvent *fEvent = MyAnalysisManager::GetInstance()->GetSimEvent();
        fEvent->SetEventCoincidence1Energy(edepStep);
    }
    if (presentVolume == fAnticShieldVolume2)
    {
        G4double edepStep2 = aStep->GetTotalEnergyDeposit();
        //G4cout<<"+++=====+++++====++++222 "<<edepStep2<<G4endl;
        SimEvent *fEvent = MyAnalysisManager::GetInstance()->GetSimEvent();
        fEvent->SetEventCoincidence2Energy(edepStep2);
    }
	if (presentVolume == fBeWindowsVolume)
    {
        G4double edepStep = aStep->GetTotalEnergyDeposit();
        //G4cout<<"+++=====+++++====++++222 "<<edepStep<<G4endl;
        SimEvent *fEvent = MyAnalysisManager::GetInstance()->GetSimEvent();
        fEvent->SetEventBeWindowsEnergy(edepStep);
    }
	
	if (presentVolume == fGasVolume)
	{
		G4int pdgID = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
		G4double edepStep = aStep->GetTotalEnergyDeposit();
		//G4cout<<"------volume "<<fGasVolume<<G4endl;
		//G4cout<<"+++++volume "<<fBeWindowsVolume<<G4endl;
		const G4ThreeVector trackPositionPoint = aStep->GetTrack()->GetVertexPosition();
		const G4LogicalVolume *whatWeWant = aStep->GetTrack()->GetLogicalVolumeAtVertex();
		//G4cout<<"====volume "<<whatWeWant<<G4endl;
		if (whatWeWant == fBeWindowsVolume && pdgID == 11 && trackPositionPoint != postPoint )
		{
			SimEvent *fEvent = MyAnalysisManager::GetInstance()->GetSimEvent();
       		fEvent->SetNumCrossBe(theSumFromBe);
			//G4cout<<"+++++  "<<fBeWindowsVolume<<G4endl;
			//theSumFromBe = theSumFromBe+1;
			//G4cout<<"-------sum "<<theSumFromBe<<G4endl;
		}
		postPoint = trackPositionPoint;
	}*/
}
