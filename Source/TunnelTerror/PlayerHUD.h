// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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
	void SetSlotIcon(int32 SlotNum);
	void ClearSlotIcon(int32 SlotNum);
	
protected:

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
	
};
