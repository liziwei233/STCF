//*********************************************
//  This is auto generated by G4gen 0.6
//                                  author:Qian

#ifndef MyDetectorConstruction_h
#define MyDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4CSGSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Region;
class G4Material;
class G4UserLimits;
class G4GlobalMagFieldMessenger;
class G4VisAttributes;

class MyDetectorMessenger;

//-------------------------------
// detector parameter variables

#define SIZE 50
#define WORLD 0
#define OUTTER 1
#define INNER 2
#define INNERCT 3

//-------------------------------
// detector parameter class
class MyDetectorParameters
{
public:
  MyDetectorParameters()
  {
    Name = "";
    Matt = "";
    VType = "";
    MID = 0;
    for (int i = 0; i < 10; i++)
      Siz[i] = 0;
    for (int i = 0; i < 3; i++)
      Pos[i] = 0;
    for (int i = 0; i < 3; i++)
      Rot[i] = 0;
    Material = 0;
    visAtt = 0;
    Region = 0;
    RegionName = "";
  }

  virtual ~MyDetectorParameters(){};

  G4int MID;
  G4String Name;
  G4String Matt;
  G4String VType;
  G4double Siz[10];
  G4double Pos[3];
  G4double Rot[3];
  G4Material *Material;
  G4VisAttributes *visAtt;
  G4Region *Region;
  G4String RegionName;
};

//-------------------------------
// detector contructions
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  MyDetectorConstruction();
  virtual ~MyDetectorConstruction();

  G4VPhysicalVolume *DefineVolumes();
  virtual G4VPhysicalVolume *Construct();

  void SetMaterial(int, G4String);
  void Update();

public:
  void SetWorldSizeHalfX(G4double x) { fDetPar[WORLD]->Siz[0] = x; }
  void SetWorldSizeHalfY(G4double y) { fDetPar[WORLD]->Siz[1] = y; }
  void SetWorldSizeHalfZ(G4double z) { fDetPar[WORLD]->Siz[2] = z; }
  G4double GetWorldSizeHalfX() { return fDetPar[WORLD]->Siz[0]; }
  G4double GetWorldSizeHalfY() { return fDetPar[WORLD]->Siz[1]; }
  G4double GetWorldSizeHalfZ() { return fDetPar[WORLD]->Siz[2]; }

private:
  void DefineMaterials();

  MyDetectorMessenger *fDetectorMessenger;

  G4bool checkOverlaps;
  G4UserLimits *fStepLimit;

  MyDetectorParameters *fDetPar[SIZE];
  G4CSGSolid *fSolid[SIZE];
  G4LogicalVolume *fLogic[SIZE];
  G4VPhysicalVolume *fPhysc[SIZE];

};

#endif
