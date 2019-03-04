// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameMode.h"
#include "Runtime/NavigationSystem/Public/NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "ActorPool.h"


AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	NavmeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bounds Volume Pool"));
}


void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume * BoundsVolumeToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Found volume: %s"), *BoundsVolumeToAdd->GetName());
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

	while (VolumeIterator)
	{
		NavmeshBoundsVolumePool->Add(* VolumeIterator);
		
		++VolumeIterator;
	}
}