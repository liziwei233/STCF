#include "TPad.h"
#include "TView.h"
#include "MyMainFrameGui.h"
#include "MyGuiActionClass.h"
#include "MyBeamTest.h"
#include "CombineData.h"

MyGuiActionClass *gMyGuiActionClass = (MyGuiActionClass *)0;

//______________________________________________________________________________
//
MyGuiActionClass::MyGuiActionClass(const char *realpath, int flag)
{
    BatGuiFlag = flag;
    env = new TEnv(gSystem->WorkingDirectory() + TString("/.env"));
    env->SaveLevel(kEnvLocal);

    gMyStyle = new MyStyle();
    gMyStyle->SetDrawOption("c");
    gMyStyle->SetColorPattern(MATHEMATIC_STYLE);

    realPath = TString(realpath);
    gMyCommonRICH = new MyCommonRICH();
    gMyDatabaseClass = new MyDatabaseClass(realpath);

    //for beamtest
    gMyBeamTest = new MyBeamTest(env);

    // pages
    nTabPage = 4;
    sTabPage[0] = TString("Beamtest");
    sTabPage[1] = TString("Simulation");
    sTabPage[2] = TString("Material");
    sTabPage[3] = TString("Detector");

    // page
    int ipage = 0;
    iButton[ipage] = SetDSPath;
    sButton[ipage].push_back("Set Data-Structure Path");
    sButton[ipage].push_back("Read binary data to Raw-root file");
    sButton[ipage].push_back("Check the Raw-Root file");
    sButton[ipage].push_back("Generate DST-root file");
    sButton[ipage].push_back("Check the DST-root file");
    sButton[ipage].push_back("Combine All Detectors' DST-root file");
    sButton[ipage].push_back("Load the Combined DST-root file");
    sButton[ipage].push_back("Slider");
    sButton[ipage].push_back("LR-L");
    sButton[ipage].push_back("LR-R");
    sButton[ipage].push_back("Zoom to default view");
    sButton[ipage].push_back("SEP");
    sButton[ipage].push_back("SEP");
    sButton[ipage].push_back("Analysis the Combined DST-root file");
    nButton[ipage] = (int)sButton[ipage].size();

    // page
    ipage++;
    iButton[ipage] = LoadDetFile;
    sButton[ipage].push_back("Load Hitmap");
    sButton[ipage].push_back("Show the FCN plot");
    sButton[ipage].push_back("Show the Specified Particle RICH");
    sButton[ipage].push_back("Show Multi-Particles RICH");
    sButton[ipage].push_back("Show the SCAN XY-hitmap and N-Photon map");
    sButton[ipage].push_back("Show the SCAN Resolution Map");
    sButton[ipage].push_back("Show the PID efficiency Map");
    sButton[ipage].push_back("SEP");
    sButton[ipage].push_back("SEP");
    sButton[ipage].push_back("Generate the Specified Particle RICH");
    sButton[ipage].push_back("Generate Multi-Particles RICH");
    sButton[ipage].push_back("SEP");
    sButton[ipage].push_back("Set && save to the configure ROOT file");
    sButton[ipage].push_back("SCAN to generate the XY-hitmap");
    sButton[ipage].push_back("SCAN to generate the Resolution-Map");
    sButton[ipage].push_back("SCAN to generate the PID efficiency Map");
    nButton[ipage] = (int)sButton[ipage].size();

    // page
    ipage++;
    iButton[ipage] = MatList;
    sButton[ipage] = gMyDatabaseClass->GetMaterialList();
    sButton[ipage].push_back("SEP");
    sButton[ipage].push_back("Draw Sel-Mat");
    nButton[ipage] = (int)sButton[ipage].size();

    // page
    ipage++;
    iButton[ipage] = DetList;
    sButton[ipage] = gMyDatabaseClass->GetDetectorList();
    sButton[ipage].push_back("SEP");
    sButton[ipage].push_back("Draw Sel-Det");
    nButton[ipage] = (int)sButton[ipage].size();

    // init parameters
    nRadLayer = env->GetValue("nRadLayer", 2);
    for (int i = 0; i < nRadLayer; i++)
    {
        sRadLayer.push_back(env->GetValue(Form("sRad%d", i), (i == 0) ? "C6F14" : "Quartz"));
        tRadLayer.push_back(env->GetValue(Form("tRad%d", i), (i == 0) ? 10 : 4));
    }

    tTransLayer = env->GetValue("tTransLayer", 100);
    nTransLayer = env->GetValue("nTransLayer", 2);
    for (int i = 0; i < nTransLayer; i++)
    {
        sTransLayer.push_back(env->GetValue(Form("sTrans%d", i), (i == 0) ? "Ar" : "CH4"));
        pTransLayer.push_back(env->GetValue(Form("pTrans%d", i), 50));
    }

    nImpurities = env->GetValue("nImpurities", 2);
    for (int i = 0; i < nImpurities; i++)
    {
        sImpurities.push_back(env->GetValue(Form("sImp%d", i), (i == 0) ? "H2O" : "O2"));
        pImpurities.push_back(env->GetValue(Form("pImp%d", i), 1));
    }

    PCDetector = env->GetValue("PCDetector", "CsI");
    pixel = env->GetValue("pixel", 5);

    nLambda = env->GetValue("nLambda", 61);
    lambdaMin = env->GetValue("lambdaMin", 160);
    lambdaMax = env->GetValue("lambdaMax", 220);

    particle = env->GetValue("particle", "pi");
    momentum = env->GetValue("momentum", 2.);
    Theta0 = env->GetValue("Theta0", 0.);
    mass = gMyCommonRICH->GetMass(particle);

    XBinMin = env->GetValue("XBinMin", -200);
    XBinMax = env->GetValue("XBinMax", 200);
    YBinMin = env->GetValue("YBinMin", -200);
    YBinMax = env->GetValue("YBinMax", 200);
    NXBin = XBinMax - XBinMin;
    NYBin = YBinMax - YBinMin;

    epsilon = env->GetValue("epsilon", 1e-5);
    trkStep = env->GetValue("trkStep", 1e-1);
    nphi = env->GetValue("nphi", 360);

    np = env->GetValue("np", 2);
    pMin = env->GetValue("pMin", 1.0);
    pMax = env->GetValue("pMax", 2.0);

    nthe0 = env->GetValue("nthe0", 3);
    The0Min = env->GetValue("The0Min", 0);
    The0Max = env->GetValue("The0Max", 2);

    iph = env->GetValue("iph", 1);
    irad1 = env->GetValue("irad1", 0);
    irad2 = env->GetValue("irad2", 1);
    pList = env->GetValue("pList", 1.0);
    thList = env->GetValue("thList", 0);
    nEvent = env->GetValue("nEvent", 1e4);

    nSelectedMat = env->GetValue("nSelectedMat", 2);
    for (int i = 0; i < nSelectedMat; i++)
        selectedMat.push_back(env->GetValue(Form("selectedMat%d", i), (i == 0) ? "Quartz" : "MgF2"));

    nSelectedDet = env->GetValue("nSelectedDet", 2);
    for (int i = 0; i < nSelectedDet; i++)
        selectedDet.push_back(env->GetValue(Form("selectedDet%d", i), (i == 0) ? "CsI" : "APD"));
};

MyGuiActionClass::~MyGuiActionClass()
{
    // Destructor.
    // Store env
    ReadSIMSettingsText();
    SetDetectorParameters();

    ReadBEAMSettingsText();
    gMyBeamTest->StoreEnv();

    env->SetValue("nRadLayer", nRadLayer);
    for (int i = 0; i < nRadLayer; i++)
    {
        env->SetValue(Form("sRad%d", i), sRadLayer[i].c_str());
        env->SetValue(Form("tRad%d", i), tRadLayer[i]);
    }

    env->SetValue("tTransLayer", tTransLayer);
    env->SetValue("nTransLayer", nTransLayer);
    for (int i = 0; i < nTransLayer; i++)
    {
        env->SetValue(Form("sTrans%d", i), sTransLayer[i].c_str());
        env->SetValue(Form("pTrans%d", i), pTransLayer[i]);
    }

    env->SetValue("nImpurities", nImpurities);
    for (int i = 0; i < nImpurities; i++)
    {
        env->SetValue(Form("sImp%d", i), sImpurities[i].c_str());
        env->SetValue(Form("pImp%d", i), pImpurities[i]);
    }

    env->SetValue("PCDetector", PCDetector.c_str());
    env->SetValue("pixel", pixel);

    env->SetValue("nLambda", nLambda);
    env->SetValue("lambdaMin", lambdaMin);
    env->SetValue("lambdaMax", lambdaMax);

    env->SetValue("particle", particle);
    env->SetValue("momentum", momentum);
    env->SetValue("Theta0", Theta0);

    env->SetValue("XBinMin", XBinMin);
    env->SetValue("XBinMax", XBinMax);
    env->SetValue("YBinMin", YBinMin);
    env->SetValue("YBinMax", YBinMax);

    env->SetValue("epsilon", epsilon);
    env->SetValue("trkStep", trkStep);
    env->SetValue("nphi", nphi);

    env->SetValue("np", np);
    env->SetValue("pMin", pMin);
    env->SetValue("pMax", pMax);

    env->SetValue("nthe0", nthe0);
    env->SetValue("The0Min", The0Min);
    env->SetValue("The0Max", The0Max);

    env->SetValue("iph", iph);
    env->SetValue("irad1", irad1);
    env->SetValue("irad2", irad2);
    env->SetValue("pList", pList);
    env->SetValue("thList", thList);
    env->SetValue("nEvent", nEvent);

    env->SetValue("nSelectedMat", nSelectedMat);
    for (int i = 0; i < nSelectedMat; i++)
        env->SetValue(Form("selectedMat%d", i), selectedMat[i]);

    env->SetValue("nSelectedDet", nSelectedDet);
    for (int i = 0; i < nSelectedDet; i++)
        env->SetValue(Form("selectedDet%d", i), selectedDet[i]);
    env->SaveLevel(kEnvLocal);
}

