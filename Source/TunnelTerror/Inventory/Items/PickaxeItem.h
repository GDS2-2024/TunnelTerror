// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TunnelTerror/Inventory/InventoryItem.h"
#include "PickaxeItem.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API APickaxeItem : public AInventoryItem
{
	GENERATED_BODY()

private:
	bool IsSwinging = false;
	float UseTime = 0.0f;
	float PreviousTime = 0.0f;
	// The amount of time the player must swing the pickaxe
	const float MiningThreshold = 0.5f;

public:
	APickaxeItem();

	virtual void UseItem() override;
	virtual void ReleaseUseItem() override;
	
	/**
	 * @brief If the player has held pickaxe swing long enough to mine an ore
	 * @return 
	 */
	bool HasReachedMiningThreshold() { return UseTime > MiningThreshold; }
	
	// Override Tick to update UseTime when swinging
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
};
