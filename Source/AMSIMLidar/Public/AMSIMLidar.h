// A&M SIM LLC 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAMSIMLidarModule : public IModuleInterface
{
public:

  /** IModuleInterface implementation */
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;
};
