// Copyright (c) 2025 A&M SIM LLC
// SPDX-License-Identifier: MIT
// Licensed under the MIT License; see the LICENSE file for details.

#include "Velodyne/VelodyneBaseComponent.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"
#include "Materials/MaterialParameterCollectionInstance.h"


// Sets default values for this component's properties
UVelodyneBaseComponent::UVelodyneBaseComponent()
{

  PrimaryComponentTick.bCanEverTick = false;

  // Check Platform supports multithreading
  SupportMultithread = FPlatformProcess::SupportsMultithreading();

  // Set-up initial values
  SensorModel          = EModelName::VLP_32C;
  SamplingRate         = EFrequency::SR10;
  ReturnMode           = ELaserReturnMode::Strongest;
  SensorIP             = FString(TEXT("192.168.1.201"));
  DestinationIP        = FString(TEXT("192.168.1.100"));
  ScanPort             = 2368;
  PositionPort         = 8308;
}


// Called when the game starts
void UVelodyneBaseComponent::BeginPlay()
{
  Super::BeginPlay();

  ConfigureVelodyneSensor();
}


// Called every frame
void UVelodyneBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVelodyneBaseComponent::EndPlay(EEndPlayReason::Type Reason)
{
  Super::EndPlay(Reason);
}

void UVelodyneBaseComponent::ConfigureVelodyneSensor()
{
  switch (SensorModel.GetValue()) {
  case 0:{ // VLP-16
    float Elevation[] = {-15.f, 1.f, -13.f, 3.f, -11.f, 5.f, -9.f, 7.f,
                        -7.f, 9.f, -5.f, 11.f, -3.f, 13.f, -1.f, 15.f};
    Sensor.ElevationAngle.Append(Elevation, UE_ARRAY_COUNT(Elevation));
    Sensor.NumberLaserEmitter = 16;
    Sensor.NumberDataBlock = 12;
    Sensor.NumberDataChannel = 32;
    Sensor.ModelNumber = 34;
    Sensor.MinRange = 20.f;
    Sensor.MaxRange = 10000.f;
    break;
    }
  case 1:{ // PUCK-Lite
    float Elevation[] = {-15.f, 1.f, -13.f, 3.f, -11.f, 5.f, -9.f, 7.f,
                        -7.f, 9.f, -5.f, 11.f, -3.f, 13.f, -1.f, 15.f};
    Sensor.ElevationAngle.Append(Elevation, UE_ARRAY_COUNT(Elevation));
    Sensor.NumberLaserEmitter = 16;
    Sensor.NumberDataBlock = 12;
    Sensor.NumberDataChannel = 32;
    Sensor.ModelNumber = 34;
    Sensor.MinRange = 20.f;
    Sensor.MaxRange = 10000.f;
    break;
    }
  case 2:{ // PUCK-HiRes
    float Elevation[] = {-10.f, 0.67f, -8.67f, 2.f, -7.33f, 3.33f, -6.f, 4.67f,
                        -4.67f, 6.f, -3.33f, 7.33f, -2.f, 8.67f, -0.67f, 10.f};
    Sensor.ElevationAngle.Append(Elevation, UE_ARRAY_COUNT(Elevation));
    Sensor.NumberLaserEmitter = 16;
    Sensor.NumberDataBlock = 12;
    Sensor.NumberDataChannel = 32;
    Sensor.ModelNumber = 36;
    Sensor.MinRange = 20.f;
    Sensor.MaxRange = 10000.f;
    break;
    }
  case 3:{ // VLP-32C
    float Elevation[] = {-25.f, -1.f, -1.667f, -15.639f, -11.31f, 0.f, -0.667f, -8.843f,
                        -7.254f, 0.333f, -0.333f, -6.148f, -5.333f, 1.333f, 0.667f, -4.f,
                        -4.667f, 1.667f, 1.f, -3.667f, -3.333f, 3.333f, 2.333f, -2.667f,
                        -3.f, 7.f, 4.667f, -2.333f, -2.f, 15.f, 10.333f, -1.333f};
    float AzimuthOffset[] = { 1.4f, -4.2f, 1.4f, -1.4f, 1.4f, -1.4f, 4.2f, -1.4f,
                              1.4f, -4.2f, 1.4f, -1.4f, 4.2f, -1.4f, 4.2f, -1.4f,
                              1.4f, -4.2f, 1.4f, -4.2f, 4.2f, -1.4f, 1.4f, -1.4f,
                              1.4f, -1.4f, 1.4f, -4.2f, 4.2f, -1.4f, 1.4f, -1.4f};
    Sensor.ElevationAngle.Append(Elevation, UE_ARRAY_COUNT(Elevation));
    Sensor.AzimuthOffset.Append(AzimuthOffset, UE_ARRAY_COUNT(AzimuthOffset));
    Sensor.NumberLaserEmitter = 32;
    Sensor.NumberDataBlock = 12;
    Sensor.NumberDataChannel = 32;
    Sensor.ModelNumber = 40;
    Sensor.MinRange = 50.f;
    Sensor.MaxRange = 10000.f;
    break;
    }

  case 4:{ // VLS_128
    float Elevation[] ={-11.742f,  -1.99f,    3.4f,   -5.29f,  -0.78f,   4.61f,   -4.08f,    1.31f,
                         -6.5f,    -1.11f,    4.28f,  -4.41f,   0.1f,    6.48f,   -3.2f,     2.19f,
                         -3.86f,    1.53f,   -9.244f, -1.77f,   2.74f,  -5.95f,   -0.56f,    4.83f,
                         -2.98f,    2.41f,   -6.28f,  -0.89f,   3.62f,  -5.07f,    0.32f,    7.58f,
                         -0.34f,    5.18f,   -3.64f,   1.75f, -25.0f,   -2.43f,    2.96f,   -5.73f,
                          0.54f,    9.7f,    -2.76f,   2.63f,  -7.65f,  -1.55f,    3.84f,   -4.85f,
                          3.18f,   -5.51f,   -0.12f,   5.73f,  -4.3f,    1.09f,  -16.042f,  -2.21f,
                          4.06f,   -4.63f,    0.76f,  15.0f,   -3.42f,   1.97f,   -6.85f,   -1.33f,
                         -5.62f,   -0.23f,    5.43f,  -3.53f,   0.98f, -19.582f,  -2.32f,    3.07f,
                         -4.74f,    0.65f,   11.75f,  -2.65f,   1.86f,  -7.15f,   -1.44f,    3.95f,
                         -2.1f,     3.29f,   -5.4f,   -0.01f,   4.5f,   -4.19f,    1.2f,   -13.565f,
                         -1.22f,    4.17f,   -4.52f,   0.87f,   6.08f,  -3.31f,    2.08f,   -6.65f,
                          1.42f,  -10.346f,  -1.88f,   3.51f,  -6.06f,  -0.67f,    4.72f,   -3.97f,
                          2.3f,    -6.39f,   -1.0f,    4.39f,  -5.18f,   0.21f,    6.98f,   -3.09f,
                          4.98f,   -3.75f,    1.64f,   -8.352f,-2.54f,   2.85f,   -5.84f,   -0.45f,
                          8.43f,   -2.87f,    2.52f,   -6.17f, -1.66f,   3.73f,   -4.96f,    0.43f };

    float AzimuthOffset[] = {-6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f,
                             -6.354f, -4.548f, -2.732f, -0.911f, 0.911f, 2.732f, 4.548f, 6.354f};              

    Sensor.ElevationAngle.Append(Elevation, UE_ARRAY_COUNT(Elevation));
    Sensor.AzimuthOffset.Append(AzimuthOffset, UE_ARRAY_COUNT(AzimuthOffset));
    Sensor.NumberLaserEmitter = 128;
    Sensor.NumberDataBlock = 12;
    Sensor.NumberDataChannel = 32;
    Sensor.ModelNumber = 161;
    Sensor.MinRange = 20.f;
    Sensor.MaxRange = 10000.f;
    break;
    }

  }

  switch (SamplingRate.GetValue()) {
  case 0:
    Sensor.SamplingRate = 5;
    break;
  case 1:
    Sensor.SamplingRate = 10;
    break;
  case 2:
    Sensor.SamplingRate = 15;
    break;
  case 3:
    Sensor.SamplingRate = 20;
    break;
  default:
    Sensor.SamplingRate = 10;
    break;
  }

  switch (ReturnMode.GetValue()) {
  case 0:
    Sensor.ReturnMode = 55;
    break;
  case 1: // Last Return : Not implemented
    Sensor.ReturnMode = 56;
    break;
  case 2: // Dual Return : Not implemented
    Sensor.ReturnMode = 57;
    break;
  }

  Sensor.AzimuthResolution = (Sensor.SamplingRate * 0.02);

  // Initialize raycast vector and azimuth vector
  Sensor.AzimuthAngle.Init(90.f, Sensor.NumberDataBlock * Sensor.NumberDataChannel);

  // Initialize packet vector
  Sensor.DataPacket.AddUninitialized(DATA_PACKET_PAYLOAD);
  Sensor.PositionPacket.AddUninitialized(POSITION_PACKET_PAYLOAD);
}

