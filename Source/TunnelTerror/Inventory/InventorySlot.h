// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "ItemPickup.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	FInventorySlot() : Item(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AInventoryItem* Item;

	void AddItemToSlot(AInventoryItem* NewItem);
	void EmptySlot();
	bool IsEmpty() const { return Item == nullptr; }
	void SlotSelected();
	void SlotDeselected();
};