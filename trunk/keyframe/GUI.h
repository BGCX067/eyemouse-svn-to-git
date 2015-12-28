#pragma once

#include "cv.h"
#include "highgui.h"
//
//
#include <iostream>
#include <cstring>
#include <string>
using namespace std;

using namespace std;


void on_paramChengeq(int i);
typedef void ParamChengeCallback();

const int max_val = 250;

class CGrapicInterface
{
public:
	bool m_bParamChange;
	//CGrapicInterface(void);
	//~CGrapicInterface(void);


ParamChengeCallback *m_pCallBackOnParam;
	

	void ShowImage(IplImage *img, char *  windowsName = "show");


	 CGrapicInterface()
	{
		m_bParamChange = true;
		m_pCallBackOnParam = NULL;
		 param1 = 1;
	 param2 = 30;
	 param3 = 1;
	
	};

	 bool IsNewParam()
	 {
	 bool result = m_bParamChange;
	 m_bParamChange = false;
	 return result;
	 };

	int param1 ;
	int param2 ;
	int param3 ;
	

void ShowParamWindows()
	{
	
	cvNamedWindow( "Params", 1 );
	cvCreateTrackbar("param1", "Params", &param1, max_val, on_paramChengeq);
	cvCreateTrackbar("param2", "Params", &param2, max_val, on_paramChengeq);
	cvCreateTrackbar("param3", "Params", &param3, max_val, on_paramChengeq);
	
	}




};





class CTrackinkSystem;
extern CTrackinkSystem* g_TrackinkSystem;
extern CGrapicInterface g_GUI;