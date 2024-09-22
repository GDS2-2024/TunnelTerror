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
	DOREPLIFETIME(AInventoryItem, bIsVisible)
}

void AInventoryItem::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("UseItem() called in Base Class (default implementation)"));
}

void AInventoryItem::ReleaseUseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("ReleaseUseItem() called in Base Class (default implementation)"));
}

void AInventoryItem::OnRep_ItemMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemMesh Replicated"));
}

void AInventoryItem::OnRep_Visibility()
{
	//UE_LOG(LogTemp, Warning, TEXT("ItemMesh Visibility Replicated"));
	if (bIsVisible)
	{
		ShowItem();
	} else
	{
		HideItem();
	}
}

void AInventoryItem::SetVisibility(bool newVisibility)
{
	bIsVisible = newVisibility;
	OnRep_Visibility();
}

void AInventoryItem::ShowItem()
{
	if (ItemMesh)
	{
		ItemMesh->SetVisibility(true);
		TArray<USceneComponent*> ChildrenArray;
		ItemMesh->GetChildrenComponents(true, ChildrenArray);

		// Check if there are any children and set their visibility to true
		for (USceneComponent* Element : ChildrenArray)
		{
			if (Element)
			{
				Element->SetVisibility(true);
			}
		}
	}
}

void AInventoryItem::HideItem()
{
	if (ItemMesh)
	{
		ItemMesh->SetVisibility(false);
		TArray<USceneComponent*> ChildrenArray;
		ItemMesh->GetChildrenComponents(true, ChildrenArray);

		// Check if there are any children and set their visibility to false
		for (USceneComponent* Element : ChildrenArray)
		{
			if (Element)
			{
				Element->SetVisibility(false);
			}
		}
	}
}
