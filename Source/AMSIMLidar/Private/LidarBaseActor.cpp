// Copyright (c) 2025 A&M SIM LLC
// SPDX-License-Identifier: MIT
// Licensed under the MIT License; see the LICENSE file for details.

#include "LidarBaseActor.h"


// Sets default values
ALidarBaseActor::ALidarBaseActor() : PacketTimestamp(0)
{

  PrimaryActorTick.bCanEverTick = false;


  LidarMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
  RootComponent = LidarMeshComponent;

  UdpScanComponent = CreateDefaultSubobject<UUDPComponent>("UdpComponent");
  this->AddOwnedComponent(UdpScanComponent);
}

// Called when the game starts or when spawned
void ALidarBaseActor::BeginPlay()
{
  Super::BeginPlay();
}

void ALidarBaseActor::EndPlay(EEndPlayReason::Type Reason)
{
  Super::EndPlay(Reason);
}