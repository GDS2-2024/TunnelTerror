// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorEscape.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// Sets default values
AElevatorEscape::AElevatorEscape()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	samplesNeeded = 5;
	currentSamples = 0;
}

// Called when the game starts or when spawned
void AElevatorEscape::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere = FindComponentByClass<USphereComponent>();

	if (CollisionSphere)
	{
		// Bind the overlap event to the OnOverlapBegin function
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AElevatorEscape::OnOverlapBegin);
		CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AElevatorEscape::OnOverlapEnd);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CollisionSphere is nullptr in AElevatorEscape::AElevatorEscape"));
	}
}

// Called every frame
void AElevatorEscape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElevatorEscape::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Player hit elevator"));
		PlayerCharacter->ElevatorEscape = this;
	}
}

void AElevatorEscape::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Player moved away from elevator"));
		PlayerCharacter->ElevatorEscape = nullptr;
	}
}

void AElevatorEscape::AddSample(int newSamples)
{
	//UE_LOG(LogTemp, Log, TEXT("%d"), newSamples);
	if (currentSamples <= samplesNeeded)
	{
		UE_LOG(LogTemp, Log, TEXT("Current samples before: %d"), currentSamples);
		currentSamples = currentSamples + newSamples;
		UE_LOG(LogTemp, Log, TEXT("Current samples after: %d"), currentSamples);
	}

	if (currentSamples == samplesNeeded)
	{
		PlayDoorOpenAnimation();
	}
}