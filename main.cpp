//  main.cpp Final Project: Boat/Ship On Water Waves
//  Author: Ted Pascua
//  Date  : 11/15/14.
//  Purpose Make A Boat Float ON Water
//  Copyright (c) 2014 Ted Pascua. All rights reserved.
//  Fall 2014 CMPS 371 Computer Graphics
//Simple Controls: H J K L: for movement of the boat
//                 F: for freezing the waves. D: to make it dark around the boat
//                 C: change the sky faster.
//Look at handleKeyPresss for more wave controls.
//-----Directory-----+
//     At Core       |
//     Waves Physics |
//     Box   Physics | Note: Boat Place itself withing The Box Points
//     Boat          | Note: boat = ship for now
//     Extras        |
//     Not Nessary   |
//--------------------
#include "Main.h"
#include "Waves.h"
#include "Box.h"
#include "Boat.h"

//Global Access

//=========================================================================
//At Core
//=========================================================================
void display(); 
void handleKeypress(unsigned char key, int x, int y) {
    void moveBoat(unsigned char take);
    switch (key) {
        case 27: exit(0); break;//Escape keyƒ
        case 'a': endsAngle(); break;
        case 's': safeWaveDrop();
        case 'f': freeze ^= 1;  break;
        case 'c': pace +=1; break;
        case 'd': darkness ^= 1;  pace = 0.0425;
                  r = 0; g = 61; b = 245; zone = 5; break;
        case 'o': rWavesUp(); break;
        case 'p': rWavesDown(); break;
        case 'j': case 'k': case 'l': case 'h': moveBoat(key); break;
        case 'u': lcap -= 0.1; break;
        case 'i': lcap += 0.1; break;
        case '=': speed += 0.0025; break;
        case '-': speed -= 0.0025; break;
        case '.': rOn ^= 1; break;
        case ',': lOn ^= 1; break;
        case 'n': rcap -= 0.1; break;
        case 'm': rcap += 0.1; break;
        case 'q': exit(0); break;
    }
}
void update(int value) {
    glutPostRedisplay(); glutTimerFunc(15, update, 0);
}

int main(int argc, char ** argv) {
    mainSetup();
    glutInit(&argc, argv); glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    //Create Window
    glutCreateWindow(WINDOW_TITLE);
    //Setup GLUT Callback Functions
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(25, update, 0);
    glutDisplayFunc(display);
    glutMainLoop(); //The Main Loop
    return 0;
}
void mainSetup(){//Setup Points to draw at
    wavePoints(); setupWaves();
    boxPoints(); setupBox();
    setupShip(); flagPoints();
}
void display(){//Begin to display. Gets phsics done first then call draw Objects
    glClear( GL_COLOR_BUFFER_BIT );
    physics()   ; glClear( GL_COLOR_BUFFER_BIT );
    drawObjects(); glutSwapBuffers();
}
void physics(){//Cause the phyiscs of waves and boat
    skyChange();
    lWaves(); maintainWaves(); rWaves(); boxGravity();
    maintainBox();
}
void drawObjects(){//Draw The Object
    glMatrixMode( GL_PROJECTION ); glLoadIdentity();
    glOrtho( -mMat, mMat, -mMat, mMat, -1, 1);
    //draw Boat !NOTE!: withing drawWaves the Draw Ship Function is called
    drawWaves(); drawBox(); fakeDarkness();
}

