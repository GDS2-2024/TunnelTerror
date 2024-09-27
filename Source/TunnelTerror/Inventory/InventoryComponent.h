// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemPickup.h"
#include "InventorySlot.h"
#include "InventoryItem.h"
#include "Items/PickaxeItem.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUNNELTERROR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	// Add Item to the given Inventory Slot index
	void AddItem(AInventoryItem* Item, int32 SlotIndex);
	// Remove the item from Inventory at the given slot
	void RemoveItem(int32 SlotIndex);
	// Finds all plant samples in inventory and removes them
	void RemoveSamples();
	bool HasEmptySlot() const;
	AInventoryItem* GetSelectedItem();
	int32 GetNumOfItems() { return NumOfItems; }
	int32 GetMaxSlots() {return MaxSlots; }
	int32 GetIndexOfItem(AInventoryItem* Item);
	int32 GetAvailableSlotIndex();
	// Returns the Pickaxe if the player has one
	APickaxeItem* GetPlayersPickaxe();
	
	UPROPERTY(VisibleAnywhere, Replicated)
    	int32 SelectedSlotIndex;
	
	UFUNCTION(Server, Reliable)
	void ServerSetSelectedSlot(int32 SlotIndex);

	UFUNCTION(Server, Reliable)
	void ServerSetItemVisibility();
	
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 NewestItemSlotIndex;

	UFUNCTION(Server, Reliable)
	void ServerAddItem(AInventoryItem* Item);

	// Remove Item from a slot via RPC
	UFUNCTION(Server, Reliable)
	void ServerRemoveItem(int32 SlotIndex);
	
	// Function to be called when the inventory slots are replicated
    UFUNCTION()
    void OnRep_InventorySlots();
	
	UPROPERTY(VisibleAnywhere)
	int32 MaxSlots;

	UPROPERTY(ReplicatedUsing = OnRep_InventorySlots, VisibleAnywhere)
	TArray<FInventorySlot> InventorySlots = TArray<FInventorySlot>();

	UPROPERTY(Replicated, VisibleAnywhere)
	int32 NumOfItems;
	

	
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
	
	// Tells the Server to update anims based on given item
	UFUNCTION(Server, Reliable)
	void ServerUpdateAnims();
	
	//Multicast equip/unequip for Animations
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipTorch(bool equip);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipCompass(bool equip);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipPickaxe(bool equip);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSwingPickaxe(bool swing);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
