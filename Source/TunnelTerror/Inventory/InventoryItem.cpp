// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

#include "Net/UnrealNetwork.h"

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
	DOREPLIFETIME(AInventoryItem, ItemName);
	DOREPLIFETIME(AInventoryItem, InventoryIcon);
	DOREPLIFETIME(AInventoryItem, ItemMesh);
}

void AInventoryItem::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("UseItem() called in Base Class (default implementation)"));
}

void AInventoryItem::ReleaseUseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("ReleaseUseItem() called in Base Class (default implementation)"));
}

void AInventoryItem::ShowItem()
{
	if (ItemMesh)
	{
		ItemMesh->SetVisibility(true);
	}
}

void AInventoryItem::HideItem()
{
	if (ItemMesh)
	{
		ItemMesh->SetVisibility(false);
	}
}