//=========================================================================
//Waves Physics
//=========================================================================
void wavePoints(){
    int capP = 30; float extendX = 0.2;
    float stepX = (fMat + extendX)/capP;
    float stepY = capP/(mMat);
    float atX = -fMat, atY = 0;
    wpoints = 0;
    for(int i = 0; i < capP*2; i++){
        wmass[i].pos[0] = atX; wmass[i].pos[1] = 0; wpoints++;
        atX += stepX; atY += stepY;
    }
}
void setupWaves(){
    getWavesLines();
    float vel[2]; vel[0] *= 0.01; vel[1] *= 0.01;
    wsprings = 0;
    for(int i = 0; i < wpoints; i++){
        wmass[i].mass = 1.0; wmass[i].oomass = 1.0 / wmass[i].mass;
        wmass[i].vel[0] = vel[0] + rnd()*0.02-0.01;
        wmass[i].vel[1] = vel[1] + rnd()*0.02-0.01;
        wmass[i].force[0] = wmass[i].force[1] = 0;
        if(i != wpoints -1){
            wspring[i].mass[0] = i; wspring[i].mass[1] = i + 1;
            wspring[i].length = wLines[i];
            wspring[i].stiffness = rnd() * 0.2 + 0.1; wsprings++;
        }
    }
    for(int i = 0; i < wpoints - 2; i++){
        wspring[wsprings].mass[0] = i;
        wspring[wsprings].mass[1] = i + 2;
        wspring[wsprings].length = dwLines[i];
        wspring[wsprings].stiffness = 0.1; wsprings++;
    }
    safeWaveDrop();
}
void getWavesLines(){
    float checkX, checkY;
    for(int i = 0; i < wpoints; i++){
        checkX = abs(wmass[i+1].pos[0] - wmass[i].pos[0]);
        checkX = pow(checkX, 2);
        checkY = abs(wmass[i+1].pos[1] - wmass[i].pos[1]);
        checkY = pow(checkY, 2);
        wLines[i] = sqrt(checkX + checkY);
    }
    for(int i = 0; i <wpoints - 2; i++){
        checkX = abs(wmass[i+2].pos[0] - wmass[i].pos[0]);
        checkY = abs(wmass[i+2].pos[1] - wmass[i].pos[1]);
        dwLines[i] = sqrt(checkX + checkY);
    }
    /*
     for(int i = 0; i < nbpoints - 2; i++){
     bspring[bnsprings].mass[0] = i;
     bspring[bnsprings].mass[1] = i + 2;
     bspring[bnsprings].length = bdLines[i];
     bspring[bnsprings].stiffness = 0.1;
     bnsprings++;
     }*/
}

void maintainWaves(){
    int m0, m1; float penalty = 0.01f;
    float dist, oodist, factor;
    float springforce[2], springVec[2];
    for(int i = 0; i < wpoints; i++){
        if(i == 0 || i == wpoints - 1){
            wmass[i].vel[0] = wmass[i].vel[1] = 0;
            wmass[i].force[0] = wmass[i].force[1] = 0; continue;
        }
        wmass[i].vel[0] += wmass[i].force[0] * wmass[i].oomass;
        wmass[i].vel[1] += wmass[i].force[1] * wmass[i].oomass;
        wmass[i].pos[0] += wmass[i].vel[0];
        wmass[i].pos[1] += wmass[i].vel[1];
        wmass[i].force[0] = wmass[i].force[1] = 0;
        if(wmass[i].pos[0] < float(-mMat)){
            // wmass[i].force[0] = penalty * -wmass[i].pos[0];
            //  wmass[i].force[0] += rnd() *0.02-0.01;
        }
        if(wmass[i].pos[0] > (float)mMat){
            //   wmass[i].force[0]  = penalty * ((float)mMat - wmass[i].pos[0]);
            //   wmass[i].force[0] += rnd()*0.02-0.01;
        }
        if(wmass[i].pos[1] < -mMat){
            wmass[i].force[1]  = penalty * -wmass[i].pos[1];
            wmass[i].force[1] += rnd()*0.02-0.01;
        }
        if(wmass[i].pos[1] > (float)(mMat)){
            wmass[i].force[1]  = penalty * ((float)mMat - wmass[i].pos[1]);
            wmass[i].force[1] += rnd()*0.002-0.001;
        }
        if(freeze == 1){wmass[i].vel[0] *= 0.800; wmass[i].vel[1] *= 0.800;}
        else wmass[i].vel[0] *= 0.999; wmass[i].vel[1] *= 0.990;
    }
    for(int i = 0; i < wsprings; i++){
        m0 = wspring[i].mass[0];
        m1 = wspring[i].mass[1];
        
        springVec[0] = wmass[m0].pos[0] - wmass[m1].pos[0];
        springVec[1] = wmass[m0].pos[1] - wmass[m1].pos[1];
        
        dist = sqrt(springVec[0]*springVec[0] + springVec[1]*springVec[1]);
        
        if(dist == 0.0) dist = 0.1;
        oodist = 1.0/dist;
        springVec[0] *= oodist;
        springVec[1] *= oodist;
        
        factor = -(dist - wspring[i].length) * wspring[i].stiffness;
        springforce[0] = springVec[0] * factor;
        springforce[1] = springVec[1] * factor;
        wmass[m0].force[0] += springforce[0];
        wmass[m0].force[1] += springforce[1];
        wmass[m1].force[0] -= springforce[0];
        wmass[m1].force[1] -= springforce[1];
        
        springforce[0] = (wmass[m1].vel[0] - wmass[m0].vel[0]) * 0.001;
        springforce[1] = (wmass[m1].vel[1] - wmass[m0].vel[0]) * 0.001;
        wmass[m0].force[0] += springforce[0];
        wmass[m0].force[1] += springforce[1];
        wmass[m1].force[0] -= springforce[0];
        wmass[m1].force[1] -= springforce[1];
    }
}

