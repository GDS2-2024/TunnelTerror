// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"

AItemPickup::AItemPickup()
{
	bReplicates = true;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates and attaches the Actor Components to this actor.
	PickupCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Pickup Collider"));
	// Specifies that the root transform of this actor should be the transform of the Collider component.
	SetRootComponent(PickupCollider);
	// Attaches the static mesh component to be a child of the collider. This means that when the actor's root transform
	// is moved (i.e. the collider transform) then the mesh will move with it.
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(GetRootComponent());
	PickupPrompt = CreateDefaultSubobject<UWidgetComponent>(TEXT("Pickup Prompt"));
	PickupPrompt->SetupAttachment(GetRootComponent());
}

void AItemPickup::BeginPlay()
{
	Super::BeginPlay();

	if (PickupCollider)
	{
		// This attaches the OnPickupBeginOverlap function to be called when the OnComponentBeginOverlap event is triggered.
		// We use .AddDynamic to add a function to a multicast dynamic delegate event.
		PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &AItemPickup::OnPickupBeginOverlap);
		PickupCollider->OnComponentEndOverlap.AddDynamic(this, &AItemPickup::OnPickupEndOverlap);
		UE_LOG(LogTemp, Display, TEXT("Pickup overlap event added."))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PickupCollider is null in the ItemPickup class."))
	}
}

void AItemPickup::ShowPrompt_Implementation(bool bIsVisible)
{
	PickupPrompt->SetHiddenInGame(!bIsVisible);
}

void AItemPickup::MulticastDestroyPickup_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("MulticastDestroyPickup called on a client. Destroying pickup."));
	Destroy();
}

void AItemPickup::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	if(GetLocalRole() == ROLE_Authority) // Only the server should handle the pickup
	{
		//UE_LOG(LogTemp, Log, TEXT("On Pickup BEGIN Overlap (SERVER)"))
		if (ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(OtherActor))
		{
			// Set Player's item variable
			Player->CollidedPickup = this;
			ShowPrompt(true);
		}
	}
}

void AItemPickup::OnPickupEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(GetLocalRole() == ROLE_Authority) // Only the server should handle the pickup
	{
		//UE_LOG(LogTemp, Log, TEXT("On Pickup END Overlap (SERVER)"))
		if (ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(OtherActor))
		{
			// Set Player's item variable to NULLPTR
			Player->CollidedPickup = nullptr;
			ShowPrompt(false);
		}
	}
}

