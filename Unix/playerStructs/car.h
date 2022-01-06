#ifndef CAR_H
#define CAR_H

struct vec2 carSize;

struct car {
  struct vec2 pos;
  struct vec2 vel;
  struct rigidbody rb;
  double ang;
  int model;
  double maxspeed;//25
  double acc;//15
  double steering;//5
  Uint8 onGrass;
  int boost;
  int boostTime;
  int boxes;
  Uint8 lastDirection;
  Uint8 inBoxPickUp;
  Uint8 inBoxDropOff;
  Uint8 halfway;
  int lap;
};

int renderCar(SDL_Renderer* rend, SDL_Window* win, struct car* toRender) {
  char path[50];
  sprintf(path,"res/carModel%d.png",toRender->model);
  return renderSpriteObjectWithAng(path, rend, win, carSize, toRender->pos, toRender->ang, SDL_FLIP_NONE);
}
int renderSelf(SDL_Renderer* rend, SDL_Window* win, struct car* toRender) {
  char path[50];
  sprintf(path,"res/carModel%d.png",toRender->model);
  SDL_Rect dest;
  dest.w = carSize.x/camDimensions.x*Width;
  dest.h = carSize.y/camDimensions.y*Height;
  dest.x = Width/2-dest.w/2;
  dest.y = Height/2-dest.h/2;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
  SDL_GetRenderDrawColor(rend,&r,&g,&b,&a);
  SDL_SetRenderDrawColor(rend,91,91,91,255);
  char boxCount[50];
  sprintf(boxCount,":");
  renderString(rend, win, boxCount, newVec2(200,25), 20, 0);
  sprintf(boxCount,"%d",toRender->boxes);
  renderString(rend, win, boxCount, newVec2(235,25), 20, 0);
  char lapCount[50];
  sprintf(lapCount,"%d/3",toRender->lap);
  renderString(rend, win, lapCount, newVec2(10,550), 20, 0);
  // char half[50];
  // sprintf(half,"%d",toRender->halfway);
  // renderString(rend, win, half, newVec2(10,500), 10, 0);
  SDL_Rect box;
  box.w = 175;
  box.h = 175;
  box.x = 0;
  box.y = 0;
  renderTextureEx("res/box.png",rend,win,NULL,&box,0,NULL,SDL_FLIP_NONE);
  if (toRender->boost > 0) {
    SDL_Rect starOverlay;
    starOverlay.w = 175;
    starOverlay.h = 175*toRender->boost/toRender->boostTime;
    starOverlay.x = 350;
    starOverlay.y = 175-starOverlay.h;
    SDL_Rect srcStar;
    srcStar.w = 100;
    srcStar.h = 100*toRender->boost/toRender->boostTime;
    srcStar.x = 0;
    srcStar.y = 100-srcStar.h;
    renderTextureEx("res/star.png",rend,win,&srcStar,&starOverlay,0,NULL,SDL_FLIP_NONE);
  }
  SDL_SetRenderDrawColor(rend,r,g,b,a);
  SDL_Rect mapOverlay;
  mapOverlay.w = 200;
  mapOverlay.h = 200;
  mapOverlay.x = 1070;
  mapOverlay.y = 10;
  int mapOverlayCheck = renderTextureEx("res/mapTransparent.png", rend, win, NULL, &mapOverlay, 0, NULL, SDL_FLIP_NONE);
  if (mapOverlayCheck != 0) {
    return mapOverlayCheck;
  }
  int x = 200*toRender->pos.x/(1080*6) + 1170;
  int y = -200*toRender->pos.y/(720*6) + 110;
  SDL_Rect myPos;
  myPos.w = 5;
  myPos.h = 5;
  myPos.x = x;
  myPos.y = y;
  int myPosCheck = renderTextureEx(path, rend, win, NULL, &myPos, 0, NULL, SDL_FLIP_NONE);
  if (myPosCheck != 0) {
    return myPosCheck;
  }
  return renderTextureEx(path,rend,win,NULL,&dest,0,NULL,SDL_FLIP_NONE);
}
int tickCar(double horizontalInput, double verticalInput, struct car* toTick) {
  double turnCoef = 3;
  double speedCoef = 8;
  double slippyVal = 7;
  double magnitude = ((toTick->vel.x*toTick->vel.x)+(toTick->vel.y*toTick->vel.y));
  if (toTick->onGrass != 0) {
    speedCoef = 3;
  }
  if (toTick->boost > 0 && state[SDL_SCANCODE_SPACE]) {
    speedCoef = 12;
    toTick->boost -= deltaTime;
  }
  if (camAngle < 0) {
    camAngle += 360;
  }
  if (camAngle > 360) {
    camAngle -= 360;
  }
  if (magnitude > 0) {
    double driftTurnCoef = 3;
    if (verticalInput != 0) {
      driftTurnCoef = 1;
      toTick->lastDirection = verticalInput;
    }
    if (toTick->lastDirection >= 0) {
      camAngle -= horizontalInput * toTick->steering * (((toTick->vel.x*toTick->vel.x)+(toTick->vel.y*toTick->vel.y))/toTick->maxspeed)*deltaTime*turnCoef*driftTurnCoef;
    }
    else {
      camAngle += horizontalInput * toTick->steering * (((toTick->vel.x*toTick->vel.x)+(toTick->vel.y*toTick->vel.y))/toTick->maxspeed)*deltaTime*turnCoef*driftTurnCoef;
    }
  }
  if (magnitude > toTick->maxspeed) {
    toTick->vel.x *= toTick->maxspeed/magnitude;
    toTick->vel.y *= toTick->maxspeed/magnitude;
  }
  struct vec2 newForce = newVec2((sin(camAngle*0.017453292519900000246))*toTick->acc*(verticalInput)*deltaTime,(cos(camAngle*0.017453292519900000246))*toTick->acc*(verticalInput)*deltaTime);
  toTick->vel = addForce(toTick->vel, newForce, 1);
  camPos.x += toTick->vel.x*deltaTime*speedCoef;
  camPos.y += toTick->vel.y*deltaTime*speedCoef;
  toTick->pos.x = camPos.x;
  toTick->pos.y = camPos.y;
  toTick->vel.x /= 1+(deltaTime/slippyVal);
  toTick->vel.y /= 1+(deltaTime/slippyVal);
  toTick->rb.hitBox.points[0] = newVec2(5+toTick->pos.x,11+toTick->pos.y);
  toTick->rb.hitBox.points[1] = newVec2(5+toTick->pos.x,-11+toTick->pos.y);
  toTick->rb.hitBox.points[2] = newVec2(-5+toTick->pos.x,11+toTick->pos.y);
  toTick->rb.hitBox.points[3] = newVec2(-5+toTick->pos.x,-11+toTick->pos.y);

  if (checkCollision(&innerWall,&(toTick->rb)) != 0) {
    camPos.x -= toTick->vel.x*deltaTime*speedCoef;
    camPos.y -= toTick->vel.y*deltaTime*speedCoef;
    toTick->pos.x = camPos.x;
    toTick->pos.y = camPos.y;
    toTick->rb.hitBox.points[0] = newVec2(5+toTick->pos.x,11+toTick->pos.y);
    toTick->rb.hitBox.points[1] = newVec2(5+toTick->pos.x,-11+toTick->pos.y);
    toTick->rb.hitBox.points[2] = newVec2(-5+toTick->pos.x,11+toTick->pos.y);
    toTick->rb.hitBox.points[3] = newVec2(-5+toTick->pos.x,-11+toTick->pos.y);
  }
  if (checkCollision(&outerWall,&(toTick->rb)) != 0) {
    camPos.x -= toTick->vel.x*deltaTime*speedCoef;
    camPos.y -= toTick->vel.y*deltaTime*speedCoef;
    toTick->pos.x = camPos.x;
    toTick->pos.y = camPos.y;
    toTick->rb.hitBox.points[0] = newVec2(5+toTick->pos.x,11+toTick->pos.y);
    toTick->rb.hitBox.points[1] = newVec2(5+toTick->pos.x,-11+toTick->pos.y);
    toTick->rb.hitBox.points[2] = newVec2(-5+toTick->pos.x,11+toTick->pos.y);
    toTick->rb.hitBox.points[3] = newVec2(-5+toTick->pos.x,-11+toTick->pos.y);
  }
  toTick->onGrass = 0;
  if (checkCollision(&outerGrass,&(toTick->rb)) != 0) {
    toTick->onGrass = 1;
  }
  if (checkCollision(&innerGrass,&(toTick->rb)) != 0) {
    toTick->onGrass = 1;
  }
  int patchInd;
  for (patchInd = 0; patchInd < 7; patchInd++) {
    if (checkCollision(&(grassPatches[patchInd]),&(toTick->rb)) != 0) {
      toTick->onGrass = 1;
    }
  }
  int pickupCollision = 0;
  int boxPickUpInd;
  for (boxPickUpInd = 0; boxPickUpInd < 10; boxPickUpInd++) {
    if (checkCollision(&(boxPickUps[boxPickUpInd]),&(toTick->rb)) != 0) {
      pickupCollision = 1;
    }
  }
  if (pickupCollision != 0) {
    if (toTick->inBoxPickUp == 0) {
      toTick->inBoxPickUp = 1;
      toTick->boxes++;
    }
  }
  else {
    toTick->inBoxPickUp = 0;
  }
  int dropoffCollision = 0;
  int boxDropOffInd;
  for (boxDropOffInd = 0; boxDropOffInd < 9; boxDropOffInd++) {
    if (checkCollision(&(boxDropOffs[boxDropOffInd]),&(toTick->rb)) != 0) {
      dropoffCollision = 1;
    }
  }
  if (dropoffCollision != 0) {
    if (toTick->inBoxDropOff == 0 && toTick->boxes > 0) {
      toTick->inBoxDropOff = 1;
      toTick->boxes--;
      toTick->boost = toTick->boostTime;
      //TODO: powerup
    }
  }
  else {
    toTick->inBoxDropOff = 0;
  }
  if (checkCollision(&(checkpoints[0]),&(toTick->rb)) != 0 && toTick->halfway != 0) {
    toTick->lap++;
    toTick->halfway = 0;
    if (toTick->lap==3) {
      winner = toTick->model;
      return 0;
    }
  }
  if (checkCollision(&(checkpoints[1]),&(toTick->rb)) != 0 && toTick->halfway == 0) {
    toTick->halfway = 1;
  }

  // camPos.x -= horizontalInput*deltaTime*50;
  // camPos.y += verticalInput*deltaTime*50;
  // toTick->pos.x = camPos.x;
  // toTick->pos.y = camPos.y;
  // toTick->rb.hitBox.points[0] = newVec2(5+toTick->pos.x,11+toTick->pos.y);
  // toTick->rb.hitBox.points[1] = newVec2(5+toTick->pos.x,-11+toTick->pos.y);
  // toTick->rb.hitBox.points[2] = newVec2(-5+toTick->pos.x,11+toTick->pos.y);
  // toTick->rb.hitBox.points[3] = newVec2(-5+toTick->pos.x,-11+toTick->pos.y);

  return 0;
}
#endif
