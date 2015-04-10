//
//  Boat.h
//  Clean Boat Project
//
//  Created by Ted Pascua on 11/18/14.
//  Copyright (c) 2014 Ted Pascua. All rights reserved.
//

#ifndef Clean_Boat_Project_Boat_h
#define Clean_Boat_Project_Boat_h

//Flag
struct flagMass{
    float mass; float oomass  ; float force[2];
    float pos[2]; float vel[2]; float deep = 0;
}flag[MAX_CONTROL_POINTS];
/*
struct flagSprings{
    int mass[2]; float length; float stiffness;
} fspring[MAX_CONTROL_POINTS];

float fLines[MAX_CONTROL_POINTS-1];
float fdLines[MAX_CONTROL_POINTS-2];
float fnLines[2];*/
int fsprings = 0; int nfpoints = 5;
float backflag[4][2];

//Flag was suppose to have a cloth movement but
//due to time constraint was canceled.

//Boat Variable Access
float ship[MAX_CONTROL_POINTS][2]; int shipPoints = 0;
float rot1;

//Boat Get Points
void drawBoatPoints(); void setupShip(); void flagPoints();

//Boat Draw 
void drawShip(); void drawFlag();

#endif
