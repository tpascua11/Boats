//
//  Waves.h
//  Clean Boat Project
//
//  Created by Ted Pascua on 11/18/14.
//  Copyright (c) 2014 Ted Pascua. All rights reserved.
//

#ifndef Clean_Boat_Project_Waves_h
#define Clean_Boat_Project_Waves_h

//Waves Object with Springs
struct MassWave{
    float mass; float oomass; float force[2];
    float pos[2]; float vel[2];
} wmass[MAX_CONTROL_POINTS];
struct WaveSprings{
    int mass[2]; float length; float stiffness;
} wspring[MAX_CONTROL_POINTS];
int wpoints = 0;
float wLines[MAX_CONTROL_POINTS-1];
float dwLines[MAX_CONTROL_POINTS-1];
float twLines[MAX_CONTROL_POINTS-1];
int wsprings = 0; int waveGate = 0;

//Waves Variables Access
int stop = 0; float speed = 0;
int    rOn = 0, lOn = 1;
float lcap = 0, rcap = 0;
float wl = 0.03; float lAt = 1; int incl = 0; int rh = 1;
float wr = 0.03; float rAt = 1; int incr = 0; int lh = 1;

//This Fuctions Will Setup Up The Waves
void wavePoints(); void getWavesLines(); void setupWaves();

//These Functions Add Physics to The Waves
void maintainWaves();
void safeWaveDrop(); void lWaves()  ; void rWaves();

//Stored Into Extra(minor physics change)
void rWavesUp(); void rWavesDown();

//Drawing at all Wave Points location
void drawWaves();

#endif
