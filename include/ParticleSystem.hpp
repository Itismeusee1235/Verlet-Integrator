#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "Vector.hpp"
#include "QuadTree.hpp"
#include <SDL2/SDL.h>
#include <bits/stdc++.h>

const int SUB_STEPS = 8;
const int SOLVER_STEPS = 4;

class ParticleSystem {

  std::vector<Particle> particles;
  int particleId = 0;
  std::vector<Particle*> neighbors;
  std::vector<Rod> rods;

  QuadNode tree;

  Vec2 gravity;
  float timeStep;
  double damping = 0.998;

  double MAX_X;
  double MAX_Y;

  Vec2 boundaryCentre;
  double boundaryRadius;

  int particleRadius = 5;

  public:
  ParticleSystem(double w, double h);
  void TimeStep();
  void Draw(SDL_Renderer* renderer);
  Particle& AddParticle(Vec2 pos, Vec2 vel = ZeroVec);
  void AddRod(int id1, int id2, double length);
  void AddRod(Vec2 pos1, Vec2 pos2, double lenght, Vec2 vel1 = ZeroVec, Vec2 vel2 = ZeroVec);
  void AddTri(Vec2 pos1, Vec2 pos2, Vec2 pos3, double lenght, Vec2 vel1 = ZeroVec, Vec2 vel2 = ZeroVec, Vec2 vel3 = ZeroVec);
  std::vector<Particle> Query(Vec2 pos, double radius);

  private:
  void Verlet(double time);
  void AccumulateForces();
  void ApplyConstraints();

  void BorderConstraint(int index);
  void BoundaryConstraint(int index);
  void CollisionConstraint(int index);
  void RodConstraint();
  void SDL_DrawCircle(SDL_Renderer* renderer, int x, int y, int r);
};

#endif
