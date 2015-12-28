//#pragma once
//
//#include "cv.h"
//#include "highgui.h"
//#include <stdio.h>
//#include <ctype.h>
//#include <vector>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
////..
////opengl related includes
//#include <glut.h>    // Header File For The GLUT Library
////#include <GL/gl.h>	// Header File For The OpenGL32 Library
////#include <GL/glu.h>	// Header File For The GLu32 Library
////#include <GL/freeglut.h>
//
//
//const int USE_RANSAC=0;
//const int RANSAC_SAMPLES=10;
//const double RANSAC_DISTANCE_THRESHOLD = 10.0;
//const int RANSAC_ITERATIONS = 20;
//const float EHCIMODELSCALE = 1.0;//50;
//const int EHCIFOCUS  = 1000;
//
//const int MODELSCALE  = 1000; // to do musi zalezec od rozdzielczosci frame
//
//
//void checkTransparency();
//
////=======================================================================================================
// void getReferenceCoordinate(float* x, float* y, float* z);
////=======================================================================================================
//
//void drawCentralAxis();
////=======================================================================================================
//void drawReferenceAxis();
////=======================================================================================================
//void loadRaw(char* file);
////=======================================================================================================
//void drawSinusoidalHead(float scale, int headWidth, int headHeight, int myRefX,int myRefY);
////=======================================================================================================
//void drawHeadModel(float scale,int headWidth,int headHeight, int myRefX,int myRefY);
////=======================================================================================================
//void checkTransparency();
////===========================================================================================================
//void setGLProjectionMatrix(double projectionMatrix[16]);
////=================================================================================
///* The main drawing function. */
//void DrawGLScene(void);
///* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
//void ReSizeGLScene(GLsizei Width, GLsizei Height);
///* The function called whenever a normal key is pressed. */
//void keyPressed(unsigned char key, int x, int y);
///* A general OpenGL initialization function.  Sets all of the initial parameters. */
//void InitGL(GLsizei Width, GLsizei Height);
//void openGLCustomInit( );
//////////////////////////////////////////////////////////////////
//class CHeadPositionDebug
//{
//public:
//	CHeadPositionDebug(void);
//public:
//	~CHeadPositionDebug(void);
//
//
//	int go()
//{
//		//return 0;
//	//	int argc;
//	//	char* argv = new char[10];
//	//argv[0] = 0;
//		char rawFile[]="head.raw";
//	loadRaw(rawFile);
////	boost::thread thr1( boost::bind( &X::run, &x ) );
//	boost::thread thr1( openGLCustomInit);
//	//openGLCustomInit(argc,argv);
//	return 0;
//}
//
//};
//
//
//