uint8 UVelodyneBaseComponent::GetIntensity(const FString Surface, const float Distance) const
{

  // Rain backscatter override (near-sensor weak hits)
  if (bEnableRainModel
      && RainRate_mmph > 0.f
      && Distance <= (MaxBackscatterRange_m * 100.f)   // m -> cm
      && !Surface.Contains(TEXT("PM_Reflectivity_"), ESearchCase::CaseSensitive))
  {
    // Weak, noisy return
    return static_cast<uint8>(10 + 30 * FMath::FRand());
  }
  
  uint8 MaxReflectivity = 0;
  uint8 MinReflectivity = 0;

  if (Surface.Contains(TEXT("PM_Reflectivity_"), ESearchCase::CaseSensitive)) {
    
    MaxReflectivity = (uint8)FCString::Atoi(*Surface.RightChop(16));
    if(MaxReflectivity > 100)
    {
      MinReflectivity = 101;
    }
  }
  else { 
    MaxReflectivity = 20;
  }


  uint8 BaseI = static_cast<uint8>(
      ((MinReflectivity - MaxReflectivity) / (Sensor.MaxRange - Sensor.MinRange)) * Distance
      + MaxReflectivity);

  // --- Wet-road attenuation (diffuse gets darker when wet) ---
  if (bEnableWetSurface && Wetness01 > 0.f)
  {
   
    bool bIsRoad = Surface.Contains(TEXT("Road")) || Surface.Contains(TEXT("Asphalt")) || Surface.Contains(TEXT("Concrete"));
    if (!bIsRoad) { /* comment this line if the global wetness is needed */ }

    const float scale = 1.f - Wetness01 * (1.f - WetDiffuseScaleAtMax);
    const int32 WetI = FMath::RoundToInt(BaseI * scale);
    BaseI = static_cast<uint8>(FMath::Clamp(WetI, 0, 255));
  }

  return BaseI;

}

