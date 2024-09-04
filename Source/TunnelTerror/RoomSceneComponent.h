// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RoomSceneComponent.generated.h"

USTRUCT(BlueprintType)
struct FDoor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	int32 xValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	int32 yValue;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUNNELTERROR_API URoomSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoomSceneComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Config")
	int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Config")
	int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Config")
	TArray<FVector> xDoors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Config")
	TArray<FVector> yDoors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Config")
	TArray<UActorComponent*> Walls;

	TArray<FVector> GetDoorPositions(FVector Origin);
		
};
