// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "Engine/Classes/Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "NavigationSystem.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MinSpawningExtent = FVector(0, -2000, 0);
	MaxSpawningExtent = FVector(4000, 2000, 0);
	NavigationBoundsOffset = FVector(2000, 0, 0);
}

void ATile::SetPool(UActorPool * InPool)
{
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}



void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();

	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not Enough Actors in Pool!"), *GetName());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked Out [%s]"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);

	FNavigationSystem::Build(*GetWorld());
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
	
	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceActor(ToSpawn, SpawnPosition);
	}
}

TArray<FSpawnPosition> ATile::RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions;

	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (size_t i = 0; i < NumberToSpawn; i++)
	{

		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);

		bool Found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);

		if (Found)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			SpawnPositions.Add(SpawnPosition);

		}
	}
	return TArray<FSpawnPosition>();
}



bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(MinSpawningExtent, MaxSpawningExtent);

	const int MAX_ATTEMPTS = 100;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}

	}
	
	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition)
{
	AActor * Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
}


// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	Pool->Return(NavMeshBoundsVolume);


}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;

	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation, 
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	

	return !HasHit;
}

