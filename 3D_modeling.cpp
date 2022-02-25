#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

const GLdouble PI = 3.1415926;
double cameraD = 10;
int intWinWidth = 800; //Default window size
int intWinHeight = 600;
float fltAnimationOffset = 0.0; //Animation offsets for various objects
float fltTreeOffset = 0.0;
float fltBuildingOffset = -2500.0;
float fltPoleOffset = 750.0;
float fltAcceleration = 6.0; //Speed of animation
float fltColourVal1 = 0.5; //First colour value
float fltColourVal2 = 0.7; //Second colour value
float fltColourVal3 = 0.4; //Third colour value
float fltFOV = 65; //Field Of View
float fltZoom = 1.05; //Zoom amount
float fltX0 = -150.0; //Camera position
float fltY0 = 400.0;
float fltZ0 = -900.0;
float fltXRef = 145.0; //Look At reference point
float fltYRef = 70.0;
float fltZRef = 70.0;
float fltXUp = 0.0; //Up vector
float fltYUp = 1.0;
float fltZUp = 0.0;
float fltViewingAngle = 0; //Used for rotating camera
GLfloat light_x = 1.0;
GLfloat light_y = 1.0;
GLfloat light_z = 1.0;
int mov[25];
int ballm = 0;
float posx[10];
float r[10];
float g[10];
float b[10];
float carD[10];
float carD2[10];
float sizex[10];
float sizey[10];
float sizez[10];
int cartype[10];
int randm[25][3];
float rate = 1.0;
int statee = 0;
vector<GLubyte*> p;
GLuint texture[12];

struct image {
	GLint imageWidth;
	GLint imageHeight;
	GLint pixelLength;
};
image loadTexture[10];

void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
	GLubyte* pixeldata;
	FILE* pfile;
	fopen_s(&pfile, path, "rb");
	if (pfile == 0) exit(0);
	fseek(pfile, 0x0012, SEEK_SET);
	fread(&imagewidth, sizeof(imagewidth), 1, pfile);
	fread(&imageheight, sizeof(imageheight), 1, pfile);
	pixellength = imagewidth * 3;
	while (pixellength % 4 != 0)
		pixellength++;
	pixellength *= imageheight;
	pixeldata = (GLubyte*)malloc(pixellength);
	if (pixeldata == 0)
		exit(0);
	fseek(pfile, 54, SEEK_SET);
	fread(pixeldata, pixellength, 1, pfile);
	p.push_back(pixeldata);
	fclose(pfile);
}
int time_interval = 16; // declare refresh interval in ms

void when_in_mainloop() { // idle callback function
	glutPostRedisplay(); // force OpenGL to redraw the current window
}
void OnTimer(int value)
{
	for (int i = 0; i < 25; i++) {
		mov[i] += rate;
	}

	if (ballm < 400) {
		ballm += 1;
	}
	else ballm = -20;
	when_in_mainloop();
	glutTimerFunc(time_interval, OnTimer, 1);
}

void myinit() {
	const char* pictures[10] = {  // Define a char pointer for the file name to be used 
		"background.bmp",
		"stoneHouse.bmp",
		"ball.bmp",
		"door.bmp",
		"stoneDoor.bmp",
		"sail.bmp",
		"face1.bmp",
		"face2.bmp",
		"face3.bmp",
		"face4.bmp"
	};
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_FLAT);
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < 10; i++) {
		ReadImage(pictures[i], loadTexture[i].imageWidth, loadTexture[i].imageHeight, loadTexture[i].pixelLength);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texture[i]);     // Creat texture for the number i texture
		glBindTexture(GL_TEXTURE_2D, texture[i]);    // bind the coresponding texture to the unique ID
		glTexImage2D(GL_TEXTURE_2D, 0, 3, loadTexture[i].imageWidth, loadTexture[i].imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[i]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    // Apply GL_MODULATE to combine the texture with material color
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < 10; i++) {
		posx[i] = rand() % 100;
		r[i] = float(rand() % 255) / 255.0;
		g[i] = float(rand() % 255) / 255.0;
		b[i] = float(rand() % 255) / 255.0;
		sizex[i] = float(rand() % 5) / 23.00 + 0.4;
		sizey[i] = float(rand() % 5) / 23.00 + 0.5;
		sizez[i] = float(rand() % 5) / 23.00 + 0.5;
		cartype[i] = rand() % 4;
		mov[i] = 0;
	}

	for (int i = 0; i < 25; i++) {
		randm[i][0] = rand() % 400 - 100;
		randm[i][1] = rand() % 500 - 50;
		randm[i][2] = rand() % 500 + 20;
	}

	GLfloat lmodel_ambient[] = { 0.45, 0.45, 0.45, 0.3 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
}