//______________________________________________________________________________
// read / generate settings
TString MyGuiActionClass::GenerateSIMSettingsText()
{
    TString settings("");

    //settings += "\n# The pre-defined material list\nMaterial_List: ";
    //for (int i = 0; i < nButton[0]; i++)
    //    settings += sButton[0].at(i) + " ";

    //settings += "\n\n# The pre-defined detector list\nDetector_List: ";
    //for (int i = 0; i < nButton[1]; i++)
    //    settings += sButton[1].at(i) + " ";
    settings += "\n#---------------------------";
    settings += "\n#    Detector settings";
    settings += "\n#---------------------------";
    settings += "\n\n# The number of radiator layers, thickness in [mm]\n";
    for (int i = 0; i < nRadLayer; i++)
        settings += Form("Radiator %s %.1f\n", sRadLayer[i].c_str(), tRadLayer[i]);

    settings += "\n# The transition layer, content is percentile [%]\n";
    for (int i = 0; i < nTransLayer; i++)
        settings += Form("TransLayer %s %.1f\n", sTransLayer[i].c_str(), pTransLayer[i]);
    settings += "\n# The thickness of the transition layer in [mm]\n";
    settings += Form("TransThick %.1f\n", tTransLayer);

    settings += "\n# The Impurities in the transition layer, content is [ppm]\n";
    for (int i = 0; i < nImpurities; i++)
        settings += Form("Impurities %s %.1f\n", sImpurities[i].c_str(), pImpurities[i]);

    settings += "\n# The photocathode detector, pixel size in [mm]\n";
    settings += Form("PCDetector %s %.1f\n", PCDetector.c_str(), pixel);

    settings += "\n# The n_pixel range for view, [xmin, xmax, ymin, ymax]\n";
    settings += Form("PixelRange %.1f %.1f %.1f %.1f\n", XBinMin, XBinMax, YBinMin, YBinMax);

    settings += "\n# The lambda range for analysis\n";
    settings += Form("Lambda %d %.1f %.1f\n", nLambda, lambdaMin, lambdaMax);

    settings += "\n#---------------------------";
    settings += "\n#    (Multi-)Particle settings";
    settings += "\n#---------------------------";

    settings += "\n\n# The Specified (Multi-)Particle to show (momentum[GeV], angle[degree])\n";
    settings += Form("Particle: %s %.1f %.1f\n", particle.Data(), momentum, Theta0);

    settings += "\n#---------------------------";
    settings += "\n#    SCAN settings";
    settings += "\n#---------------------------";

    settings += "\n\n# Particle to SCAN (N, Min, Max) for momentum[GeV/c])\n";
    settings += Form("Momentum: %d %.1f %.1f\n", np, pMin, pMax);

    settings += "\n\n# Particle to SCAN (N, Min, Max) for theta_0[degree])\n";
    settings += Form("Theta0: %d %.1f %.1f\n", nthe0, The0Min, The0Max);

    settings += "\n\n# From ShowMom[GeV/c], ShowThe[degree] and photon from ShowRad\n";
    settings += Form("ShowMom: %.1f \n", pList);
    settings += Form("ShowThe: %.1f \n", thList);
    settings += Form("ShowRad: %d %d\n", irad1, irad2);
    settings += Form("ShowPhoton: %d\n", iph);

    settings += "\n#---------------------------";
    settings += "\n#    Precision settings";
    settings += "\n#---------------------------";

    settings += "\n\n# Step size for charge particle track[mm]\n";
    settings += Form("trkStep: %f\n", trkStep);

    settings += "\n# Number of Step for phi [0~2pi] * 360\n";
    settings += Form("nPhi: %d\n", int(nphi / 360.));

    settings += "\n# Solver precision\n";
    settings += Form("Precision: %f\n", epsilon);

    settings += "\n# nEvents for reconstruction the resolution\n";
    settings += Form("nEvent: %d\n", nEvent);

    settings += "\n#---------------------------";
    settings += "\n#    Others";
    settings += "\n#---------------------------";

    settings += "\n\n# Draw the selected material\nSelectedMat: ";
    for (int i = 0; i < nSelectedMat; i++)
        settings += selectedMat.at(i) + " ";

    settings += "\n\n# Draw the selected detector\nSelectedDet: ";
    for (int i = 0; i < nSelectedDet; i++)
        settings += selectedDet.at(i) + " ";

    settings += "\n#---------------------------";
    return settings;
}

int MyGuiActionClass::ReadSIMSettingsText(const char *txtfile)
{
    int nline = 0;
    TGLongPosition pos(0, nline);

    tRadLayer.clear();
    sRadLayer.clear();
    pTransLayer.clear();
    sTransLayer.clear();
    mapTransLayers.clear();
    pImpurities.clear();
    sImpurities.clear();
    mapImpurities.clear();

    TGText *text = new TGText();
    if (BatGuiFlag == GUI)
        text = gMyMainFrameGui->GetText1();
    else
    {
        if (!text->Load(txtfile))
        {
            cout << "##### Can't open configure file " << txtfile << ". " << endl;
            return -1;
        }
    }

    while (text->GetLineLength(nline) != -1)
    {
        if (text->GetChar(pos) != -1)
        {
            TString line(text->GetLine(pos, 100));
            pos.fY = (nline++);

            if (line.BeginsWith("#") || line.Length() < 5)
                continue;

            vector<TString> cont = ReadContent(line);
            cont.erase(cont.begin());

            if (line.BeginsWith("Radiator"))
                sRadLayer.push_back(cont[0].Data());
            if (line.BeginsWith("Radiator"))
                tRadLayer.push_back(cont[1].Atof());

            if (line.BeginsWith("TransLayer"))
                sTransLayer.push_back(cont[0].Data());
            if (line.BeginsWith("TransLayer"))
                pTransLayer.push_back(cont[1].Atof());
            if (line.BeginsWith("TransThick"))
                tTransLayer = TString(cont[0].Data()).Atof();

            if (line.BeginsWith("Impurities"))
                sImpurities.push_back(cont[0].Data());
            if (line.BeginsWith("Impurities"))
                pImpurities.push_back(cont[1].Atof());

            PCDetector = (line.BeginsWith("PCDetector")) ? cont[0].Data() : PCDetector;
            pixel = (line.BeginsWith("PCDetector")) ? cont[1].Atof() : pixel;

            XBinMin = (line.BeginsWith("PixelRange")) ? cont[0].Atof() : XBinMin;
            XBinMax = (line.BeginsWith("PixelRange")) ? cont[1].Atof() : XBinMax;
            YBinMin = (line.BeginsWith("PixelRange")) ? cont[2].Atof() : YBinMin;
            YBinMax = (line.BeginsWith("PixelRange")) ? cont[3].Atof() : YBinMax;

            nLambda = (line.BeginsWith("Lambda")) ? cont[0].Atoi() : nLambda;
            lambdaMin = (line.BeginsWith("Lambda")) ? cont[1].Atof() : lambdaMin;
            lambdaMax = (line.BeginsWith("Lambda")) ? cont[2].Atof() : lambdaMax;

            trkStep = (line.BeginsWith("trkStep")) ? cont[0].Atof() : trkStep;
            nphi = (line.BeginsWith("nPhi")) ? int(cont[0].Atof() * 360) : nphi;
            epsilon = (line.BeginsWith("Precision")) ? cont[0].Atof() : epsilon;
            nEvent = (line.BeginsWith("nEvent")) ? cont[0].Atof() : nEvent;

            selectedMat = (line.BeginsWith("SelectedMat")) ? cont : selectedMat;
            selectedDet = (line.BeginsWith("SelectedDet")) ? cont : selectedDet;

            particle = (line.BeginsWith("Particle")) ? cont[0] : particle;
            momentum = (line.BeginsWith("Particle")) ? cont[1].Atof() : momentum;
            Theta0 = (line.BeginsWith("Particle")) ? cont[2].Atof() : Theta0;

            np = (line.BeginsWith("Momentum")) ? cont[0].Atoi() : np;
            pMin = (line.BeginsWith("Momentum")) ? cont[1].Atof() : pMin;
            pMax = (line.BeginsWith("Momentum")) ? cont[2].Atof() : pMax;

            nthe0 = (line.BeginsWith("Theta0")) ? cont[0].Atoi() : nthe0;
            The0Min = (line.BeginsWith("Theta0")) ? cont[1].Atof() : The0Min;
            The0Max = (line.BeginsWith("Theta0")) ? cont[2].Atof() : The0Max;

            pList = (line.BeginsWith("ShowMom")) ? cont[0].Atof() : pList;
            thList = (line.BeginsWith("ShowThe")) ? cont[0].Atof() : thList;
            irad1 = (line.BeginsWith("ShowRad")) ? cont[0].Atoi() : irad1;
            irad2 = (line.BeginsWith("ShowRad")) ? cont[1].Atoi() : irad2;
            iph = (line.BeginsWith("ShowPhoton")) ? cont[0].Atoi() : iph;

            continue;
        }
        pos.fY = (nline++);
    }

    mass = gMyCommonRICH->GetMass(particle);
    nRadLayer = (int)tRadLayer.size();
    nTransLayer = (int)pTransLayer.size();
    nImpurities = (int)pImpurities.size();
    nSelectedMat = (int)selectedMat.size();
    nSelectedDet = (int)selectedDet.size();
    NXBin = (XBinMax - XBinMin) / pixel;
    NYBin = (YBinMax - YBinMin) / pixel;
    for (int i = 0; i < nTransLayer; i++)
        mapTransLayers[sTransLayer[i]] = pTransLayer[i];
    for (int i = 0; i < nImpurities; i++)
        mapImpurities[sImpurities[i]] = pImpurities[i];

    return 0;
}

