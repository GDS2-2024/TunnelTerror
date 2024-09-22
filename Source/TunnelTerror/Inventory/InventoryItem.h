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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_ItemMesh)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(ReplicatedUsing = OnRep_Visibility)
	bool bIsVisible;
	
	UPROPERTY()
	AActor* Player;
	
	// Function should be implemented by child classes
	virtual void UseItem();
	virtual void ReleaseUseItem();

	UFUNCTION()
	void OnRep_ItemMesh();

	UFUNCTION()
	void OnRep_Visibility();

	void SetVisibility(bool newVisibility);
	void ShowItem();
	void HideItem();
	
};
