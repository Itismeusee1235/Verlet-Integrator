#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "Vector.hpp"
#include <SDL2/SDL.h>

const int SUB_STEPS = 8;
const int SOLVER_STEPS = 4;

class ParticleSystem {

  int NUM_PARTICLES;

  Vec2* particle;
  Vec2* old_particle;
  Vec2* particle_a;
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
  void AddParticle(Vec2 pos, Vec2 vel = ZeroVec);

  private:
  void Verlet(double time);
  void AccumulateForces();
  void ApplyConstraints();

  void BorderConstraint(int index);
  void BoundaryConstraint(int index);
  void CollisionConstraint(int index);
  void SDL_DrawCircle(SDL_Renderer* renderer, int x, int y, int r);
};

#endif
