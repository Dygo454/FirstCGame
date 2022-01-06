#ifndef STRINGRENDERER_H
#define STRINGRENDERER_H

//TODO: THIS IS NEXT
struct vec2 posFromChar(char c) {
  char lowerVersion = tolower(c);
  int ascii = (int) lowerVersion;
  int x = 0;
  int y = 0;
  if (ascii >= 97 && ascii <= 122) {
    int num = (ascii-97);
    x = num%9;
    y = num/9;
  }
  else if (ascii >= 48 && ascii <= 57) {
    x = ascii-49;
    y = 3;
    if (x == -1) {
      return posFromChar('o');
    }
  }
  else if (ascii == 33) {
    x = 8;
    y = 2;
  }
  else if (ascii == 46) {
    return newVec2(41,10);
  }
  else if (ascii == 63) {
    return newVec2(42,10);
  }
  else {
    return newVec2(-1,-1);
  }
  return newVec2(x*5,y*5);
}
struct vec2 sizeFromChar(char c) {
  char lowerVersion = tolower(c);
  int ascii = (int) lowerVersion;
  if ((ascii >= 97 && ascii <= 122) || (ascii >= 48 && ascii <= 57)) {
    return newVec2(5,5);
  }
  else if (ascii == 33 || ascii == 46) {
    return newVec2(1,5);
  }
  else if (ascii == 63) {
    return newVec2(3,5);
  }
  return newVec2(-1,-1);
}

int renderChar(SDL_Renderer* rend, SDL_Window* win, char c, struct vec2 pos, double scale) {
  if (c == ':') {
    int check1 = renderChar(rend,win,'.',pos,scale);
    if (check1 != 0) {
      return check1;
    }
    pos.y -= 2*scale;
    int check2 = renderChar(rend,win,'.',pos,scale);
    if (check2 != 0) {
      return check2;
    }
  }
  if (c == '-') {
    pos.y -= 2*scale;
    int check1 = renderChar(rend,win,'.',pos,scale);
    if (check1 != 0) {
      return check1;
    }
    pos.x += scale;
    int check2 = renderChar(rend,win,'.',pos,scale);
    if (check2 != 0) {
      return check2;
    }
  }
  if (c == ',') {
    int check1 = renderChar(rend,win,'.',pos,scale);
    if (check1 != 0) {
      return check1;
    }
    pos.y += scale;
    int check2 = renderChar(rend,win,'.',pos,scale);
    if (check2 != 0) {
      return check2;
    }
  }
  if (c == '*') {
    pos.y -= 2*scale*5.5;
    int check2 = renderChar(rend,win,'.',pos,scale*3);
    if (check2 != 0) {
      return check2;
    }
  }
  if (c == '/') {
    pos.x -= scale*1.5;
    int check1 = renderChar(rend,win,'.',pos,scale);
    if (check1 != 0) {
      return check1;
    }
    pos.y -= scale;
    pos.x += scale;
    int check2 = renderChar(rend,win,'.',pos,scale);
    if (check2 != 0) {
      return check2;
    }
    pos.y -= scale;
    pos.x += scale;
    int check3 = renderChar(rend,win,'.',pos,scale);
    if (check3 != 0) {
      return check3;
    }
    pos.y -= scale;
    pos.x += scale;
    int check4 = renderChar(rend,win,'.',pos,scale);
    if (check4 != 0) {
      return check4;
    }
  }
  SDL_Rect src;
  struct vec2 srcPos = posFromChar(c);
  if (srcPos.x == -1) {
    return -1;
  }
  src.x = srcPos.x;
  src.y = srcPos.y;
  struct vec2 srcSize = sizeFromChar(c);
  if (srcSize.x == -1) {
    return -1;
  }
  src.w = srcSize.x;
  src.h = srcSize.y;
  SDL_Rect dst;
  dst.x = pos.x;
  dst.y = pos.y;
  dst.w = srcSize.x*scale;
  dst.h = srcSize.y*scale;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
  SDL_GetRenderDrawColor(rend,&r,&g,&b,&a);
  if (r == 91 && g == 91 && b == 91) {
    return renderTextureEx("res/battleFont/battleFontGrayCaps.png",rend,win,&src,&dst,0,NULL,SDL_FLIP_NONE);
  }
  return renderTextureEx("res/battleFont/battleFontWhiteCaps.png",rend,win,&src,&dst,0,NULL,SDL_FLIP_NONE);
}
int renderString(SDL_Renderer* rend, SDL_Window* win, char * s, struct vec2 pos, double scale, Uint8 alignment) {// 0 = left, 1 = center, 2 = right
  int lineSizes[10];
  int count = 0;
  int lineCount = 0;
  // pos.x += 300;
  int startingX = pos.x;
  char lastChar;
  for (int i = 0; i < strlen(s); i++) {
    if (s[i] != '\n') {
      count++;
    }
    else {
      lineSizes[lineCount] = count;
      lineCount++;
      count = 0;
    }
    lastChar = s[i];
  }
  if (lastChar != '\n') {
    lineSizes[lineCount] = count;
    lineCount++;
    count = 0;
  }
  if (alignment == 1) {
    pos.x -= lineSizes[0]*6*scale/2;
  }
  else if (alignment == 2) {
    pos.x -= lineSizes[0]*6*scale;
  }
  lineCount = 0;
  for (int i = 0; i < strlen(s); i++) {
    if (s[i] == '\n') {
      lineCount++;
      pos.y += 6*scale;
      pos.x = startingX;
      if (alignment == 1) {
        pos.x -= (lineSizes[lineCount]*6*scale)/2;
      }
      if (alignment == 2) {
        pos.x -= (lineSizes[lineCount]*6*scale);
      }
      continue;
    }
    int result = renderChar(rend,win,s[i],pos,scale);
    if (s[i] == '/') {
      pos.x -= 4*scale;
    }
    pos.x += 6*scale;
    if (result != 0 && result != -1) {
      return result;
    }
  }
  return 0;
}

#endif