vector<TString> MyGuiActionClass::ReadContent(TString LINE)
{
    TString line = LINE;
    vector<TString> text;

    line.Remove(TString::kBoth, ' ');
    line.ReplaceAll("  ", " ");
    line.ReplaceAll("  ", " ");
    line.ReplaceAll("  ", " ");

    while (line.Length() > 0 && line.Index(' ') > 0)
    {
        TString head = line;
        line.Remove(0, line.Index(' ') + 1);
        head.Remove(head.Index(' '), head.Length());
        head.Remove(TString::kBoth, ' ');
        if (head.Length() > 0)
            text.push_back(head);
        if (line.Index(' ') == -1)
            text.push_back(line);
    }

    return text;
}

//______________________________________________________________________________
// basic button actions
void MyGuiActionClass::ExecButtonClick(Long_t bid, const char *cmdStr)
{
    if (bid == LoadTextBuf)
        return DoLoadTextBuf();

    ReadSIMSettingsText();
    if (bid == DrawConfig)
        DoDrawConfig("Show the configurations");
    if (bid == LoadDetFile)
        DoLoadDetFile(cmdStr);
    if (bid == SaveDetFile)
        DoSaveDetFile(cmdStr);
    if (bid == ShowFCN)
        DoShowFCN();

    if (bid == ShowSpecRICH)
        DoShowSpecRICH("no");
    if (bid == ShowMulParRICH)
        DoShowMulParRICH("no");
    if (bid == ShowScanRICHList)
        DoGenHitMaps("no");
    if (bid == ShowRecRICHList)
        DoRecRings("no");
    if (bid == ShowPIDEff)
        DoPIDEff("no");

    if (bid == GenSpecRICH)
        DoShowSpecRICH(cmdStr);
    if (bid == GenMulParRICH)
        DoShowMulParRICH(cmdStr);
    if (bid == GenScanRICHList)
        DoGenHitMaps(cmdStr);
    if (bid == GenRecRICHList)
        DoRecRings(cmdStr);
    if (bid == GenPIDEff)
        DoPIDEff(cmdStr);

    if (MatList <= bid && bid < DetList)
    {
        if (bid - MatList > gMyDatabaseClass->GetNMaterial())
            DoDrawSelectedMat();
        else
            ShowMaterialInfo(gMyDatabaseClass->GetMaterialName(bid - MatList));
    }
    if (DetList <= bid && bid < AnalysisAction)
    {
        if (bid - DetList > gMyDatabaseClass->GetNDetector())
            DoDrawSelectedDet();
        else
            ShowDetectorInfo(gMyDatabaseClass->GetDetectorName(bid - DetList));
    }

    //for beamtest
    if (bid == LoadBeamTextBuf)
        return DoLoadBeamTextBuf();

    ReadBEAMSettingsText();
    if (bid == DrawBeamTestConfig)
        DoDrawBeamTestConfig();
    if (bid == SetDSPath)
        DoSetDSPath(cmdStr);
    if (bid == ReadRawData)
        DoReadRawData(cmdStr);
    if (bid == CheckRawRoot)
        DoCheckRawRoot(cmdStr);
    if (bid == GenDSTRoot)
        DoGenDSTRoot(cmdStr);
    if (bid == CheckDSTRoot)
        DoCheckDSTRoot();
    if (bid == CombineDSTRoot)
        DoCombineDSTRoot(cmdStr);
    if (bid == LoadDSTRoot)
        DoLoadDSTRoot(cmdStr);
    if (bid == SlidDSTHit)
        DoSlidDSTHit(cmdStr);
    if (bid == PrevDSTHit)
        DoPrevDSTHit();
    if (bid == NextDSTHit)
        DoNextDSTHit();
    if (bid == ZoomToDefault)
        DoZoomToDefault();
    if (bid == AnalysisCMDRoot)
        DoAnalysisCMDRoot();
}

void MyGuiActionClass::DoDrawConfig(TString scap)
{
    double x0 = 0.02, x1 = 0.98;
    double y0 = 0.10, y1 = 0.15, y2 = 0.50, y3 = 0.98;

    gMyMainFrameGui->SwitchCanvas(0);
    gMyMainFrameGui->ClearCanvas(0);

    //detector
    TBox *bdet = new TBox(x0, y0, x1, y1);
    bdet->SetFillColor(kOrange - 2);
    bdet->Draw();
    TText *tdet = new TText(0.5, (y0 + y1) / 2., Form("%s = %.1f[mm]", PCDetector.c_str(), pixel));
    tdet->SetTextAlign(22);
    tdet->SetTextSize(.04);
    tdet->SetTextFont(42);
    tdet->Draw();

    //trans
    TBox *btrans = new TBox(x0, y1, x1, y2);
    btrans->SetFillColor(kCyan - 10);
    btrans->Draw();

    TString ttext1(""), ttext2("");
    for (int i = 0; i < nTransLayer; i++)
    {
        ttext1 += sTransLayer[i].c_str();
        ttext2 += Form("%.1f", pTransLayer[i]);
        if (i != nTransLayer - 1)
            ttext1 += ":";
        if (i != nTransLayer - 1)
            ttext2 += ":";
    }
    TString ttext3("");
    for (int i = 0; i < nImpurities; i++)
        ttext3 += Form("%s = %.1f[ppm]  ", sImpurities[i].c_str(), pImpurities[i]);
    TText *ttrans = new TText(0.5, (y1 + y2) / 2., ttext1 + " = " + ttext2);
    ttrans->SetTextAlign(22);
    ttrans->SetTextSize(.04);
    ttrans->SetTextFont(42);
    ttrans->Draw();

    TPaveText *pt = new TPaveText(.2, .25, 0.8, .3);
    pt->AddText(ttext3);
    pt->AddText(Form("Thickness = %.1f[mm]", tTransLayer));
    pt->SetFillColor(-1);
    pt->SetBorderSize(0);
    pt->SetTextFont(42);
    pt->Draw();

    //radiators
    int color[2] = {kAzure + 1, kAzure - 9};
    for (int i = 0; i < nRadLayer; i++)
    {
        double _y0 = y2 + (nRadLayer - i) * (y3 - y2) / nRadLayer;
        double _y1 = y2 + (nRadLayer - i - 1) * (y3 - y2) / nRadLayer;
        TBox *brad = new TBox(x0, _y0, x1, _y1);
        brad->SetFillColor(color[i % 2]);
        brad->Draw();
        TText *trad = new TText(0.5, (_y0 + _y1) / 2., Form("%s = %.1f[mm]", sRadLayer[i].c_str(), tRadLayer[i]));
        trad->SetTextAlign(22);
        trad->SetTextSize(.04);
        trad->SetTextFont(42);
        trad->Draw();
    }

    // current running parameters
    TText *tcap = new TText(0.5, 0.05, scap);
    tcap->SetTextFont(132);
    tcap->SetTextAlign(22);
    tcap->SetTextSize(.03);
    tcap->Draw();

    gMyMainFrameGui->UpdateCanvas(0);
}

void MyGuiActionClass::DoLoadTextBuf()
{
    gMyMainFrameGui->LoadText1(GenerateSIMSettingsText());
}

//______________________________________________________________________________
// 与读取/保存文件相关
void MyGuiActionClass::DoReadBatchFile(const char *fname, const char *sepoch)
{
    TString rootfile(fname);
    if (!rootfile.EndsWith(".txt"))
    {
        cout << "##### Configure file must end with '.txt'" << endl;
        return;
    }

    gMyCommonRICH->SetEpoch(TString(sepoch).Atoi());

    if (ReadSIMSettingsText(fname) == -1)
        return;

    SetDetectorParameters();
    if (gMyCommonRICH->GetEpoch() == 0) // detector.root
    {
        gMyCommonRICH->GenerateDetRing();
        gMyCommonRICH->GenerateMultiParticleRICHRings();
    }

    rootfile.ReplaceAll(".txt", ".root");
    gMyCommonRICH->SaveRings(rootfile);
    gMyCommonRICH->GenerateTheScanHitMapsForEachDetector(); //hitmap.root
    gMyCommonRICH->ReconstructForEachDetector();
    gMyCommonRICH->GenerateRecOffsetSigmaMap();
    gMyCommonRICH->CalPIDEfficiency();
}

