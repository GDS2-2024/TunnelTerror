// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hazard.h"
#include "BridgeHazard.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API ABridgeHazard : public AHazard
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABridgeHazard();
	
	UFUNCTION(NetMulticast, Reliable)
	void Fall();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void FallVisual();
};