void drawWindow()
{
	glColor3f(0.4235, 0.647, 0.835);
	glutSolidTorus(0.3, 1.5, 10, 10);
	glColor3f(0.1725, 0.325, 0.5686);
	glutWireTorus(0.3, 1.5, 10, 10);
	glColor3f(0.298, 0.6078, 0.8235);
	glutSolidCylinder(1.2, 0.3, 10, 10);
}



void drawDoorLine(GLdouble r0)
{

	for (GLdouble r = r0; r < r0 + 0.5; r += 0.05)
	{
		if (r < r0 + 0.1 | r > r0 + 0.45) {
			glColor3f(0.1725, 0.325, 0.5686);
		}
		else {
			glColor3f(0.4235, 0.647, 0.835);
		}
		glBegin(GL_LINE_STRIP);
		glVertex3d(r * cos(0), -2.5, -8);
		for (GLdouble i = 0.00; i < PI; i += 0.005)
		{
			glVertex3d(r * cos(i), r * sin(i), -8);
		}
		glVertex3d(r * cos(PI), -2.5, -8);
		glVertex3d(r * cos(0), -2.5, -8);
		glEnd();
	}
}


void ball() {
	glColor4f(135.0 / 255.0, 206.0 / 255.0, 250.0 / 255.0, 0.5);
	glutSolidSphere(20, 36, 36);
}


void drawPinapple()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.808, 0.396, 0.271);
	glPushMatrix();
	glTranslatef(0, 60, 0);
	glScalef(40.0, 75.0, 40.0);
	glutSolidSphere(2, 20, 20);
	glLineWidth(2);
	glColor3f(0.718, 0.310, 0.227);
	glutWireSphere(2, 12, 18);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -53, 0);
	glScalef(0.6, 1.1, 0.8);
	glColor3f(0.565, 0.682, 0.262);
	glPushMatrix();
	glTranslatef(0, 300, 3);
	glScalef(0.85, 4.3, 0.85);
	glutSolidSphere(20, 20, 20);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(38, 300, 3);
	glRotatef(-20, 0, 0, 1);
	glScalef(0.85, 3.5, 0.85);
	glutSolidSphere(20, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-38, 300, 3);
	glRotatef(20, 0, 0, 1);
	glScalef(0.85, 3.5, 0.85);
	glutSolidSphere(20, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(60, 270, 0);
	glRotatef(-63, 0, 0, 1);
	glScalef(0.85, 3.5, 0.85);
	glutSolidSphere(20, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-60, 270, 0);
	glRotatef(63, 0, 0, 1);
	glScalef(0.85, 3.5, 0.85);
	glutSolidSphere(20, 20, 20);
	glPopMatrix();
	glPopMatrix();

	// draw window
	glPushMatrix();
	glTranslatef(30, 117, -75);
	glRotatef(-12, 0, 1, 0);
	glScalef(7.5, 9.5, 9.5);
	drawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-40, 77, -75);
	glRotatef(14, 0, 1, 0);
	glScalef(8.5, 10.5, 10.5);
	drawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 30, -60);
	glScalef(9.0, 18.0, 3.0);
	glLineWidth(4);
	drawDoorLine(2.00);
	glColor3f(0.1725, 0.325, 0.5686);
	glPointSize(4);
	glBegin(GL_POINTS);
	for (GLdouble i = 0.00; i < PI; i += 0.5)
	{
		glVertex3d(2.25 * cos(i), 2.25 * sin(i), -8);
	}
	glEnd();
	drawDoorLine(1.50);
	glColor3f(0.4235, 0.647, 0.835);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(-1.5, -2.5, -8);
	glTexCoord2f(1, 0); glVertex3d(1.5, -2.5, -8);
	glTexCoord2f(1, 1); glVertex3d(1.5, 1.4, -8);
	glTexCoord2f(0, 1); glVertex3d(-1.5, 1.4, -8);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawTrapezoid() {
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(64, 0, 0);
	glVertex3f(64, 0, 127);
	glVertex3f(0, 0, 0);
	glEnd();
}

