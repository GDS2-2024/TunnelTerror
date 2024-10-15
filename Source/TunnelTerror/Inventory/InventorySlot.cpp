// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

void FInventorySlot::AddItemToSlot(AInventoryItem* NewItem)
{
	if (IsEmpty())
	{
		Item = NewItem;
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot add item to Inventory slot as it already contains an item."))
	}
}



void FInventorySlot::EmptySlot()
{
	Item = nullptr;
}

void FInventorySlot::SlotSelected()
{
	if (Item)
	{
		Item->ShowItem();
	}
}

void FInventorySlot::SlotDeselected()
{
	if (Item)
	{
		Item->HideItem();
	}
}
