// Fill out your copyright notice in the Description page of Project Settings.


#include "PickaxeItem.h"

APickaxeItem::APickaxeItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APickaxeItem::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("UseItem() called in Pickaxe Item"));
	if (!IsSwinging)
	{
		IsSwinging = true;
		UseTime = 0.0f; // Reset the timer when we start using the item
		PreviousTime = GetWorld()->GetTimeSeconds(); // Record the time at the moment of UseItem
	}
}

void APickaxeItem::ReleaseUseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("ReleaseUseItem() called in Pickaxe Item"));
	if (IsSwinging) // Stop only if the item was in use
	{
		IsSwinging = false;
		UseTime = 0.0f; // Reset the use time when item use stops
	}
}

void APickaxeItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsSwinging)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		UseTime += (CurrentTime - PreviousTime); // Accumulate the elapsed time
		PreviousTime = CurrentTime; // Update previous time for the next frame
		
		//UE_LOG(LogTemp, Warning, TEXT("Pickaxe has been used for %.2f seconds"), UseTime);
	}
}

void APickaxeItem::BeginPlay()
{
	Super::BeginPlay();
	PreviousTime = 0.0f;
	
}