#include "./Vector.hpp"
#include <SDL2/SDL.h>

const SDL_Color WHITE { 255, 255, 255, 255 };
const SDL_Color GREEN { 0, 255, 0, 255 };

struct Particle {
  Vec2 pos;
  Vec2 oldPos;
  Vec2 normalAccumulator = ZeroVec;
  Vec2 acc;
  int id;
  double particleSize = 5;
  bool collided = false;

  Particle(Vec2 pos = ZeroVec, Vec2 oldPos = ZeroVec);

  void Draw(SDL_Renderer* renderer, SDL_Color color = WHITE)
  {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int x = pos.x;
    int y = pos.y;
    for (int j = -particleSize; j <= particleSize; j++) {
      double end = sqrt(particleSize * particleSize - j * j);
      SDL_RenderDrawLine(renderer, x + j, y + end, x + j, y - end);
    }
  }
  void Draw(SDL_Renderer* renderer, int screenX, int screenY, SDL_Color color = WHITE)
  {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int x = screenX;
    int y = screenY;
    for (int j = -particleSize; j <= particleSize; j++) {
      double end = sqrt(particleSize * particleSize - j * j);
      SDL_RenderDrawLine(renderer, x + j, y + end, x + j, y - end);
    }
  }
};

struct Rod {
  int p1_index;
  int p2_index;
  double length;

  Rod(int _p1, int _p2, double _length)
      : p1_index(_p1)
      , p2_index(_p2)
      , length(_length)
  {
  }
};
