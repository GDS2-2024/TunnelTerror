// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSceneComponent.h"

// Sets default values for this component's properties
URoomSceneComponent::URoomSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URoomSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URoomSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FVector> URoomSceneComponent::GetDoorPositions(FVector Origin)
{
    TArray<FVector> DoorPositions;
    const float CellSize = 500.0f;

    for (FVector xDoor : xDoors)
    {
        FVector DoorPosition = Origin + FVector(xDoor.X * CellSize, xDoor.Y * CellSize, 0);
        DoorPositions.Add(DoorPosition);
    }

    for (FVector yDoor : yDoors)
    {
        FVector DoorPosition = Origin + FVector(yDoor.X * CellSize, yDoor.Y * CellSize, 0);
        DoorPositions.Add(DoorPosition);
    }

    return DoorPositions;
}

