//#include "HeadPositionDebug.h"
//
//CHeadPositionDebug::CHeadPositionDebug(void)
//{
//}
//
//CHeadPositionDebug::~CHeadPositionDebug(void)
//{
//}
//
//
//
//
//
///////////////////////////////////////////////////////////////
//float GlobalPositMatrix[16];
//
//boost::mutex MatrixMutex;
//
//
//GLuint	keyframeTexture;
//
///* white ambient light at half intensity (rgba) */
//GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
//
///* super bright, full intensity diffuse light. */
//GLfloat LightDiffuse[] = { 0.8f, 0.8f, 0.8, 1.0f };
//
//GLfloat LightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//
///* position of light (x, y, z, (position of light)) */
//GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//
////openGl texture to store grabbed frame
//GLuint	frameTexture;
//
//
///* The number of our GLUT window */
//int window;
//
//double projectionMatrix[16];
//
//
//int initialGuess=1;
//int drawSine=0,drawHead=1,drawAxis=1,drawNormals=0,blending=1,drawLight=1,drawBackground=1;
//int aLastHeadW, aLastHeadH;
//int headRefX,headRefY;
//
//struct triangle{
//	float vert[3][3];
//};
//
//triangle triangles[3036];
////////////////////////////////////////////////////////////////////
//
////=======================================================================================================
//
//
// void getReferenceCoordinate(float* x, float* y, float* z)
//{
//*x = 0;
//*y = 0;
//*z = 0;
//}
//
////=======================================================================================================
//
//void drawCentralAxis(){
//
//	float deltaX,deltaY,deltaZ;
//	getReferenceCoordinate(&deltaX,&deltaY,&deltaZ);
//
//	float cx,cy,cz;
//	cx = .5 - deltaX;
//	cy = -.25 - deltaY;
//	cz = 1.0 - deltaZ;
//
//	glLineWidth(4.0);
//	glBegin(GL_LINES);
//	glColor3f(1.0f,0.0f,0.0);
//	glVertex3f( cx,cy,cz);
//	glVertex3f( cx+1000,cy,cz);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.9f,0.3f,0.0);
//	glVertex3f( cx,cy,cz);
//	glVertex3f( cx -1000,cy,cz);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f,1.0f,0.0);
//	glVertex3f( cx,cy,cz);
//	glVertex3f( cx,cy+1000,cz);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.3f,0.8f,0.0);
//	glVertex3f( cx,cy,cz);
//	glVertex3f( cx,cy-1000,cz);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f,0.0f,1.0);
//	glVertex3f( cx,cy,cz);
//	glVertex3f( cx,cy,cz+5.0f);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f,0.0f,0.5f);
//	glVertex3f( cx,cy,cz);
//	glVertex3f( cx,cy,cz-5.0f);
//	glEnd();
//	glLineWidth(1.0);
//
//}
////=======================================================================================================
//void drawReferenceAxis(){
//
//	glBegin(GL_LINES);
//	glColor3f(1.0f,0.0f,0.0);
//	glVertex3f( 0.0f,0.0f,0.0f);
//	glVertex3f( 1000.0f,0.0f,0.0f);
//	glEnd();
//	glBegin(GL_LINES);
//	glColor3f(0.9f,0.3f,0.0);
//	glVertex3f( 0.0f,0.0f,0.0f);
//	glVertex3f( -1000.0f,0.0f,0.0f);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f,1.0f,0.0);
//	glVertex3f(0.0f, 0.0f,0.0f);
//	glVertex3f(0.0f, 1000.0f,0.0f);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.3f,0.8f,0.0);
//	glVertex3f(0.0f, 0.0f,0.0f);
//	glVertex3f(0.0f, -1000.0f,0.0f);
//	glEnd();
//
///*	glBegin(GL_LINES);
//	glColor3f(0.0f,1.0f,0.0);
//	glVertex3f(100.0f, 0.0f,0.0f);
//	glVertex3f(100.0f, 1000.0f,0.0f);
//	glEnd();*/
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f,0.0f,1.0);
//	glVertex3f( 0.0f,0.0f,0.0f);
//	glVertex3f( 0.0f,0.0f,1000.0f);
//	glEnd();
//
//	glBegin(GL_LINES);
//	glColor3f(0.0f,0.0f,0.5f);
//	glVertex3f( 0.0f,0.0f,0.0f);
//	glVertex3f( 0.0f,0.0f,-1000.0f);
//	glEnd();
//
//}
////=======================================================================================================
//void loadRaw(char* file){
//
//	float p1[3],p2[3],p3[3];
//	FILE* in = fopen("data/head.raw","r");
//	if(!in){
//		printf("Cannot find head.raw file with head vertices. This file should be in a subdirectory data in the directory from which the application is being run.\n");
//		exit(EXIT_FAILURE);
//	}
//	char temp[200];
//	//fscanf(in,"%s",&temp);
//	//fscanf(in,"%f",&p1[0]);
//	//	printf("Temp %s %f\n",temp,p1[0]);
//	//	return;
//	int tcount=0;
//	float mScale= 1.0;
//	//float deltaX=1.874,deltaY=-1.999,deltaZ=-2.643;
//	float deltaX=0.0,deltaY=0.0,deltaZ=0.0;
//
//	while( fscanf(in,"%f%f%f%f%f%f%f%f%f",&p1[0],&p1[1],&p1[2],&p2[0],&p2[1],&p2[2],&p3[0],&p3[1],&p3[2])!=EOF){
//		triangles[tcount].vert[0][0]=(p1[0]+deltaX)*mScale;
//		triangles[tcount].vert[0][1]=(p1[1]+deltaY)*mScale;
//		triangles[tcount].vert[0][2]=(p1[2]+deltaZ)*mScale;
//
//		triangles[tcount].vert[1][0]=(p2[0]+deltaX)*mScale;
//		triangles[tcount].vert[1][1]=(p2[1]+deltaY)*mScale;
//		triangles[tcount].vert[1][2]=(p2[2]+deltaZ)*mScale;
//
//		triangles[tcount].vert[2][0]=(p3[0]+deltaX)*mScale;
//		triangles[tcount].vert[2][1]=(p3[1]+deltaY)*mScale;
//		triangles[tcount].vert[2][2]=(p3[2]+deltaZ)*mScale;
//
//		tcount++;
//		//printf("Tcount %d\n",tcount);
//	}
//	fclose(in);
//}
////=======================================================================================================
//void drawSinusoidalHead(float scale, int headWidth, int headHeight, int myRefX,int myRefY){
//	//TODO: FIND OUT WHY Y IS UPSIDE DOWN
//
//	float deltaX,deltaY,deltaZ;
//	getReferenceCoordinate(&deltaX,&deltaY,&deltaZ);
//
//	//this code will map everything from the upper head corner down to the bottom right corner
//	/*glBegin(GL_LINES);
//	glColor3f(0.0f,0.0f,0.5f);
//	glVertex3f(-0.332 - deltaX, +0.332 - deltaY,-0.8663 -deltaZ);
//	glVertex3f(-0.332 - deltaX, -1.332 - deltaY,-0.8663 -deltaZ);
//	glEnd();
//
//	for(double x=0;x<1.6;x+=0.111){
//		glBegin(GL_LINES);
//
//		glVertex3f(-0.332 +x - deltaX, +0.332 - deltaY,sin((-0.332 +x)*3.141593) -deltaZ);
//		glVertex3f(-0.332 +x - deltaX, -1.332 - deltaY,sin((-0.332 +x)*3.141593) -deltaZ);
//		glEnd();
//	}
//
//	glBegin(GL_LINES);
//	glVertex3f( 1.332 - deltaX, +0.332 - deltaY,-0.8663 -deltaZ);
//	glVertex3f( 1.332 - deltaX, -1.332 - deltaY,-0.8663 -deltaZ);
//	glEnd();*/
//
//	glBindTexture(GL_TEXTURE_2D, keyframeTexture);
//
//	//printf("GlRef %f %f %f\n",deltaX,deltaY,deltaZ);
//
//	//this code will map only the texture detail
//
//	double xIncrement = 0.01;
//	for(double x=0;x<= 1.0 - xIncrement; x+=xIncrement){
//		glBegin(GL_QUADS);
//		//glBegin(GL_LINES);
//		glColor3f(1.0f,1.0f,1.0f);
//
//		glTexCoord2f( x, 0);
//		glVertex3f(0 +x - deltaX, +0.0 - deltaY,.5*sin((+x)*3.141593) -deltaZ);//0-deltaZ);//sin((+x)*3.141593) -deltaZ);
//
//		glTexCoord2f( x, 1.0);
//		glVertex3f(0 +x - deltaX, -1.0 - deltaY,.5*sin((+x)*3.141593) -deltaZ);//0-deltaZ);//sin((+x)*3.141593) -deltaZ);
//
//		glTexCoord2f( x+xIncrement, 1.0);
//		glVertex3f(0 +x+xIncrement - deltaX, -1.0 - deltaY,.5*sin((+x+xIncrement)*3.141593) -deltaZ);//0-deltaZ);//sin((+x+xIncrement)*3.141593) -deltaZ);
//
//		glTexCoord2f( x+xIncrement, 0.0);
//		glVertex3f(0 +x+xIncrement - deltaX, 0.0 - deltaY,.5*sin((+x+xIncrement)*3.141593) -deltaZ);//0-deltaZ);//sin((+x+xIncrement)*3.141593) -deltaZ);
//
//		glEnd();
//	}
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//
//}
////=======================================================================================================
//void drawHeadModel(float scale,int headWidth,int headHeight, int myRefX,int myRefY){
//
//	float deltaX = 0.0;//-(myRefX/(float)headWidth*5.0-2.5);
//	float deltaY = 0.0;//myRefY/(float)headHeight*7.5-3.75;
//	float deltaZ = 0.0;//-4.0f * cos(deltaX/2.5*3.141593);
//
//
//	getReferenceCoordinate(&deltaX,&deltaY,&deltaZ);
//	deltaX-=0.5;
//	deltaY+=0.5;
//	deltaZ+=0.5;
//	//printf("Reference %f %f %f (%f)\n",deltaX,deltaY,deltaZ,scale);
//
//	for(int i=0;i<3036;i++){
//		//glColor3f (i/3036.0, 0.0, 0.0);
//
//
//		float v1x = triangles[i].vert[1][0] - triangles[i].vert[0][0];
//		float v1y = triangles[i].vert[1][1] - triangles[i].vert[0][1];
//		float v1z = triangles[i].vert[1][2] - triangles[i].vert[0][2];
//
//		float v2x = triangles[i].vert[2][0] - triangles[i].vert[0][0];
//		float v2y = triangles[i].vert[2][1] - triangles[i].vert[0][1];
//		float v2z = triangles[i].vert[2][2] - triangles[i].vert[0][2];
//
//		float nx = 5*(v1y*v2z-v2y*v1z);
//		float ny = 5*(v1z*v2x-v2z*v1x);
//		float nz = 5*(v1x*v2y-v2x*v1y);
//
//	    // calculate the length of the vector
//	    float len = (float)(sqrt((nx * nx) + (ny * ny) + (nz * nz)));
//
//	    // avoid division by 0
//	    if (len == 0.0f)
//	        len = 1.0f;
//
//	    // reduce to unit size
//	    nx /= len;
//	    ny /= len;
//	    nz /= len;
//
//
//		glNormal3f( nx,ny,nz);
//
//		if(drawNormals){
//			glBegin(GL_LINES);
//
//				glColor3f(1.0f,0.0f,0.0);
//
//				glVertex3f(	scale* (triangles[i].vert[0][0]+deltaX),
//							scale* (triangles[i].vert[0][1]+deltaY),
//							scale* (triangles[i].vert[0][2]+deltaZ));
//
//				glVertex3f(	scale* (triangles[i].vert[0][0]+deltaX+nx),
//							scale* (triangles[i].vert[0][1]+deltaY+ny),
//							scale* (triangles[i].vert[0][2]+deltaZ+nz));
//
//			glEnd();
//		}
//
//
//		glBegin(GL_TRIANGLES);
//		glColor4d (1.0, 1.0, 1.0,0.65);
//
//
//
//		for(int j=0;j<3;j++){
//			glVertex3f(	scale* (triangles[i].vert[j][0])-deltaX,
//						scale* (triangles[i].vert[j][1])-deltaY,
//						scale* (triangles[i].vert[j][2])-deltaZ);
//		}
//		glEnd();
//
//	}
//
//
//}
//
////=======================================================================================================
//void checkTransparency(){
//	//dealing with transparency
//	if(blending){
//		glEnable (GL_BLEND);
//		//glDisable(GL_DEPTH_TEST);
//		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	}
//	else{
//		glDisable (GL_BLEND);
//		glEnable(GL_DEPTH_TEST);
//	}
//}
//
//
////===========================================================================================================
//void setGLProjectionMatrix(double projectionMatrix[16]){
//	double farPlane=10000.0;
//	double nearPlane=1.0;
//	double width = 640;
//	double height = 480;
//	double focalLength = EHCIFOCUS;
//	projectionMatrix[0] = 2*focalLength/width;
//	projectionMatrix[1] = 0.0;
//	projectionMatrix[2] = 0.0;
//	projectionMatrix[3] = 0.0;
//
//	projectionMatrix[4] = 0.0;
//	projectionMatrix[5] = 2*focalLength/ height;
//	projectionMatrix[6] = 0.0;
//	projectionMatrix[7] = 0.0;
//
//	projectionMatrix[8] = 0;
//	projectionMatrix[9] = 0;
//	projectionMatrix[10] = - ( farPlane+nearPlane ) / ( farPlane - nearPlane );;
//	projectionMatrix[11] = -1.0;
//
//	projectionMatrix[12] = 0.0;
//	projectionMatrix[13] = 0.0;
//	projectionMatrix[14] = -2.0 * farPlane * nearPlane / ( farPlane - nearPlane );
//	projectionMatrix[15] = 0.0;
//
//}
//
//	CvMatr32f rotation_matrix = new float[9];
//	CvVect32f translation_vector = new float[3];
//
////=================================================================================
///* The main drawing function. */
//void DrawGLScene(void)
//{
//	static int bootstrap = 0;
//
//	double glPositMatrix[16];
//	//cvWaitKey(0);
//	int detected=0;
//	MatrixMutex.lock();
//
//	glPositMatrix[0] = rotation_matrix[0];
//	glPositMatrix[1] = rotation_matrix[3];
//	glPositMatrix[2] = rotation_matrix[6];
//	glPositMatrix[3] = 0.0;
//
//	glPositMatrix[4] = rotation_matrix[1];
//	glPositMatrix[5] = rotation_matrix[4];
//	glPositMatrix[6] = rotation_matrix[7];
//	glPositMatrix[7] = 0.0;
//
//	glPositMatrix[8] =  rotation_matrix[2];
//	glPositMatrix[9] =  rotation_matrix[5];
//	glPositMatrix[10] = rotation_matrix[8];
//	glPositMatrix[11] = 0.0;
//
//	glPositMatrix[12] =  translation_vector[0];
//	glPositMatrix[13] =  translation_vector[1];
//	glPositMatrix[14] =  translation_vector[2];
//	glPositMatrix[15] = 1.0; //homogeneous
//	MatrixMutex.unlock();
//
//	//getGlPositMatrix(glPositMatrix);
//
//		//getGlPositMatrix(glPositMatrix);
//
//
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
//
//	checkTransparency();
//
//
//	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // set light position.
//
//	if(initialGuess){
//		/*setInitialRTMatrix(rotation_matrix,translation_vector);
//		updateGlPositMatrix(rotation_matrix,translation_vector);*/
//		initialGuess = false;
//	}
//
//
//	if(initialGuess){
//	
//		bootstrap=0;
//	}
//	else{
//
//		//drawGrabbedFrame();
//
//		//loads reference point pose into OpenGL modelview matrix
//		glMatrixMode(GL_PROJECTION);
//		glLoadMatrixd( projectionMatrix );
//			//glLoadIdentity();				// Reset The Projection Matrix
//
//		//look in +z direction, the same as POSIT
//		gluLookAt(0, 0, 0 , 0, 0, +1.0, 0, +1, 0);
//
//
//		glMatrixMode( GL_MODELVIEW );
//		//glLoadIdentity();				// Reset The Projection Matrix
//
//		glLoadMatrixd( glPositMatrix );
//
//
//
//		//if(drawAxis){
//			drawCentralAxis();
//			//drawReferenceAxis();
//		//}
//
//		//if(drawHead)
//			drawHeadModel(1.4f/5.0f,aLastHeadW,aLastHeadH,headRefX,headRefY);
//
//		//glTranslatef(200.0,0,0.0f);
//		//glutWireSphere(80.0, 200, 200);
//	}
//	if(initialGuess){
//		bootstrap = 1;
//	}
//
//
//	glutSwapBuffers();
//	//cvWaitKey();
//
//	//automatic reinitialization
//	//TODO: reactivate automatic reinitialization
//
//
//
//	               // double headX = glPositMatrix[12];
//                //double headY = glPositMatrix[13];
//                //double headZ = glPositMatrix[14];
//                //
//                //printf("Head Coordinates X: %+7.3lf Y: %+7.3lf Z:%+7.3lf /n\n",
//                //                headX,headY,headZ);
//
//}
//
///* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
//void ReSizeGLScene(GLsizei Width, GLsizei Height)
//{
//	if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
//		Height=1;
//
//	glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
//	glMatrixMode(GL_MODELVIEW);
//}
//
///* The function called whenever a normal key is pressed. */
//void keyPressed(unsigned char key, int x, int y)
//{
//
//	switch (key) {
//	case 27: // kill everything.
//		/* shut down our window */
//		glutDestroyWindow(window);
//		/* exit the program...normal termination. */
//		exit(1);
//		break; // redundant.
//
//	case 'a':
//		drawAxis = !drawAxis;
//		break;
//	case 's':
//		drawSine = !drawSine;
//		break;
//	case 'h':
//	case 'H':
//		drawHead = !drawHead;
//		break;
//
//	case 'd':
//	case 'D':
//		drawBackground = !drawBackground;
//		break;
//	case 'b':
//	case 'B':
//		blending = !blending;
//		break;
//	case 'n':
//	case 'N':
//		drawNormals = !drawNormals;
//		break;
//	case 'i':
//	case 'I':
//		initialGuess = !initialGuess;
//		break;
//	case 76:
//	case 108: // switch the lighting
//		drawLight = !drawLight;
//		if (!drawLight) {
//			glDisable(GL_LIGHTING);
//		} else {
//			glEnable(GL_LIGHTING);
//		}
//		break;
//
//
//	default:
//		break;
//	}
//}
//
///* A general OpenGL initialization function.  Sets all of the initial parameters. */
//void InitGL(GLsizei Width, GLsizei Height)	// We call this right after our OpenGL window is created.
//{
//
//	rotation_matrix[0]=1.0; rotation_matrix[1]=  0; rotation_matrix[2]=  0;
//	rotation_matrix[3]=  0; rotation_matrix[4]=1.0; rotation_matrix[5]=  0;
//	rotation_matrix[6]=  0; rotation_matrix[7]=  0; rotation_matrix[8]=1.0;
//
//	//translation_vector[0]=   -90;
//	//translation_vector[1]=    80;
//	//translation_vector[2]=+300.0;
//
//	translation_vector[0]=   0;
//	translation_vector[1]=    0;
//	translation_vector[2]=10;
//
//	setGLProjectionMatrix(projectionMatrix);
//
//
//	glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.
//
//
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// This Will Clear The Background Color To Black
//	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
//	glDepthFunc(GL_LESS);			// The Type Of Depth Test To Do
//	glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
//	glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
//
//	glEnable ( GL_COLOR_MATERIAL ) ;
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();				// Reset The Projection Matrix
//
//	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
//
//	glMatrixMode(GL_MODELVIEW);
//
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightAmbient);
//	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
//
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  // add lighting. (diffuse).
//	glEnable(GL_LIGHT1);                             // turn light 1 on.
//	glEnable(GL_LIGHTING);
//
//	//generate texture
//    glGenTextures(1, &frameTexture);
//
//}
//
//void openGLCustomInit( ){
//
//	//glutInit(&argc, argv);
//  char* a = "dupa";
//  int b = 1;
//	glutInit(&b, &a);
//
//	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
//
//	/* get a 640 x 480 window */
//	glutInitWindowSize(640, 480);
//
//	/* the window starts at the upper left corner of the screen */
//	glutInitWindowPosition(500, 0);
//
//	/* Open a window */
//	window = glutCreateWindow("6DOF Head Tracking OpenGL - EHCI - Daniel Lelis Baggio");
//
//	/* Register the function to do all our OpenGL drawing. */
//	glutDisplayFunc(&DrawGLScene);
//
//	/* Go fullscreen.  This is as soon as possible. */
//	//    glutFullScreen();
//	//   glutReshapeWindow(640,480);
//
//	/* Even if there are no events, redraw our gl scene. */
//	glutIdleFunc(&DrawGLScene);
//
//	/* Register the function called when our window is resized. */
//	glutReshapeFunc(&ReS
//
//	/* Register the function called when the keyboard is pressed. */
//	glutKeyboardFunc(&keyPressed);
//
//	/* Register the function called when special keys (arrows, page down, etc) are pressed. */
//	//    glutSpecialFunc(&specialKeyPressed);
//
//	/* Initialize our window. */
//	InitGL(640, 480);
//
//
//	/* Start Event Processing Engine */
//	glutMainLoop();
//
//}
