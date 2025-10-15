#include "LidarThreadProcess.h"
#include "LidarBaseActor.h"

void LidarThreadProcess::Process()
{
  if(!LidarActor)
  {
    return;
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


  LidarActor->LidarThreadTick();
}

bool LidarThreadProcess::LidarThreadInit()
{
  //Any third party C++ to do on init
  return true;
}

void LidarThreadProcess::LidarThreadShutdown()
{
  //Any third party C++ to do on shutdown
}