void safeWaveDrop(){
    for(int i = 0; i < wpoints; i++){
        wmass[i].vel[0] = wmass[i].vel[1] = 0;
        wmass[i].force[0] = wmass[i].force[1] = 0;
    }
}
void lWaves(){
    if(freeze == 1 || lOn == 0 ) return;
    if(wmass[0].pos[1] >=   mMat/2 + lcap) wl = -(0.06 + speed);
    if(wmass[0].pos[1] <= -(mMat/2 + lcap)) wl = 0.06 + speed;  incl++;
    if(incl == lAt){
        wmass[0].pos[1] += wl; incl = 0;
    }
}
void rWaves(){
    if(freeze == 1 || rOn == 0) return;
    if(wmass[wpoints - 1].pos[1] >=   mMat/2 + rcap) wr = -(0.06 + speed);
    if(wmass[wpoints - 1].pos[1] <= -(mMat/2 + rcap)) wr = 0.06 + speed;  incr++;
    if(incr == rAt){
        wmass[wpoints -1].pos[1] += wr; incr = 0;
    }
}

void drawWaves(){
    glPushMatrix();
    glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor4ub(0, 255, 255, 100); if(freeze == 1) glColor4ub(165, 242, 243, 170);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-fMat, -fMat); glVertex2f(-fMat, 0);
    glVertex2f(fMat, 0); glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(fMat, 0); glVertex2f(fMat, -fMat);
    glVertex2f(-fMat, -fMat); glEnd();
    glColor4ub(165, 42, 42, 255);
    
    drawShip();
    //Orignal
    //Waves
    glColor4ub(0, 255, 255, 100); if(freeze == 1) glColor4ub(165, 242, 243, 230);
    for(int i = 0; i < wpoints-1; i++){
        glBegin (GL_TRIANGLE_FAN);
        glVertex2f(wmass[i].pos[0], wmass[i].pos[1]);
        glVertex2f(wmass[i].pos[0], -mMat);
        glVertex2f(wmass[i+1].pos[0], -mMat);
        glEnd();
        glBegin (GL_TRIANGLE_FAN);
        glVertex2f(wmass[i].pos[0], wmass[i].pos[1]);
        glVertex2f(wmass[i+1].pos[0], wmass[i+1].pos[1]);
        glVertex2f(wmass[i+1].pos[0], -mMat);
        glEnd();
    }
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
//=========================================================================
//Box Physics
//=========================================================================
void boxPoints(){
    box[0].pos[0] = 1  ; box[0].pos[1] = 0;
    box[1].pos[0] = 2  ; box[1].pos[1] = 0;
    box[2].pos[0] = 3  ; box[2].pos[1] = 0;
    box[3].pos[0] = 4  ; box[3].pos[1] = 0;
    box[4].pos[0] = 5 ; box[4].pos[1] = 0;
    
    float checkX, checkY;
    for(int i = 0; i < nbpoints - 1; i++){
        checkX = abs(box[i+1].pos[0] - box[i].pos[0]);
        checkX = pow(checkX, 2);
        checkY = abs(box[i+1].pos[1] - box[i].pos[1]);
        checkY = pow(checkY, 2);
        bLines[i] = sqrt(checkX + checkY);
        printf("\n %d to %d distance is %f \n",
               i, i+1, bLines[i]);
    }
    for(int i = 0; i <nbpoints - 2; i++){
        checkX = pow(box[i+2].pos[0] - box[i].pos[0], 2);
        checkY = pow(box[i+2].pos[1] - box[i].pos[1], 2);
        bdLines[i] = sqrt(checkX + checkY);
        printf("\n %d to %d distance is %f \n",
               i, i+2, bdLines[i]);
    }
    checkX = pow(box[nbpoints-1].pos[0] - box[0].pos[0], 2);
    checkY = pow(box[nbpoints-1].pos[1] - box[0].pos[1], 2);
    bnLines[0] = sqrt(checkX + checkY);
    printf("\n %d to %d distance is %f \n",
           0, nbpoints-1, bnLines[0]);
}
void setupBox(){
    float vel[2];
    vel[0] *= 0.01; vel[1] *= 0.01;
    bnsprings = 0;
    
    for(int i = 0; i < nbpoints; i++){
        box[i].mass = 1.0; box[i].oomass = 1.0 / box[i].mass;
        box[i].vel[0] = vel[0] + rnd()*0.02-0.01;
        box[i].vel[1] = vel[1] + rnd()*0.02-0.01;
        box[i].force[0] = box[i].force[1] = 0;
        if(i != nbpoints-1){
            bspring[i].mass[0] = i; bspring[i].mass[1] = i + 1;
            bspring[i].length = bLines[i];
            bspring[i].stiffness = 0.1;
            bnsprings++;
        }
    }
    for(int i = 0; i < nbpoints - 2; i++){
        bspring[bnsprings].mass[0] = i;
        bspring[bnsprings].mass[1] = i + 2;
        bspring[bnsprings].length  = bdLines[i];
        bspring[bnsprings].stiffness = 0.1;
        bnsprings++;
    }
    bspring[bnsprings].mass[0] = 0;
    bspring[bnsprings].mass[1] = nbpoints - 1;
    bspring[bnsprings].length = bnLines[0];
    bspring[bnsprings].stiffness = 0.1;
    bnsprings++;
    
    printf("Amount of bsprings : %d", bnsprings);
    boxSafeDrop();
}