void UVelodyneBaseComponent::GetScanData()
{
  if (bEnableRainModel && UDS_MPC)
{
    if (UMaterialParameterCollectionInstance* Inst = GetWorld()->GetParameterCollectionInstance(UDS_MPC))
    {
        float Rain01 = 0.f;
        Inst->GetScalarParameterValue(UDS_RainParam, Rain01);
        RainRate_mmph = FMath::Clamp(Rain01, 0.f, 1.f) * UDS_RainToMMph;
    }
}


if (bEnableWetSurface && Wet_MPC)
{
    if (auto* Inst = GetWorld()->GetParameterCollectionInstance(Wet_MPC))
    {
        float WetVal = 0.f;
        Inst->GetScalarParameterValue(WetParamName, WetVal);
        Wetness01 = FMath::Clamp(WetVal, 0.f, 1.f);
    }
}


  // complex collisions: true
  FCollisionQueryParams TraceParams = FCollisionQueryParams(TEXT("LaserTrace"), true, GetOwner());
  TraceParams.bReturnPhysicalMaterial = true;
  TraceParams.bTraceComplex = true;

  // Get owner's location and rotation
  FVector LidarPosition = this->GetActorLocation();
  FRotator LidarRotation = this->GetActorRotation();

  // Initialize array for raycast result
  Sensor.RecordedHits.Init(FHitResult(ForceInit), Sensor.NumberDataBlock * Sensor.NumberDataChannel);
    // Use a stable per-packet azimuth base (no accumulation)
  static float AzimuthBaseDeg = 90.f;
  const float BaseAz = AzimuthBaseDeg;
  if (Sensor.AzimuthAngle.Num() != Sensor.NumberDataBlock * Sensor.NumberDataChannel)
    Sensor.AzimuthAngle.SetNum(Sensor.NumberDataBlock * Sensor.NumberDataChannel);


  // Calculate batch size for 'ParallelFor' based on workable thread
  const int ThreadNum = FPlatformMisc::NumberOfWorkerThreadsToSpawn();
  const int DivideEnd = FMath::FloorToInt((float)(Sensor.RecordedHits.Num() / ThreadNum));

  ParallelFor
  (
    ThreadNum,
    [&](int32 PFIndex)
    {
      int StartAt = PFIndex * DivideEnd;
      if (StartAt >= Sensor.RecordedHits.Num()) {
        return;
      }

      int EndAt = StartAt + DivideEnd;
      if (PFIndex == (ThreadNum - 1)) {
        EndAt = Sensor.RecordedHits.Num();
      }

      for (int32 Index = StartAt; Index < EndAt; ++Index)
      {

        const int blockIdx = (Index / Sensor.NumberDataChannel);
        const int blocksPerAz = (Sensor.ModelNumber == 161 ? 4 : 1);
        const int azStepIdx   = blockIdx / blocksPerAz;
        const float HAngle      = float(azStepIdx) * Sensor.AzimuthResolution;

        const float VAngle = (float)Sensor.ElevationAngle[Index % Sensor.NumberLaserEmitter];

        Sensor.AzimuthAngle[Index] = FMath::Fmod(BaseAz + HAngle + 360.f, 360.f);

        FRotator LaserRotation(0.f, 0.f, 0.f);
        switch(Sensor.ModelNumber) {
        case 40: // VLP-32C
          LaserRotation.Add(VAngle, Sensor.AzimuthAngle[Index] + Sensor.AzimuthOffset[Index % Sensor.NumberLaserEmitter], 0.f);
          break;
        case 161: // VLS-128
          LaserRotation.Add(VAngle, Sensor.AzimuthAngle[Index] + Sensor.AzimuthOffset[Index % Sensor.NumberLaserEmitter], 0.f);
          break;
        default:
          LaserRotation.Add(VAngle, Sensor.AzimuthAngle[Index], 0.f);
          break;
        }
        FRotator Rotation = UKismetMathLibrary::ComposeRotators(LaserRotation, LidarRotation);

        FVector BeginPoint = LidarPosition + Sensor.MinRange * UKismetMathLibrary::GetForwardVector(Rotation);
        FVector EndPoint = LidarPosition + Sensor.MaxRange * UKismetMathLibrary::GetForwardVector(Rotation);

        GetWorld()->LineTraceSingleByChannel(
            Sensor.RecordedHits[Index], BeginPoint, EndPoint, ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);       

        if (bEnableRainModel && RainRate_mmph > 0.f)
        {
          // Units: UE distance is cm. Convert when modeling physics in meters.
          const float PathLen_m = (EndPoint - BeginPoint).Size() * 0.01f;
          const float lambda    = RainBackscatterCoeff * (RainRate_mmph / 50.f); // tune vs. 50 mm/h baseline

          // --- Attenuate/drop real surface hit ---
          if (Sensor.RecordedHits[Index].bBlockingHit)
          {
            const float D_m = Sensor.RecordedHits[Index].Distance * 0.01f; // cm -> m
            const float atten = FMath::Exp(-lambda * D_m * AttenuationScale);
            // Drop with probability (1 - atten), plus any extra dropout.
            const float keepProb = FMath::Clamp(atten * (1.f - ExtraDropout), 0.f, 1.f);
            if (FMath::FRand() > keepProb)
            {
             Sensor.RecordedHits[Index].Init();            // clears to "no hit"
              Sensor.RecordedHits[Index].bBlockingHit = false;
            }
          }

          // --- Near-sensor backscatter (raindrop false hit) ---
          // Probability a drop intersects the path
          const float pBackscatter = 1.f - FMath::Exp(-lambda * PathLen_m);
          if (FMath::FRand() < pBackscatter)
          {
            // Sample a distance biased near the sensor (exponential)
            const float u = FMath::FRand();
            const float t_m = FMath::Clamp(-FMath::Loge(FMath::Max(1e-6f, 1.f - u)) / FMath::Max(1e-4f, lambda),
                                       0.2f, MaxBackscatterRange_m);
            const float d_cm = t_m * 100.f;

            // If there was a farther surface, keep the closer "rain" as strongest
            const bool hadHit = Sensor.RecordedHits[Index].bBlockingHit;
            const float oldD  = hadHit ? Sensor.RecordedHits[Index].Distance : (PathLen_m * 100.f);

            if (!hadHit || d_cm < oldD)
            {
              // Build a synthetic hit at distance d_cm along the ray
              Sensor.RecordedHits[Index].bBlockingHit = true;
              Sensor.RecordedHits[Index].Distance     = d_cm;   // in cm
              Sensor.RecordedHits[Index].ImpactPoint  = BeginPoint + (EndPoint - BeginPoint).GetSafeNormal() * d_cm;
              Sensor.RecordedHits[Index].PhysMaterial = nullptr; // we’ll handle intensity for "rain" below

            }
          }
        }
      }
    },
    !SupportMultithread
  );

  // advance azimuth base cursor for next packet
  {
    const int blocksPerAz = (Sensor.ModelNumber == 161 ? 4 : 1);
    const int steps = Sensor.NumberDataBlock / blocksPerAz; // VLS-128: 12/4=3; others: 12
    AzimuthBaseDeg = FMath::Fmod(BaseAz + steps * Sensor.AzimuthResolution, 360.f);
  }
}