void MyGuiActionClass::DoSaveDetFile(const char *fname)
{
    if (fname == NULL)
        return;

    DoShowSpecRICH("yes");
    DoShowMulParRICH("yes");

    gMyCommonRICH->SaveRings(fname);

    //update button status
    gMyMainFrameGui->EnableGenScanRICHButton();
}

void MyGuiActionClass::DoLoadDetFile(const char *fname)
{
    if (fname == NULL)
        return;
    int ret = gMyCommonRICH->LoadRings(fname);
    // return: -1 文件不存在
    //          1 gdet文件存在
    //          2 hitmap文件存在
    //          3 recmap文件存在
    //          4 pidmap文件存在

    if (ret == -1) //文件不存在
        return;

    gMyMainFrameGui->EnableShowSpecButton();
    gMyMainFrameGui->EnableShowMulParButton();
    gMyMainFrameGui->EnableGenScanRICHButton();

    if (ret > 1)
    {
        gMyMainFrameGui->EnableShowScanRICHButton();
        gMyMainFrameGui->EnableGenRecRICHButton();
    }

    if (ret > 2)
    {
        gMyMainFrameGui->EnableShowRecRICHButton();
        gMyMainFrameGui->EnableGenPIDEffButton();
    }

    if (ret > 3)
        gMyMainFrameGui->EnableShowPIDEffButton();

    GetDetectorParameters();
    DoLoadTextBuf();
    DoDrawConfig("Show the configurations");
}

//______________________________________________________________________________
// hitmap analysis button actions
void MyGuiActionClass::DoShowSpecRICH(TString cmdStr)
{
    SetDetectorParameters();
    if (cmdStr == "yes" || gMyCommonRICH->GetDetHitMap()->GetEntries() == 0)
        gMyCommonRICH->GenerateDetRing();

    if (BatGuiFlag == BATCH)
        return;

    gMyMainFrameGui->ClearAllCanvas();
    //1. 阳极上看到的光子击中
    gMyMainFrameGui->SwitchCanvas(1);
    gMyCommonRICH->DrawDetHitMap("colz");
    gMyMainFrameGui->UpdateCanvas(1);
    //sleep(1);

    //2. 击中的光子的波长分布，(暂未生成不同辐射体的光子波长分布，只生成了总的)
    gMyMainFrameGui->SwitchCanvas(2);
    gMyCommonRICH->GetDetWLMap()->Draw();
    gMyMainFrameGui->UpdateCanvas(2);
    //sleep(1);

    //3. 投影到X上的分布
    gMyMainFrameGui->SwitchCanvas(3);
    gMyCommonRICH->GetDetHitMap()->ProjectionX()->Draw();
    gMyMainFrameGui->UpdateCanvas(3);
    //sleep(1);

    //4. 显示哪几个辐射体的光子输出,(当用户输入过少的ShowRad会有bug，后面再改)
    gMyMainFrameGui->SwitchCanvas(4);
    gMyCommonRICH->DrawDetHitMap(irad1, "colz");
    gMyMainFrameGui->UpdateCanvas(4);
    //sleep(1);
    gMyMainFrameGui->SwitchCanvas(5);
    gMyCommonRICH->DrawDetHitMap(irad2, "colz");
    gMyMainFrameGui->UpdateCanvas(5);

    //sleep(1);
    gMyMainFrameGui->SwitchCanvas(1);

    //update button status
    gMyMainFrameGui->EnableShowSpecButton();
}

void MyGuiActionClass::DoShowMulParRICH(TString cmdStr)
{
    SetDetectorParameters();
    if (cmdStr == "yes" || gMyCommonRICH->GetDetListNumber() != NHYPO)
        gMyCommonRICH->GenerateMultiParticleRICHRings();

    gMyMainFrameGui->ClearAllCanvas();

    //1. 不同粒子的光子击中分布图
    gMyMainFrameGui->SwitchCanvas(1);
    gMyStyle->SetTitle("Cherenkov Rings for p/K/#pi/#mu/e");
    gMyStyle->SetXLabel("X[mm]");
    gMyStyle->SetYLabel("Y[mm]");
    gMyStyle->SetDrawOption("");
    for (int ihypo = 0; ihypo < gMyCommonRICH->GetDetector()->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, Form("%s Nph=%.1f", gMyCommonRICH->GetDetList(ihypo)->particle.Data(), gMyCommonRICH->GetDetListHitMap(ihypo)->Integral()));
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetListHitMap(0), gMyCommonRICH->GetDetListHitMap(1), gMyCommonRICH->GetDetListHitMap(2), gMyCommonRICH->GetDetListHitMap(3), gMyCommonRICH->GetDetListHitMap(4));
    gMyMainFrameGui->UpdateCanvas(1);

    //2. 分别显示n种粒子的击中分布图，要看更详细的就用单个的看
    for (int ihypo = 0; ihypo < gMyCommonRICH->GetDetector()->nhypo; ihypo++)
    {
        gMyMainFrameGui->SwitchCanvas(2 + ihypo);
        gMyCommonRICH->DrawDetListHitMap(ihypo, "colz");
        gMyMainFrameGui->UpdateCanvas(2 + ihypo);
    }
    gMyMainFrameGui->SwitchCanvas(1);

    //update button status
    gMyMainFrameGui->EnableShowMulParButton();
}

void MyGuiActionClass::DoGenHitMaps(TString cmdStr)
{
    SetDetectorParameters();
    if (cmdStr == "yes" || gMyCommonRICH->GetDetScanNumber() == 0)
        gMyCommonRICH->GenerateTheScanHitMapsForEachDetector();
    gMyCommonRICH->GenerateTheNPhotonMap();

    gMyMainFrameGui->ClearAllCanvas();

    MyRICHDetector *gDet = gMyCommonRICH->GetDetector();
    int imom = gMyCommonRICH->GetMomID(pList);
    int ithe = gMyCommonRICH->GetThetaID(thList);
    imom = (0 <= imom && imom < gDet->np) ? imom : 0;
    ithe = (0 <= ithe && ithe < gDet->nthe0) ? ithe : 0;
    double mom = gMyCommonRICH->GetDetScan(imom, ithe, 0)->momentum;
    double the = gMyCommonRICH->GetDetScan(imom, ithe, 0)->Theta0;

    //1. 用ShowMom和ShowThe指定了动量和入射角度的四种粒子的光子击中分布图
    gMyStyle->SetTitle(Form("Cherenkov Rings for [%.1fGeV/c, %.1f#circ] e/#mu/#pi/K/p", mom, the));
    gMyStyle->SetXLabel("X[mm]");
    gMyStyle->SetYLabel("Y[mm]");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, Form("%s Nph=%.1f", gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle.Data(), gMyCommonRICH->GetDetScanHitMap(imom, ithe, ihypo)->Integral()));
    gMyStyle->SetDrawOption("");
    gMyMainFrameGui->SwitchCanvas(1);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetScanHitMap(imom, ithe, 0), gMyCommonRICH->GetDetScanHitMap(imom, ithe, 1), gMyCommonRICH->GetDetScanHitMap(imom, ithe, 2), gMyCommonRICH->GetDetScanHitMap(imom, ithe, 3), gMyCommonRICH->GetDetScanHitMap(imom, ithe, 4));
    gMyMainFrameGui->UpdateCanvas(1);
    //sleep(1);

    //2. ShowMom的粒子产生的切伦科夫光子数随不同入射角度的变化
    gMyMainFrameGui->SwitchCanvas(2);
    gMyStyle->SetTitle(Form("Number of photon for [%.1fGeV/c] e/#mu/#pi/K/p", mom));
    gMyStyle->SetXLabel("#Theta [degree]");
    gMyStyle->SetYLabel("Number of photon");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyStyle->SetDrawOption("");
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetScanNphMapAtMom(0, mom), -1, -1, gMyCommonRICH->GetDetScanNphMapAtMom(1, mom), gMyCommonRICH->GetDetScanNphMapAtMom(2, mom), gMyCommonRICH->GetDetScanNphMapAtMom(3, mom), gMyCommonRICH->GetDetScanNphMapAtMom(4, mom));
    gMyMainFrameGui->UpdateCanvas(2);
    //sleep(1);

    //3. ShowThe的粒子产生的切伦科夫光子数随不同入射动量的变化
    gMyMainFrameGui->SwitchCanvas(3);
    gMyStyle->SetTitle(Form("Number of photon for [#theta=%.1f#circ] e/#mu/#pi/K/p", the));
    gMyStyle->SetXLabel("momentum [GeV/c]");
    gMyStyle->SetYLabel("Number of photon");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyStyle->SetDrawOption("");
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetScanNphMapAtTheta(0, the), -1, -1, gMyCommonRICH->GetDetScanNphMapAtTheta(1, the), gMyCommonRICH->GetDetScanNphMapAtTheta(2, the), gMyCommonRICH->GetDetScanNphMapAtTheta(3, the), gMyCommonRICH->GetDetScanNphMapAtTheta(4, the));
    gMyMainFrameGui->UpdateCanvas(3);
    //sleep(1);

    //4. 对指定动量和角度的入射粒子产生光子数的二维分布图
    gMyMainFrameGui->SwitchCanvas(4);
    gMyCommonRICH->GetDetScanNPhMap(particle)->Draw("colz");
    gMyMainFrameGui->UpdateCanvas(4);
    //sleep(1);

    //5. 对这一指定的粒子，不同辐射体产生的光子数随角度变化
    gMyMainFrameGui->SwitchCanvas(5);
    gMyStyle->SetTitle(Form("Number of photon from each radiator for [%.1fGeV/c] %s", mom, particle.Data()));
    gMyStyle->SetXLabel("#Theta [degree]");
    gMyStyle->SetYLabel("Number of photon");
    gMyStyle->ClearPreset1DHist();
    for (int irad = 0; irad < gDet->nRadLayer; irad++)
    {
        gMyStyle->SetLegends(irad, gDet->sRadLayer[irad].c_str());
        gMyStyle->SetHistogram(gMyCommonRICH->GetDetScanNphEachRadMapAtMom(particle, mom, irad));
    }
    gMyStyle->Draw1DHistograms();
    gMyMainFrameGui->UpdateCanvas(5);
    //sleep(1);

    //6. 对这一指定的粒子，不同辐射体产生的光子数随动量变化
    gMyMainFrameGui->SwitchCanvas(6);
    gMyStyle->SetTitle(Form("Number of photon from each radiator for [#theta=%.1f#circ] %s", the, particle.Data()));
    gMyStyle->SetXLabel("momentum [GeV/c]");
    gMyStyle->SetYLabel("Number of photon");
    gMyStyle->ClearPreset1DHist();
    for (int irad = 0; irad < gDet->nRadLayer; irad++)
    {
        gMyStyle->SetLegends(irad, gDet->sRadLayer[irad].c_str());
        gMyStyle->SetHistogram(gMyCommonRICH->GetDetScanNphEachRadMapAtTheta(particle, the, irad));
    }
    gMyStyle->Draw1DHistograms(0);
    gMyMainFrameGui->UpdateCanvas(6);
    //sleep(1);

    //7. 对这一指定的粒子，不同辐射体产生的光子数hitmap
    gMyMainFrameGui->SwitchCanvas(7);
    gMyCommonRICH->DrawScanDetListHitMap(particle, imom, ithe, irad1, "colz");
    gMyMainFrameGui->UpdateCanvas(7);
    //sleep(1);

    //8. 对这一指定的粒子，不同辐射体产生的光子数hitmap
    gMyMainFrameGui->SwitchCanvas(8);
    gMyCommonRICH->DrawScanDetListHitMap(particle, imom, ithe, irad2, "colz");
    gMyMainFrameGui->UpdateCanvas(8);
    //sleep(1);

    gMyMainFrameGui->SwitchCanvas(1);

    //update button status
    gMyMainFrameGui->EnableShowScanRICHButton();
    gMyMainFrameGui->EnableGenRecRICHButton();
}