void maintainBox(){
    int m0, m1; float penalty = 0.01f;
    float dist, oodist, factor;
    float springforce[2], springVec[2];
    
    for(int i = 0; i < nbpoints; i++){
        box[i].vel[0] += box[i].force[0] * box[i].oomass;
        box[i].vel[1] += box[i].force[1] * box[i].oomass;
        box[i].pos[0] += box[i].vel[0];
        box[i].pos[1] += box[i].vel[1];
        box[i].force[0] = box[i].force[1] = 0;
        if(box[i].pos[0] < -fMat){
            box[i].force[0] = penalty * -box[i].pos[0];
            box[i].force[0] += rnd() *0.02-0.01;
        }
        if(box[i].pos[0] > fMat){
            box[i].force[0]  = penalty * ((float)mMat - box[i].pos[0]);
            box[i].force[0] += rnd()*0.02-0.01;
        }
        if(box[i].pos[1] < -fMat){
            //box[i].force[1]  = penalty * -box[i].pos[1];
            //box[i].force[1] += rnd()*0.02-0.01;
             box[i].vel[1] = 0;
        }
        if(box[i].pos[1] > fMat){
            //box[i].force[1]  = penalty * ((float)fMat - box[i].pos[1]);
            //box[i].force[1] += rnd()*0.002-0.001;
             box[i].force[1] = 0; box[i].vel[1] = 0;
        }
        if(freeze == 1 && box[i].deep == 1){box[i].vel[0] *= 0.850; box[i].vel[1] *= 0.850;}
        else if(box[i].deep == 1){ box[i].vel[0] *= 0.999; box[i].vel[1] *= 0.699;}
        else if(box[i].deep == 0){ box[i].vel[0] *= 0.999; box[i].vel[1] *= 0.999;}
        //box[i].vel[0] *= 0.999; box[i].vel[1] *= 0.999;
    }
    
    for (int i=0; i< bnsprings; i++) {
        m0 = bspring[i].mass[0];
        m1 = bspring[i].mass[1];
        //forces are applied here //vector between the two masses
        springVec[0] = box[m0].pos[0] - box[m1].pos[0];
        springVec[1] = box[m0].pos[1] - box[m1].pos[1];
        //distance between the two masses
        //CHANGE WENT HERE!!!!
        dist = sqrt(springVec[0]*springVec[0] + springVec[1]*springVec[1]);
        //====================================================================
        if (dist == 0.0) dist = 0.1;
        oodist = 1.0 / dist;
        springVec[0] *= oodist;
        springVec[1] *= oodist;
        //the spring force is added to the mass force
        factor = -(dist - bspring[i].length) * bspring[i].stiffness;
        springforce[0] = springVec[0] * factor;
        springforce[1] = springVec[1] * factor;
        //apply force and negative force to each end of the spring...
        box[m0].force[0] += springforce[0];
        box[m0].force[1] += springforce[1];
        box[m1].force[0] -= springforce[0];
        box[m1].force[1] -= springforce[1];
        //damping
        springforce[0] = (box[m1].vel[0] - box[m0].vel[0]) * 0.1;
        springforce[1] = (box[m1].vel[1] - box[m0].vel[1]) * 0.1;
        box[m0].force[0] += springforce[0];
        box[m0].force[1] += springforce[1];
        box[m1].force[0] -= springforce[0];
        box[m1].force[1] -= springforce[1];
    }
}
void boxGravity(){
    for(int i = 0; i < nbpoints; i++){
        box[i].force[1] -= 0.01; //if(i == 2) box[i].force[0] += 0.001;
        floatOn2(i);
    }
}
void floatOn2(int boxPoints){
    void newBuo(int take);
    float distanceW = 0, lowDist = mMat; float reachAngle;
    float distance = 0 ;float atAngle  ; float lp;
    float checkX, checkY; int belowY = 0; int b = boxPoints;
    float xDiff, yDiff;
    int slideOn;
    int nX = 0, nY = 0;//Negative Zone.
    for(int i = 0; i < wpoints; i++){
        if(wmass[i].pos[1] > wmass[i+1].pos[1]){
            lp = wmass[i+1].pos[1]; slideOn = 1;
        }
        else{
            lp = wmass[i].pos[1]; slideOn = -1;
        }
        
        if(box[b].pos[0] >= wmass[i].pos[0] && box[b].pos[0] <= wmass[i + 1].pos[0]){
            if(box[b].pos[1] <= lp){
                newBuo(b); box[b].deep = 1; return;
            }
        }
        //This was used to detect water at an angle, but did not work out correctly
        /*
        distanceW = distanceCord(wmass[i].pos[0], wmass[i].pos[1], wmass[i+1].pos[0], wmass[i+1].pos[1]);
        xDiff = wmass[i+1].pos[0] - wmass[i].pos[1];
        yDiff = wmass[i+1].pos[1] - wmass[i].pos[1];
        reachAngle = atan2(yDiff, xDiff) * (180 / PI);
        xDiff = box[b].pos[0] - wmass[i].pos[0];
        yDiff = box[b].pos[1] - wmass[i].pos[1];
        atAngle = atan2(yDiff, xDiff) * (180/ PI);
        distance = distanceCord(wmass[i].pos[0], wmass[i].pos[1], box[b].pos[0], box[b].pos[1]);
        if(atAngle < reachAngle && distance < distanceW){
            newBuo(); return;
        }*/
    }
    box[b].deep = 0;
}
void newBuo(int take){
    if(freeze == 1){ box[take].vel[1] += 0.01; return;}
    if(box[take].deep == 1) box[take].aclUp += 0.0003;
    else box[take].aclUp = 0;
    
    box[take].vel[1] += 0.03 + box[take].aclUp;
}

