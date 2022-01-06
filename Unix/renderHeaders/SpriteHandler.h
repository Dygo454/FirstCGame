#ifndef SPRITEHANDLER_H
#define SPRITEHANDLER_H

int renderTexture(const char* path, SDL_Renderer* rend, SDL_Window* win) {
  SDL_Surface* surface = IMG_Load(path);
  if (!surface) {
    printf("Error loading image: %s\n",SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 4;
  }
  SDL_Texture* text = SDL_CreateTextureFromSurface(rend, surface);
  SDL_FreeSurface(surface);
  if (!surface) {
    printf("Error creating texture: %s\n",SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 5;
  }
  SDL_RenderCopy(rend,text,NULL,NULL);
  SDL_DestroyTexture(text);
  return 0;
}
int renderTextureEx(const char* path, SDL_Renderer* rend, SDL_Window* win, SDL_Rect* srcrect, SDL_Rect* dstrect, double ang, SDL_Point *center, SDL_RendererFlip flip) {
  SDL_Surface* surface = IMG_Load(path);
  if (!surface) {
    printf("Error loading image: %s\n",SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 4;
  }
  SDL_Texture* text = SDL_CreateTextureFromSurface(rend, surface);
  SDL_FreeSurface(surface);
  if (!surface) {
    printf("Error creating texture: %s\n",SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 5;
  }
  SDL_RenderCopyEx(rend,text,srcrect,dstrect,ang,center,flip);
  SDL_DestroyTexture(text);
  return 0;
}
int renderSpriteSheet(const char* path, SDL_Renderer* rend, SDL_Window* win, int width, int height, int x, int y, int swidth, int sheight, int sx, int sy, double ang, SDL_Point *center, SDL_RendererFlip flip) {
  if (width == -1) {
    width = swidth;
  }
  if (height == -1) {
    height = sheight;
  }
  SDL_Rect src;
  src.w = swidth;
  src.h = sheight;
  src.x = sx*swidth;
  src.y = sy*sheight;
  SDL_Rect dest;
  dest.w = width;
  dest.h = height;
  dest.x = x;
  dest.y = y;
  int retVal = renderTextureEx(path, rend, win, &src, &dest, ang, center, flip);
  return retVal;
}
int renderSpriteObject(const char* path, SDL_Renderer* rend, SDL_Window* win, struct vec2 size, struct vec2 pos, SDL_RendererFlip flip) {
  SDL_Rect dest;
  dest.w = size.x/camDimensions.x*Width;
  dest.h = size.y/camDimensions.y*Height;
  dest.x = ((pos.x-size.x/2)-(camPos.x-camDimensions.x/2))/camDimensions.x*Width;
  dest.y = ((camPos.y+camDimensions.y/2)-(pos.y+size.y/2))/camDimensions.y*Height;
  return renderTextureEx(path,rend,win,NULL,&dest,0,NULL,flip);
}
int renderSpriteObjectWithAng(const char* path, SDL_Renderer* rend, SDL_Window* win, struct vec2 size, struct vec2 pos, double ang, SDL_RendererFlip flip) {
  SDL_Rect dest;
  dest.w = size.x/camDimensions.x*Width;
  dest.h = size.y/camDimensions.y*Height;
  double theta = (ang-camAngle)*0.017453292519900000246;
  // int x = ((pos.x-size.x/2)-(camPos.x-camDimensions.x/2));
  // int y = ((camPos.y+camDimensions.y/2)-(pos.y+size.y/2));
  // dest.x = (x*cos((ang-camAngle)*0.017453292519900000246)+y*sin((ang-camAngle)*0.017453292519900000246))/camDimensions.x*Width;
  // dest.y = (x*sin((ang-camAngle)*0.017453292519900000246)+y*cos((ang-camAngle)*0.017453292519900000246))/camDimensions.y*Height;
  // int x = (pos.x-camPos.x)*cos(camAngle)+(pos.y-camPos.y)*sin(ang-camAngle)+camPos.x;
  // int y = (pos.x-camPos.x)*sin(camAngle)+(pos.y-camPos.y)*cos(ang-camAngle)+camPos.y;
  // dest.x = ((pos.x-size.x/2)-(camPos.x-camDimensions.x/2))/camDimensions.x*Width;
  // dest.y = ((camPos.y+camDimensions.y/2)-(pos.y+size.y/2))/camDimensions.y*Height;

  int x = (pos.x-camPos.x)*cos(-theta)-(pos.y-camPos.y)*sin(-theta)+camPos.x;
  int y = (pos.x-camPos.x)*sin(-theta)+(pos.y-camPos.y)*cos(-theta)+camPos.y;
  int inGameX = ((x-size.x/2)-(camPos.x-camDimensions.x/2));
  int inGameY = ((camPos.y+camDimensions.y/2)-(y+size.y/2));
  dest.x = inGameX/camDimensions.x*Width;
  dest.y = inGameY/camDimensions.y*Height;
  return renderTextureEx(path,rend,win,NULL,&dest,ang-camAngle,NULL,flip);
}

#endif
