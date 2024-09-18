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

	virtual void UseItem() override;
};
