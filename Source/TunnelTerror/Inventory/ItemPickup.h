// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Components/BoxComponent.h"
#include "ItemPickup.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API AItemPickup : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	AItemPickup();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PickupMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* PickupCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInventoryItem> CorrespondingItemClass;

	
	UFUNCTION()
	void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo);
	
};
