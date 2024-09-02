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
	void ChangeSelectedSlot(int32 NewSelection);
	AInventoryItem* GetSelectedItem();
	int32 GetNumOfItems() { return NumOfItems; }
	INT32 GetMaxSlots() {return MaxSlots; }

	UPROPERTY(VisibleAnywhere)
    	int32 SelectedSlotIndex;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	int32 MaxSlots;

	UPROPERTY(ReplicatedUsing = OnRep_InventorySlots, VisibleAnywhere)
	TArray<FInventorySlot> InventorySlots;

	UPROPERTY(Replicated, VisibleAnywhere)
	int32 NumOfItems;

	UPROPERTY(VisibleAnywhere)
	FInventorySlot SelectedSlot;

	// Function to be called when the inventory slots are replicated
	UFUNCTION()
	void OnRep_InventorySlots();

	// Tell the server to show item on all clients
	UFUNCTION(Server, Reliable)
	void ServerShowItem(AInventoryItem* Item);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowItem(AInventoryItem* Item);
	
	// Tell the server to hide item on all clients
	UFUNCTION(Server, Reliable)
	void ServerHideItem(AInventoryItem* Item);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHideItem(AInventoryItem* Item);
	
	// Finds an empty slot in the inventory
	FInventorySlot* GetAvailableSlot();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