void drawquad(int y, int x1, int z1, int x2, int z2, int r, int a, int y2, int x3, int z3, int p) {
	GLUquadricObj* objCylinder = gluNewQuadric();
	int y1 = y + y2;
	if (z3 == 4) {
		glColor3ub(247, 186, 11);
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, texture[6 + statee]);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.1, 0.1); glVertex3f(-x1, y2, -z1);
		glTexCoord2f(0.9, 0.1); glVertex3f(x1, y2, -z1);
		glTexCoord2f(0.9, 0.9); glVertex3f(x2 + p, y1, -z2);
		glTexCoord2f(0.1, 0.9); glVertex3f(-x2 + p, y1, -z2);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glColor3ub(247, 186, 11);
	}
	else {
		glBegin(GL_QUADS);
		glVertex3f(-x1, y2, -z1);
		glVertex3f(x1, y2, -z1);
		glVertex3f(x2 + p, y1, -z2);
		glVertex3f(-x2 + p, y1, -z2);
		glEnd();
	}

	glBegin(GL_QUADS);

	glVertex3f(x1, y2, z1);
	glVertex3f(-x1, y2, z1);
	glVertex3f(-x1, y2, -z1);
	glVertex3f(x1, y2, -z1);

	glVertex3f(x1, y2, z1);
	glVertex3f(x1, y2, -z1);
	glVertex3f(x2 + p, y1, -z2);
	glVertex3f(x2 + p, y1, z2);


	glVertex3f(x2 + p, y1, z2);
	glVertex3f(-x2 + p, y1, z2);
	glVertex3f(-x2 + p, y1, -z2);
	glVertex3f(x2 + p, y1, -z2);

	glVertex3f(x1, y2, z1);
	glVertex3f(-x1, y2, z1);
	glVertex3f(-x2 + p, y1, z2);
	glVertex3f(x2 + p, y1, z2);

	glVertex3f(-x1, y2, -z1);
	glVertex3f(-x1, y2, z1);
	glVertex3f(-x2 + p, y1, z2);
	glVertex3f(-x2 + p, y1, -z2);

	glEnd();

	if (a > 0) {
		glColor3f(0.231, 0.208, 0.365);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
		glLineWidth(6);
		glColor3f(0.3059, 0.435, 0.596);
		glVertex3f(x1, y2, z1);
		glVertex3f(-x1, y2, z1);
		glVertex3f(-x1, y2, -z1);
		glVertex3f(x1, y2, -z1);

		glVertex3f(x2, y1, z2);
		glVertex3f(-x2, y1, z2);
		glVertex3f(-x2, y1, -z2);
		glVertex3f(x2, y1, -z2);
		glEnd();
	}
}

