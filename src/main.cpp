#include "../include/Vector.hpp"
#include "../include/ParticleSystem.hpp"
#include <SDL2/SDL_keycode.h>
#include <iostream>

using namespace std;

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

  ParticleSystem system(WIDTH, HEIGHT);

  Uint64 last = SDL_GetPerformanceCounter();
  Uint64 now;
  double deltaTime;

  bool exit = false;

  int frameCount = 0;
  bool spawn = false;
  double angle = 0;
  double speed = 1;
  bool selectMode = false;
  vector<int> selectedIds;
  Vec2 pos1;
  Vec2 pos2;

  Particle p1 = system.AddParticle(Vec2(400, 400));
  Particle p2 = system.AddParticle(Vec2(450, 350));
  Particle p3 = system.AddParticle(Vec2(450, 450));
  Particle p4 = system.AddParticle(Vec2(350, 450));
  Particle p5 = system.AddParticle(Vec2(350, 350));

  system.AddRod(p1.id, p2.id, 10);
  system.AddRod(p1.id, p3.id, 10);
  system.AddRod(p1.id, p4.id, 10);
  system.AddRod(p1.id, p5.id, 10);

  system.AddRod(p2.id, p3.id, 100);
  system.AddRod(p2.id, p5.id, 100);

  system.AddRod(p4.id, p5.id, 100);
  system.AddRod(p4.id, p3.id, 100);

  bool run = false;

  while (!exit) {

    now = SDL_GetPerformanceCounter();
    deltaTime = (double)(now - last) / (double)SDL_GetPerformanceFrequency();
    last = now;
    // cout << 1 / deltaTime << " " << SDL_GetPerformanceFrequency() << endl;
    // cout << addMode << endl;
    cout << selectMode << " " << selectedIds.size() << endl;

    int m_X;
    int m_Y;
    SDL_GetMouseState(&m_X, &m_Y);

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
        if (ev.key.keysym.sym == SDLK_c) {
          system.Empty();
        }
        if (ev.key.keysym.sym == SDLK_SPACE) {
          run = !run;
        }
        if (ev.key.keysym.sym == SDLK_b) {
          p1 = system.AddParticle(Vec2(400, 400));
          p2 = system.AddParticle(Vec2(450, 350));
          p3 = system.AddParticle(Vec2(450, 450));
          p4 = system.AddParticle(Vec2(350, 450));
          p5 = system.AddParticle(Vec2(350, 350));

          system.AddRod(p1.id, p2.id, 10);
          system.AddRod(p1.id, p3.id, 10);
          system.AddRod(p1.id, p4.id, 10);
          system.AddRod(p1.id, p5.id, 10);

          system.AddRod(p2.id, p3.id, 100);
          system.AddRod(p2.id, p5.id, 100);

          system.AddRod(p4.id, p5.id, 100);
          system.AddRod(p4.id, p3.id, 100);
        }
        if (ev.key.keysym.sym == SDLK_p) {
          spawn = !spawn;
        }
        if (ev.key.keysym.sym == SDLK_s) {
          selectMode = !selectMode;
        }
        if (ev.key.keysym.sym == SDLK_r) {
          if (selectedIds.size() >= 2) {
            int p1 = selectedIds.back();
            selectedIds.pop_back();
            int p2 = selectedIds.back();
            selectedIds.pop_back();
            system.AddRod(p1, p2, 50);
          }
        }
      } else if (ev.type == SDL_MOUSEBUTTONDOWN) {
        if (selectMode) {
          vector<Particle> hits;

          hits = system.Query(Vec2(m_X, 800 - m_Y), 4);
          for (auto i : hits) {
            cout << "hey" << endl;
            if (find(selectedIds.begin(), selectedIds.end(), i.id) == selectedIds.end()) {
              selectedIds.push_back(i.id);
              break;
            }
          }

        } else {

          system.AddParticle(Vec2(m_X, 800 - m_Y));
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    system.Draw(renderer);

    if (selectMode) {

      SDL_SetRenderDrawColor(renderer, 255, 122, 122, 255);
      SDL_DrawCircle(renderer, m_X, m_Y, 4);
    }

    SDL_RenderPresent(renderer);

    if (run) {
      if (frameCount >= 10 && spawn) {
        Vec2 dir(m_X - 400, 400 - m_Y);

        frameCount = 0;
        system.AddParticle(Vec2(400, 400), dir.normalized() * speed);
        angle += 0.05;
      }
      system.TimeStep();
    }

    frameCount++;
  }
  SDL_Quit();
  return 0;
}
