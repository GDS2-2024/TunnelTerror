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
