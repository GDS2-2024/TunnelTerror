#include "LevelGenerator.h"
#include "RoomComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
    Super::BeginPlay();
    InitializeGrid(Width, Height);
    SpawnMap(Width, Height);
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALevelGenerator::InitializeGrid(int32 GridWidth, int32 GridHeight)
{
    Grid.Empty();

    for (int32 RowIndex = 0; RowIndex < GridHeight; ++RowIndex)
    {
        FGridRow NewRow;
        NewRow.BoolValues.SetNumZeroed(GridWidth);

        for (int32 ColIndex = 0; ColIndex < GridWidth; ++ColIndex)
        {
            NewRow.Add(ColIndex, false);
        }

        Grid.Add(NewRow);
    }

    UE_LOG(LogTemp, Log, TEXT("Grid initialized with dimensions %dx%d."), GridWidth, GridHeight);
}

URoomComponent* ALevelGenerator::SpawnRoom(int32 CurrentI, int32 CurrentJ, TSubclassOf<AActor> ActorToSpawn)
{
    const float CellSize = 500.0f;
    

    FVector SpawnLocation(CurrentJ * CellSize, CurrentI * CellSize, 0.0f);
    AActor* RoomSpawned = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, FRotator::ZeroRotator);
    URoomComponent* RC = RoomSpawned->GetComponentByClass<URoomComponent>();

    for (FVector space : RC->gridSpaces)
    {
        int32 i = CurrentI + static_cast<int32>(space.X);
        int32 j = CurrentJ + static_cast<int32>(space.Y);


        if (i >= 0 && i < Height && j >= 0 && j < Width)
        {
            Grid[j].BoolValues[i] = true;

            FVector Place(j * CellSize, i * CellSize, 400.0f);
            DrawDebugSphere(GetWorld(), Place, 50.0f, 12, FColor::Red, true, -1.0f, 0, 2.0f);
        }
        else
        {

            UE_LOG(LogTemp, Warning, TEXT("Out of bounds: i = %d, j = %d"), i, j);
        }
    }

    LastX = RC->xDoors[1].X;
    LastY = RC->xDoors[1].Y;

    return RC;
}

void ALevelGenerator::SpawnMap(int32 GridWidth, int32 GridHeight)
{
    bool bSpawnRoom = false;
    int32 CurrentI = 0;
    int32 CurrentJ = 0;

    URoomComponent* RC = SpawnRoom(CurrentI, CurrentJ, Room1);

    for (int n = 0; n < 5; n++)
    {
        int32 NextI = CurrentI + RC->xDoors[1].Y;
        int32 NextJ = CurrentJ + RC->xDoors[1].X; 

        TSubclassOf<AActor> ActorToSpawnNext = bSpawnRoom ? Corridor1 : Room1;
        RC = SpawnRoom(NextI, NextJ, ActorToSpawnNext);

        CurrentI = NextI;
        CurrentJ = NextJ;

        bSpawnRoom = !bSpawnRoom;
    }
}

    