FVector UVelodyneBaseComponent::GetActorLocation()
{
  return GetOwner()->GetActorLocation();
}

FRotator UVelodyneBaseComponent::GetActorRotation()
{
  return GetOwner()->GetActorRotation();
}

uint32 UVelodyneBaseComponent::GetTimestampMicroseconds()
{
  return (uint32)(fmod(GetWorld()->GetTimeSeconds(), 3600.f) * 1000000); // sec -> microsec
}

void UVelodyneBaseComponent::GenerateDataPacket(uint32 TimeStamp)
{

  uint8 DataFlag[2];
  uint8 AzimuthData[2];
  uint8 DistanceData[2];
  uint8 IntensityData[1];
  uint8 TailData[6];

  int32 PacketIndex = 0;
  FString OutputString; // String to store data for writing to a file

  for(int32 Index = 0; Index < Sensor.RecordedHits.Num(); Index++){

    if(Index % (Sensor.NumberDataChannel) == 0){
      // Add data flag (VLS-128 needs EE/DD/CC/BB cycle)
      const int blockIdx = (Index / Sensor.NumberDataChannel);
      if (Sensor.ModelNumber == 161) {
        switch (blockIdx % 4) {
          case 0: DataFlag[0]=0xFF; DataFlag[1]=0xEE; break;
          case 1: DataFlag[0]=0xFF; DataFlag[1]=0xDD; break;
          case 2: DataFlag[0]=0xFF; DataFlag[1]=0xCC; break;
          default:DataFlag[0]=0xFF; DataFlag[1]=0xBB; break;
        }
      } else {
        DataFlag[0] = 0xFF; DataFlag[1] = 0xEE;
      }
      FMemory::Memcpy(Sensor.DataPacket.GetData() + PacketIndex, DataFlag, UE_ARRAY_COUNT(DataFlag));
      PacketIndex += UE_ARRAY_COUNT(DataFlag);

      // Azimuth data
      uint16 Azimuth = (uint16)(Sensor.AzimuthAngle[Index] * 100);
      AzimuthData[0] = Azimuth & 0x00FF;
      AzimuthData[1] = (Azimuth & 0xFF00) >> 8;

      FMemory::Memcpy(Sensor.DataPacket.GetData() + PacketIndex, AzimuthData, UE_ARRAY_COUNT(AzimuthData));
      PacketIndex += UE_ARRAY_COUNT(AzimuthData);

      // Append Azimuth information to the output string
      OutputString += FString::Printf(TEXT("Azimuth: %f\n"), Sensor.AzimuthAngle[Index]);

    }

    // Range data : converting from cm to mm
    uint16 Distance = 0;
    if (Sensor.RecordedHits[Index].bBlockingHit) {
      Distance = ((Sensor.RecordedHits[Index].Distance + Sensor.MinRange) * 10) / 2; // 2mm resolution
    }
    DistanceData[0] = Distance & 0x00FF;
    DistanceData[1] = (Distance & 0xFF00) >> 8;
    FMemory::Memcpy(Sensor.DataPacket.GetData() + PacketIndex, DistanceData, UE_ARRAY_COUNT(DistanceData));
    PacketIndex += UE_ARRAY_COUNT(DistanceData);

    // Append Distance information to the output string
    OutputString += FString::Printf(TEXT("Distance: %d mm\n"), Distance * 2); // converting back to mm

    // Intensity data
    auto PhysMat = Sensor.RecordedHits[Index].PhysMaterial;
    if (PhysMat != nullptr) {
      IntensityData[0] = GetIntensity(*PhysMat->GetName(), (Distance * 2) / 10);
    }
    else {
      IntensityData[0] = 0x00;
    }
    FMemory::Memcpy(Sensor.DataPacket.GetData() + PacketIndex, IntensityData, UE_ARRAY_COUNT(IntensityData));
    PacketIndex += UE_ARRAY_COUNT(IntensityData);

    // Append Intensity information to the output string
    OutputString += FString::Printf(TEXT("Intensity: %d\n"), IntensityData[0]);

  }

  // Add timestamp (0 ~ 35999) and factory bytes
  TailData[0] = TimeStamp & 0x000000FF;
  TailData[1] = (TimeStamp & 0x0000FF00) >> 8;
  TailData[2] = (TimeStamp & 0x00FF0000) >> 16;
  TailData[3] = (TimeStamp & 0xFF000000) >> 24;
  TailData[4] = Sensor.ReturnMode;
  TailData[5] = Sensor.ModelNumber;
  FMemory::Memcpy(Sensor.DataPacket.GetData() + DATA_PACKET_PAYLOAD - 6, TailData, UE_ARRAY_COUNT(TailData));
  PacketIndex += UE_ARRAY_COUNT(TailData);

  // Append the timestamp and sensor model information to the output string
  OutputString += FString::Printf(TEXT("Timestamp: %u\n"), TimeStamp);
  OutputString += FString::Printf(TEXT("Sensor Model Number: %u\n"), Sensor.ModelNumber);
}

