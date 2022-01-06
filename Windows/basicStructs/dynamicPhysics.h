#ifndef DYNAMICPHYSICS_H
#define DYNAMICPHYSICS_H

struct vec2 addForce(struct vec2 originalVel, struct vec2 appliedForce, double mass) {
  return newVec2(originalVel.x+appliedForce.x/mass,originalVel.y+appliedForce.y/mass);
}

struct colider {
  int size;
  struct vec2 points[500];
};

struct rigidbody {
  struct colider hitBox;
  Uint8 isClosed;
  Uint8 isTrigger;
  Uint8 isFixed;
  Uint8 inverse;
};

Uint8 checkCollision(struct rigidbody* rb1, struct rigidbody* rb2) {
  struct rigidbody* checker;
  struct rigidbody* checkee;
  if (rb1->isClosed != 0) {
    checker = rb1;
    checkee = rb2;
  }
  else {
    checker = rb2;
    checkee = rb1;
  }
  int ind;
  for (ind = 0; ind < checkee->hitBox.size; ind++) {
    double testx = checkee->hitBox.points[ind].x;
    double testy = checkee->hitBox.points[ind].y;
    int i;
    int j;
    int c = 0;
    for (i = 0, j = checker->hitBox.size-1; i < checker->hitBox.size; j = i++) {
      double projectedX = (checker->hitBox.points[i].x-checker->hitBox.points[j].x)/(checker->hitBox.points[i].y-checker->hitBox.points[j].y);
      projectedX *= (checker->hitBox.points[i].y-testy);
      projectedX += testx;
      if (((checker->hitBox.points[i].y>testy) != (checker->hitBox.points[j].y>testy)) && (projectedX < checker->hitBox.points[i].x)) {
        c++;
      }
    }
    if ((c%2 != 0) ^ checker->inverse) {
      return 1;
    }
  }
  return 0;
}

#endif
