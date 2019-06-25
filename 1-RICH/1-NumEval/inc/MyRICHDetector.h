#ifndef _MyRICHDetector_h_
#define _MyRICHDetector_h_

#include <map>
#include <string>
#include "TF1.h"
#include "TH2.h"
#include "TRandom.h"
#include "TObject.h"

using namespace std;

class MyRICHDetector : public TObject
{
public:
    MyRICHDetector(int tid = 0)
    {
        Init(tid);
    }

    MyRICHDetector(const MyRICHDetector &det, int tid = -9999)
    {
        ((MyRICHDetector &)det).MyCopy(*this, tid);
    }

    ~MyRICHDetector()
    {
        if (fWaveLengthHist != 0)
            delete fWaveLengthHist;
        if (fHitMap != 0)
            delete fHitMap;
        for (int i = 0; i < (int)fHitMapEachRad.size(); i++)
            if (fHitMapEachRad[i] != 0)
                delete fHitMapEachRad[i];
    };

    //---------------
    //1. 定义探测器结构
    int nRadLayer;            //可以有多层辐射体层, 最上层为起始层0
    vector<double> tRadLayer; //辐射体层厚度，thickness
    vector<string> sRadLayer;
    void SetRadiator(int n, vector<double> t, vector<string> s)
    {
        nRadLayer = n;
        tRadLayer = t;
        sRadLayer = s;
    }
    double GetRadTotLeng()
    {
        double len = 0;
        for (int i = 0; i < nRadLayer; i++)
            len += tRadLayer[i];
        return len;
    }
    string GetLayerName(double l)
    {
        for (int i = 0; i < nRadLayer; i++)
        {
            l -= tRadLayer[i];
            if (l <= 0)
                return sRadLayer[i];
        }
        return "";
    }

    int nTransLayer; // 只有一层传输层，但是这层里可以是多个气体的混合
    double tTransLayer;
    vector<double> pTransLayer; //气体混合百分比，percentage
    vector<string> sTransLayer;
    void SetTransLayer(double t, int n, vector<double> p, vector<string> s)
    {
        tTransLayer = t;
        nTransLayer = n;
        pTransLayer = p;
        sTransLayer = s;
    }

    int nImpurities;            // 传输层里的杂质成分
    vector<double> pImpurities; //ppm
    vector<string> sImpurities;
    void SetImpurities(int n, vector<double> p, vector<string> s)
    {
        nImpurities = n;
        pImpurities = p;
        sImpurities = s;
    }

    string PCDetector; // 读出的光电器件类型
    double pixel;
    void SetDetector(string det, double pix)
    {
        PCDetector = det;
        pixel = pix;
        NXBin = (XBinMax - XBinMin) / pixel;
        NYBin = (YBinMax - YBinMin) / pixel;
    }

    double ztot;
    double CalZtot()
    {
        ztot = tTransLayer;
        for (int i = 0; i < nRadLayer; i++)
            ztot += tRadLayer[i];
        return ztot;
    }
    double CalZ0(int id)
    {
        double z0 = 0;
        for (int i = 0; i < id; i++)
            z0 += tRadLayer[i];
        return z0;
    }

    //---------------
    //2. 计算分析所需要用的参数
    int nLambda;
    double lambdaMin, lambdaMax, lambdaStep;
    void SetLambdaRange(int n, double min, double max)
    {
        nLambda = n;
        lambdaMin = min;
        lambdaMax = max;
        lambdaStep = (max - min) / n;
    }

    TString particle;
    double mass, momentum, Theta0, theta0;
    void SetParticleGun(TString par, double m)
    {
        mass = m;
        particle = par;
    }
    void SetParticleGun(double mom, double t0)
    {
        momentum = mom;
        Theta0 = t0;                    // in degree
        theta0 = t0 * 3.1415927 / 180.; // in rad
    }

    int NXBin, NYBin;
    int XBinMin, XBinMax, YBinMin, YBinMax;
    void SetDetectorViewSize(double xmin, double xmax, double ymin, double ymax)
    {
        XBinMin = xmin;
        XBinMax = xmax;
        YBinMin = ymin;
        YBinMax = ymax;
        NXBin = (XBinMax - XBinMin) / pixel;
        NYBin = (YBinMax - YBinMin) / pixel;
    }