void UVelodyneBaseComponent::GeneratePositionPacket(uint32 TimeStamp)
{
  // Packet should be encoded based on Sensor Model & Scanning Mode
  uint8 UnusedPacket[187]   = {0};
  uint8 ReservedPacket[178] = {0};
  uint8 NMEAPacket[128]     = {0};
  uint8 TimeStampPacket[4]  = {0};
  uint8 OneBytePacket[1]    = {0};
  uint8 TwoBytePacket[2]    = {0};
  uint8 FourBytePacket[4]   = {0};

  int32 PacketIndex = 0;

  // reserved (unused)
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, UnusedPacket, UE_ARRAY_COUNT(UnusedPacket));
  PacketIndex += UE_ARRAY_COUNT(UnusedPacket);

  // Temperature of top board (0 to 150°C)
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, OneBytePacket, UE_ARRAY_COUNT(OneBytePacket));
  PacketIndex += UE_ARRAY_COUNT(OneBytePacket);

  // Temperature of bottom board (0 to 150°C)
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, OneBytePacket, UE_ARRAY_COUNT(OneBytePacket));
  PacketIndex += UE_ARRAY_COUNT(OneBytePacket);

  // Temperature when ADC calibration last ran (0 to 150°C)
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, OneBytePacket, UE_ARRAY_COUNT(OneBytePacket));
  PacketIndex += UE_ARRAY_COUNT(OneBytePacket);

  // Change in temperature since last ADC calibration (-150 to 150°C)
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, TwoBytePacket, UE_ARRAY_COUNT(TwoBytePacket));
  PacketIndex += UE_ARRAY_COUNT(TwoBytePacket);

  // Elapsed seconds since last ADC calibration (0 to 2^32-1)
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, FourBytePacket, UE_ARRAY_COUNT(FourBytePacket));
  PacketIndex += UE_ARRAY_COUNT(FourBytePacket);


  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, OneBytePacket, UE_ARRAY_COUNT(OneBytePacket));
  PacketIndex += UE_ARRAY_COUNT(OneBytePacket);

  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, OneBytePacket, UE_ARRAY_COUNT(OneBytePacket));
  PacketIndex += UE_ARRAY_COUNT(OneBytePacket);

  // µsec since top of the hour
  TimeStampPacket[0] = TimeStamp & 0x000000FF;
  TimeStampPacket[1] = (TimeStamp & 0x0000FF00) >> 8;
  TimeStampPacket[2] = (TimeStamp & 0x00FF0000) >> 16;
  TimeStampPacket[3] = (TimeStamp & 0xFF000000) >> 24;
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, TimeStampPacket, UE_ARRAY_COUNT(TimeStampPacket));
  PacketIndex += UE_ARRAY_COUNT(TimeStampPacket);

  // Pulse Per Second (PPS) status
  // 0: Absent
  // 1: Synchronizing
  // 2: Locked
  // 3: Error
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, OneBytePacket, UE_ARRAY_COUNT(OneBytePacket));
  PacketIndex += UE_ARRAY_COUNT(OneBytePacket);

  // Thermal status
  // 0: Ok
  // 1: Thermal shutdown
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, OneBytePacket, UE_ARRAY_COUNT(OneBytePacket));
  PacketIndex += UE_ARRAY_COUNT(OneBytePacket);

  // Last shutdown temperature
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, OneBytePacket, UE_ARRAY_COUNT(OneBytePacket));
  PacketIndex += UE_ARRAY_COUNT(OneBytePacket);

  // Temperature of unit at power up
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, OneBytePacket, UE_ARRAY_COUNT(OneBytePacket));
  PacketIndex += UE_ARRAY_COUNT(OneBytePacket);

  {
    FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, NMEAPacket, UE_ARRAY_COUNT(NMEAPacket));
  }

  PacketIndex += UE_ARRAY_COUNT(NMEAPacket);

  // reserved
  FMemory::Memcpy(Sensor.PositionPacket.GetData() + PacketIndex, ReservedPacket, UE_ARRAY_COUNT(ReservedPacket));
}

