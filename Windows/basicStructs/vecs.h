#ifndef VECS_H
#define VECS_H

struct vec2 {
  double x;
  double y;
};

struct vec3 {
  double x;
  double y;
  double z;
};

struct vec2 newVec2(double x, double y) {
  struct vec2 ret;
  ret.x = x;
  ret.y = y;
  return ret;
}

#endif