    int nphi;
    double epsilon, trkStep, phiStep;
    void SetPrecision(double ep, double tp, int np)
    {
        epsilon = ep;
        trkStep = tp;
        nphi = np;
        phiStep = 2 * TMath::Pi() / nphi;
    }

    //---------------
    //2.1 扫描用的参数
    int np, nthe0;
    double pMin, pMax, pStep;
    double The0Min, The0Max, The0Step; // in degree
    double the0Min, the0Max, the0Step; // in rad

    void SetMomentumScanRange(int n, double min, double max)
    {
        np = n;
        pMin = min;
        pMax = max;
        pStep = (n > 0 && max != min) ? (max - min) / n : 0;
    }
    void SetTheta0ScanRange(int n, double min, double max)
    {
        nthe0 = n;
        The0Min = min;
        The0Max = max;
        The0Step = (n > 0 && The0Max != The0Min) ? (The0Max - The0Min) / nthe0 : 0;

        the0Min = min * 3.1415927 / 180.;
        the0Max = max * 3.1415927 / 180.;
        the0Step = (n > 0 && the0Max != the0Min) ? (the0Max - the0Min) / nthe0 : 0;
    }

    int nhypo;
    void SetNHypothesis(int nh) { nhypo = nh; }

    //---------------
    //3. 计算各种分布图
    //3.1-- 阳极上接收到的光子数随波长的分布
    TH1F *fWaveLengthHist;
    TH1F *GetWaveLengthHist(int flag = 0)
    {
        if (flag == 0 && fWaveLengthHist != 0)
            return fWaveLengthHist;

        if (fWaveLengthHist != 0)
            delete fWaveLengthHist;
        fWaveLengthHist = new TH1F(Form("fwavelength%d", id), Form("Cherenkov wavelength for %s @ %.1f GeV", particle.Data(), momentum), nLambda, lambdaMin, lambdaMax);
        fWaveLengthHist->GetXaxis()->SetTitle("X[mm]");
        fWaveLengthHist->GetYaxis()->SetTitle("Y[mm]");
        return fWaveLengthHist;
    }

    //3.2-- 在阳极XY平面上的RICH环总分布图
    TH2F *fHitMap;
    TH2F *GetHitMap(int flag = 0)
    {
        if (flag == 0 && fHitMap != 0)
            return fHitMap;

        if (fHitMap != 0)
            delete fHitMap;
        fHitMap = new TH2F(Form("fHitMap%d", id), Form("Cherenkov Hitmap for %s @ %.1f GeV", particle.Data(), momentum), NXBin, XBinMin, XBinMax, NYBin, YBinMin, YBinMax);
        fHitMap->GetXaxis()->SetTitle("X[mm]");
        fHitMap->GetYaxis()->SetTitle("Y[mm]");
        return fHitMap;
    }

    //-- 从每个辐射体出射的光子数在阳极上的分布，[nRad]
    vector<TH2F *> fHitMapEachRad;
    TH2F *GetDetHitMap(int id) { return fHitMapEachRad[id]; }
    void Gen2DRingListForEachRad()
    {
        for (int i = 0; i < (int)fHitMapEachRad.size(); i++)
            if (fHitMapEachRad[i] != 0)
                delete fHitMapEachRad[i];

        fHitMapEachRad.clear();
        for (int i = 0; i < nRadLayer; i++)
        {
            fHitMapEachRad.push_back(new TH2F(Form("fHitMapEachRad%d_%d", id, i), Form("Cherenkov Ring for %s @ %.1f GeV from %s", particle.Data(), momentum, sRadLayer[i].c_str()), NXBin, XBinMin, XBinMax, NYBin, YBinMin, YBinMax));
            fHitMapEachRad[i]->GetXaxis()->SetTitle("X[mm]");
            fHitMapEachRad[i]->GetYaxis()->SetTitle("Y[mm]");
        }
    }

