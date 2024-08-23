// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoomComponent.generated.h"

USTRUCT(BlueprintType)
struct FDoorOld
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	int32 xValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	int32 yValue;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TUNNELTERROR_API URoomComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoomComponent();

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
	TArray<FVector> gridSpaces;

	TArray<FVector> GetDoorPositions(FVector Origin);
};