//______________________________________________________________________________
// reconstruction / PID efficiency analysis button actions
void MyGuiActionClass::DoRecRings(TString cmdStr)
{
    SetDetectorParameters();
    if (cmdStr == "yes")
    {
        if (!gMyCommonRICH->ReconstructForEachDetector())
            return;
    }

    if (gMyCommonRICH->GetDetRectNumber() == 0)
        gMyCommonRICH->GenerateRecOffsetSigmaMap();

    MyRICHDetector *gDet = gMyCommonRICH->GetDetector();
    int irad = irad1;
    int imom = gMyCommonRICH->GetMomID(pList);
    int ithe = gMyCommonRICH->GetThetaID(thList);
    imom = (0 <= imom && imom < gDet->np) ? imom : 0;
    ithe = (0 <= ithe && ithe < gDet->nthe0) ? ithe : 0;
    double mom = gMyCommonRICH->GetDetScan(imom, ithe, 0)->momentum;
    double the = gMyCommonRICH->GetDetScan(imom, ithe, 0)->Theta0;
    TString rad = gDet->sRadLayer[irad];

    gMyCommonRICH->GenerateRecHistograms(particle, irad, imom, ithe, iph);
    gMyMainFrameGui->ClearAllCanvas();

    //-----offset
    //1. 指定irad辐射体，重建offset随着粒子不同动量和角度的分布
    gMyMainFrameGui->SwitchCanvas(1);
    gMyCommonRICH->GetDetRecOffsetMap()->Draw("colz");
    gMyMainFrameGui->UpdateCanvas(1);

    //2. 相同动量的粒子重建offset随着不同入射角度的分布
    gMyStyle->SetTitle(Form("Reconstruct mean #theta_c from %d nPhoton for [%.1fGeV/c] from %s", iph, mom, rad.Data()));
    gMyStyle->SetXLabel("#theta_0[#circ]");
    gMyStyle->SetYLabel("Reconstruct #theta_c mean[rad]");
    gMyStyle->SetDrawOption("");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyMainFrameGui->SwitchCanvas(2);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetRecOffsetVsThetaPlot(0), -1, -1, gMyCommonRICH->GetDetRecOffsetVsThetaPlot(1), gMyCommonRICH->GetDetRecOffsetVsThetaPlot(2), gMyCommonRICH->GetDetRecOffsetVsThetaPlot(3), gMyCommonRICH->GetDetRecOffsetVsThetaPlot(4));
    gMyMainFrameGui->UpdateCanvas(2);

    //3. 相同入射角度的粒子重建offset随着不同动量的分布
    gMyStyle->SetTitle(Form("Reconstruct mean #theta_c from %d nPhoton for [%.1f#circ] from %s", iph, the, rad.Data()));
    gMyStyle->SetXLabel("momentum [GeV/c]");
    gMyStyle->SetYLabel("Reconstruct #theta_c mean[rad]");
    gMyStyle->SetDrawOption("");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyMainFrameGui->SwitchCanvas(3);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetRecOffsetVsMomPlot(0), -1, -1, gMyCommonRICH->GetDetRecOffsetVsMomPlot(1), gMyCommonRICH->GetDetRecOffsetVsMomPlot(2), gMyCommonRICH->GetDetRecOffsetVsMomPlot(3), gMyCommonRICH->GetDetRecOffsetVsMomPlot(4));
    gMyMainFrameGui->UpdateCanvas(3);

    //-----sigma
    //4. 指定irad辐射体，重建sigma随着粒子不同动量和角度的分布
    gMyMainFrameGui->SwitchCanvas(4);
    gMyCommonRICH->GetDetRecSigmaMap()->Draw("colz");
    gMyMainFrameGui->UpdateCanvas(4);

    //5. 相同动量的粒子重建sigma随着不同入射角度的分布
    gMyStyle->SetTitle(Form("Reconstruct sigma #theta_c from %d nPhoton for [%.1fGeV/c] from %s", iph, mom, rad.Data()));
    gMyStyle->SetXLabel("#theta_0[#circ]");
    gMyStyle->SetYLabel("Reconstruct #theta_c sigma[rad]");
    gMyStyle->SetDrawOption("");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyMainFrameGui->SwitchCanvas(5);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetRecSigmaVsThetaPlot(0), -1, -1, gMyCommonRICH->GetDetRecSigmaVsThetaPlot(1), gMyCommonRICH->GetDetRecSigmaVsThetaPlot(2), gMyCommonRICH->GetDetRecSigmaVsThetaPlot(3), gMyCommonRICH->GetDetRecSigmaVsThetaPlot(4));
    gMyMainFrameGui->UpdateCanvas(5);

    //6. 相同入射角度的粒子重建sigma随着不同动量的分布
    gMyStyle->SetTitle(Form("Reconstruct sigma #theta_c from %d nPhoton for [%.1f#circ] from %s", iph, the, rad.Data()));
    gMyStyle->SetXLabel("momentum [GeV/c]");
    gMyStyle->SetYLabel("Reconstruct #theta_c sigma[rad]");
    gMyStyle->SetDrawOption("");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyMainFrameGui->SwitchCanvas(6);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetRecSigmaVsMomPlot(0), -1, -1, gMyCommonRICH->GetDetRecSigmaVsMomPlot(1), gMyCommonRICH->GetDetRecSigmaVsMomPlot(2), gMyCommonRICH->GetDetRecSigmaVsMomPlot(3), gMyCommonRICH->GetDetRecSigmaVsMomPlot(4));
    gMyMainFrameGui->UpdateCanvas(6);

    //7. 相同动量，相同入射角度的粒子重建的二维分布
    gMyStyle->SetTitle(Form("Cherenkov Rings for [%.1fGeV/c, %.1f#circ] #mu/#pi/K/p", mom, the));
    gMyStyle->SetXLabel("Number of photon");
    gMyStyle->SetYLabel("Reconstruct #theta_c[#circ]");
    gMyStyle->SetDrawOption("");
    gMyMainFrameGui->SwitchCanvas(7);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetRecRing(imom, ithe, 0), gMyCommonRICH->GetDetRecRing(imom, ithe, 1), gMyCommonRICH->GetDetRecRing(imom, ithe, 2), gMyCommonRICH->GetDetRecRing(imom, ithe, 3), gMyCommonRICH->GetDetRecRing(imom, ithe, 4));
    gMyMainFrameGui->UpdateCanvas(7);

    //8. 相同动量，相同入射角度的粒子重建offset随着不同光子数的分布
    gMyStyle->SetTitle(Form("Reconstruct offset #theta_c for [%.1fGeV/c, %.1f#circ] from %s", mom, the, rad.Data()));
    gMyStyle->SetXLabel("Number of photon");
    gMyStyle->SetYLabel("Reconstruct offset[rad]");
    gMyStyle->SetDrawOption("ep");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyMainFrameGui->SwitchCanvas(8);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetRecOffsetVsNphPlot(0), -1, -1, gMyCommonRICH->GetDetRecOffsetVsNphPlot(1), gMyCommonRICH->GetDetRecOffsetVsNphPlot(2), gMyCommonRICH->GetDetRecOffsetVsNphPlot(3), gMyCommonRICH->GetDetRecOffsetVsNphPlot(4));
    gMyMainFrameGui->UpdateCanvas(8);

    //9. 相同动量，相同入射角度的粒子重建sigma随着不同光子数的分布
    gMyStyle->SetTitle(Form("Reconstruct sigma #theta_c nPhoton for [%.1fGeV/c, %.1f#circ] from %s", mom, the, rad.Data()));
    gMyStyle->SetXLabel("Number of photon");
    gMyStyle->SetYLabel("Reconstruct #theta_c sigma[rad]");
    gMyStyle->SetDrawOption("ep");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyMainFrameGui->SwitchCanvas(9);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetDetRecSigmaVsNphPlot(0), -1, -1, gMyCommonRICH->GetDetRecSigmaVsNphPlot(1), gMyCommonRICH->GetDetRecSigmaVsNphPlot(2), gMyCommonRICH->GetDetRecSigmaVsNphPlot(3), gMyCommonRICH->GetDetRecSigmaVsNphPlot(4));
    gMyMainFrameGui->UpdateCanvas(9);

    //10. 显示单个的填图及拟合结果
    gMyMainFrameGui->SwitchCanvas(10);
    gMyMainFrameGui->DivideCanvas(10, 2, 2);
    gMyMainFrameGui->SwitchDivCanvas(10, 1);
    if (gMyCommonRICH->GetRecMap(particle, irad, imom, ithe, iph) != NULL)
        gMyCommonRICH->GetRecMap(particle, irad, imom, ithe, iph)->Draw();
    gMyMainFrameGui->SwitchDivCanvas(10, 2);
    if (gMyCommonRICH->GetRecMap(particle, irad, imom, ithe, iph + 1) != NULL)
        gMyCommonRICH->GetRecMap(particle, irad, imom, ithe, iph + 1)->Draw();
    gMyMainFrameGui->SwitchDivCanvas(10, 3);
    if (gMyCommonRICH->GetRecMap(particle, irad, imom, ithe, iph + 2) != NULL)
        gMyCommonRICH->GetRecMap(particle, irad, imom, ithe, iph + 2)->Draw();
    gMyMainFrameGui->SwitchDivCanvas(10, 4);
    if (gMyCommonRICH->GetRecMap(particle, irad, imom, ithe, iph + 3) != NULL)
        gMyCommonRICH->GetRecMap(particle, irad, imom, ithe, iph + 3)->Draw();
    gMyMainFrameGui->UpdateCanvas(10);

    gMyMainFrameGui->SwitchCanvas(1);

    //update button status
    gMyMainFrameGui->EnableShowRecRICHButton();
    gMyMainFrameGui->EnableGenPIDEffButton();
}

