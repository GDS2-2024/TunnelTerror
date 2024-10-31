// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorSafeZone.h"
#include "Components/CapsuleComponent.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"

// Sets default values
AElevatorSafeZone::AElevatorSafeZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AElevatorSafeZone::BeginPlay()
{
	Super::BeginPlay();

	CollisionCapsule = FindComponentByClass<UCapsuleComponent>();

	if (CollisionCapsule)
	{
		// Bind the overlap event to the OnOverlapBegin function
		CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AElevatorSafeZone::OnOverlapBegin);
		CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AElevatorSafeZone::OnOverlapEnd);
	}
}

// Called every frame
void AElevatorSafeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElevatorSafeZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Player is in safe zone"));
		PlayerCharacter->SetIsInSafeZone(true);
	}
}

void AElevatorSafeZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Player has left the safe zone"));
		PlayerCharacter->SetIsInSafeZone(false);
	}
}