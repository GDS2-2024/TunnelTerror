// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"


void UPlayerHUD::SetSlotIcon(int32 SlotNum, UTexture2D* IconToSet)
{
	switch (SlotNum)
	{
	case 1 :
		Slot1->SetBrushFromTexture(IconToSet);
		break;
	case 2 :
		Slot2->SetBrushFromTexture(IconToSet);
		break;
	case 3 :
		Slot3->SetBrushFromTexture(IconToSet);
		break;
	case 4 :
		Slot4->SetBrushFromTexture(IconToSet);
		break;
	case 5 :
		Slot5->SetBrushFromTexture(IconToSet);
		break;
	default:
		break;
	}
}

void UPlayerHUD::ClearSlotIcon(int32 SlotNum)
{
	
}

void UPlayerHUD::SetSlotSelection(int32 SlotNum)
{
	// Get the Canvas Panel Slot of the Border
	if (UCanvasPanelSlot* SelectedSlotCanvas = Cast<UCanvasPanelSlot>(SelectedSlot->Slot))
	{
		switch (SlotNum)
		{
		case 1 :
			if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Slot1->Slot))
			{
				FVector2d NewPos = FVector2d(ImageSlot->GetPosition().X-5,ImageSlot->GetPosition().Y-5);
				SelectedSlotCanvas->SetPosition(NewPos);
			}
			break;
		case 2 :
			if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Slot2->Slot))
			{
				FVector2d NewPos = FVector2d(ImageSlot->GetPosition().X-5,ImageSlot->GetPosition().Y-5);
				SelectedSlotCanvas->SetPosition(NewPos);
			}
			break;
		case 3 :
			if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Slot3->Slot))
			{
				FVector2d NewPos = FVector2d(ImageSlot->GetPosition().X-5,ImageSlot->GetPosition().Y-5);
				SelectedSlotCanvas->SetPosition(NewPos);
			}
			break;
		case 4 :
			if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Slot4->Slot))
			{
				FVector2d NewPos = FVector2d(ImageSlot->GetPosition().X-5,ImageSlot->GetPosition().Y-5);
				SelectedSlotCanvas->SetPosition(NewPos);
			}
			break;
		case 5 :
			if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Slot5->Slot))
			{
				FVector2d NewPos = FVector2d(ImageSlot->GetPosition().X-5,ImageSlot->GetPosition().Y-5);
				SelectedSlotCanvas->SetPosition(NewPos);
			}
			break;
		default:
			break;
		}
	}

}
