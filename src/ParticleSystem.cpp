#include "../include/ParticleSystem.hpp"
#include "../include/Vector.hpp"
#include <SDL2/SDL.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>

std::random_device rd;
std::mt19937 gen;
std::uniform_real_distribution<float> Pos(0, 10000);

ParticleSystem::ParticleSystem(double w, double h)
    : tree(Vec2(w / 2, h / 2), Vec2(w, h), Vec2(20, 20))
{

  MAX_X = w;
  MAX_Y = h;

  boundaryCentre.x = w / 2;
  boundaryCentre.y = h / 2;

  boundaryRadius = 300;

  gravity = Vec2(0, -500);
  timeStep = 0.016;
}

void ParticleSystem::AddParticle(Vec2 pos, Vec2 vel)
{
  Particle newParticle;
  newParticle.pos = pos;
  newParticle.oldPos = pos - vel;
  newParticle.acc = ZeroVec;
  newParticle.id = particleId;
  newParticle.particleSize = particleRadius;
  particles.push_back(newParticle);
  particleId++;
}

void ParticleSystem::Verlet(double time)
{

  for (int i = 0; i < particles.size(); i++) {
    Vec2& pos = particles[i].pos;
    Vec2 temp = pos;
    Vec2& oldPos = particles[i].oldPos;
    Vec2& acc = particles[i].acc;

    pos += (pos - oldPos) * damping + acc * time * time;
    oldPos = temp;
  }
}

void ParticleSystem::AccumulateForces()
{
  for (int i = 0; i < particles.size(); i++) {
    particles[i].acc = gravity;

    // for (int j = 0; j < NUM_PARTICLES; j++) {
    //   if (i == j) {
    //     continue;
    //   }
    //
    //   Vec2 distance = particle[j] - particle[i];
    //   double mag = distance.norm2();
    //   if (mag > 0 && mag < 300) {
    //     particle_a[i] += distance.unit() * (1000 / (mag + 10));
    //   }
    // }
  }
}

void ParticleSystem::ApplyConstraints()
{
  for (int i = 0; i < particles.size(); i++) {
    BoundaryConstraint(i);
    CollisionConstraint(i);
    // BorderConstraint(i);
  }
}

void ParticleSystem::BoundaryConstraint(int i)
{
  Vec2 pos = particles[i].pos;
  Vec2 diff = boundaryCentre - pos;
  if (diff.norm() + particleRadius > boundaryRadius) {
    Vec2 dir = (pos - boundaryCentre).unit();
    float penetration = (pos - boundaryCentre).norm() + particleRadius - boundaryRadius;
    particles[i].pos -= dir * penetration;
  }
}

void ParticleSystem::BorderConstraint(int i)
{
  double overlap, vel;
  Vec2 pos = particles[i].pos;
  Vec2 old_pos = particles[i].oldPos;

  if (pos.y - particleRadius < 0) {
    overlap = 0 - pos.y;
    vel = pos.y - old_pos.y;

    pos.y = 0 + overlap;
    old_pos.y = pos.y + vel;

  } else if (pos.y - particleRadius > MAX_Y) {

    overlap = MAX_Y - pos.y;
    vel = pos.y - old_pos.y;

    pos.y = MAX_Y + overlap;
    old_pos.y = pos.y + vel;
  }

  if (pos.x - particleRadius < 0) {

    overlap = 0 - pos.x;
    vel = pos.x - old_pos.x;

    pos.x = 0 + overlap;
    old_pos.x = pos.x + vel;

  } else if (pos.x - particleRadius > MAX_X) {
    overlap = MAX_X - pos.x;
    vel = pos.x - old_pos.x;

    pos.x = MAX_X + overlap;
    old_pos.x = pos.x + vel;
  }
}

void ParticleSystem::CollisionConstraint(int i)
{
  double penetration, mag, r_sum;
  neighbors.clear();
  tree.Query(particles[i], neighbors, 2 * particleRadius + 1);
  for (int j = 0; j < neighbors.size(); j++) {

    if (particles[i].id >= neighbors[j]->id) {
      continue;
    }
    Vec2 dir = particles[i].pos - neighbors[j]->pos;
    mag = dir.norm();
    r_sum = 2 * particleRadius;

    if (mag < r_sum && mag > 1e-6) {
      penetration = r_sum - mag;
      Vec2 shift = dir.unit() * penetration / 2;
      particles[i].pos += shift;
      // particles[i].oldPos += shift;
      neighbors[j]->pos -= shift;
      // neighbors[j]->oldPos -= shift;
    }
  }
}

void ParticleSystem::TimeStep()
{
  // particle[0].print();
  std::cout << " << " << particles.size() << std::endl;
  double time = timeStep / SUB_STEPS;
  for (int i = 0; i < SUB_STEPS; i++) {
    AccumulateForces();
    Verlet(time);
    tree.Empty();
    tree.InsertParticles(particles);
    for (int j = 0; j < SOLVER_STEPS; j++) {

      ApplyConstraints();
    }
  }
}

void ParticleSystem::Draw(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 155, 155, 155, 255);
  SDL_DrawCircle(renderer, boundaryCentre.x, boundaryCentre.y, boundaryRadius);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  for (int i = 0; i < particles.size(); i++) {
    particles[i].Draw(renderer, particles[i].pos.x, MAX_Y - particles[i].pos.y);
  }
  tree.Draw(renderer, MAX_X, MAX_Y, false);
}

void ParticleSystem::SDL_DrawCircle(SDL_Renderer* renderer, int x, int y, int r)
{
  for (int i = -r; i <= r; i++) {
    double end = sqrt(r * r - i * i);
    SDL_RenderDrawLine(renderer, x + i, y + end, x + i, y - end);
  }
}