void MyGuiActionClass::DoPIDEff(TString cmdStr)
{
    SetDetectorParameters();
    if (cmdStr == "yes")
    {
        if (!gMyCommonRICH->CalPIDEfficiency())
            return;
    }

    gMyMainFrameGui->ClearAllCanvas();

    MyRICHDetector *gDet = gMyCommonRICH->GetDetector();
    int imom = gMyCommonRICH->GetMomID(pList);
    int ithe = gMyCommonRICH->GetThetaID(thList);
    imom = (0 <= imom && imom < gDet->np) ? imom : 0;
    ithe = (0 <= ithe && ithe < gDet->nthe0) ? ithe : 0;
    double mom = gMyCommonRICH->GetDetScan(imom, ithe, 0)->momentum;
    double the = gMyCommonRICH->GetDetScan(imom, ithe, 0)->Theta0;

    gMyCommonRICH->GeneratePIDHistograms(particle, imom, ithe);

    //-----pid-map
    for (int i = 0; i < NHYPO; i++)
    {
        gMyMainFrameGui->SwitchCanvas(1 + i);
        gMyCommonRICH->GetPIDMap(i)->Draw("colz");
        gMyMainFrameGui->UpdateCanvas(1 + i);
    }

    //-----pid vs. momentum
    gMyStyle->SetTitle(Form("Reconstruct pid efficienchy for [%.1f#circ] from %s", the, particle.Data()));
    gMyStyle->SetXLabel("momentum [GeV/c]");
    gMyStyle->SetYLabel("Reconstruct PID efficiency[%]");
    gMyStyle->SetDrawOption("ep");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyMainFrameGui->SwitchCanvas(6);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetPIDMapVsMom(0), -1, -1, gMyCommonRICH->GetPIDMapVsMom(1), gMyCommonRICH->GetPIDMapVsMom(2), gMyCommonRICH->GetPIDMapVsMom(3), gMyCommonRICH->GetPIDMapVsMom(4));
    gMyMainFrameGui->UpdateCanvas(6);

    //-----pid vs. theta
    gMyStyle->SetTitle(Form("Reconstruct pid efficienchy for [%.1fGeV/c] from %s", mom, particle.Data()));
    gMyStyle->SetXLabel("#theta_0[#circ]");
    gMyStyle->SetYLabel("Reconstruct PID efficiency[%]");
    gMyStyle->SetDrawOption("ep");
    for (int ihypo = 0; ihypo < gDet->nhypo; ihypo++)
        gMyStyle->SetLegends(ihypo, gMyCommonRICH->GetDetScan(imom, ithe, ihypo)->particle);
    gMyMainFrameGui->SwitchCanvas(7);
    gMyStyle->DrawHistograms(gMyCommonRICH->GetPIDMapVsTheta(0), -1, -1, gMyCommonRICH->GetPIDMapVsTheta(1), gMyCommonRICH->GetPIDMapVsTheta(2), gMyCommonRICH->GetPIDMapVsTheta(3), gMyCommonRICH->GetPIDMapVsTheta(4));
    gMyMainFrameGui->UpdateCanvas(7);

    gMyMainFrameGui->SwitchCanvas(1);

    //update button status
    gMyMainFrameGui->EnableShowPIDEffButton();
}

//______________________________________________________________________________
// for beamtest data analysis
void MyGuiActionClass::DoLoadBeamTextBuf()
{
    gMyMainFrameGui->LoadText2(GenerateBEAMSettingsText());
}

TString MyGuiActionClass::GenerateBEAMSettingsText()
{
    return gMyBeamTest->GenerateSettingText();
}

int MyGuiActionClass::ReadBEAMSettingsText()
{
    return gMyBeamTest->ReadSettingsText(gMyMainFrameGui->GetText2());
}

void MyGuiActionClass::DoDrawBeamTestConfig()
{
    gMyMainFrameGui->SwitchCanvas(0);
    gMyMainFrameGui->ClearCanvas(0);
    gMyBeamTest->DrawConfig();
    gMyMainFrameGui->UpdateCanvas(0);
}

void MyGuiActionClass::DoZoomToDefault()
{
    TView *view = gPad->GetView();
    if (view == NULL)
        return;
    view->RotateView(90, 0);
    view->ZoomIn();
    view->ZoomIn();
    view->ZoomIn();
    view->ZoomIn();
    view->ZoomIn();
    view->ZoomIn();
    gPad->Modified();
    gPad->Update();
}

//_________________________
// read beam-test dat file and save to -raw.root
void MyGuiActionClass::DoSetDSPath(const char *fname)
{
    gMyBeamTest->SetDSPath(fname);
    DoLoadBeamTextBuf();
}

void MyGuiActionClass::DoReadRawData(TString cmdStr)
{
    if (!CheckIdelDatFile(cmdStr))
        return;

    TString fileDir = gMyBeamTest->GetDSPath();
    fileDir.ReplaceAll("idle.dat", "");

    TString fileDir1(fileDir + "/RICH/");
    TString fileDir2(fileDir + "/TrackAGET/");
    TString fileDir3(fileDir + "/TrackVMM/");
    TString fileDir4(fileDir + "/Combine/");

    vector<TString> datList1;
    vector<TString> datList2;
    vector<TString> datList3;

    GetFileList(fileDir1, ".dat", datList1);
    GetFileList(fileDir2, ".dat", datList2);
    GetFileList(fileDir3, ".bin", datList3);

    if (datList1.size() > 0)
    {
        ReadRICHData2Root(datList1, gMyBeamTest->GenPath(RICH, RAW), 0);
        GenerateRICHPed(gMyBeamTest->GenPath(RICH, RAW), gMyBeamTest->GenPath(RICH, PED), gMyBeamTest->GetRICHDet());
    }

    if (datList2.size() > 0)
    {
        ReadTrackAGTData2Root(datList2, gMyBeamTest->GenPath(TrackerAGET, RAW), 0);
        GenerateAGETPed(gMyBeamTest->GenPath(TrackerAGET, RAW), gMyBeamTest->GenPath(TrackerAGET, PED), gMyBeamTest->GetTrackAGET());
    }

    if (datList3.size() > 0)
        ReadTrackVMMData2Root(datList3, gMyBeamTest->GenPath(TrackerVMM, RAW), 1);
}

