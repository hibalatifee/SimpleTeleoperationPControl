//#ifdef __windows__
//#undef __windows__
//#endif

#include<iostream>
#include<conio.h>
#include "Phantom_omni.h" // Phantom headerfile (containing all callbacks and functions)

using namespace std;
#pragma warning(disable:4996)
//-----------------------------------------------------------------------------------------
#include <afxwin.h>      //MFC core and standard components
#include "resource.h"    //main symbols
//-----------------------------------------------------------------------------------------

//#include <zmq.h>
//#include<math.h>
//#include<string>
//#include<sstream>
//#include<ostream>


//Globals
CButton * p_start;
CButton * p_init;
CButton * p_calib;
CButton * p_stop;
CEdit   * p_edit;

CStatic * p_disp;
CStatic * p_mf;
CStatic * p_sv;

//void *context = zmq_ctx_new();
//void *publisher = zmq_socket(context, ZMQ_PUB);
//char publish[50];

class Phantom : public CDialog
{
public:
	Phantom(CWnd* pParent = NULL) : CDialog(Phantom::IDD, pParent)
	{}
	// Dialog Data, name of dialog form
	enum { IDD = IDD_DIALOG1};
protected:
	virtual void DoDataExchange(CDataExchange* pDX) 
	
	{ CDialog::DoDataExchange(pDX); }
	//Called right after constructor. Initialize things here.

	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();
	
		p_start = (CButton *)GetDlgItem(CB_start);
		p_calib = (CButton *)GetDlgItem(CB_calibarate);
		p_init =  (CButton *)GetDlgItem(CB_initiliaze);
		p_stop =(CButton *)GetDlgItem(CB_stop);
		
			return true;
	}
public:


	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedstart();
	afx_msg void OnBnClickedinitiliaze();
	afx_msg void OnBnClickedcalibarate();
	afx_msg void OnBnClickedstop();
	afx_msg void OnPaint();

	afx_msg void OnBnClickedcamera();
};
//---------------------------------------------------------------------
class Phantom_winapp : public CWinApp
{
public:
	Phantom_winapp() {  }
public:
	virtual BOOL InitInstance()
	{
		CWinApp::InitInstance();
		Phantom dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		return FALSE;
	} //close function

};
//-----------------------For Message Handlers, identifiers and macros---------------------------
//Need a Message Map Macro for both CDialog and CWinApp
BEGIN_MESSAGE_MAP(Phantom, CDialog)   // phantom is owner class name, CDialog is a base class name

	ON_BN_CLICKED(CB_start, &Phantom::OnBnClickedstart)
	ON_BN_CLICKED(CB_initiliaze, &Phantom::OnBnClickedinitiliaze)
	ON_BN_CLICKED(CB_calibarate, &Phantom::OnBnClickedcalibarate)
	ON_BN_CLICKED(CB_stop, &Phantom::OnBnClickedstop)


	ON_BN_CLICKED(CB_camera, &Phantom::OnBnClickedcamera)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------------------


Phantom_winapp  theApp;  //Starts the Application

/*

void Phantom::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	char str[50];
	double time = 0;
	int x1 = 40;
	int y1 = 150;

	int x2 = 220;
	int y2 = 150;

	int x3 = 40;
	int y3 = 370;

	int x4 = 220;
	int y4 = 370;

	//if (Phantom_Started)
	{
		//master 1
		dc.TextOut(x1 - 20, y1, "Position [mm]:");
		sprintf(str, "X %3.1f", mst.position[0]);
		dc.TextOut(x1, y1 + 20, str);
		sprintf(str, "Y %3.1f", mst.position[1]);
		dc.TextOut(x1, y1 + 40, str);
		sprintf(str, "Z %3.1f", mst.position[2]);
		dc.TextOut(x1, y1 + 60, str);
	
	}
	CDialog::OnPaint();
}
*/


void Phantom::OnBnClickedstart()
{
	// TODO: Add your control notification handler code here
	//p_disp = (CStatic *)GetDlgItem(CS_disp);
	//p_disp->SetWindowText(L" You pressed Start Button");
	omni_start();
	//sprintf(publish, "%0.5f %0.5f %0.5f", slv.position[0] , slv.position[1] , slv.position[2]);
	//zmq_send(publisher, publish, 50, 0);
}

void Phantom::OnBnClickedinitiliaze()
{
	// TODO: Add your control notification handler code here
	omni_init();
	
	//zmq_bind(publisher, "tcp://*:5556");

	AllocConsole();
	freopen("CONOUT$", "w", stdout);  //reopen stream with different file or mode 
	//freopen("CONIN$", "r", stdout); //reading from console window  
	
	p_init = (CButton *)GetDlgItem(CB_initiliaze);
	p_init->EnableWindow(FALSE);
}


void Phantom::OnBnClickedcalibarate()
{
	// TODO: Add your control notification handler code here
	omni_calibarate();

	p_calib = (CButton *)GetDlgItem(CB_calibarate);
	p_calib->EnableWindow(FALSE);

	
}


void Phantom::OnBnClickedstop()
{

	close();
	FreeConsole();
}




void Phantom::OnBnClickedcamera()
{
	/*
	
	// TODO: Add your control notification handler code here
	cvNamedWindow("Streaming", CV_WINDOW_AUTOSIZE);
	CvCapture* capture=cvCreateCameraCapture(1);
	IplImage* frame;

	while (1)
	{
		frame=cvQueryFrame(capture);
		if(!frame) break;
		cvShowImage("Streaming",frame);
		char c=cvWaitKey(33);
		if(c==27) break;
	}

	cvReleaseCapture(&capture);
	cvDestroyWindow("Streaming");

	
	*/
	
	}

