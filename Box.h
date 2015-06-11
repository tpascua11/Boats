//  Box.h
//  Clean Boat Project
//
//  Created by Ted Pascua on 11/18/14.
//  Copyright (c) 2014 Ted Pascua. All rights reserved.

#ifndef Clean_Boat_Project_Box_h
#define Clean_Boat_Project_Box_h
//Box Object With Spring
struct BoxMass{
    float mass  ; float oomass; float force[2];
    float pos[2]; float vel[2]; float deep = 0;
    float aclUp = 0;
} box[MAX_CONTROL_POINTS];
struct BoxSprings{
    int mass[2]; float length; float stiffness;
}bspring[MAX_CONTROL_POINTS];
int bnsprings = 0; int nbpoints = 5;
float bLines[MAX_CONTROL_POINTS-1];
float bdLines[MAX_CONTROL_POINTS-2];
float bnLines[2];

//Setup Box
void setupBox()  ; void boxSafeDrop();
void boxPoints() ;

//Box Collusions
void maintainBox(); void boxSafeDrop();
void boxGravity(); void floatOn2(int boxPoints);

//Draw and Get Angle
float endsAngle();
void drawBox();
#endif
