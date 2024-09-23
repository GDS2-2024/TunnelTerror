// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hazard.h"
#include "Stalactite.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API AStalactite : public AHazard
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AStalactite();

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* FallTrigger;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnFallTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo);


	UFUNCTION(NetMulticast, Reliable)
	void Fall();
	UFUNCTION(BlueprintNativeEvent)
	void FallVisual();
};
