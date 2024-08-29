// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API AInventoryItem : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	AInventoryItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh;
	
	// Function should be implemented by child classes
	virtual void UseItem();
	
};
