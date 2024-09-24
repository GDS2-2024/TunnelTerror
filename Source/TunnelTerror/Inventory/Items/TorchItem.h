// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SpotLight.h"
#include "Components/SpotLightComponent.h"
#include "TunnelTerror/Inventory/InventoryItem.h"
#include "TorchItem.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API ATorchItem : public AInventoryItem
{
	GENERATED_BODY()

public:
	
	ATorchItem();

	virtual void BeginPlay() override;
	
};
