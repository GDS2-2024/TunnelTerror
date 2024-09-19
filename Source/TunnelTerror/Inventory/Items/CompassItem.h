// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TunnelTerror/Inventory/InventoryItem.h"
#include "CompassItem.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API ACompassItem : public AInventoryItem
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	FVector ElevatorPosition;
	
public:

	ACompassItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UStaticMeshComponent* CompassNeedle;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
