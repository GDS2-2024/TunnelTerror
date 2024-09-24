// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Inventory/ItemPickup.h"
#include "BridgeSabotager.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API ABridgeSabotager : public AItemPickup
{
	
	GENERATED_BODY()

protected:
	bool bIsSabotaged = false;
	
public:
	UFUNCTION(Server, Reliable)
	void OnInteract();

	virtual void OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo) override;

	void OnBridgeSabotaged();
};
