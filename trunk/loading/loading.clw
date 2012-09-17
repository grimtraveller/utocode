; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLoadingDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "loading.h"

ClassCount=4
Class1=CLoadingApp
Class2=CLoadingDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_LOADING_DIALOG
Class4=CMainDlg
Resource4=IDD_MAIN_DIALOG

[CLS:CLoadingApp]
Type=0
HeaderFile=loading.h
ImplementationFile=loading.cpp
Filter=N

[CLS:CLoadingDlg]
Type=0
HeaderFile=loadingDlg.h
ImplementationFile=loadingDlg.cpp
Filter=D
LastObject=CLoadingDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=loadingDlg.h
ImplementationFile=loadingDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LOADING_DIALOG]
Type=1
Class=CLoadingDlg
ControlCount=2
Control1=IDC_LOADINGBMP,static,1342177294
Control2=IDC_TEXTMSG,static,1342308352

[DLG:IDD_MAIN_DIALOG]
Type=1
Class=CMainDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CMainDlg]
Type=0
HeaderFile=maindlg.h
ImplementationFile=maindlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

