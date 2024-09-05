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

USTRUCT(BlueprintType)
struct FRoom
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> Actor;

    UPROPERTY(EditDefaultsOnly)
    TArray<FString> Doors;
};

USTRUCT(BlueprintType)
struct FPlace
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    FRoom Room;

    UPROPERTY(EditDefaultsOnly)
    int32 currentI;

    UPROPERTY(EditDefaultsOnly)
    int32 currentJ;

    UPROPERTY(EditDefaultsOnly)
    int32 lastDoor;

    void Add(FRoom actor, int32 I, int32 J, int32 door)
    {
        Room = actor;
        currentI = I;
        currentJ = J;
        lastDoor = door;
    }

    bool operator==(const FPlace& Other) const
    {
        return currentI == Other.currentI 
            && currentJ == Other.currentJ 
            && lastDoor == Other.lastDoor;
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

    int32 rooms;
    int32 number = 0;

    void InitializeGrid(int32 GridWidth, int32 GridHeight);
    void SpawnPath(int32 LastDoor, FRoom StartRoom, int32 CurrentI, int32 CurrentJ, bool start);
    URoomComponent* SpawnRoom(int32 CurrentI, int32 CurrentJ, TSubclassOf<AActor> ActorToSpawn, bool isX);
    void MarkGridAsOccupied(URoomComponent* RoomComponent, FVector Origin);
    bool CanPlaceRoom(int32 CurrentI, int32 CurrentJ, URoomComponent* RC);
    void SpawnAnotherPath(FPlace place);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FPlace> Doorways;

    UPROPERTY(EditDefaultsOnly)
    FRoom LastActor;

    UPROPERTY(EditDefaultsOnly)
    FRoom EntranceRoom;

    UPROPERTY(EditDefaultsOnly)
    TArray<FRoom> Rooms;

    UPROPERTY(EditDefaultsOnly)
    TArray<FRoom> Corridors;

    UPROPERTY(EditDefaultsOnly)
    int32 LastY = 0;

    UPROPERTY(EditDefaultsOnly)
    int32 LastX = 0;

    INT32 addPath = 0;

private:
    FVector GetRandomRoomSpawnLocation();
    TSubclassOf<AActor> GetRandomRoom();
    TSubclassOf<AActor> GetRandomCorridor();

};