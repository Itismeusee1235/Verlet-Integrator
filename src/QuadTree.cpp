#include "../include/QuadTree.hpp"
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <random>

std::random_device color_dev;
std::mt19937 color_gen(color_dev());
std::uniform_int_distribution<uint8_t> color(0, 255);

Particle::Particle(Vec2 pos, Vec2 oldPos)
{
  this->pos = pos;
  this->oldPos = oldPos;
}

QuadNode::QuadNode(Vec2 pos, Vec2 dim, Vec2 minDim)
{
  this->quadPos = pos;
  this->quadDim = dim;
  this->minDim = minDim;

  parentNode = nullptr;
  for (int i = 0; i < 4; i++) {
    children[i] = nullptr;
  }

  // drawColor.r = color(color_gen);
  // drawColor.g = color(color_gen);
  // drawColor.b = color(color_gen);
  // drawColor.a = 255;
  drawColor = GREEN;
  pDrawColor = WHITE;
}

QuadNode::QuadNode(QuadNode* parentNode, Vec2 pos, Vec2 dim, Vec2 minDim)
{
  this->quadPos = pos;
  this->quadDim = dim;
  this->minDim = minDim;
  this->parentNode = parentNode;
  for (int i = 0; i < 4; i++) {
    children[i] = nullptr;
  }

  // drawColor.r = color(color_gen);
  // drawColor.g = color(color_gen);
  // drawColor.b = color(color_gen);
  // drawColor.a = 255;
  drawColor = GREEN;
  pDrawColor = WHITE;
}

QuadNode::~QuadNode()
{
  for (int i = 0; i < 4; i++) {
    if (children[i] != nullptr) {
      delete children[i];
      children[i] = nullptr;
    }
  }
}

void QuadNode::Empty()
{
  for (int i = 0; i < 4; i++) {
    if (children[i] != nullptr) {
      delete children[i];
      children[i] = nullptr;
    }
  }
  isLeaf = true;
  boundaryParticles.clear();
  particles.clear();
}

bool QuadNode::InsertParticle(Particle* particle)
{
  if (!this->particleFullyInNode(*particle)) {
    return false;
  }

  if (children[0] == nullptr) {
    if (particles.size() > MAXOBJECTCOUNT && (quadDim.x / 2 >= minDim.x && quadDim.y / 2 > minDim.y)) {
      quadDim.print();
      // minDim.print();
      // printf("--%d\n", particles.size());
      Split();

      for (int i = 0; i < particles.size(); i++) {
        if (!this->insertIntoChildren(particles[i])) {

          this->boundaryParticles.push_back(particles[i]);
        }
      }
      this->InsertParticle(particle);

      this->particles.clear();

    } else {

      this->particles.push_back(particle);
    }
  } else {
    if (!this->insertIntoChildren(particle)) {
      this->boundaryParticles.push_back(particle);
    }
  }

  return true;
}

void QuadNode::InsertParticles(std::deque<Particle>& inParticles)
{
  for (int i = 0; i < inParticles.size(); i++) {
    this->InsertParticle(&inParticles[i]);
  }
}

bool QuadNode::Query(Particle& particle, std::vector<Particle*>& out)
{
  if (this->particleIntersectsNode(particle)) {
    for (int i = 0; i < particles.size(); i++) {
      out.push_back(particles[i]);
    }
    for (int i = 0; i < boundaryParticles.size(); i++) {
      out.push_back(boundaryParticles[i]);
    }

    if (this->children[0] != nullptr) {
      this->children[0]->Query(particle, out);
      this->children[1]->Query(particle, out);
      this->children[2]->Query(particle, out);
      this->children[3]->Query(particle, out);
    }
  } else {
    return false;
  }
  return true;
}

void QuadNode::Split()
{

  Vec2 newDim = quadDim / 2;
  Vec2 newPos(quadPos.x + quadDim.x / 4, quadPos.y + quadDim.y / 4);
  children[0] = new QuadNode(this, newPos, newDim, minDim);
  newPos.x = quadPos.x - quadDim.x / 4;
  children[1] = new QuadNode(this, newPos, newDim, minDim);
  newPos.y = quadPos.y - quadDim.y / 4;
  children[2] = new QuadNode(this, newPos, newDim, minDim);
  newPos.x = quadPos.x + quadDim.x / 4;
  children[3] = new QuadNode(this, newPos, newDim, minDim);
}

void QuadNode::Draw(SDL_Renderer* renderer, int MAX_X, int MAX_Y, bool drawP)
{
  if (children[0] == nullptr) {
    SDL_Rect rec;

    // rec.w = quadDim.x - 2;
    // rec.h = quadDim.y - 2;
    // rec.x = quadPos.x - quadDim.x / 2 + 1;
    // rec.y = quadPos.y - quadDim.y / 2 + 1;
    rec.w = quadDim.x;
    rec.h = quadDim.y;
    rec.x = quadPos.x - quadDim.x / 2;
    rec.y = quadPos.y - quadDim.y / 2;

    SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
    SDL_RenderDrawRect(renderer, &rec);

    if (drawP) {
      for (int i = 0; i < this->particles.size(); i++) {
        particles[i]->Draw(renderer, pDrawColor);
      }
    }

  } else {

    if (drawP) {
      for (int i = 0; i < this->boundaryParticles.size(); i++) {
        boundaryParticles[i]->Draw(renderer, pDrawColor);
      }
    }
    for (int i = 0; i < 4; i++) {
      children[i]->Draw(renderer, MAX_X, MAX_Y, drawP);
    }
  }
}

bool QuadNode::particleFullyInNode(Particle& particle)
{
  double xMin = quadPos.x - quadDim.x / 2 + particle.particleSize;
  double xMax = quadPos.x + quadDim.x / 2 - particle.particleSize;
  double yMin = quadPos.y - quadDim.y / 2 + particle.particleSize;
  double yMax = quadPos.y + quadDim.y / 2 - particle.particleSize;

  bool xSatisfied = (xMin <= particle.pos.x) && (xMax >= particle.pos.x);
  bool ySatisfied = (yMin <= particle.pos.y) && (yMax >= particle.pos.y);

  return xSatisfied && ySatisfied;
}

bool QuadNode::particleIntersectsNode(Particle& particle)
{
  double xMin = quadPos.x - quadDim.x / 2 - particle.particleSize;
  double xMax = quadPos.x + quadDim.x / 2 + particle.particleSize;
  double yMin = quadPos.y - quadDim.y / 2 - particle.particleSize;
  double yMax = quadPos.y + quadDim.y / 2 + particle.particleSize;

  if (particle.pos.x >= xMin && particle.pos.x <= xMax && particle.pos.y >= yMin && particle.pos.y <= yMax) {
    return true;
  }
  return false;
}
bool QuadNode::insertIntoChildren(Particle* particle)
{
  if (children[0]->InsertParticle(particle)) {
    return true;
  }
  if (children[1]->InsertParticle(particle)) {
    return true;
  }
  if (children[2]->InsertParticle(particle)) {
    return true;
  }
  if (children[3]->InsertParticle(particle)) {
    return true;
  }
  return false;
}
