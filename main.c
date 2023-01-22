#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define MAXJL 10
#define DEG2RAD M_PI / 180

typedef struct{
    double x;
    double y;
} coord;

typedef struct jointAndLink{
    double linkLength;
    double jointAngle;
    coord jointPos;
    struct jointAndLink *child;
} jointLink;

void setJoints(int nJoints, double angles[MAXJL], jointLink *baseJL);

coord getLinkTipPos(jointLink *jl);

void initArm(int nArgs, ...);

int main(){
    int nJoints = 3;
    jointLink *jl1 = malloc(sizeof(jointLink));
    jl1->linkLength = 10;
    jl1->jointAngle = 0;
    jl1->jointPos = (coord){0, 0};

    jointLink *jl2 = malloc(sizeof(jointLink));
    jl2->linkLength = 5;
    jl2->jointAngle = M_PI/2;

    jointLink *jl3 = malloc(sizeof(jointLink));
    jl3->linkLength = 5;
    jl3->jointAngle = M_PI/2;

    initArm(3, jl1, jl2, jl3);

    coord armTipPos = getLinkTipPos(jl3);
    printf("arm tip pos: (%lf, %lf)\n", armTipPos.x, armTipPos.y);

    // double angles[MAXJL] = {DEG2RAD*90, DEG2RAD*(90), DEG2RAD*}
    // setJoints(nJoints, )

    // printf("hello\n");
    // coord linkEndPos = getTipPos(jl1);
    // printf("jlengt %lf\n", jl1->linkLength);
    // printf("jangle %lf\n", jl1->jointAngle);
    // printf("jx     %lf\n", jl1->jointPos.x);
    // printf("jy     %lf\n", jl1->jointPos.y);
    // printf("Link end pos: (%lf, %lf)\n", linkEndPos.x, linkEndPos.y);
    return 0;
}


void initArm(int nArgs, ...){
    va_list argList;
    va_start(argList, nArgs);
    
    jointLink *currJL;
    jointLink *prevJL;
    jointLink *baseJL;
    for(int i=0; i<nArgs; i++){
        currJL = va_arg(argList, jointLink*);
        if(i == 0){
            baseJL = currJL;
        }
        else{
            prevJL->child = currJL;
        }
        prevJL = currJL;
    }
    currJL->child = NULL;

    double angles[MAXJL];
    for(int j=0; j<MAXJL; j++){
        angles[j] = 0;
    }
    setJoints(nArgs, angles, baseJL);

    va_end(argList);
}


void setJoints(int nJoints, double angles[MAXJL], jointLink *baseJL){
    //TODO links currently don't move relative to parents
    jointLink *currJL = baseJL;
    coord prevTipPos;
    for(int i=0; i<nJoints; i++){
        if(i != 0){
            currJL->jointPos = prevTipPos;
        }
        currJL->jointAngle = angles[i];
        prevTipPos =getLinkTipPos(currJL);
    }
}

coord getLinkTipPos(jointLink *jl){
    double x = jl->jointPos.x + jl->linkLength * cos(jl->jointAngle);
    double y = jl->jointPos.y + jl->linkLength * sin(jl->jointAngle);
    coord retCoord = {x, y};
    return retCoord;
}