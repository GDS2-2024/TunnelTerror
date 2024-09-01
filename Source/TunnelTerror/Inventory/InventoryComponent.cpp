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

// Add Item to an available slot
void UInventoryComponent::AddItem(AInventoryItem* Item)
{
	if (FInventorySlot* EmptySlot = GetAvailableSlot(); EmptySlot != nullptr)
	{
		EmptySlot->AddItemToSlot(Item);
		NumOfItems += 1;
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't add item, EmptySlot is null."));
	}
}

// Add Item to the given Inventory Slot
void UInventoryComponent::AddItem(AInventoryItem* Item, FInventorySlot& Slot)
{
	Slot.AddItemToSlot(Item);
	NumOfItems += 1;
	UE_LOG(LogTemp, Warning, TEXT("Item added to Inventory"))
}

// Remove the item from Inventory at the given slot
void UInventoryComponent::RemoveItem(FInventorySlot& Slot)
{
	Slot.EmptySlot();
	NumOfItems -= 1;
}

bool UInventoryComponent::HasEmptySlot() const
{
	return NumOfItems < InventorySlots.Max();
}

void UInventoryComponent::ChangeSelectedSlot(int32 NewSelection)
{
	//Hide the previous selection
	if (!SelectedSlot.IsEmpty())
	{
		//SelectedSlot.Item->HideItem();
		ServerHideItem(SelectedSlot.Item);
	}
	//Set the new selection
	SelectedSlotIndex = NewSelection;
	SelectedSlot = InventorySlots[NewSelection-1];
	if (!SelectedSlot.IsEmpty())
	{
		//SelectedSlot.Item->ShowItem();
		ServerShowItem(SelectedSlot.Item);
	}
}

AInventoryItem* UInventoryComponent::GetSelectedItem()
{
	return SelectedSlot.Item;
}

void UInventoryComponent::OnRep_InventorySlots()
{
	// Handle client-side logic when inventory slots are updated
	UE_LOG(LogTemp, Log, TEXT("Inventory slots replicated to the client."));
	ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(GetOwner());
	if (AInventoryItem* NewItem = InventorySlots[GetNumOfItems()-1].Item)
	{
		Player->ClientUpdateInventoryUI(NewItem);
	}
}

// Show Item
void UInventoryComponent::ServerShowItem_Implementation(AInventoryItem* Item)
{
	MulticastShowItem(Item);	
}

void UInventoryComponent::MulticastShowItem_Implementation(AInventoryItem* Item)
{
	Item->ShowItem();
}

// Hide Item
void UInventoryComponent::ServerHideItem_Implementation(AInventoryItem* Item)
{
	MulticastHideItem(Item);
}

void UInventoryComponent::MulticastHideItem_Implementation(AInventoryItem* Item)
{
	Item->HideItem();
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


