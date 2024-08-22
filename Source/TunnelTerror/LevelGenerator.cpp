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

void ALevelGenerator::SpawnMap(int32 GridWidth, int32 GridHeight)
{
    const float CellSize = 500.0f;
    bool bSpawnRoom = true;

    int32 CurrentI = 0;
    int32 CurrentJ = 0;

    while (CurrentI < GridHeight && CurrentJ < GridWidth)
    {
        if (!Grid[CurrentI].BoolValues[CurrentJ])
        {
            FVector SpawnLocation(CurrentJ * CellSize, CurrentI * CellSize, 0.0f);
            int32 RandomNumber = FMath::RandRange(1, 2);

            switch (RandomNumber) {
                case 1:
                    ActorToSpawnNext = bSpawnRoom ? Room1 : Corridor1;
                    break;
                case 2:
                    ActorToSpawnNext = bSpawnRoom ? Room2 : Corridor1;
                    break;
            }

            UE_LOG(LogTemp, Log, TEXT("Spawning actor at (%d, %d)"), CurrentI, CurrentJ);

            AActor* RoomSpawned = GetWorld()->SpawnActor<AActor>(ActorToSpawnNext, SpawnLocation, FRotator::ZeroRotator);
            if (!RoomSpawned)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn actor!"));
                return; 
            }

            URoomComponent* RC = RoomSpawned->FindComponentByClass<URoomComponent>();
            if (!RC)
            {
                UE_LOG(LogTemp, Error, TEXT("URoomComponent not found on the spawned actor!"));
                return; 
            }

            for (int32 x = CurrentI; x < CurrentI + RC->Height; x++) 
            {
                for (int32 y = CurrentJ; y < CurrentJ + RC->Width; y++) 
                {
                    if (x < GridHeight && y < GridWidth)
                    {
                        Grid[x].BoolValues[y] = true;

                        FVector Place(y * CellSize, x * CellSize, 400.0f); 

                        DrawDebugSphere(GetWorld(), Place, 50.0f, 12, FColor::Red, true, -1.0f, 0, 2.0f);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Skipping out-of-bounds grid index (%d, %d)"), x, y);
                    }
                }
            }

            
            int32 NextI = CurrentI + RC->xDoors[1].Y; 
            int32 NextJ = CurrentJ + RC->xDoors[1].X;


            if (NextI >= 0 && NextJ >= 0 && NextI < GridHeight && NextJ < GridWidth && !Grid[NextI].BoolValues[NextJ])
            {
                CurrentI = NextI;
                CurrentJ = NextJ;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Next spawn location invalid or already occupied: (%d, %d)"), NextI, NextJ);
                return; 
            }

            bSpawnRoom = !bSpawnRoom;
        }
        else
        {
            bool bFoundNextSpot = false;
            for (int32 i = CurrentI; i < GridHeight; ++i)
            {
                for (int32 j = (i == CurrentI ? CurrentJ + 1 : 0); j < GridWidth; ++j)
                {
                    if (!Grid[i].BoolValues[j])
                    {
                        CurrentI = i;
                        CurrentJ = j;
                        bFoundNextSpot = true;
                        break;
                    }
                }
                if (bFoundNextSpot) break;
            }
            if (!bFoundNextSpot) break; 
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Map spawning completed."));
}