    //-- 重建后的结果，X轴：光子数，Y轴：事例中的光子平均后的切伦科夫角, [nRad]
    /* 
    int NPhoton = 50;
    vector<vector<TH2F *>> fRecRingEachRad;
    void Gen2DRecRingListForEachRad()
    {
        for (int i = 0; i < (int)fRecRingEachRad.size(); i++)
            for (int j = 0; j < (int)fRecRingEachRad[i].size(); j++)
                delete fRecRingEachRad[i][j];
        
        fRecRingEachRad.clear();
        fRecRingEachRad.resize(nRadLayer);
        for(int i=0; i<nRadLayer; i++)
        {
            fRecRingEachRad[i].resize(NHYPO);
            for(int j=0; j<NHYPO; j++)
            {
                fRecRingEachRad[i].push_back(new TH2F(Form("fRecRingRad%d_%d_%d", id, i, j), Form("Reconstructed Cherenkov Ring for %s @ %.1f GeV", hypo[i], momentum), NPhoton, 0, NPhoton, 360, 0, TMath::Pi() / 2));
                fRecRingEachRad[i][j]->GetXaxis()->SetTitle("Number of photon");
                fRecRingEachRad[i][j]->GetYaxis()->SetTitle("Reconstructed Theta_c[rad]");
            }
        }
    }


    vector<vector<vector<TH1F *>>> fTC; //[nRad][nHypo][nPhoton]
    void Gen1DRecRingListForEachRad()
    {
        for (int i = 0; i < (int)fRecRingEachRad.size(); i++)
            for (int j = 0; j < (int)fRecRingEachRad[i].size(); j++)
            {
                TH2F *ftmp = fRecRingEachRad[i][j];

                for (int k=0; k<NPhoton; k++)
                {
                    int ph = ftmp->GetXaxis()->GetBinLowEdge();
                    TH1F *fproj = ftmp->ProjectionX(Form("fTC%d_%d_%d", id, ihypo, i), i, i);
                }
            }


        for (int i = 0; i < NPhoton; i++)
        {
            if (fTC[ihypo][i] != 0)
                delete fTC[ihypo][i];

            fTC[ihypo][i] = fRecRing[ihypo]->ProjectionX(Form("fTC%d_%d_%d", id, ihypo, i), i, i);

            if (fTC[ihypo][i]->Integral() == 0)
                continue;

            Double32_t bmin = -1, bmax = -1;
            for (int ibin = 0; ibin < fTC[ihypo][i]->GetXaxis()->GetNbins(); ibin++)
            {
                if (fTC[ihypo][i]->GetBinContent(ibin) != 0 && bmin == -1)
                    bmin = fTC[ihypo][i]->GetBinCenter(ibin);
                if (fTC[ihypo][i]->GetBinContent(ibin) != 0)
                    bmax = fTC[ihypo][i]->GetBinCenter(ibin);
            }
            fTC[ihypo][i]->GetXaxis()->SetRangeUser(bmin, bmax);
            fTC[ihypo][i]->Fit("gaus", "Q");
        }

        return fTC[ihypo];
    }
    */

    //---------------
    //0. 基本信息
    int id;
    void SetID(int tid) { id = tid; }

    //1. 基本函数
    void Init(int tid)
    {
        id = tid;
        fHitMap = 0;
        fWaveLengthHist = 0;
    };

    void BuildFrom(MyRICHDetector &det)
    {
        ((MyRICHDetector &)det).MyCopy(*this);
    };

private:
    void MyCopy(MyRICHDetector &det, int tid = -9999)
    {
        det.SetRadiator(nRadLayer, tRadLayer, sRadLayer);
        det.SetTransLayer(tTransLayer, nTransLayer, pTransLayer, sTransLayer);

        det.SetImpurities(nImpurities, pImpurities, sImpurities);
        det.SetDetector(PCDetector, pixel);
        det.SetParticleGun(particle, mass);
        det.SetParticleGun(momentum, Theta0);
        det.SetLambdaRange(nLambda, lambdaMin, lambdaMax);
        det.SetDetectorViewSize(XBinMin, XBinMax, YBinMin, YBinMax);
        det.SetMomentumScanRange(np, pMin, pMax);
        det.SetTheta0ScanRange(nthe0, The0Min, The0Max);
        det.SetPrecision(epsilon, trkStep, nphi);
        det.Init((tid == -9999) ? id : tid);
        det.Gen2DRingListForEachRad();
        if (fHitMap != 0)
            det.fHitMap = (TH2F *)fHitMap->Clone();
        if (fWaveLengthHist != 0)
            det.fWaveLengthHist = (TH1F *)fWaveLengthHist->Clone();
        //det.fHitMap->SetDirectory(0);
        //det.fWaveLengthHist->SetDirectory(0);
    }

    ClassDef(MyRICHDetector, 1) //Jet class
};

#endif