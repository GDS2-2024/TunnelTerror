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
	SelectedSlot = InventorySlots[0];
	SelectedSlotIndex = 1;
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
	InventorySlots[SlotIndex].EmptySlot();
	NumOfItems -= 1;
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

void UInventoryComponent::ChangeSelectedSlot(int32 NewSelection)
{
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());
	if (Player->IsLocallyControlled())
	{
		// Only clients should call the server RPC
		if (!SelectedSlot.IsEmpty())
		{
			ServerHideItem(SelectedSlot.Item); //Hide the previous selection
		}
		if (NewSelection > 0 && NewSelection <= InventorySlots.Num())
		{
			SelectedSlotIndex = NewSelection;
			SelectedSlot = InventorySlots[NewSelection - 1];
			if (!SelectedSlot.IsEmpty())
			{
				ServerShowItem(SelectedSlot.Item);
			}
		}
	}
}

AInventoryItem* UInventoryComponent::GetSelectedItem()
{
	return SelectedSlot.Item;
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
	UE_LOG(LogTemp, Warning, TEXT("Inventory slots replicated to the client."));
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());

	int32 SlotIndex = 0; // Array Slot Index
	for (FInventorySlot Slot : InventorySlots)
	{
		if (Slot.IsEmpty())
		{
			Player->ClientRemoveInventoryUI(SlotIndex);
		} else
		{
			Player->ClientAddInventoryUI(Slot.Item);
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
	}
		
}

void UInventoryComponent::MulticastShowItem_Implementation(AInventoryItem* Item)
{
	if (Item)
	{
		Item->ShowItem();
	}
}

// Hide Item
void UInventoryComponent::ServerHideItem_Implementation(AInventoryItem* Item)
{
	//UE_LOG(LogTemp, Log, TEXT("ServerHideItem()"))
	if (Item)
	{
		MulticastHideItem(Item);
	}
}

void UInventoryComponent::MulticastHideItem_Implementation(AInventoryItem* Item)
{
	if (Item)
	{
		Item->HideItem();
	}
}

FInventorySlot* UInventoryComponent::GetAvailableSlot()
{
	if (HasEmptySlot())
	{
		for (FInventorySlot& Slot : InventorySlots)
		{
			if (Slot.IsEmpty())
			{
				return &Slot;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full, can't return an available slot."));
	}

	return nullptr; // Return nullptr if no available slot is found
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
	UE_LOG(LogTemp, Warning, TEXT("Player does not have a pickaxe"))
	return nullptr;
}

void UInventoryComponent::ServerAddItem_Implementation(AInventoryItem* Item)
{
	if (HasEmptySlot())
	{
		UE_LOG(LogTemp, Log, TEXT("Adding Item to Inventory Slot"))
		if (FInventorySlot* EmptySlot = GetAvailableSlot())
		{
			EmptySlot->AddItemToSlot(Item);
			NumOfItems += 1;
			Item->Player = GetOwner();
			// Hide if not current slot
			//SelectedSlot = EmptySlot;
			if (SelectedSlot.Item != EmptySlot->Item)
			{
				ServerHideItem(Item);
			}
		}
		Item->Player = GetOwner();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't add item, EmptySlot is null."));
	}
}

void UInventoryComponent::ServerRemoveItem_Implementation(int32 SlotIndex)
{
	if (SlotIndex >= 0 && SlotIndex < InventorySlots.Num())
	{
		InventorySlots[SlotIndex].EmptySlot();
		NumOfItems--;
	}
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

	DOREPLIFETIME(UInventoryComponent, InventorySlots);
	DOREPLIFETIME(UInventoryComponent, NumOfItems);
}


