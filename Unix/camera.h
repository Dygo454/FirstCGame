#ifndef CAMERA_H
#define CAMERA_H

struct vec2 camPos;
struct vec2 camDimensions;
double camAngle;

void camInit() {
  camPos = newVec2(2839,767);
  double size = 450;
  camAngle = 0;
  camDimensions = newVec2(size,(int) ((double) Height/(double) Width * size));
}

#endif
