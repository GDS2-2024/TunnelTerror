// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	NumOfItems = 0;
	MaxSlots = 5;
	InventorySlots.SetNum(MaxSlots);
	SelectedSlotIndex = 0;
	NewestItemSlotIndex = 0;
}

// Add Item to the given Inventory Slot
void UInventoryComponent::AddItem(AInventoryItem* Item, int32 SlotIndex)
{
	InventorySlots[SlotIndex].AddItemToSlot(Item);
	NumOfItems += 1;
	UE_LOG(LogTemp, Warning, TEXT("Item added to Inventory"))
}

// Remove the item from Inventory at the given slot
void UInventoryComponent::RemoveItem(int32 SlotIndex)
{
	if (!InventorySlots[SlotIndex].IsEmpty())
	{
		InventorySlots[SlotIndex].EmptySlot();
		NumOfItems -= 1;
	}
}

void UInventoryComponent::RemoveSamples()
{
	int32 slotIndex = 0;
	for (FInventorySlot& Slot : InventorySlots)
	{
		UE_LOG(LogTemp, Log, TEXT("Slot index: %d"), slotIndex)
		if (!Slot.IsEmpty())
		{
			if (Slot.Item->ItemName == "Plant Sample")
			{
				UE_LOG(LogTemp, Log, TEXT("Slot: %d IS a plant sample"), slotIndex)
				RemoveItem(slotIndex);
				OnRep_InventorySlots();
			}
		}
		slotIndex++;
	}
}

bool UInventoryComponent::HasEmptySlot() const
{
	return NumOfItems < InventorySlots.Max();
}

AInventoryItem* UInventoryComponent::GetSelectedItem()
{
	return InventorySlots[SelectedSlotIndex].Item;
}

int32 UInventoryComponent::GetIndexOfItem(AInventoryItem* Item)
{
	int32 slotNum = 0;
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item == Item)
		{
			return slotNum;
		}
		slotNum++;
	}
	UE_LOG(LogTemp, Error, TEXT("Error can't find index of item in inventory slots"))
	return -1;
}

void UInventoryComponent::OnRep_InventorySlots()
{
	// Handle client-side logic when inventory slots are updated
	//UE_LOG(LogTemp, Warning, TEXT("Inventory slots replicated to the client."));
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());

	int32 SlotIndex = 0; // Array Slot Index
	for (FInventorySlot Slot : InventorySlots)
	{
		if (Slot.IsEmpty())
		{
			Player->ClientRemoveInventoryUI(SlotIndex);
		} else
		{
			Player->ClientAddInventoryUI(Slot.Item, NewestItemSlotIndex);
		}
		SlotIndex++;
	}
}

// Show Item
void UInventoryComponent::ServerShowItem_Implementation(AInventoryItem* Item)
{
	//UE_LOG(LogTemp, Log, TEXT("ServerShowItem()"))
	if (Item)
	{
		MulticastShowItem(Item);
	} else
	{
		//UE_LOG(LogTemp, Error, TEXT("SERVER SHOW() Item is null"))
	}
		
}

void UInventoryComponent::MulticastShowItem_Implementation(AInventoryItem* Item)
{
	if (Item)
	{
		Item->ShowItem();
	} else
	{
		//UE_LOG(LogTemp, Error, TEXT("MULTICAST SHOW() Item is null"))
	}
}

// Hide Item
void UInventoryComponent::ServerHideItem_Implementation(AInventoryItem* Item)
{
	if (Item)
	{
		MulticastHideItem(Item);
	} else
	{
		//UE_LOG(LogTemp, Error, TEXT("SERVER HIDE() Item is null"))
	}
}

void UInventoryComponent::MulticastHideItem_Implementation(AInventoryItem* Item)
{
	if (Item)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Client: HIDE() Item"))
		Item->HideItem();
	} else
	{
		//UE_LOG(LogTemp, Error, TEXT("Client: HIDE() Item is null"))
	}
}

