// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
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

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDestroyPickup();

	UFUNCTION(Client, Reliable)
	void ShowPrompt(bool bIsVisible);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PickupName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PickupMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* PickupCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* PickupPrompt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInventoryItem> CorrespondingItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;
	
	UFUNCTION()
	virtual void OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo);
	
	UFUNCTION()
	void OnPickupEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                        AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
