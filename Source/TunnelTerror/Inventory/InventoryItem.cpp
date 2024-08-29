// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

AInventoryItem::AInventoryItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(ItemMesh);
}

void AInventoryItem::UseItem()
{
	// Default implementation (could be empty)
	UE_LOG(LogTemp, Warning, TEXT("UseItem() called in Base Class (default implementation)"));
}
