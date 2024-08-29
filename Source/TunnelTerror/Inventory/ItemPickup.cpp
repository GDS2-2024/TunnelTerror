// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"

AItemPickup::AItemPickup()
{
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
}

void AItemPickup::BeginPlay()
{
	Super::BeginPlay();

	if (PickupCollider)
	{
		// This attaches the OnPickupOverlap function to be called when the OnComponentBeginOverlap event is triggered.
		// We use .AddDynamic to add a function to a multicast dynamic delegate event.
		PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &AItemPickup::OnPickupOverlap);
		UE_LOG(LogTemp, Display, TEXT("Pickup overlap event added."))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PickupCollider is null in the ItemPickup class."))
	}
}

void AItemPickup::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	//UE_LOG(LogTemp, Display, TEXT("Overlap event occurred in PickupBase"))
	if (ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has collided with item pickup"))
		if (CorrespondingItemClass)
		{
			// Spawn an instance of the inventory item
			AInventoryItem* InventoryItem = GetWorld()->SpawnActor<AInventoryItem>(CorrespondingItemClass);
			InventoryItem->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            
			if (InventoryItem)
			{
				// Pass the spawned item to the player's inventory
				Player->EquipToInventory(InventoryItem);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to spawn inventory item from CorrespondingItemClass"));
			}
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("CorrespondingItemClass is NULL"));
		}
	}
}
