#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "./Vector.hpp"
#include <SDL2/SDL.h>
#include "./Constructs.hpp"
#include <vector>
#include <deque>

class QuadNode {

  public:
  QuadNode(Vec2 pos, Vec2 dim, Vec2 minDim);
  QuadNode(QuadNode* parentNode, Vec2 pos, Vec2 dim, Vec2 minDim);

  bool InsertParticle(Particle* particle);
  void InsertParticles(std::vector<Particle>& particles);
  void Draw(SDL_Renderer* renderer, int MAX_X, int MAX_Y, bool drawP);
  void Query(Particle& particle, std::vector<Particle*>& out, double overlapRadius);
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
  bool particleOverlapsNode(Particle& particle, double radius);
  bool insertIntoChildren(Particle* particle);
  void Split();
};

#endif
