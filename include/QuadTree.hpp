#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "./Vector.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <deque>

const SDL_Color WHITE { 255, 255, 255, 255 };
const SDL_Color GREEN { 0, 255, 0, 255 };

struct Particle {
  Vec2 pos;
  Vec2 oldPos;
  Vec2 acc;
  int id;
  double particleSize = 5;

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
};

class QuadNode {

  public:
  QuadNode(Vec2 pos, Vec2 dim, Vec2 minDim);
  QuadNode(QuadNode* parentNode, Vec2 pos, Vec2 dim, Vec2 minDim);

  bool InsertParticle(Particle* particle);
  void InsertParticles(std::deque<Particle>& particles);
  void Draw(SDL_Renderer* renderer, int MAX_X, int MAX_Y, bool drawP);
  bool Query(Particle& particle, std::vector<Particle*>& out);
  void Empty();

  int getObjectCount()
  {
    return particles.size();
  }

  ~QuadNode();
  bool isLeaf = true;

  private:
  Vec2 quadPos;
  Vec2 quadDim;
  Vec2 minDim;

  int objectID = 0;

  SDL_Color drawColor;
  SDL_Color pDrawColor;

  const int MAXOBJECTCOUNT = 4;

  QuadNode* parentNode;
  QuadNode* children[4];

  std::vector<Particle*> particles;
  std::vector<Particle*> boundaryParticles;

  bool particleFullyInNode(Particle& particle);
  bool particleIntersectsNode(Particle& particle);
  bool insertIntoChildren(Particle* particle);
  void Split();
};

#endif
