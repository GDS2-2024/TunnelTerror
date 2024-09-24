// Fill out your copyright notice in the Description page of Project Settings.


#include "SporeField.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"
#include "Components/SphereComponent.h"
#include <Net/UnrealNetwork.h>

// Sets default values
ASporeField::ASporeField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASporeField::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		DestroySporeFieldImplementation();
	}

	CollisionSphere = FindComponentByClass<USphereComponent>();

	if (CollisionSphere)
	{
		// Bind the overlap event to the OnOverlapBegin function
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASporeField::OnOverlapBegin);
		CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ASporeField::OnOverlapEnd);
	}
}

// Called every frame
void ASporeField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASporeField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is the player character
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// Call the InfectPlayer function
		PlayerCharacter->StartSporeInfection();
	}
}

void ASporeField::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->EndSporeInfection();
	}
}

void ASporeField::DestroySporeFieldImplementation()
{
	spawnChance = 5;
	int32 randomNumber = FMath::RandRange(1, spawnChance);

	if (randomNumber != 1)
	{
		MulticastDestroySporeField();
	}
}

void ASporeField::MulticastDestroySporeField_Implementation()
{
	Destroy();
}

void ASporeField::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASporeField, spawnChance);
}