// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemPickup.h"
#include "InventorySlot.h"
#include "InventoryItem.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUNNELTERROR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	// Add Item to an available slot
	void AddItem(AInventoryItem* Item);
	// Add Item to the given Inventory Slot
	void AddItem(AInventoryItem* Item, FInventorySlot& Slot);
	// Remove the item from Inventory at the given slot
	void RemoveItem(FInventorySlot& Slot);
	bool HasEmptySlot() const;
	void ChangeSelectedSlot(FInventorySlot NewSelection);
	AInventoryItem* GetSelectedItem();

private:
	UPROPERTY(VisibleAnywhere)
	int32 NumOfItems;
	UPROPERTY(VisibleAnywhere)
	TArray<FInventorySlot> InventorySlots;
	UPROPERTY(VisibleAnywhere)
	int32 MaxSlots;
	UPROPERTY(VisibleAnywhere)
	FInventorySlot SelectedSlot;

	// Finds an empty slot in the inventory
	FInventorySlot* GetAvailableSlot();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