void MyGuiActionClass::DoCheckRawRoot(TString cmdStr)
{
    int ID = RICH;
    if (cmdStr == "TrackerAGET")
        ID = TrackerAGET;
    if (cmdStr == "TrackerVMM")
        ID = TrackerVMM;

    if (cmdStr != "TrackerVMM")
    {
        TH1::AddDirectory(kFALSE);
        TH1F *fNoiseMean;
        TH1F *fNoiseRMS;
        TGraph2D *fWaveAll[10];

        TFile fPedFile(gMyBeamTest->GenPath(ID, PED));
        if (!fPedFile.IsOpen())
            return;
        cout << "--> Opening pedestal file: " << gMyBeamTest->GenPath(ID, PED) << endl;

        fNoiseMean = (TH1F *)fPedFile.Get("fNoiseMean");
        fNoiseRMS = (TH1F *)fPedFile.Get("fNoiseRMS");
        fWaveAll[0] = (TGraph2D *)fPedFile.Get("gWaveAll0");
        fWaveAll[1] = (TGraph2D *)fPedFile.Get("gWaveAll1");
        fWaveAll[2] = (TGraph2D *)fPedFile.Get("gWaveAll2");
        fWaveAll[3] = (TGraph2D *)fPedFile.Get("gWaveAll3");

        int cid = 1;
        gMyMainFrameGui->ClearAllCanvas();
        gMyMainFrameGui->SwitchCanvas(cid);
        if (fNoiseMean != NULL)
            fNoiseMean->Draw();
        gMyMainFrameGui->UpdateCanvas(cid++);

        gMyMainFrameGui->SwitchCanvas(cid);
        if (fNoiseRMS != NULL)
            fNoiseRMS->Draw();
        gMyMainFrameGui->UpdateCanvas(cid++);

        gMyMainFrameGui->SwitchCanvas(cid);
        if (fWaveAll[0] != NULL)
            fWaveAll[0]->Draw("pcol");
        gMyMainFrameGui->UpdateCanvas(cid++);

        gMyMainFrameGui->SwitchCanvas(cid);
        if (fWaveAll[1] != NULL)
            fWaveAll[1]->Draw("pcol");
        gMyMainFrameGui->UpdateCanvas(cid++);

        gMyMainFrameGui->SwitchCanvas(cid);
        if (fWaveAll[2] != NULL)
            fWaveAll[2]->Draw("pcol");
        gMyMainFrameGui->UpdateCanvas(cid++);

        gMyMainFrameGui->SwitchCanvas(cid);
        if (fWaveAll[3] != NULL)
            fWaveAll[3]->Draw("pcol");
        gMyMainFrameGui->UpdateCanvas(cid++);

        fPedFile.Close();
    }

    env->SetValue("checkRawRoot", gMyBeamTest->GenPath(ID, RAW));
    env->SetValue("checkPedRoot", gMyBeamTest->GenPath(ID, PED));
    env->SetValue("checkRawType", ID);
    env->Save();
    gSystem->Exec(Form("(root %s/data/checkRawRoot.C)", realPath.Data()));
}

//_________________________
// convert beam-test raw root file to a new data structure
void MyGuiActionClass::DoGenDSTRoot(TString cmdStr)
{
    if (!CheckIdelDatFile(cmdStr))
        return;

    bool SaveWaveFlag = kFALSE;
    gMyBeamTest->ConvtRICHRoot(gMyBeamTest->GetDSPath().Data(), SaveWaveFlag);
    gMyBeamTest->ConvtTrackAGTRoot(gMyBeamTest->GetDSPath().Data(), SaveWaveFlag);
    gMyBeamTest->ConvtTrackVMMRoot(gMyBeamTest->GetDSPath().Data());
}

void MyGuiActionClass::DoCheckDSTRoot()
{
    env->SetValue("checkDSTRoot", gMyBeamTest->GenPath(RICH, DST));
    env->Save();
    gSystem->Exec(Form("(root %s/data/checkDSTRoot.C)", realPath.Data()));
}

//_________________________
// Combine Results
void MyGuiActionClass::DoCombineDSTRoot(TString cmdStr)
{
    if (!CheckIdelDatFile(cmdStr))
        return;

    gMyBeamTest->CombineDSTRoot(gMyBeamTest->GetDSPath().Data());
}

//_________________________
// Load Results
void MyGuiActionClass::DoLoadDSTRoot(TString cmdStr)
{
    if (cmdStr == "no")
        return;

    DoDrawBeamTestConfig();

    if (!gMyBeamTest->GetDSPath().EndsWith("idle.dat"))
    {
        cout << "#### Error, please select the 'idle.dat' file." << endl;
        return;
    }

    gMyMainFrameGui->SetWindowName(gMyBeamTest->GetDSPath().ReplaceAll("idle.dat", ""));
    gMyMainFrameGui->EnableShowGeoSliderButton();

    //读入文件
    int cid = 0;
    gMyMainFrameGui->SwitchCanvas(cid);
    gMyMainFrameGui->ClearCanvas(cid);
    gMyBeamTest->LoadDSTRoot(gMyBeamTest->GetDSPath().Data());
    DoZoomToDefault();
    gMyMainFrameGui->UpdateCanvas(cid++);
    gMyMainFrameGui->SetGeoSliderRange(gMyBeamTest->GetDSTEntries()); //根据文件内的事例数，设置slider范围

    //画分布图-RICH
    gStyle->SetPalette(57);
    gMyMainFrameGui->SwitchCanvas(cid);
    gMyMainFrameGui->ClearCanvas(cid);
    TCanvas *can1 = gMyMainFrameGui->GetCanvas(cid);
    can1->Divide(3, 2);
    can1->cd(1);
    gMyBeamTest->GetRICHDet()[0]->GetAllHit()->Draw("colz");
    can1->cd(2);
    gMyBeamTest->GetRICHDet()[0]->GetCharge()->Draw();
    can1->cd(4);
    gMyBeamTest->GetRICHDet()[0]->GetAllHit2()->Draw("colz");
    can1->cd(3);
    gMyBeamTest->GetRICHDet()[0]->GetAllHit()->ProjectionX()->Draw();
    can1->cd(6);
    gMyBeamTest->GetRICHDet()[0]->GetAllHit()->ProjectionY()->Draw();
    gMyMainFrameGui->UpdateCanvas(cid++);

    //Track02
    gMyMainFrameGui->SwitchCanvas(cid);
    gMyMainFrameGui->ClearCanvas(cid);
    TCanvas *can2 = gMyMainFrameGui->GetCanvas(cid);
    can2->Divide(3, 2);
    can2->cd(1);
    gMyBeamTest->GetTrackAGET()[0]->GetAllHit()->Draw("colz");
    can2->cd(4);
    gMyBeamTest->GetTrackAGET()[0]->GetAllHit2()->Draw("colz");
    can2->cd(2);
    gMyBeamTest->GetTrackAGET()[0]->GetCharge()->Draw();
    can2->cd(3);
    gMyBeamTest->GetTrackAGET()[0]->GetHitXY()[0]->Draw();
    can2->cd(6);
    gMyBeamTest->GetTrackAGET()[0]->GetHitXY()[1]->Draw();
    gMyMainFrameGui->UpdateCanvas(cid++);

    //Track03
    gMyMainFrameGui->SwitchCanvas(cid);
    gMyMainFrameGui->ClearCanvas(cid);
    TCanvas *can3 = gMyMainFrameGui->GetCanvas(cid);
    can3->Divide(3, 2);
    can3->cd(1);
    gMyBeamTest->GetTrackAGET()[1]->GetAllHit()->Draw("colz");
    can3->cd(4);
    gMyBeamTest->GetTrackAGET()[1]->GetAllHit2()->Draw("colz");
    can3->cd(2);
    gMyBeamTest->GetTrackAGET()[1]->GetCharge()->Draw();
    can3->cd(3);
    gMyBeamTest->GetTrackAGET()[1]->GetHitXY()[0]->Draw();
    can3->cd(6);
    gMyBeamTest->GetTrackAGET()[1]->GetHitXY()[1]->Draw();
    gMyMainFrameGui->UpdateCanvas(cid++);

    //Track06
    gMyMainFrameGui->SwitchCanvas(cid);
    gMyMainFrameGui->ClearCanvas(cid);
    TCanvas *can4 = gMyMainFrameGui->GetCanvas(cid);
    can4->Divide(3, 2);
    can4->cd(1);
    gMyBeamTest->GetTrackAGET()[2]->GetAllHit()->Draw("colz");
    can4->cd(4);
    gMyBeamTest->GetTrackAGET()[2]->GetAllHit2()->Draw("colz");
    can4->cd(2);
    gMyBeamTest->GetTrackAGET()[2]->GetCharge()->Draw();
    can4->cd(3);
    gMyBeamTest->GetTrackAGET()[2]->GetHitXY()[0]->Draw();
    can4->cd(6);
    gMyBeamTest->GetTrackAGET()[2]->GetHitXY()[1]->Draw();
    gMyMainFrameGui->UpdateCanvas(cid++);

    //Track04
    gMyMainFrameGui->SwitchCanvas(cid);
    gMyMainFrameGui->ClearCanvas(cid);
    TCanvas *can5 = gMyMainFrameGui->GetCanvas(cid);
    can5->Divide(3, 2);
    can5->cd(1);
    gMyBeamTest->GetTrackVMM()[0]->GetAllHit()->Draw("colz");
    can5->cd(4);
    gMyBeamTest->GetTrackVMM()[0]->GetAllHit2()->Draw("colz");
    can5->cd(2);
    gMyBeamTest->GetTrackVMM()[0]->GetCharge()->Draw();
    can5->cd(3);
    gMyBeamTest->GetTrackVMM()[0]->GetHitXY()[0]->Draw();
    can5->cd(6);
    gMyBeamTest->GetTrackVMM()[0]->GetHitXY()[1]->Draw();
    gMyMainFrameGui->UpdateCanvas(cid++);

    //Track05
    gMyMainFrameGui->SwitchCanvas(cid);
    gMyMainFrameGui->ClearCanvas(cid);
    TCanvas *can6 = gMyMainFrameGui->GetCanvas(cid);
    can6->Divide(3, 2);
    can6->cd(1);
    gMyBeamTest->GetTrackVMM()[1]->GetAllHit()->Draw("colz");
    can6->cd(4);
    gMyBeamTest->GetTrackVMM()[1]->GetAllHit2()->Draw("colz");
    can6->cd(2);
    gMyBeamTest->GetTrackVMM()[1]->GetCharge()->Draw();
    can6->cd(3);
    gMyBeamTest->GetTrackVMM()[1]->GetHitXY()[0]->Draw();
    can6->cd(6);
    gMyBeamTest->GetTrackVMM()[1]->GetHitXY()[1]->Draw();
    gMyMainFrameGui->UpdateCanvas(cid++);

    /*
    TH2F *ftmp = gMyBeamTest->GetTrackAGET()[2]->GetAllHit();
    double meanX = ftmp->GetMean(1);
    double meanY = ftmp->GetMean(2);
    cout<<"Get events from ("<<meanX<<", "<<meanY<<")"<<endl;
    double entry = 0;
    for(int i=0; i<128; i++)
        for(int j=0; j<128; j++)
        {
            if(fabs(ftmp->GetXaxis()->GetBinCenter(i)-meanX)<5/0.4 && 
               fabs(ftmp->GetYaxis()->GetBinCenter(j)-meanY)<5/0.4)
                entry+=ftmp->GetBinContent(i, j);
        }
    cout<<"In area: "<<entry<<" / Total entry: "<<ftmp->GetEntries()<<endl;
    */

    gMyMainFrameGui->SwitchCanvas(0);
}

