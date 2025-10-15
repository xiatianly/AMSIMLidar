// A&M SIM LLC 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Velodyne/VelodyneBaseComponent.h"
#include "LidarBaseActor.h"
#include "VelodyneLidarActor.generated.h"

/**
 *
 */
UCLASS()
class AMSIMLIDAR_API AVelodyneLidarActor : public ALidarBaseActor
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  AVelodyneLidarActor();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AMSIMLidar")
  class UVelodyneBaseComponent* LidarComponent;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  // Called when the game end
  virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:
  /**
  * Set UDP communication parameters for scan data
  */
  virtual void ConfigureUDPScan() override;

  /**
  * Main routine
  * calculate raytracing and generate LiDAR packet data
  */
  virtual void LidarThreadTick() override;
};
