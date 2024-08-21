// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

USTRUCT(BlueprintType)
struct FGridRow
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<int32> Elements;


    void Add(int32 Element) 
    {
        Elements.Add(Element);
    }

    int32 Num() const
    {
        return Elements.Num();
    }
    
};

UCLASS()
class TUNNELTERROR_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TArray<FGridRow> Grid;

    void InitializeGrid(int32 GridWidth, int32 GridHeight);
    void SpawnMap(int32 GridWidth, int32 GridHeight);

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor>ActorToSpawn;
};
