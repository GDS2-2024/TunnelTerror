// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Inventory/ItemPickup.h"
#include "TorchHazard.generated.h"

class UPointLightComponent;

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API ATorchHazard : public AItemPickup
{
	GENERATED_BODY()
	
public:
	ATorchHazard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPointLightComponent* PointLight2;

	virtual void OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo) override;

	void OnInteract();

	UFUNCTION(NetMulticast, Reliable)
	void TurnOff();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOffBP();

	bool bIsOff = false;

protected:
	UPROPERTY(BlueprintReadWrite)
	double SabotageRange = 4000;
};
