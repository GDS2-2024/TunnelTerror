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
    bool bSpawnRoom = true;

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
    URoomComponent* RC = SpawnRoom(0, 0, Room1);

    for (int i = 1; i < RC->xDoors.Num(); i++) {
        SpawnRoom(RC->xDoors[i].Y, RC->xDoors[i].X, Corridor1);
    }
}

    /*while (CurrentI < GridHeight && CurrentJ < GridWidth)
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

            AActor* RoomSpawned = GetWorld()->SpawnActor<AActor>(ActorToSpawnNext, SpawnLocation, FRotator::ZeroRotator);
            if (!RoomSpawned)
            {
                UE_LOG(LogTemp, Error, TEXT("Can't spawn room"));
                return; 
            }

            URoomComponent* RC = RoomSpawned->FindComponentByClass<URoomComponent>();
            if (!RC)
            {
                UE_LOG(LogTemp, Error, TEXT("No URoomComponent"));
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
                        UE_LOG(LogTemp, Warning, TEXT("out of bounds"));
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
                UE_LOG(LogTemp, Warning, TEXT("Can't spawn here"));
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
    }*/


