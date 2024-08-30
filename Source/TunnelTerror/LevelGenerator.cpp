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

URoomComponent* ALevelGenerator::SpawnRoom(int32 CurrentI, int32 CurrentJ, TSubclassOf<AActor> ActorToSpawn, bool isX)
{
    UE_LOG(LogTemp, Warning, TEXT("spawning room at: %d, J: %d"), CurrentI, CurrentJ);
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

    return RC;
}

void ALevelGenerator::SpawnMap(int32 GridWidth, int32 GridHeight)
{
    int32 CurrentI = 50;
    int32 CurrentJ = 0;
    int32 NextI = 0;
    int32 NextJ = 0;
    TSubclassOf<AActor> ActorToSpawnNext = nullptr;

    LastActor = Rooms[0];
    URoomComponent* RC = SpawnRoom(CurrentI, CurrentJ, LastActor.Actor, true);

    for (int i = 0; i < 10; i++)
    {
        bool bFoundRoom = false;

        if (LastActor.Doors[1] == "X") {
            NextI = CurrentI + RC->xDoors[0].Y;
            NextJ = CurrentJ + RC->xDoors[0].X;
        }
        else if (LastActor.Doors[1] == "Y") {
            NextI = CurrentI + RC->yDoors[0].Y;
            NextJ = CurrentJ + RC->yDoors[0].X;
        }

        TArray<FRoom> options = (i % 2 == 0) ? Corridors : Rooms;

        while (options.Num() > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, options.Num() - 1);
            ActorToSpawnNext = options[RandomIndex].Actor;
            const FRoom& NextActor = options[RandomIndex];

            if (LastActor.Doors[1] != NextActor.Doors[0])
            {
                options.RemoveAt(RandomIndex);
                continue;
            }

            AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawnNext);
            URoomComponent* NextRC = SpawnedActor->GetComponentByClass<URoomComponent>();
            SpawnedActor->Destroy();

            if (NextRC) {
                UE_LOG(LogTemp, Warning, TEXT("yay"));
            }

            if (CanPlaceRoom(NextI, NextJ, NextRC))
            {
                RC = SpawnRoom(NextI, NextJ, ActorToSpawnNext, true);
                UE_LOG(LogTemp, Warning, TEXT("Room spawned: %s"), *ActorToSpawnNext->GetName());

                CurrentI = NextI;
                CurrentJ = NextJ;
                LastActor = NextActor;
                bFoundRoom = true;
                break;
            }
            else
            {
                options.RemoveAt(RandomIndex);
            }
        }

        if (!bFoundRoom)
        {
            UE_LOG(LogTemp, Error, TEXT("Could not find a suitable room or corridor to spawn."));
            return;
        }
    }
}

bool ALevelGenerator::CanPlaceRoom(int32 CurrentI, int32 CurrentJ, URoomComponent* RC)
{
    if (RC == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("URoomComponent is nullptr"));
        return false;
    }

    if (RC->gridSpaces.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("URoomComponent->gridSpaces is empty"));
        return false;
    }

    for (FVector space : RC->gridSpaces)
    {
        int32 i = CurrentI + static_cast<int32>(space.X);
        int32 j = CurrentJ + static_cast<int32>(space.Y);

        if (i < 0 || i >= Height || j < 0 || j >= Width || Grid[j].BoolValues[i])
        {
            return false;
        }
    }

    return true;
}

    


