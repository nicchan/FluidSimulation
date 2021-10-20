#ifndef FLUID_H
#define FLUID_H


int fluidProto();


void swapTextureIDs(GLuint& idA, GLuint& idB);
int IX(int x, int y, int width, int numColorChannel);
float* createBoundaryOffsetData(int fluidTexWidth, int fluidTexHeight, int numColorChannel);


#endif
