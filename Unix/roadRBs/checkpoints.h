#ifndef CHECKPOINTS_H
#define CHECKPOINTS_H

struct rigidbody checkpoints[2];

void checkpointsInit() {
  struct colider col0;
  col0.points[0] = newVec2(2969,785);
  col0.points[1] = newVec2(2639,784);
  col0.points[2] = newVec2(2627,797);
  col0.points[3] = newVec2(2969,797);
  col0.size = 4;
  checkpoints[0].hitBox = col0;
  checkpoints[0].isClosed = 1;
  checkpoints[0].isTrigger = 1;
  checkpoints[0].isFixed = 1;
  checkpoints[0].inverse = 0;
  struct colider col1;
  col1.points[0] = newVec2(-2579,-1075);
  col1.points[1] = newVec2(-2962,-1075);
  col1.points[2] = newVec2(-2962,-1087);
  col1.points[3] = newVec2(-2579,-1087);
  col1.size = 4;
  checkpoints[1].hitBox = col1;
  checkpoints[1].isClosed = 1;
  checkpoints[1].isTrigger = 1;
  checkpoints[1].isFixed = 1;
  checkpoints[1].inverse = 0;
}

#endif