FString UVelodyneBaseComponent::DecToHex(int DecimalNumber)
{
  // char array to store hexadecimal number
  char HexaDeciNum[100];

  // counter for hexadecimal number array
  int i = 0;
  while (DecimalNumber != 0)
  {
    // temporary variable to store remainder
    int Temp = 0;

    // storing remainder in temp variable.
    Temp = DecimalNumber % 16;

    // check if Temp < 10
    if (Temp < 10) {
      HexaDeciNum[i] = Temp + 48;
      i++;
    }
    else {
      HexaDeciNum[i] = Temp + 55;
      i++;
    }

    DecimalNumber = DecimalNumber / 16;
  }

  FString Answer;

  // printing hexadecimal number array in reverse order
  for (int j = i - 1; j >= 0; j--) {
    Answer += HexaDeciNum[j];
  }

  return Answer;
}

void UVelodyneBaseComponent::ASCIItoHEX(FString Ascii, uint8 Hex[])
{
  // Initialize final String
  FString StrHex = "";


  for (int i = 0; i < Ascii.Len(); i++)
  {

    FString Part = DecToHex((int)Ascii[i]);

    StrHex += Part;
  }

  // Return the final string hex
  strcpy( reinterpret_cast<char*>( Hex ), TCHAR_TO_ANSI(*StrHex));
}


void UVelodyneBaseComponent::WriteHitsToFile(const FString& FilePath)
{
    FString OutputString1;

    // Log the number of recorded hits
    UE_LOG(LogTemp, Warning, TEXT("Number of recorded hits: %d"), Sensor.RecordedHits.Num());
    
    // Iterate through the recorded hits and extract location information
    for (const FHitResult& HitResult : Sensor.RecordedHits)
    {
        // If a hit was recorded
        if (HitResult.bBlockingHit)
        {
            FVector HitLocation = HitResult.ImpactPoint; // Get the location of the hit
            FString Line = FString::Printf(TEXT("%f, %f, %f\n"), 
                HitLocation.X, HitLocation.Y, HitLocation.Z);
            
            // Append the hit location to the output string
            OutputString1 += Line;
        }
    }

    // Write the string to the file at the specified file path
    FFileHelper::SaveStringToFile(OutputString1, *FilePath);
}

