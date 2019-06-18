#include "MyMainFrameGui.h"

MyMainFrameGui *gMyMainFrameGui = (MyMainFrameGui *)0;

using namespace std;

const char *filetypes[] = {"ROOT files", "*.root",
                           "All files", "*",
                           0, 0};

//______________________________________________________________________________
//
MyMainFrameGui::MyMainFrameGui(const TGWindow *p, int w, int h) : TGMainFrame(p, w, h)
{
    TGGC myGC = *gClient->GetResourcePool()->GetFrameGC();
    TGFont *myfont = gClient->GetFont("-adobe-helveticaa-n-r-*-*-16-*-*-*-*-*-iso8859-1");
    if (myfont)
        myGC.SetFont(myfont->GetFontHandle());

    for (int i = 0; i < 100; i++)
        fCA[i] = 0;

    TGLayoutHints *LayoutX = new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX, 2, 2, 2, 2);
    TGLayoutHints *LayoutY = new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandY, 2, 2, 2, 2);
    TGLayoutHints *LayoutXY = new TGLayoutHints(kLHintsLeft | kLHintsTop | kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2);

    TGHorizontalFrame *fHFrame0 = new TGHorizontalFrame(this, 10, 10, kHorizontalFrame);
    AddFrame(fHFrame0, LayoutXY);

    TGVerticalFrame *fVFrame1 = new TGVerticalFrame(fHFrame0, 10, 10, kVerticalFrame);
    TGVerticalFrame *fVFrame2 = new TGVerticalFrame(fHFrame0, 10, 10, kVerticalFrame);
    TGVerticalFrame *fVFrame3 = new TGVerticalFrame(fHFrame0, 10, 10, kVerticalFrame);
    fHFrame0->AddFrame(fVFrame1, LayoutY);
    fHFrame0->AddFrame(fVFrame2, LayoutY);
    fHFrame0->AddFrame(fVFrame3, LayoutXY);

    //1. Setting Text
    {
        fSettingText = new TGTextEdit(fVFrame1, 400, 580);
        fSettingText->LoadBuffer(gMyNumEvalClass->GenerateSettingsText());
        fSettingText->SetFont(myfont->GetFontStruct());
        fVFrame1->AddFrame(fSettingText, LayoutXY);

        TGHorizontalFrame *fHFrame10 = new TGHorizontalFrame(fVFrame1, 10, 10, kHorizontalFrame);
        fVFrame1->AddFrame(fHFrame10, LayoutX);

        TGTextButton *fButtonTmp1 = new TGTextButton(fHFrame10, "Load Set-text", LoadTextBuf);
        fButtonTmp1->Associate(this);
        fHFrame10->AddFrame(fButtonTmp1, LayoutX);

        TGTextButton *fButtonTmp2 = new TGTextButton(fHFrame10, "Draw Configure", DrawConfig);
        fButtonTmp2->Associate(this);
        fHFrame10->AddFrame(fButtonTmp2, LayoutX);
    }

    //2. Buttons Tab
    {
        TGTab *fTab0 = new TGTab(fVFrame2, 400, 580);
        fTab0->SetTab(0);
        fTab0->Resize(fTab0->GetDefaultSize());
        fVFrame2->AddFrame(fTab0, LayoutXY);

        // 2.1 page0
        for (int i = 0; i < gMyNumEvalClass->GetNTabPage(); i++)
        {
            TGCompositeFrame *fTabPage = fTab0->AddTab(gMyNumEvalClass->GetTabPageName(i));
            fTabPage->SetLayoutManager(new TGVerticalLayout(fTabPage));

            for (int j = 0; j < gMyNumEvalClass->GetNPageButton(i); j++)
            {
                TGTextButton *fButtonTmp = new TGTextButton(fTabPage, gMyNumEvalClass->GetPageButtonName(i, j), gMyNumEvalClass->GetIndexButton(i) + j);
                fButtonTmp->Associate(this);
                fTabPage->AddFrame(fButtonTmp, LayoutX);
            }
        }
    }

    //3.
    {
        fCTab = new TGTab(fVFrame3, 580, 580);
        const int NPage1 = 6;

        for (int i = 0; i < NPage1; i++)
        {
            TGCompositeFrame *fTabPage = fCTab->AddTab((i==0)?"Config":Form("Res%d", i));
            TRootEmbeddedCanvas *fEmbeddedCanvas = new TRootEmbeddedCanvas(0, fTabPage, 580, 360);
            fTabPage->AddFrame(fEmbeddedCanvas, LayoutXY);
            fCA[i] = fEmbeddedCanvas->GetCanvas();
            fCA[i]->SetFillColor(0);
            fCA[i]->SetBorderMode(0);
        }
        fCTab->SetTab(0);
        fCTab->Resize(fCTab->GetDefaultSize());
        fVFrame3->AddFrame(fCTab, LayoutXY);
    }

    SetWindowName("STCF-RICH");
    SetIconName("STCF-RICH");
    MapSubwindows();
    Resize(GetDefaultSize()); // this is used here to init layout algoritme
    MapWindow();
}

MyMainFrameGui::~MyMainFrameGui()
{
    // Destructor.
    CloseWindow();
}

void MyMainFrameGui::CloseWindow()
{
    // Destructor.
    DeleteWindow();
    gApplication->Terminate(0);
}

//______________________________________________________________________________
//

Bool_t MyMainFrameGui::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
    switch (GET_MSG(msg))
    {
    case kC_COMMAND:
        switch (GET_SUBMSG(msg))
        {

        case kCM_BUTTON:
        case kCM_MENU:
            static TString dir(".");
            TGFileInfo fi;
            fi.fFilename = NULL;
            fi.fFileTypes = filetypes;
            fi.fIniDir = StrDup(dir);
            TString fname;

            if (parm1 == Save3Rings || parm1 == Load3Rings)
            {
                new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fi);
                if (fi.fFilename == NULL)
                    return kTRUE;
                fname = fi.fFilename;
                if (!fname.EndsWith("root"))
                    fname += ".root";
            }

            gMyNumEvalClass->ExecButtonClick(parm1, fname.Data());
            break;
        }
        break;
    }
    parm2 = 0.;
    return kTRUE;
}