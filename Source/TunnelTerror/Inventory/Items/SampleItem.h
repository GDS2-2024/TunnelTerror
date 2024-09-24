// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TunnelTerror/Inventory/InventoryItem.h"
#include "SampleItem.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API ASampleItem : public AInventoryItem
{
	GENERATED_BODY()

	virtual void UseItem() override;
};
