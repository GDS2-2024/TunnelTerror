// Fill out your copyright notice in the Description page of Project Settings.

#include "InfectionTrap.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AInfectionTrap::AInfectionTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanTimeOut = true;
	despawnTimer = 60.0f;
	activate = 3.0f;
}

// Called when the game starts or when spawned
void AInfectionTrap::BeginPlay()
{
	Super::BeginPlay();
	
	activateTimer = 0.0f;

	USkeletalMeshComponent* SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();

	if (SkeletalMesh)
	{
		TArray<USceneComponent*> ChildComponents;
		SkeletalMesh->GetChildrenComponents(true, ChildComponents);

		for (USceneComponent* Child : ChildComponents)
		{
			// Check if the child is a USphereComponent
			USphereComponent* SphereComponent = Cast<USphereComponent>(Child);
			if (SphereComponent)
			{
				CollisionSphere = SphereComponent;
				break;
			}
		}

		if (CollisionSphere)
		{
			// Bind the overlap event to the OnOverlapBegin function
			CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInfectionTrap::OnOverlapBegin);
			CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

// Called every frame
void AInfectionTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (activateTimer < activate)
	{
		activateTimer += DeltaTime;
	}
	else
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	if (bCanTimeOut)
	{
		despawnTimer -= DeltaTime;
	}

	if (despawnTimer <= 0) {
		PlantDie();
	}
}

void AInfectionTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is the player character
	ATunnelTerrorCharacter* PlayerCharacter = Cast<ATunnelTerrorCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// Call the InfectPlayer function
		InfectPlayer(PlayerCharacter);
	}
}

void AInfectionTrap::InfectPlayer(ATunnelTerrorCharacter* TargetCharacter)
{
	TargetCharacter->DecreaseHealth(100.0f, "Plant Trap");
	UE_LOG(LogTemp, Log, TEXT("The players health is: %f"), TargetCharacter->health);
}

void AInfectionTrap::PlantDie()
{
	DeathAnim();
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
