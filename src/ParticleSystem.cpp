#include "../include/ParticleSystem.hpp"
#include "../include/Vector.hpp"
#include <SDL2/SDL.h>
#include <cmath>
#include <random>
#include <iostream>

std::random_device rd;
std::mt19937 gen;
std::uniform_real_distribution<float> Pos(0, 10000);

ParticleSystem::ParticleSystem(double w, double h)
{

  MAX_X = w;
  MAX_Y = h;

  boundaryCentre.x = w / 2;
  boundaryCentre.y = h / 2;

  boundaryRadius = 300;

  gravity = Vec2(0, -500);
  timeStep = 0.016;

  particle = (Vec2*)malloc(0);
  old_particle = (Vec2*)malloc(0);
  particle_a = (Vec2*)malloc(0);
}

void ParticleSystem::AddParticle(Vec2 pos, Vec2 vel)
{
  Vec2* temp;
  int make_size = (NUM_PARTICLES + 1) * sizeof(Vec2);
  int cpy_size = (NUM_PARTICLES) * sizeof(Vec2);

  temp = (Vec2*)malloc(make_size);
  memcpy(temp, particle, cpy_size);
  free(particle);
  particle = temp;

  temp = (Vec2*)malloc(make_size);
  memcpy(temp, old_particle, cpy_size);
  free(old_particle);
  old_particle = temp;

  temp = (Vec2*)malloc(make_size);
  memcpy(temp, particle_a, cpy_size);
  free(particle_a);
  particle_a = temp;

  particle[NUM_PARTICLES] = pos;
  old_particle[NUM_PARTICLES] = pos - vel;
  particle_a[NUM_PARTICLES] = ZeroVec;
  NUM_PARTICLES++;
}

void ParticleSystem::Verlet(double time)
{
  for (int i = 0; i < NUM_PARTICLES; i++) {
    Vec2& x = particle[i];
    Vec2 temp = x;
    Vec2& oldx = old_particle[i];
    Vec2& a = particle_a[i];
    x += (x - oldx) * damping + a * time * time;
    oldx = temp;
  }
}

void ParticleSystem::AccumulateForces()
{
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particle_a[i] = gravity;

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
  for (int i = 0; i < NUM_PARTICLES; i++) {
    BoundaryConstraint(i);
    CollisionConstraint(i);
    // BorderConstraint(i);
  }
}

void ParticleSystem::BoundaryConstraint(int i)
{
  Vec2 diff = boundaryCentre - particle[i];
  if (diff.norm() + particleRadius > boundaryRadius) {
    Vec2 dir = (particle[i] - boundaryCentre).unit();
    float penetration = (particle[i] - boundaryCentre).norm() + particleRadius - boundaryRadius;
    particle[i] -= dir * penetration;
  }
}

void ParticleSystem::BorderConstraint(int i)
{
  double overlap, vel;
  if (particle[i].y - particleRadius < 0) {
    overlap = 0 - particle[i].y;
    vel = particle[i].y - old_particle[i].y;

    particle[i].y = 0 + overlap;
    old_particle[i].y = particle[i].y + vel;

  } else if (particle[i].y - particleRadius > MAX_Y) {

    overlap = MAX_Y - particle[i].y;
    vel = particle[i].y - old_particle[i].y;

    particle[i].y = MAX_Y + overlap;
    old_particle[i].y = particle[i].y + vel;
  }

  if (particle[i].x - particleRadius < 0) {

    overlap = 0 - particle[i].x;
    vel = particle[i].x - old_particle[i].x;

    particle[i].x = 0 + overlap;
    old_particle[i].x = particle[i].x + vel;

  } else if (particle[i].x - particleRadius > MAX_X) {
    overlap = MAX_X - particle[i].x;
    vel = particle[i].x - old_particle[i].x;

    particle[i].x = MAX_X + overlap;
    old_particle[i].x = particle[i].x + vel;
  }
}

void ParticleSystem::CollisionConstraint(int j)
{
  double penetration, mag, r_sum;
  for (int i = j + 1; i < NUM_PARTICLES; i++) {

    Vec2 dir = particle[i] - particle[j];
    mag = dir.norm();
    r_sum = 2 * particleRadius;

    if (mag < r_sum && mag > 1e-6) {
      penetration = r_sum - mag;
      Vec2 shift = dir.unit() * penetration / 2;
      particle[i] += shift;
      // old_particle[i] += shift;
      particle[j] -= shift;
      // old_particle[j] -= shift;
    }
  }
}

void ParticleSystem::TimeStep()
{
  // particle[0].print();

  double time = timeStep / SUB_STEPS;
  for (int i = 0; i < SUB_STEPS; i++) {
    AccumulateForces();
    Verlet(time);
    for (int j = 0; j < SOLVER_STEPS; j++) {

      ApplyConstraints();
    }
  }
}

void ParticleSystem::Draw(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 155, 155, 155, 150);
  SDL_DrawCircle(renderer, boundaryCentre.x, boundaryCentre.y, boundaryRadius);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (int i = 0; i < NUM_PARTICLES; i++) {
    SDL_DrawCircle(renderer, particle[i].x, MAX_Y - particle[i].y, particleRadius);
  }
}

void ParticleSystem::SDL_DrawCircle(SDL_Renderer* renderer, int x, int y, int r)
{
  for (int i = -r; i <= r; i++) {
    double end = sqrt(r * r - i * i);
    SDL_RenderDrawLine(renderer, x + i, y + end, x + i, y - end);
  }
}