// SpongeBob
void drawbron() {

	glPushMatrix();
	glTranslatef(0, 34, 0);
	drawquad(100, 40, 20, 40, 20, 0, 0, 0, 0, 4, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 13, 0);
	glColor3f(1, 1, 1);
	drawquad(20, 40, 20, 40, 20, 0, 0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(165, 42, 42);
	drawquad(15, 40, 20, 40, 20, 0, 0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(247, 186, 11);
	glTranslatef(23, -30, -5);
	drawquad(30, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(0, 0, 0);
	glTranslatef(23, -36, -5);
	drawquad(7, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(247, 186, 11);
	glTranslatef(-23, -30, -8);
	drawquad(30, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(0, 0, 0);
	glTranslatef(-23, -36, -8);
	drawquad(7, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(247, 186, 11);
	glTranslatef(65, 30, 0);
	glRotatef(30, 0, 0, 1);
	drawquad(60, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(247, 186, 11);
	glTranslatef(-65, 30, 0);
	glRotatef(-30, 0, 0, 1);
	drawquad(60, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0);
	glPopMatrix();
}

void drawStoneFace()
{
	// body
	glColor3f(0.3059, 0.535, 0.796);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	GLUquadric* quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluCylinder(quadricObj, 30, 20, 100, 20, 20);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glColor3ub(19, 32, 44);
	glTranslatef(-4, 0, 0);
	glRotatef(-6, 0, 1, 0);
	glPushMatrix();
	glTranslatef(37, 5, 50);
	glRotatef(90, 0, 1, 0);
	glScalef(10, 4, 1.5);
	glutSolidCube(4);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4, 0, 0);
	glRotatef(6, 0, 1, 0);
	glPushMatrix();
	glTranslatef(-37, 5, 50);
	glRotatef(90, 0, 1, 0);
	glScalef(10, 4, 1.5);
	glutSolidCube(4);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-40, -18, 0);
	glRotatef(8, 1, 0, 0);
	//IIIIIIIII
	glPushMatrix();
	glTranslatef(40, 50, 40);
	glRotatef(90, 1, 0, 0);

	drawquad(40, 10, 1, 2, 1, 0, 0, 0, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(40, 50, 80);
	glScalef(16, 2, 2);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(29, 50, 73);
	glRotatef(90, 1, 0, 0);
	glScalef(3, 3, 3);

	drawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(51, 50, 73);
	glRotatef(90, 1, 0, 0);
	glScalef(3, 3, 3);
	drawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(40, 19.8, 10);
	glRotatef(-8, 1, 0, 0);
	glScalef(4, 6, 5);
	glRotatef(90, 1, 0, 0);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	for (GLdouble i = 0.00; i < PI; i += 0.005)
	{
		glTexCoord2f(1 / 2.2 * cos(i), 1 / 3.8 * sin(i));
		glVertex3d(2.2 * cos(i), -2, -5);
		glVertex3d(2.2 * cos(i), 3.8 * sin(i), -5);
		glVertex3d(2.2 * cos(i), -4, -5);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();
}

void drawTyre() {
	glBegin(GL_POLYGON);
	glLineWidth(2);
	for (int i = 0; i < 360; i++)
	{
		glVertex3f(8 * cos(2 * PI * i / 360), 8 * sin(2 * PI * i / 360), 0);
	}
	glEnd();
}

void drawTyres() {
	glColor3f(0, 0, 0);

	glPushMatrix();
	glTranslatef(20, 20, -35);
	drawTyre();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-30, 20, -35);
	drawTyre();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 20, 35);
	drawTyre();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-30, 20, 35);
	drawTyre();
	glPopMatrix();
}


void drawcar() {
	glPushMatrix();
	glTranslatef(9.5, 48, 0);

	glScalef(0.8, 1, 1);
	drawquad(23, 53, 20, 38, 20, 0, 0, 0, 0, 0, 13);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 20, 0);
	glColor3f(1, 1, 1);
	drawquad(35, 50, 30, 50, 30, 0, 0, 0, 0, 0, 0);
	glPopMatrix();

	drawTyres();
}

void drawcar1() {
	int n = 360;
	glPushMatrix();
	glTranslatef(3, 39, 0);
	glScalef(0.8, 1, 1);
	glColor3f(1, 1, 1);
	drawquad(8, 20, 25, 13, 25, 0, 0, 0, 0, 0, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 20, 0);
	glColor3f(254.0 / 255.0, 235.0 / 255.0, 105.0 / 255.0);
	drawquad(15, 45, 25, 37, 25, 0, 0, 0, 0, 0, 20);
	glPopMatrix();

	drawTyres();
}

void drawcar2() {
	glPushMatrix();
	glLineWidth(1);
	glTranslated(0, 30, 0);
	glScalef(1.5, 1.5, 1.5);
	// Body of the car
	glPushMatrix();
	glScalef(50, 20, 30);

	glutSolidCube(1);
	glColor4f(1, 1, 1, 0.5);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9, 10, 0);
	glScalef(30, 20, 20);
	glColor3f(0.24, 0.2, 0);
	glutSolidCube(1);
	glColor4f(1, 1, 1, 0.5);
	glutWireCube(1);
	glPopMatrix();

	// tyres
	glPushMatrix();
	glTranslatef(20, -5, 13);
	glScalef(5, 6, 5);
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCylinder(1, 1, 100, 10);
	glColor3f(float(rand() % 255) / 255.0, float(rand() % 255) / 255.0, float(rand() % 255) / 255.0);
	glutWireCylinder(1, 1, 100, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20, -5, 13);
	glScalef(5, 6, 5);
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCylinder(1, 1, 100, 10);
	glColor3f(float(rand() % 255) / 255.0, float(rand() % 255) / 255.0, float(rand() % 255) / 255.0);
	glutWireCylinder(1, 1, 100, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20, -5, -18);
	glScalef(5, 6, 5);
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCylinder(1, 1, 100, 10);
	glColor3f(float(rand() % 255) / 255.0, float(rand() % 255) / 255.0, float(rand() % 255) / 255.0);
	glutWireCylinder(1, 1, 100, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, -5, -18);
	glScalef(5, 6, 5);
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCylinder(1, 1, 100, 10);
	glColor3f(float(rand() % 255) / 255.0, float(rand() % 255) / 255.0, float(rand() % 255) / 255.0);
	glutWireCylinder(1, 1, 100, 10);
	glPopMatrix();

	glPopMatrix();
}

void drawcar3() {
	glPushMatrix();
	glTranslatef(0, 20, 0);
	glScalef(1.5, 1.5, 1.5);
	glLineWidth(1);

	//Body of the bus
	glPushMatrix();
	glScalef(50, 50, 30);
	glutSolidCube(1);
	glColor4f(0, 0, 0, 0.5);
	glutWireCube(1);
	glPopMatrix();

	//tyres
	glPushMatrix();
	glTranslatef(20, -5, 13);
	glScalef(5, 6, 5);
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCylinder(1, 1, 100, 10);
	glColor3f(0, 0, 0);
	glutWireCylinder(1, 1, 100, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20, -5, 13);
	glScalef(5, 6, 5);
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCylinder(1, 1, 100, 10);
	glColor3f(0, 0, 0);
	glutWireCylinder(1, 1, 100, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20, -5, -18);
	glScalef(5, 6, 5);
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCylinder(1, 1, 100, 10);
	glColor3f(0, 0, 0);
	glutWireCylinder(1, 1, 100, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, -5, -18);
	glScalef(5, 6, 5);
	glColor3f(0.4, 0.4, 0.4);
	glutSolidCylinder(1, 1, 100, 10);
	glColor3f(0, 0, 0);
	glutWireCylinder(1, 1, 100, 10);
	glPopMatrix();

	glPopMatrix();
}

void drawcars()
{
	for (int i = 0; i < 10; i++) {
		carD[i] = posx[i] * (10-i) * i + i * (10-i) * 2 + mov[i] / 3 - 650;
		glPushMatrix();
		glTranslatef(carD[i], 0, 20);
		glScalef(sizex[i], sizey[i], sizez[i]);
		glRotatef(180, 0, 1, 0);
		glColor3f(r[i], g[i], b[i]);
		switch (cartype[i]) {
		case 0:
			drawcar();
			break;
		case 1:
			drawcar1();
			break;
		case 2:
			drawcar2();
			break;
		case 3:
			drawcar3();
			break;
		}
		glPopMatrix();
	}

	for (int i = 0; i < 10; i++) {
		carD2[i] = 60 + posx[i] * i * (10-i)*2 - mov[i] / 3;
		glPushMatrix();
		glTranslatef(carD2[i], 0, -35);
		glScalef(sizex[i], sizey[i], sizez[i]);
		glColor3f(r[i], g[i], b[i]);
		switch (cartype[i]) {
		case 0:
			drawcar();
			break;
		case 1:
			drawcar1();
			break;
		case 2:
			drawcar2();
			break;
		case 3:
			drawcar3();
			break;
		}
		glPopMatrix();
	}
}


void display()
{
	glPushMatrix();
	glColor3f(1, 1, 1);
	glScalef(10, 10, 10);
	glutSolidOctahedron();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-120, 38.0, 0);
	glScalef(1.5, 1.5, 1.5);
	drawPinapple();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glColor3f(125.0 / 255.0, 125.0 / 255.0, 125.0 / 255.0);
	glTranslatef(-120, 0, -200);
	drawquad(5, 35, 120, 35, 120, 0, 0, 0, 0, 0, 0);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(240, 0, 0);
	glScalef(4.5, 4.5, 4.5);
	glRotatef(-90, 1, 0, 0);
	drawStoneFace();
	glPopMatrix();


	glColor3ub(138, 89, 105);
	glPushMatrix();
	glTranslatef(600, 10, 0);
	glScalef(3, 3.3, 3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	GLUquadric* quadricObj = gluNewQuadric();
	gluQuadricTexture(quadricObj, GL_TRUE);
	gluSphere(quadricObj, 30, 20, 20);
	//gluDeleteQuadric(quadricObj);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	//road
	int p = 0;
	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glColor3f(205.0 / 255.0, 133.0 / 255.0, 63.0 / 255.0);
		glTranslatef(250, 0, -130 - p);
		drawquad(10, 35, 10, 35, 10, 0, 0, 0, 0, 0, 0);
		glPopMatrix();
		p = p + 53;
	}

	glPushMatrix();
	glColor3f(125.0 / 255.0, 125.0 / 255.0, 125.0 / 255.0);
	glTranslatef(575, 0, -200);
	drawquad(5, 35, 120, 35, 120, 0, 0, 0, 0, 0, 0);
	glPopMatrix();


	glColor3f(125.0 / 255.0, 125.0 / 255.0, 125.0 / 255.0);
	glTranslatef(65, 0, -360);
	drawquad(5, 1200, 35, 1200, 35, 0, 0, 0, 0, 0, 0);


	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(3000, 0, 3000);
	glTexCoord2f(1, 0); glVertex3d(-3000, 0, 3000);
	glTexCoord2f(1, 1); glVertex3d(-3000, 0, -3000);
	glTexCoord2f(0, 1); glVertex3d(3000, 0, -3000);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0); glVertex3d(-1300, -400, 1000);
	glTexCoord2f(1, 0); glVertex3d(2000, -400, 1000);
	glTexCoord2f(1, 1); glVertex3d(2000, 1600, 1000);
	glTexCoord2f(0, 1); glVertex3d(-1300, 1600, 1000);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	drawcars();

	for (int i = 0; i < 25; i++) {
		glPushMatrix();
		glTranslatef(randm[i][0], randm[i][1] + ballm, randm[i][2]);
		ball();
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(-350, 55, 150);
	glScalef(1.1, 1.1, 1.1);
	drawbron();
	glPopMatrix();
}


void displayObject()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fltFOV, 1, 0.1, 5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(fltX0 * fltZoom, fltY0 * fltZoom, fltZ0 * fltZoom, fltXRef, fltYRef, fltZRef, fltXUp, fltYUp, fltZUp);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2, 0.2, 0.8, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	display();
	glutSwapBuffers();
	glLoadIdentity();
	glFlush();
}

