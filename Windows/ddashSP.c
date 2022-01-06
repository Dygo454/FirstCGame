#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<SDL.h>
#include<SDL_timer.h>
#include<SDL_image.h>

//#include <sys/socket.h>
//#include <stdlib.h>
//#include <netinet/in.h>

#define PORT 6969

const Uint8 *state;
double deltaTime = 0;
int mousex = -1;
int mousey = -1;
int winner = -1;
int starting = 0;

#include"settings.h"
#include"basicStructs/vecs.h"
#include"basicStructs/dynamicPhysics.h"
#include"camera.h"
#include"renderHeaders/SpriteHandler.h"
#include"renderHeaders/StringRenderer.h"
#include"roadRBs/innerWall.h"
#include"roadRBs/outerWall.h"
#include"roadRBs/outerGrass.h"
#include"roadRBs/innerGrass.h"
#include"roadRBs/grassPatches.h"
#include"roadRBs/boxPickUps.h"
#include"roadRBs/boxDropOffs.h"
#include"roadRBs/checkpoints.h"
#include"playerStructs/car.h"

struct car testCar;

void init() {
  starting = 1;
  winner = -1;
  camInit();
  innerWallInit();
  outerWallInit();
  outerGrassInit();
  innerGrassInit();
  grassPatchesInit();
  boxPickUpsInit();
  boxDropOffsInit();
  checkpointsInit();
  carSize = newVec2(10,22);//TODO: add scalling for different resolutions
  testCar.pos = newVec2(2839,767);
  testCar.ang = 0;
  testCar.model = 0;
  testCar.maxspeed = 25;
  testCar.acc = 15;
  testCar.steering = 5;
  testCar.onGrass = 0;
  struct colider testcol;
  testcol.size = 4;
  testcol.points[0] = newVec2(5+testCar.pos.x,11+testCar.pos.y);
  testcol.points[1] = newVec2(5+testCar.pos.x,-11+testCar.pos.y);
  testcol.points[2] = newVec2(-5+testCar.pos.x,11+testCar.pos.y);
  testcol.points[3] = newVec2(-5+testCar.pos.x,-11+testCar.pos.y);
  testCar.rb.hitBox = testcol;
  testCar.rb.isClosed = 1;
  testCar.rb.isTrigger = 0;
  testCar.rb.isFixed = 0;
  testCar.rb.inverse = 0;
  testCar.boxes = 0;
  testCar.lastDirection = 0;
  testCar.inBoxPickUp = 0;
  testCar.inBoxDropOff = 0;
  testCar.boost = 0;
  testCar.boostTime = 250;
  testCar.halfway = 0;
  testCar.lap = 0;
}

int tick() {
  //start tick here
  int vIn = (state[SDL_SCANCODE_W]||state[SDL_SCANCODE_UP]) - (state[SDL_SCANCODE_S]||state[SDL_SCANCODE_DOWN]);
  int hIn = (state[SDL_SCANCODE_A]||state[SDL_SCANCODE_LEFT]) - (state[SDL_SCANCODE_D]||state[SDL_SCANCODE_RIGHT]);
  int carTickCheck = tickCar(hIn,vIn,&testCar);
  if (carTickCheck != 0) {
    return carTickCheck;
  }
  //end tick here
  return 0;
}

int render(SDL_Renderer* rend, SDL_Window* win) {
  SDL_SetRenderDrawColor(rend,255,255,255,255);
  SDL_RenderClear(rend);
  SDL_SetRenderDrawColor(rend,0,0,0,255);

  //start of render
  int bgRendCheck = renderSpriteObjectWithAng("res/map.png", rend, win, newVec2(1080*6,720*6), newVec2(0,0), 0, SDL_FLIP_NONE);
  if (bgRendCheck != 0) {
    return bgRendCheck;
  }

  int rendCarCheck = renderSelf(rend,win,&testCar);
  if (rendCarCheck != 0) {
    return rendCarCheck;
  }
  //end of render

  SDL_RenderPresent(rend);
  return 0;
}

int main(int argc, char *argv[]) {
  init();
  Uint32 flag = SDL_INIT_VIDEO|SDL_INIT_TIMER;
  if (SDL_Init(flag) != 0) {
    printf("Error initializing SDL: %s\n",SDL_GetError());
    return 1;
  }
  SDL_Window* win = SDL_CreateWindow("Delivery Dash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width,Height,0);
  if (!win) {
    printf("Error creating window: %s\n",SDL_GetError());
    SDL_Quit();
    return 2;
  }
  //stuff start
  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer* rend = SDL_CreateRenderer(win,-1,render_flags);
  if (!rend) {
    printf("Error creating renderer: %s\n",SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 3;
  }
  SDL_SetRenderDrawBlendMode(rend,SDL_BLENDMODE_BLEND);

  SDL_Event e;
  double lastTime = (double) time(NULL);
  state = SDL_GetKeyboardState(NULL);
  Uint8 running = 1;
  Uint8 mouseDown = 0;
  int frames = 0;
  double start = clock();
  while (running != 0){
    frames++;
    mouseDown = 0;
    if (SDL_PollEvent(&e)){
      switch (e.type) {
        case SDL_QUIT:
          running = 0;
          break;
        case SDL_MOUSEMOTION:
          mousex = e.motion.x;
          mousey = e.motion.y;
          break;
        case SDL_KEYDOWN:
          state = SDL_GetKeyboardState(NULL);
          break;
        case SDL_MOUSEBUTTONDOWN:
          mouseDown = 1;
          break;
        default:
          break;
          //nothing
      }
    }
    if (((double) time(NULL)-lastTime) > 1) {
      frames = 0;
      lastTime = time(NULL);
    }
    deltaTime = ((double) clock() - start)/CLOCKS_PER_SEC*6.5;//TODO: test on other computers cuz 6.5 might not be right :]
    start = clock();

    if (winner == -1) {
      int tickErrorCheck = tick();
      if (tickErrorCheck != 0) {
        printf("Error code: %d\n",tickErrorCheck);
        return tickErrorCheck;
      }
      int rendErrorCheck = render(rend,win);
      if (rendErrorCheck != 0) {
        printf("Error code: %d\n",rendErrorCheck);
        SDL_Delay(500);
        return rendErrorCheck;
      }
    }
    else {
      SDL_SetRenderDrawColor(rend,255,255,255,255);
      SDL_RenderClear(rend);
      SDL_SetRenderDrawColor(rend,0,0,0,255);
      char winnerScreen[50];
      sprintf(winnerScreen,"res/doneBG%d.png",winner);
      renderTexture(winnerScreen,rend,win);
      SDL_RenderPresent(rend);
      if (mouseDown != 0 && mousex >= 64 && mousex <= 327 && mousey >= 592 && mousey <= 664) {
        running = 0;
      }
      if (mouseDown != 0 && mousex >= 830 && mousex <= 1108 && mousey >= 591 && mousey <= 662) {
        running = 0;
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return main(0,NULL);
      }
    }
  }
  //stuff end

  // SDL_DestroyTexture(text);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}