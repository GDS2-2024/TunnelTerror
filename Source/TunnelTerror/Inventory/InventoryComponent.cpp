// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

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
}

// Add Item to an available slot
void UInventoryComponent::AddItem(AInventoryItem* Item)
{
	if (FInventorySlot* EmptySlot = GetAvailableSlot(); EmptySlot != nullptr)
	{
		EmptySlot->AddItemToSlot(Item);
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

void UInventoryComponent::ChangeSelectedSlot(FInventorySlot NewSelection)
{
	SelectedSlot = NewSelection;
}

AInventoryItem* UInventoryComponent::GetSelectedItem()
{
	return SelectedSlot.Item;
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

