// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemPickup.h"
#include "ShopPickup.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API AShopPickup : public AItemPickup
{
	GENERATED_BODY()
	
	virtual void OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo) override;
	
	virtual void OnPickupEndOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
