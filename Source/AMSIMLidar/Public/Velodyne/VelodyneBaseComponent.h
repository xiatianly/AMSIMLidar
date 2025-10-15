// A&M SIM LLC 2025 All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/PhysicsInterfaceCore.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Materials/MaterialParameterCollection.h"
#include "VelodyneBaseComponent.generated.h"


class UPhysicalMaterial;
class PacketGenerationTask;

#define FIRING_CYCLE 55296 // nanosecond
#define PACKET_HEADER_SIZE 42
#define DATA_PACKET_PAYLOAD 1206
#define POSITION_PACKET_PAYLOAD 512



UENUM(BlueprintType)
enum EModelName
{
  VLP16        UMETA(DisplayName = "VLP-16"),
  PUCK_LITE    UMETA(DisplayName = "Puck-Lite"),
  PUCK_HIRES   UMETA(DisplayName = "Puck-HiRes"),
  VLP_32C      UMETA(DisplayName = "VLP-32C"),
  VLS_128    UMETA(DisplayName = "VLS-128")
};

UENUM(BlueprintType)
enum EFrequency
{
  SR05 UMETA(DisplayName = "5 Hz"),
  SR10 UMETA(DisplayName = "10 Hz"),
  SR15 UMETA(DisplayName = "15 Hz"),
  SR20 UMETA(DisplayName = "20 Hz")
};

UENUM(BlueprintType)
enum ELaserReturnMode
{
  Strongest  UMETA(DisplayName = "Strongest"),
  //LastReturn UMETA(DisplayName = "Last Return"),
  //DualReturn UMETA(DisplayName = "Dual Return")
};

USTRUCT()
struct FVelodyneLidar
{
  GENERATED_BODY()

public:
  uint8 NumberLaserEmitter;
  uint8 NumberDataBlock;
  uint8 NumberDataChannel;
  uint8 SamplingRate;
  uint8 ReturnMode;
  uint8 ModelNumber;
  float MinRange;
  float MaxRange;
  float AzimuthResolution;
  TArray<float>      ElevationAngle;
  TArray<float>      AzimuthOffset;
  TArray<float>      AzimuthAngle;
  TArray<uint8>      DataPacket;
  TArray<uint8>      PositionPacket;
  TArray<FHitResult> RecordedHits;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AMSIMLIDAR_API UVelodyneBaseComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  UVelodyneBaseComponent();

  UPROPERTY(EditAnywhere, Category="Weather|Rain") bool bEnableRainModel = false;
  UPROPERTY(EditAnywhere, Category="Weather|Rain", meta=(ClampMin="0"))
  float RainRate_mmph = 0.f;               // e.g. 0..80 (mm/h)
  UPROPERTY(EditAnywhere, Category="Weather|Rain") float RainBackscatterCoeff = 0.04f; // tune
  UPROPERTY(EditAnywhere, Category="Weather|Rain") float MaxBackscatterRange_m = 2.0f; // near-sensor sparkles
  UPROPERTY(EditAnywhere, Category="Weather|Rain") float AttenuationScale = 1.0f;      // extra absorption
  UPROPERTY(EditAnywhere, Category="Weather|Rain") float ExtraDropout = 0.0f;          // add'l miss prob 0..0.5

  // Optional: pull rain amount (0..1) from UDS Material Parameter Collection
  UPROPERTY(EditAnywhere, Category="Weather|Rain") UMaterialParameterCollection* UDS_MPC = nullptr;
  UPROPERTY(EditAnywhere, Category="Weather|Rain") FName UDS_RainParam = "RainAmount"; // UDS scalar 0..1
  UPROPERTY(EditAnywhere, Category="Weather|Rain") float UDS_RainToMMph = 80.f;        // map 1.0 → 80 mm/h

// --- Wet road / surface reflectivity ---
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|WetSurface")
bool bEnableWetSurface = false;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|WetSurface", meta=(ClampMin="0", ClampMax="1"))
float Wetness01 = 0.0f;             // 0=dry, 1=fully wet film

// How much to reduce diffuse reflectivity at Wetness01=1 (e.g., 0.6 means 40% darker)
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|WetSurface", meta=(ClampMin="0", ClampMax="1"))
float WetDiffuseScaleAtMax = 0.6f;

// Optional: read wetness from the same MPC you use for rain
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|WetSurface")
UMaterialParameterCollection* Wet_MPC = nullptr;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather|WetSurface")
FName WetParamName = TEXT("RoadWetness01");  // scalar 0..1 in your MPC

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velodyne")
  TEnumAsByte<EModelName> SensorModel;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velodyne")
  TEnumAsByte<EFrequency> SamplingRate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velodyne")
  TEnumAsByte<ELaserReturnMode> ReturnMode;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velodyne")
  FString SensorIP;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velodyne")
  FString DestinationIP;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velodyne")
  int32 ScanPort;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velodyne")
  int32 PositionPort;

  FVelodyneLidar Sensor;

  /**
  * Get scanning data using trace.
  */
  void GetScanData();

  /**
  * Generate velodyne packet data using raycast results.
  *
  * @param TimeStamp current time of game in microseconds
  */
  void GenerateDataPacket(uint32 TimeStamp);

  /**
  * Generate position packet data based on GNSS measurement.
  * !! Not implemented yet !!
  *
  * @param TimeStamp current time of game in microseconds
  */
  void GeneratePositionPacket(uint32 TimeStamp);

  /**
  * Calculate intensity based on physics material reflectivity
  *
  * @param Surface Name of physical material
  * @param Distance Range from sensor origin
  *
  * @return Intensity Object's intensity calculated from surface reflectivity and distance
  */
  uint8 GetIntensity(const FString Surface, const float Distance) const;

  /**
  * Get current location of Actor.
  */
  FVector GetActorLocation();

  /**
  * Get current rotation of Actor.
  */
  FRotator GetActorRotation();

  /**
  * Get current time of game.
  */
  uint32 GetTimestampMicroseconds();

  /**
  * Convert decimal to hexadecimal.
  */
  FString DecToHex(int DecimalNumber);

  /**
  * Convert ASCII to HEX.
  */
  void ASCIItoHEX(FString Ascii, uint8 Hex[]);

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

  // Called when the game end
  virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  void WriteHitsToFile(const FString& FilePath);  // Declaration of the function

private:
  bool SupportMultithread = true;

  void ConfigureVelodyneSensor();
};
