// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"
#include "Components/Image.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetSlotIcon(int32 SlotNum, UTexture2D* IconToSet);
	void ClearSlotIcon(int32 SlotNum);
	void SetSlotSelection(int32 SlotNum);

	UFUNCTION(BlueprintCallable)
	void SetCurrencyUI(int32 number);
	
protected:

	// Item slots, icon is assigned to here
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* Slot1;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* Slot2;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* Slot3;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* Slot4;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* Slot5;

	// Background slots, used to show which is selected
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* BackgroundSlot1;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* BackgroundSlot2;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* BackgroundSlot3;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* BackgroundSlot4;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* BackgroundSlot5;

	//Currency counter
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrencyUI;

	FSlateColor ItemPresent = FSlateColor(FColor(255,255,255,255));
	FSlateColor EmptyItem = FSlateColor(FColor(255,255,255,0));
	
	FSlateColor DeselectedColour = FSlateColor(FColor(0,0,0,127));
	FSlateColor SelectedColour = FSlateColor(FColor(0,255,0,127));
	
};
