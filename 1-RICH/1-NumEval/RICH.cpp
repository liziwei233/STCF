#include "MyGuiActionClass.h"
#include "MyMainFrameGui.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXBUFSIZE 1024

int main(int argc, char **argv)
{
    char resolved_path[100];
    realpath(argv[0],resolved_path);

    if (argc == 1) //默认采用gui模式
    {
        TApplication *theApp;
        theApp = new TApplication("App", &argc, argv);

        gStyle->SetOptStat(0);
        gStyle->SetOptFit(1);
        gStyle->SetStatFont(42);

        gMyGuiActionClass = new MyGuiActionClass(gSystem->DirName(resolved_path));
        gMyGuiActionClass->SetNThread(4);
        gMyMainFrameGui = new MyMainFrameGui(gClient->GetRoot(), 1250, 600);

        // run ROOT application
        theApp->Run();

        delete theApp;

        return 0;
    }

    //采用batch模式
    //输入参数： 保存的文件路径及文件名
    gMyGuiActionClass = new MyGuiActionClass(gSystem->DirName(resolved_path), BATCH);
    gMyGuiActionClass->SetNThread(1);
    gMyGuiActionClass->DoReadBatchFile(argv[1], argv[2]); //batch file

    return 0;
}