void MyGuiActionClass::DoAnalysisCMDRoot()
{
    gMyBeamTest->AnalysisDSTRoot(gMyBeamTest->GetDSPath().Data());
}
//______________________________________________________________________________
// show infor button actions
void MyGuiActionClass::ShowMaterialInfo(TString matName)
{
    if (nLambda == 0 || lambdaMin >= lambdaMax)
        return;

    gMyMainFrameGui->ClearAllCanvas();
    double ppm = mapImpurities[matName.Data()];

    gMyMainFrameGui->SwitchCanvas(1);
    gMyDatabaseClass->GetMatAbsGraph(matName, nLambda, lambdaMin, lambdaMax, ppm)->Draw();
    gMyMainFrameGui->UpdateCanvas(1);
    gMyMainFrameGui->SwitchCanvas(2);
    gMyDatabaseClass->GetMatTrsGraph(matName, nLambda, lambdaMin, lambdaMax, ppm)->Draw();
    gMyMainFrameGui->UpdateCanvas(2);
    gMyMainFrameGui->SwitchCanvas(3);
    gMyDatabaseClass->GetMatRefGraph(matName, nLambda, lambdaMin, lambdaMax)->Draw();
    gMyMainFrameGui->UpdateCanvas(3);
}

void MyGuiActionClass::ShowDetectorInfo(TString detName)
{
    gMyMainFrameGui->ClearAllCanvas();
    gMyMainFrameGui->SwitchCanvas(1);
    gMyDatabaseClass->GetDetQEGraph(detName, nLambda, lambdaMin, lambdaMax)->Draw();
    gMyMainFrameGui->UpdateCanvas(1);
}

void MyGuiActionClass::DoDrawSelectedMat()
{
    int gid = 0;
    gMyStyle->ClearPresetGraphs();
    for (int i = 0; i < nSelectedMat; i++)
    {
        TString matName = selectedMat[i];
        double ppm = mapImpurities[matName.Data()];
        TGraph *g1 = gMyDatabaseClass->GetMatTrsGraph(matName, nLambda, lambdaMin, lambdaMax, ppm);
        if (g1 != NULL)
        {
            gMyStyle->SetGraph(g1);
            gMyStyle->SetLegends(gid++, matName);
        }
    }
    gMyStyle->SetTitle("Transmission of 10mm material");
    gMyStyle->SetXLabel("Wavelength [nm]");
    gMyStyle->SetYLabel("Transmission [%]");

    gMyMainFrameGui->ClearAllCanvas();
    gMyMainFrameGui->SwitchCanvas(1);
    gMyStyle->DrawPresetGraph();
    gMyMainFrameGui->UpdateCanvas(1);

    gid = 0;
    gMyStyle->ClearPresetGraphs();
    for (int i = 0; i < nSelectedMat; i++)
    {
        TString matName = selectedMat[i];
        TGraph *g2 = gMyDatabaseClass->GetMatRefGraph(matName, nLambda, lambdaMin, lambdaMax);
        if (g2 != NULL)
        {
            gMyStyle->SetGraph(g2);
            gMyStyle->SetLegends(gid++, matName);
        }
    }
    gMyStyle->SetTitle("Refractive index");
    gMyStyle->SetXLabel("Wavelength [nm]");
    gMyStyle->SetYLabel("Refractive index");

    gMyMainFrameGui->SwitchCanvas(2);
    gMyStyle->DrawPresetGraph();
    gMyMainFrameGui->UpdateCanvas(2);

    gMyMainFrameGui->SwitchCanvas(1);
}

void MyGuiActionClass::DoDrawSelectedDet()
{
    int gid = 0;
    gMyStyle->ClearPresetGraphs();
    for (int i = 0; i < nSelectedDet; i++)
    {
        TString detName = selectedDet[i];
        TGraph *g1 = gMyDatabaseClass->GetDetQEGraph(detName, nLambda, lambdaMin, lambdaMax);
        if (g1 != NULL)
        {
            gMyStyle->SetGraph(g1);
            gMyStyle->SetLegends(gid++, detName);
        }
    }

    gMyMainFrameGui->ClearAllCanvas();
    gMyMainFrameGui->SwitchCanvas(1);
    gMyStyle->DrawPresetGraph();
    gMyMainFrameGui->UpdateCanvas(1);
}

void MyGuiActionClass::DoShowFCN()
{
    SetDetectorParameters();
    gMyMainFrameGui->SwitchCanvas(1);
    gMyCommonRICH->DrawFCN(0);
    gMyMainFrameGui->UpdateCanvas(1);
}

//______________________________________________________________________________
// private func
void MyGuiActionClass::SetDetectorParameters()
{
    gMyCommonRICH->SetNEvent(nEvent);
    gMyCommonRICH->SetPrecision(epsilon);
    gMyCommonRICH->SetDatabase(gMyDatabaseClass);

    MyRICHDetector *gDet = gMyCommonRICH->GetDetector();

    gDet->SetRadiator(nRadLayer, tRadLayer, sRadLayer);
    gDet->SetTransLayer(tTransLayer, nTransLayer, pTransLayer, sTransLayer);
    gDet->SetImpurities(nImpurities, pImpurities, sImpurities);
    gDet->SetDetector(PCDetector, pixel);

    gDet->SetParticleGun(particle, mass);
    gDet->SetParticleGun(momentum, Theta0);
    gDet->SetLambdaRange(nLambda, lambdaMin, lambdaMax);
    gDet->SetDetectorViewSize(XBinMin, XBinMax, YBinMin, YBinMax);

    gDet->SetMomentumScanRange(np, pMin, pMax);
    gDet->SetTheta0ScanRange(nthe0, The0Min, The0Max);
    gDet->SetPrecision(epsilon, trkStep, nphi);
}

void MyGuiActionClass::GetDetectorParameters()
{
    MyRICHDetector *gDet = gMyCommonRICH->GetDetector();
    nRadLayer = gDet->nRadLayer;
    tRadLayer = gDet->tRadLayer;
    sRadLayer = gDet->sRadLayer;

    tTransLayer = gDet->tTransLayer;
    nTransLayer = gDet->nTransLayer;
    pTransLayer = gDet->pTransLayer;
    sTransLayer = gDet->sTransLayer;

    nImpurities = gDet->nImpurities;
    pImpurities = gDet->pImpurities;
    sImpurities = gDet->sImpurities;

    PCDetector = gDet->PCDetector;
    pixel = gDet->pixel;

    particle = gDet->particle;
    momentum = gDet->momentum;
    Theta0 = gDet->Theta0;
    mass = gDet->mass;

    nLambda = gDet->nLambda;
    lambdaMin = gDet->lambdaMin;
    lambdaMax = gDet->lambdaMax;

    XBinMin = gDet->XBinMin;
    XBinMax = gDet->XBinMax;
    YBinMin = gDet->YBinMin;
    YBinMax = gDet->YBinMax;

    np = gDet->np;
    pMin = gDet->pMin;
    pMax = gDet->pMax;
    nthe0 = gDet->nthe0;
    The0Min = gDet->The0Min;
    The0Max = gDet->The0Max;

    epsilon = gDet->epsilon;
    trkStep = gDet->trkStep;
    nphi = gDet->nphi;
}