void reshapeWindow(GLint intNewWidth, GLint intNewHeight)
{
	glViewport(0, 0, intNewWidth, intNewHeight);
}

void keyboard_input(unsigned char key, int x, int y) {
	switch (key) {
	case 'l':
		fltX0 -= cameraD;
		break;
	case 'r':
		fltX0 += cameraD;
		break;
	case 'd':
		fltY0 += cameraD;
		break;
	case 'u':
		fltY0 -= cameraD;
		break;
	case'p':
		fltZ0 += cameraD;
		break;
	case'e':
		fltZ0 -= cameraD;
		break;
	case 'i':
		fltZoom -= 0.005;
		break;
	case 'o':
		fltZoom += 0.005;
		break;
	case 'm':
		fltYRef += cameraD;
		break;
	case 'n':
		fltYRef -= cameraD;
		break;
	case ',':
		fltXRef -= cameraD;
		break;
	case '.':
		fltXRef += cameraD;
		break;
	case 'b':
		if (rate > 0.2) {
			rate -= 0.2;
		}
	case 'a':
		rate += 0.2;
	}

	glutPostRedisplay();
}

void movingCar() {
	for (int i = 0; i < 10; i++) {
		if (carD[i] > 500.0) {
			posx[i] = rand() % 10 * i;
			r[i] = float(rand() % 255) / 255.0;
			g[i] = float(rand() % 255) / 255.0;
			b[i] = float(rand() % 255) / 255.0;
			sizex[i] = float(rand() % 5) / 23.00 + 0.4;
			sizey[i] = float(rand() % 5) / 23.00 + 0.5;
			sizez[i] = float(rand() % 5) / 23.00 + 0.5;
			mov[i] = 0;
			cartype[i] = rand() % 4;
		}
	}
	for (int i = 0; i < 10; i++) {
		if (carD2[i] < -500.0) {
			posx[i] = rand() % 10 * i;
			r[i] = float(rand() % 255) / 255.0;
			g[i] = float(rand() % 255) / 255.0;
			b[i] = float(rand() % 255) / 255.0;
			sizex[i] = float(rand() % 5) / 23.00 + 0.4;
			sizey[i] = float(rand() % 5) / 23.00 + 0.5;
			sizez[i] = float(rand() % 5) / 23.00 + 0.5;
			mov[i] = 0;
			cartype[i] = rand() % 4;
		}
	}
	glutPostRedisplay();
}

void mouse_input(int button, int state, int x, int y) 
{
	if (state == GLUT_DOWN)	// make the text shining
		if (statee == 3) {
			statee = 0;
		}
		else { statee += 1; }
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(intWinWidth, intWinHeight);
	glutCreateWindow("SpongeBob");
	myinit();
	glutKeyboardFunc(keyboard_input);
	glutTimerFunc(time_interval, OnTimer, 1);
	glutMouseFunc(mouse_input);
	glutReshapeFunc(reshapeWindow);
	glutDisplayFunc(displayObject);
	glutIdleFunc(movingCar);
	glutMainLoop();
}