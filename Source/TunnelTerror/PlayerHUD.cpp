// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"


void UPlayerHUD::SetSlotIcon(int32 SlotNum, UTexture2D* IconToSet)
{
	switch (SlotNum)
	{
	case 1 :
		Slot1->SetBrushFromTexture(IconToSet);
		Slot1->SetBrushTintColor(ItemPresent);
		break;
	case 2 :
		Slot2->SetBrushFromTexture(IconToSet);
		Slot2->SetBrushTintColor(ItemPresent);
		break;
	case 3 :
		Slot3->SetBrushFromTexture(IconToSet);
		Slot3->SetBrushTintColor(ItemPresent);
		break;
	case 4 :
		Slot4->SetBrushFromTexture(IconToSet);
		Slot4->SetBrushTintColor(ItemPresent);
		break;
	case 5 :
		Slot5->SetBrushFromTexture(IconToSet);
		Slot5->SetBrushTintColor(ItemPresent);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("SetSlotIcon() SlotNum is out of bounds: %d"), SlotNum)
		break;
	}
}

void UPlayerHUD::ClearSlotIcon(int32 SlotNum)
{
	switch (SlotNum)
	{
	case 1 :
		Slot1->SetBrushFromTexture(nullptr);
		Slot1->SetBrushTintColor(EmptyItem);
		break;
	case 2 :
		Slot2->SetBrushFromTexture(nullptr);
		Slot2->SetBrushTintColor(EmptyItem);
		break;
	case 3 :
		Slot3->SetBrushFromTexture(nullptr);
		Slot3->SetBrushTintColor(EmptyItem);
		break;
	case 4 :
		Slot4->SetBrushFromTexture(nullptr);
		Slot4->SetBrushTintColor(EmptyItem);
		break;
	case 5 :
		Slot5->SetBrushFromTexture(nullptr);
		Slot5->SetBrushTintColor(EmptyItem);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("ClearSlotIcon() SlotNum is out of bounds: %d"), SlotNum)
		break;
	}
}

void UPlayerHUD::SetSlotSelection(int32 SlotNum)
{
	
	switch (SlotNum)
	{
	case 1 :
		BackgroundSlot1->SetBrushTintColor(SelectedColour);
		BackgroundSlot2->SetBrushTintColor(DeselectedColour);
		BackgroundSlot3->SetBrushTintColor(DeselectedColour);
		BackgroundSlot4->SetBrushTintColor(DeselectedColour);
		BackgroundSlot5->SetBrushTintColor(DeselectedColour);
		break;
	case 2 :
		BackgroundSlot1->SetBrushTintColor(DeselectedColour);
		BackgroundSlot2->SetBrushTintColor(SelectedColour);
		BackgroundSlot3->SetBrushTintColor(DeselectedColour);
		BackgroundSlot4->SetBrushTintColor(DeselectedColour);
		BackgroundSlot5->SetBrushTintColor(DeselectedColour);
		break;
	case 3 :
		BackgroundSlot1->SetBrushTintColor(DeselectedColour);
		BackgroundSlot2->SetBrushTintColor(DeselectedColour);
		BackgroundSlot3->SetBrushTintColor(SelectedColour);
		BackgroundSlot4->SetBrushTintColor(DeselectedColour);
		BackgroundSlot5->SetBrushTintColor(DeselectedColour);
		break;
	case 4 :
		BackgroundSlot1->SetBrushTintColor(DeselectedColour);
		BackgroundSlot2->SetBrushTintColor(DeselectedColour);
		BackgroundSlot3->SetBrushTintColor(DeselectedColour);
		BackgroundSlot4->SetBrushTintColor(SelectedColour);
		BackgroundSlot5->SetBrushTintColor(DeselectedColour);
		break;
	case 5 :
		BackgroundSlot1->SetBrushTintColor(DeselectedColour);
		BackgroundSlot2->SetBrushTintColor(DeselectedColour);
		BackgroundSlot3->SetBrushTintColor(DeselectedColour);
		BackgroundSlot4->SetBrushTintColor(DeselectedColour);
		BackgroundSlot5->SetBrushTintColor(SelectedColour);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("SetSlotSelection() SlotNum is out of bounds: %d"), SlotNum)
		break;
	}
}
