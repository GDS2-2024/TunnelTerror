#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomComponent.h"
#include "LevelGenerator.generated.h"

USTRUCT(BlueprintType)
struct FGridRow
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<int32> Elements;

    UPROPERTY()
    TArray<bool> BoolValues;

    void Add(int32 Element, bool BoolValue)
    {
        Elements.Add(Element);
        BoolValues.Add(BoolValue);
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
    ALevelGenerator();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TArray<FGridRow> Grid;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 Height;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 Width;

    void InitializeGrid(int32 GridWidth, int32 GridHeight);
    void SpawnMap(int32 GridWidth, int32 GridHeight);
    URoomComponent* SpawnRoom(int32 CurrentI, int32 CurrentJ, TSubclassOf<AActor> ActorToSpawn);
    void MarkGridAsOccupied(URoomComponent* RoomComponent, FVector Origin);

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> Room1;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> Room2;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> Corridor1;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> CornerCorridor1;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> CornerCorridor2;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> TCorridor1;

    UPROPERTY(EditDefaultsOnly, Category = "Rooms")
    TArray<TSubclassOf<AActor>> Rooms;

    UPROPERTY(EditDefaultsOnly, Category = "Corridors")
    TArray<TSubclassOf<AActor>> Corridors;

    UPROPERTY(EditDefaultsOnly)
    int32 LastY = 0;

    UPROPERTY(EditDefaultsOnly)
    int32 LastX = 0;

private:
    FVector GetRandomRoomSpawnLocation();
    TSubclassOf<AActor> GetRandomRoom();
    TSubclassOf<AActor> GetRandomCorridor();

};