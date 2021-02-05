#include "../headers/dev.h"

void dev_loop(GameObject *G)
{
  if (G->dev.show_FPS == 1)
    displayFPS(&(G->dev));
}

void displayFPS(GameDev *dev)
{
  dev->currTick = SDL_GetTicks();
  dev->frameDelays[dev->frameDelayIndex] = dev->currTick - dev->prevTick;
  dev->prevTick = dev->currTick;

  if (dev->frameDelayCount < 10)
    dev->frameDelayCount++;

  (dev->frameDelayIndex)++;
  dev->frameDelayIndex %= 10;

  dev->frameDelayAvg = 0;
  for (int i = 0; i < dev->frameDelayCount; i++)
    dev->frameDelayAvg += dev->frameDelays[i];

  dev->frameDelayAvg /= (float)dev->frameDelayCount;

  if (dev->frameDelayAvg != 0)
    dev->FPS = 1000 / dev->frameDelayAvg;
  else
    dev->FPS = dev->FPS_cap;

  ///TODO: render this
  printf("%d FPS\n", dev->FPS);
}

void cap_FPS(GameDev dev)
{
  float frameTime = 1000 / (float)dev.FPS_cap;
  float currFrameDelay = SDL_GetTicks() - dev.currTick;
  if (frameTime > currFrameDelay)
    SDL_Delay(frameTime - currFrameDelay);
}
