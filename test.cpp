#include <vector>
#include <deque>
#include "./include/QuadTree.hpp"

const int WIDTH = 800;
const int HEIGHT = 800;

int main()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to init SDL %s", SDL_GetError());
    return -1;
  }

  SDL_Window* window = SDL_CreateWindow("Boids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    printf("Error Making window : %s", SDL_GetError());
    return -1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    printf("Failed to maker renderer : %s", SDL_GetError());
    return -1;
  }

  QuadNode tree(Vec2(400, 400), Vec2(800, 800), Vec2(20, 20));
  double r = 5;

  bool exit = false;
  bool pdraw = true;
  std::deque<Particle> particles;
  Particle mouse;
  std::vector<Particle*> outs;

  while (!exit) {

    int m_X;
    int m_Y;
    SDL_GetMouseState(&m_X, &m_Y);
    mouse.pos.x = m_X;
    mouse.pos.y = m_Y;

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        exit = true;
        break;
      } else if (ev.type == SDL_KEYDOWN) {
        if (ev.key.keysym.sym == SDLK_q) {
          exit = true;
          break;
        }
        if (ev.key.keysym.sym == SDLK_BACKSPACE) {
          pdraw = !pdraw;
        }

      } else if (ev.type == SDL_MOUSEBUTTONDOWN) {
        Vec2 newpos(m_X, m_Y);
        Particle p(newpos);
        particles.push_back(p);
        tree.InsertParticle(&particles.back());
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    tree.Empty();
    tree.InsertParticles(particles);
    tree.InsertParticle(&mouse);

    tree.Query(mouse, outs);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    tree.Draw(renderer, WIDTH, HEIGHT, pdraw);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < outs.size(); i++) {
      outs[i]->Draw(renderer, SDL_Color { 255, 0, 0, 255 });
    }

    outs.clear();

    SDL_RenderPresent(renderer);
  }
  SDL_Quit();
  return 0;
}
