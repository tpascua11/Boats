//  Main.h
//  Clean Boat Project
//
//  Created by Ted Pascua on 11/18/14.
//  Copyright (c) 2014 Ted Pascua. All rights reserved.
//
#ifndef Clean_Boat_Project_Main_h
#define Clean_Boat_Project_Main_h

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
//#include <OpenGL/gl.h>
//#include <GLUT/glut.h>

#include <GL/glx.h>

#include <math.h>
#include <cmath>
#include <unistd.h>
#include <time.h>
#include <string.h>

using namespace std;
#define MAX_CONTROL_POINTS 200
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define rnd() (float)rand() / (float) RAND_MAX
#define PI 3.14159265358979323846
#define MAX_MATRIX 8
int mMat   = MAX_MATRIX; //This changes Matrix Coordinates(PURE)
float fMat = MAX_MATRIX; //This changes Matrix Coordinates(Not Pure)

const char* WINDOW_TITLE = "Boat-Project";
//Background Colors Variables
float r = 245, g = 61, b = 0;
int zone = 0  ; float pace = 0.25;
int freeze = 0; int darkness = 0;

//Main Functions
void mainSetup();
void handleKeypress(unsigned char key, int x, int y);
void update(int value);
void drawObjects();
void physics();

//For Extra
void skyChange(); void fakeDarkness();
void findY()    ;
//Not Nessary but Usefull
void normalize2d(float x, float y);

#endif