int32 UInventoryComponent::GetAvailableSlotIndex()
{
	int32 slotIndex = 0;
	if (HasEmptySlot())
	{
		for (FInventorySlot& Slot : InventorySlots)
		{
			if (Slot.IsEmpty())
			{
				return slotIndex;
			}
			slotIndex++;
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full, can't return an available slot INDEX."));
	}
	return -1;
}

APickaxeItem* UInventoryComponent::GetPlayersPickaxe()
{
	for (FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item)
		{
			if (APickaxeItem* Pickaxe = Cast<APickaxeItem>(Slot.Item) )
			{
				return Pickaxe;
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Player does not have a pickaxe"))
	return nullptr;
}

void UInventoryComponent::ServerSetItemVisibility_Implementation()
{
	//UE_LOG(LogTemp, Log, TEXT("ServerSetItemVisibility_Implementation"))
	ServerShowItem(InventorySlots[SelectedSlotIndex].Item);
    for (int index = 0; index < InventorySlots.Num(); index++)
    {
       	if (index != SelectedSlotIndex)
       	{
    		//UE_LOG(LogTemp, Warning, TEXT("Server Hiding Slot: %d"), index)
       		ServerHideItem(InventorySlots[index].Item);
       	}
    }
}

void UInventoryComponent::ServerSetSelectedSlot_Implementation(int32 SlotIndex)
{
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());
	SelectedSlotIndex = SlotIndex;
	if (InventorySlots[SelectedSlotIndex].Item)
	{
		if (InventorySlots[SelectedSlotIndex].Item->ItemName.ToString() == "Compass"
			|| InventorySlots[SelectedSlotIndex].Item->ItemName.ToString() == "Plant Sample")
		{
			Player->EquipCompassAnim(true);
			MulticastEquipCompass(true);
		} else
		{
			Player->EquipCompassAnim(false);
			MulticastEquipCompass(false);
		}
		if (InventorySlots[SelectedSlotIndex].Item->ItemName.ToString() == "Pickaxe")
		{
			Player->EquipPickaxeAnim(true);
			MulticastEquipPickaxe(true);
		} else
		{
			Player->EquipPickaxeAnim(false);
			MulticastEquipPickaxe(false);
		}
		if (InventorySlots[SelectedSlotIndex].Item->ItemName.ToString() == "Torch")
		{
			Player->EquipTorchAnim(true);
			MulticastEquipTorch(true);
		} else
		{
			Player->EquipTorchAnim(false);
			MulticastEquipTorch(false);
		}
		if (InventorySlots[SelectedSlotIndex].Item->ItemName.ToString() == "WeedKiller")
		{
			Player->EquipWeedKillerAnim(true);
			MulticastEquipWeedKiller(true);
		} else
		{
			Player->EquipWeedKillerAnim(false);
			MulticastEquipWeedKiller(false);
		}
	} else
	{
		Player->EquipCompassAnim(false);
		MulticastEquipCompass(false);
		Player->EquipPickaxeAnim(false);
		MulticastEquipPickaxe(false);
		Player->EquipTorchAnim(false);
		MulticastEquipTorch(false);
	}
	ServerSetItemVisibility();
}

void UInventoryComponent::ServerAddItem_Implementation(AInventoryItem* Item)
{
	if (HasEmptySlot())
	{
		int32 NewSlotIndex = GetAvailableSlotIndex();
		NewestItemSlotIndex = NewSlotIndex;
		UE_LOG(LogTemp, Log, TEXT("Adding Item to Inventory Slot"))
		InventorySlots[NewSlotIndex].AddItemToSlot(Item);
		NumOfItems += 1;
		Item->Player = GetOwner();
		ServerSetItemVisibility();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't add item, EmptySlot is null."));
	}
}

void UInventoryComponent::ServerRemoveItem_Implementation(int32 SlotIndex)
{
	InventorySlots[SlotIndex].EmptySlot();
	NumOfItems--;
	ServerSetItemVisibility();
}

void UInventoryComponent::MulticastEquipCompass_Implementation(bool equip)
{
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());
	Player->EquipCompassAnim(equip);
}

void UInventoryComponent::MulticastEquipPickaxe_Implementation(bool equip)
{
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());
	Player->EquipPickaxeAnim(equip);
}

void UInventoryComponent::MulticastSwingPickaxe_Implementation(bool swing)
{
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());
	Player->SwingPickaxeAnim(swing);
}

void UInventoryComponent::MulticastEquipTorch_Implementation(bool equip)
{
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());
	Player->EquipTorchAnim(equip);
}

void UInventoryComponent::MulticastEquipWeedKiller_Implementation(bool equip)
{
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());
	Player->EquipWeedKillerAnim(equip);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventorySlots)
	DOREPLIFETIME(UInventoryComponent, NumOfItems)
	DOREPLIFETIME(UInventoryComponent, SelectedSlotIndex)
	DOREPLIFETIME(UInventoryComponent, NewestItemSlotIndex)
}


