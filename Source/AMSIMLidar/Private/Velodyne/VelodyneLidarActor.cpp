// A&M SIM LLC 2025 All Rights Reserved

#include "Velodyne/VelodyneLidarActor.h"

// Sets default values
AVelodyneLidarActor::AVelodyneLidarActor()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;

  LidarComponent = CreateDefaultSubobject<UVelodyneBaseComponent>(TEXT("VelodyneComponent"));
  this->AddOwnedComponent(LidarComponent);
}

// Called when the game starts or when spawned
void AVelodyneLidarActor::BeginPlay()
{
  Super::BeginPlay();

  if(UdpScanComponent)
  {
    ConfigureUDPScan();
    UdpScanComponent->OpenSendSocket(UdpScanComponent->Settings.SendIP, UdpScanComponent->Settings.SendPort);
    UdpScanComponent->OpenReceiveSocket(UdpScanComponent->Settings.ReceiveIP, UdpScanComponent->Settings.SendPort);
  }

  FTimespan ThreadSleepTime =
  FTimespan::FromMilliseconds(LidarComponent->Sensor.NumberDataBlock * (LidarComponent->Sensor.NumberDataChannel / LidarComponent->Sensor.NumberLaserEmitter) * (0.000001f * FIRING_CYCLE));
  FString UniqueThreadName = "LidarThread";

  LidarThread = new LidarThreadProcess(ThreadSleepTime,*UniqueThreadName, this);

  if(LidarThread)
  {
    LidarThread->Init();
    LidarThread->LidarThreadInit();
    UE_LOG(LogTemp, Warning, TEXT("Lidar thread initialized!"));
  }
}

void AVelodyneLidarActor::EndPlay(EEndPlayReason::Type Reason)
{
  if(LidarThread)
  {
    LidarThread->LidarThreadShutdown();
    LidarThread->Stop();
  }


  while(!LidarThread->ThreadHasStopped())
  {
    FPlatformProcess::Sleep(0.1);
  }
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  //Do this last
  delete LidarThread;

  Super::EndPlay(Reason);
}


void AVelodyneLidarActor::LidarThreadTick()
{
  float TimeDiffMs = 0;


  if(LidarThread && LidarThread->IsThreadPaused())
  {
    return;
  }

  if ( BeginTimestamp == 0 )
  {
    PacketTimestamp = 0;
  }
  else
  {
    PacketTimestamp += (uint32)(1e6 * (FPlatformTime::Seconds() - BeginTimestamp));
  }
  BeginTimestamp = FPlatformTime::Seconds();

  // Generate raycasting data
  LidarComponent->GetScanData();

  // Generate veldyne data packet
  LidarComponent->GenerateDataPacket(PacketTimestamp);

  // Multicast Delegate event for broadcating packet data
  UdpScanComponent->EmitBytes(LidarComponent->Sensor.DataPacket);

  TimeDiffMs = (float)(FPlatformTime::Seconds() - BeginTimestamp);

  if( TimeDiffMs > 0.0f )
  {
    FPlatformProcess::SleepNoStats(TimeDiffMs);
  }
  else
  {
    FPlatformProcess::SleepNoStats(0);
  }
}

void AVelodyneLidarActor::ConfigureUDPScan()
{
  UdpScanComponent->Settings.SendIP    = LidarComponent->DestinationIP;
  UdpScanComponent->Settings.ReceiveIP = LidarComponent->SensorIP;
  UdpScanComponent->Settings.SendPort  = LidarComponent->ScanPort;
  UdpScanComponent->Settings.SendSocketName = FString(TEXT("ue5-scan-send"));
  UdpScanComponent->Settings.BufferSize = PACKET_HEADER_SIZE + DATA_PACKET_PAYLOAD;
}
