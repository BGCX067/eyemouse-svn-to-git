#include "GUI.h"

//CGrapicInterface::CGrapicInterface(void)
//{
//}
//
//CGrapicInterface::~CGrapicInterface(void)
//{
//}
//

void on_paramChengeq(int i)
{
	g_GUI.m_bParamChange = true;
	/*if (g_GUI.m_pCallBackOnParam)
		g_GUI.m_pCallBackOnParam();*/
;
}
;

//============================================================================
void CGrapicInterface::ShowImage(IplImage *img, char *  windowsName )
	{

		cvNamedWindow( windowsName, 1 );
		cvShowImage(windowsName, img);
	}
//============================================================================

CGrapicInterface g_GUI;
