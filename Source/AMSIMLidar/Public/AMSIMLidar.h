// Copyright (c) 2025 A&M SIM LLC
// SPDX-License-Identifier: MIT
// Licensed under the MIT License; see the LICENSE file for details.

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
