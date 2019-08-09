//*********************************************
//  This is auto generated by G4gen 0.6
//                                  author:Qian

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "Verbose.hh"

#include "MyDetectorMessenger.hh"
#include "MyDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorMessenger::MyDetectorMessenger(MyDetectorConstruction *Det)
    : G4UImessenger(),
      fDetectorConstruction(Det)
{
  if (verbose)
    G4cout << "====>MyDetectorMessenger::MyDetectorMessenger(MyDetectorConstruction *Det)" << G4endl;

  fMyDirectory = new G4UIdirectory("/MySimulation/");
  fMyDirectory->SetGuidance("UI commands specific to this example.");

  fDetDirectory = new G4UIdirectory("/MySimulation/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fWorldMatCmd = new G4UIcmdWithAString("/MySimulation/det/setWorldMaterial", this);
  fWorldMatCmd->SetGuidance("Select Material of the World.");
  fWorldMatCmd->SetParameterName("choice", false);
  fWorldMatCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fWorldSizeXCmd = new G4UIcmdWithADoubleAndUnit("/MySimulation/det/setWorldX", this);
  fWorldSizeXCmd->SetUnitCategory("Length");
  fWorldSizeXCmd->SetParameterName("WorldX", false);
  fWorldSizeXCmd->SetRange("WorldX>0.");
  fWorldSizeXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWorldSizeXCmd->SetToBeBroadcasted(false);

  fWorldSizeYCmd = new G4UIcmdWithADoubleAndUnit("/MySimulation/det/setWorldY", this);
  fWorldSizeYCmd->SetUnitCategory("Length");
  fWorldSizeYCmd->SetParameterName("WorldY", false);
  fWorldSizeYCmd->SetRange("WorldY>0.");
  fWorldSizeYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWorldSizeYCmd->SetToBeBroadcasted(false);

  fWorldSizeZCmd = new G4UIcmdWithADoubleAndUnit("/MySimulation/det/setWorldZ", this);
  fWorldSizeZCmd->SetUnitCategory("Length");
  fWorldSizeZCmd->SetParameterName("WorldZ", false);
  fWorldSizeZCmd->SetRange("WorldZ>0.");
  fWorldSizeZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fWorldSizeZCmd->SetToBeBroadcasted(false);

  fUpdateCmd = new G4UIcmdWithoutParameter("/MySimulation/det/update", this);
  fUpdateCmd->SetGuidance("Update calorimeter geometry.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
  fUpdateCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorMessenger::~MyDetectorMessenger()
{
  delete fWorldSizeZCmd;
  delete fWorldSizeYCmd;
  delete fWorldSizeXCmd;
  delete fWorldMatCmd;

  delete fUpdateCmd;
  delete fDetDirectory;
  delete fMyDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
  if (verbose)
    G4cout << "void MyDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)" << G4endl;

  if (command == fWorldSizeXCmd)
    fDetectorConstruction->SetWorldSizeHalfX(0.5 * fWorldSizeXCmd->GetNewDoubleValue(newValue));
  if (command == fWorldSizeYCmd)
    fDetectorConstruction->SetWorldSizeHalfY(0.5 * fWorldSizeYCmd->GetNewDoubleValue(newValue));
  if (command == fWorldSizeZCmd)
    fDetectorConstruction->SetWorldSizeHalfZ(0.5 * fWorldSizeZCmd->GetNewDoubleValue(newValue));
  if (command == fWorldMatCmd)
    fDetectorConstruction->SetMaterial(WORLD, newValue);

  if (command == fUpdateCmd)
    fDetectorConstruction->Update();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
