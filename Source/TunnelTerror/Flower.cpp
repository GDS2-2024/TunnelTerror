// Fill out your copyright notice in the Description page of Project Settings.

#include "Flower.h"
#include "TunnelTerrorCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UFlower::UFlower()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFlower::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere = GetOwner()->FindComponentByClass<USphereComponent>();

	if (CollisionSphere)
	{
		// Bind the overlap event to the OnOverlapBegin function
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UFlower::OnOverlapBegin);
	}
	// ...
	
}


// Called every frame
void UFlower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Function called when overlap begins
void UFlower::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is the player character
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// Call the InfectPlayer function
		InfectPlayer(PlayerCharacter);
	}
}

void UFlower::InfectPlayer(ATunnelTerrorCharacter* TargetCharacter)
{
	TargetCharacter->DecreaseHealth(100.0f);
	UE_LOG(LogTemp, Log, TEXT("The players health is: %f"), TargetCharacter->health);
}