void boxSafeDrop(){
    for(int i = 0; i < nbpoints; i++){
        box[i].vel[0] = box[i].vel[1] = 0;
        box[i].force[0] = box[i].force[1] = 0;
    }
}
void drawBox(){
    glColor3ub(255, 0, 255);
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < nbpoints; i++){
        glVertex2f(box[i].pos[0], box[i].pos[1]);
    } glEnd();
}
//========================================================================
//Boat Points
//========================================================================
void setupShip(){
    //Left Side
    shipPoints = 17;
    ship[0][0] = -4; ship[0][1] = 0;
    ship[1][0] = -4; ship[1][1] = 1;
    ship[2][0] = -2; ship[2][1] = 1;
    ship[3][0] = -2; ship[3][1] = 0;
    ship[4][0] = -2; ship[4][1] = -2;
    
    ship[5][0] = -2; ship[5][1] = 0;
    ship[6][0] = -2; ship[6][1] = -2;
    ship[7][0] =  1; ship[7][1] = -2;
    ship[8][0] =  3; ship[8][1] = 0;
    
    ship[9][0]  = 2 ; ship[9][1] = 0;
    ship[10][0] = 3; ship[10][1] = 0;
    ship[11][0] = 3; ship[11][1] = 1;
    ship[12][0] = 2; ship[12][1] = 1;
    
    ship[13][0] = 1  ; ship[13][1] = 0;
    ship[14][0] = 1  ; ship[14][1] = 4;
    ship[15][0] = 0.5; ship[15][1] = 4;
    ship[16][0] = 0.5; ship[16][1] = 0;
}
void flagPoints(){
    backflag[0][0] = 2   ; backflag[0][1] = 0.5;
    backflag[1][0] = 2   ; backflag[1][1] = 5;
    backflag[2][0] = -0.5; backflag[2][1] = 5;
    backflag[3][0] = -0.5; backflag[3][1] = 0.5;
    
    flag[0].pos[0] = -0.5; flag[0].pos[1] = 0.5;
    flag[1].pos[0] =  0.5; flag[1].pos[1] = 1.5;
    flag[2].pos[0] =  1  ; flag[2].pos[1] = 2.5;
    flag[3].pos[0] =  1.5; flag[3].pos[1] = 3.5;
    flag[4].pos[0] =  2  ; flag[4].pos[1] = 5;
    nfpoints = 5;
}

