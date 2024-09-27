// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"

AInventoryItem::AInventoryItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(ItemMesh);
}

void AInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInventoryItem, ItemName)
	DOREPLIFETIME(AInventoryItem, InventoryIcon)
	DOREPLIFETIME(AInventoryItem, ItemMesh)
	DOREPLIFETIME(AInventoryItem, Player)
}

void AInventoryItem::UseItem()
{
	//UE_LOG(LogTemp, Warning, TEXT("UseItem() called in Base Class (default implementation)"));
}

void AInventoryItem::ReleaseUseItem()
{
	//UE_LOG(LogTemp, Warning, TEXT("ReleaseUseItem() called in Base Class (default implementation)"));
}

void AInventoryItem::ShowItem()
{
	UE_LOG(LogTemp, Error, TEXT("AInventoryItem::ShowItem() Called on: %d"), GetNetMode())
	if (ItemMesh)
	{
		ItemMesh->SetVisibility(true, true);
		UE_LOG(LogTemp, Error, TEXT("AInventoryItem::ShowItem() Set visibility to true: %s"), *ItemName.ToString())
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("AInventoryItem::ShowItem() ItemMesh is null"))
	}
}

void AInventoryItem::HideItem()
{
	UE_LOG(LogTemp, Error, TEXT("AInventoryItem::HideItem() Called on: %d"), GetNetMode())
	if (ItemMesh)
	{
		ItemMesh->SetVisibility(false, true);
		UE_LOG(LogTemp, Error, TEXT("AInventoryItem::HideItem() Set visibility to false: %s"), *ItemName.ToString())
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("AInventoryItem::HideItem() ItemMesh is null"))
	}
}
