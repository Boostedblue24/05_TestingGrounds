// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"
#include "Runtime/NavigationSystem/Public/NavMesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor * UActorPool::Checkout()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checkout"), *GetName());

	return nullptr;
}

void UActorPool::Return(AActor * ActorToReturn)
{
	if (!ensure(ActorToReturn))
	{
		UE_LOG(LogTemp, Warning, TEXT("Returned Null Actor"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[%s] Returned"), *ActorToReturn->GetName());
	
}

void UActorPool::Add(AActor * ActorToAdd)
{
	if (!ensure(ActorToAdd))
	{
		UE_LOG(LogTemp, Warning, TEXT("Added Null Actor"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] Actor Added"), *ActorToAdd->GetName());
	
	Return(ActorToAdd);
}
