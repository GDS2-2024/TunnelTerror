// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantSnap.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"
#include "Components/BoxComponent.h"
#include "InfectionTrap.h"

// Sets default values
APlantSnap::APlantSnap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlantSnap::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox = FindComponentByClass<UBoxComponent>();
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlantSnap::OnOverlapBegin);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APlantSnap::OnOverlapEnd);
	}
}

// Called every frame
void APlantSnap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlantSnap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is the player character
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		AInfectionTrap* Trap = Cast<AInfectionTrap>(GetAttachParentActor());
		if (Trap)
		{
			Trap->Snap();
		}
	}
}

void APlantSnap::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		AInfectionTrap* Trap = Cast<AInfectionTrap>(GetAttachParentActor());
		if (Trap)
		{
			Trap->StopSnap();
		}
	}
}