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
	
	bool IsSabotaged();
	void SetIsSabotaged(bool sabotaged);

protected:
	UPROPERTY(VisibleAnywhere)
	bool bIsSabotaged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
