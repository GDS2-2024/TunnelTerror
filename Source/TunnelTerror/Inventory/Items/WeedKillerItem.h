// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InventoryItem.h"
#include "WeedKillerItem.generated.h"

class AInfectionTrap;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class TUNNELTERROR_API AWeedKillerItem : public AInventoryItem
{
	GENERATED_BODY()

	AWeedKillerItem();
	bool bIsActivated = false;

	virtual void UseItem() override;
	virtual void ReleaseUseItem() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnUseItem();
	UFUNCTION(BlueprintImplementableEvent)
	void OnReleaseUseItem();

	UFUNCTION(Reliable, NetMulticast)
	void OnUseItemVisual();
	UFUNCTION(Reliable, NetMulticast)
	void OnReleaseItemVisual();
};
