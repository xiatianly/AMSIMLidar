#pragma once

#include "LidarThreadBase.h"

class ALidarBaseActor;
/**
 *
 */
class AMSIMLIDAR_API LidarThreadProcess : public LidarThreadBase
{
public:
  typedef LidarThreadBase Super;

  LidarThreadProcess(const FTimespan& InThreadSleepTime, const TCHAR* InThreadName, ALidarBaseActor* InLidarActor)
  : Super(InThreadSleepTime, InThreadName), LidarActor(InLidarActor)
  {}

  virtual void Process() override;

  bool LidarThreadInit();
  void LidarThreadShutdown();

protected:

  ALidarBaseActor* LidarActor = nullptr;
};