float endsAngle(){
    float xDiff, yDiff;
    xDiff = box[nbpoints-1].pos[0] - box[0].pos[0];
    yDiff = box[nbpoints-1].pos[1] - box[0].pos[1];
    float give = atan2(yDiff, xDiff) * (180 / PI);
    //printf("Angle is: %f \n", give);
    return give;
}

void drawShip(){
    rot1 = endsAngle();
    glPushMatrix();
    glTranslatef(box[2].pos[0], box[2].pos[1]+0.4, 0);
    glScalef(0.5,0.5,0);
    glRotatef(rot1, 0, 0, 1);
    glBegin(GL_POLYGON);
    for(int i = 0; i < shipPoints; i++){
        if(i == 5 || i == 9 ||  i == 13){
            glEnd(); glBegin(GL_POLYGON);
        }
        glVertex2f(ship[i][0], ship[i][1]);
    }
    glEnd();
    drawFlag(); glPopMatrix();
}
void drawFlag(){
    glColor4ub(215, 215, 215, 250);
    glBegin(GL_QUADS);
    for(int i = 0; i < 4; i++){
        glVertex2f(backflag[i][0], backflag[i][1]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor4ub(235, 230, 235, 100);
    for(int i = 0; i < nfpoints; i++){
        glVertex2f(flag[i].pos[0], flag[i].pos[1]);
    }
    glVertex2f(   2, 0.5);
    glEnd();
}
//=======================================================================
//Extras
//=======================================================================
void rWavesUp(){
    wmass[wpoints-1].pos[1] += 0.11;
}
void rWavesDown(){
    wmass[wpoints-1].pos[1] -= 0.11;
}
void fakeDarkness(){
    if(darkness != 1) return;
    glPushMatrix();
    glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor4ub(0, 0, 0, 230);
    glTranslatef(box[2].pos[0], box[2].pos[1], 0);
    
    glBegin(GL_QUADS);
    glVertex2f(2, fMat*3); glVertex2f(2, -fMat*3);
    glVertex2f(fMat*3, -fMat*3); glVertex2f(fMat*3, fMat*3); glEnd();
    
    glBegin(GL_QUADS);
    glVertex2f(-2, fMat*3); glVertex2f(-2, -fMat*3);
    glVertex2f(-fMat*3, -fMat*3); glVertex2f(-fMat*3, fMat*3);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex2f(-2, 3); glVertex2f(2, 3);
    glVertex2f(2, fMat*3); glVertex2f(-2, fMat*3);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex2f(-2, -1.5); glVertex2f(2, -1.5);
    glVertex2f(2, -fMat*3); glVertex2f(-2, -fMat*3);
    glEnd();
    
    glColor4ub(0, 0, 0, 195);
    glBegin(GL_QUADS);
    glVertex2f(-2, -1.5); glVertex2f(2, -1.5);
    glVertex2f(2, 3); glVertex2f(-2, 3);
    glEnd();
}
void skyChange(){
    if(zone == 0){
        if(r < 255) r += pace; if(g < 102) g += pace; if(b < 51) b+= pace;
        //printf("what is r: %f | g: %f | b %f \n", r, g, b);
        if(r >= 255 && g >= 102 && b >= 51){
            zone = 1; pace = 0.12; printf(" Zone 1 \n");
        }
    }
    if(zone == 1){
        if(r < 255) r += pace; if(g < 204) g+= pace; if(b < 51) b += pace;
        if(g >= 204){ zone = 2; pace = 0.15; printf(" Zone 2 \n");}
    }
    if(zone == 2){
        if(r > 51) r -= pace; if(b < 255) b += pace;
        if(r <= 51 && b >= 255){ zone = 3; pace = 0.25; printf("Zone 3 \n");}
    }
    if(zone == 3){
        if(r > 0) r -= pace; if(g > 184) g -= pace; if(b > 245) b -= pace;
        if(r <= 0 && g <= 184 && b <= 245){ zone = 4; pace = 0.15; printf("Zone 4 \n");}
    }
    if(zone == 4){
        if(g > 61) g -= pace;
        if(g <= 61){
            zone = 5; pace = 0.0425; printf("Zone 5 \n");
        }
    }
    if(zone == 5){
        if(r < 41) r+= pace; if(g > 41) g -= pace; if(b > 92) b -= pace;
        if(r >= 41 && g <= 41 && b <= 92){
            zone = 6; r = 51; g = 51; b = 102; printf("Zone 6 \n"); pace = 0.050;
        }
    }
    if(zone == 6){
        if(r < 245) r+= pace; if(g < 61) g+= pace; if(b > 0) b -= pace;
        if(r >= 245 && g >= 61 && b <= 0){ zone = 0; pace = 0.5; printf("Zone 0 \n");}
    }
    glClearColor(r/255, g/255, b/255, 0 );
}
void moveBoat(unsigned char take){
    switch(take){
        case 'j':
            box[2].vel[1] = -fMat; box[2].vel[0] = 0;
            box[2].force[0] = box[2].force[1] = 0;
            box[2].pos[1] = -fMat; break;
        case 'k':
            box[2].vel[1] = fMat; box[2].vel[0] = 0;
            box[2].force[0] = box[2].force[1] = 0;
            box[2].pos[1] = -fMat; break;
            
        case 'h':
            for(int i = 0; i < nbpoints; i++){
                box[i].vel[1] = 0; box[i].vel[0] =0;
                box[i].force[0] = -0.030 ;box[i].force[1] = 0;
            } break;
        case 'l':
            for(int i = 0; i < nbpoints; i++){
                box[i].vel[1] = 0; box[i].vel[0] = 0;
                box[i].force[0] = box[i].force[1] = 0;
                box[i].force[0] = 0.030 ;box[i].force[1] = 0;
            } break;
    }
}

//======================================================================
//Not Nessary but usefull
//======================================================================
void normalize2d(float x, float y){
    float length = x*x + y*y;
    if(length == 0.0)return;

    length = 1.0/sqrt(length);
    x *= length; y *= length;
}
float distanceCord(float x, float y, float x2, float y2){
    float checkX, checkY;
    checkX = pow((x2 - x),2);
    checkY = pow((y2 - y),2);
    return sqrt(checkX + checkY);
}
void findY(){
    printf("X at %f  ",  wmass[wpoints].pos[0]);
    printf("Y at %f \n", wmass[wpoints].pos[1]);
}

