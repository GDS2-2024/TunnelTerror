#include "LevelGenerator.h"
#include "RoomComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority()) 
    {
        Seed = FMath::Rand();
        OnRep_Seed(); 
    }

    FMath::RandInit(Seed);
    InitializeGrid(Width, Height);
    SpawnPath(1, EntranceRoom, 50, 0, true);
    UE_LOG(LogTemp, Error, TEXT("Rooms: %d"), rooms);
    
}

void ALevelGenerator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ALevelGenerator, Seed);
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALevelGenerator::OnRep_Seed()
{
    FMath::RandInit(Seed);
    InitializeGrid(Width, Height);
    SpawnPath(1, EntranceRoom, 50, 0, true);
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

URoomComponent* ALevelGenerator::SpawnRoom(int32 CurrentI, int32 CurrentJ, TSubclassOf<AActor> ActorToSpawn, bool start)
{
    UE_LOG(LogTemp, Warning, TEXT("spawning room at: %d, J: %d"), CurrentI, CurrentJ);
    const float CellSize = 500.0f;

    FVector SpawnLocation(CurrentJ * CellSize, CurrentI * CellSize, 0.0f);
    AActor* RoomSpawned = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, FRotator::ZeroRotator);
    URoomComponent* RC = RoomSpawned->GetComponentByClass<URoomComponent>();

    //spawn items in the rooms
    if (!RC->SpawnLocations.IsEmpty()) {
        //for (int n = 0; n < RC->SpawnLocations.Num(); n++) {
            int32 rand = FMath::RandRange(0, RC->SpawnLocations.Num() - 1);
            FVector location = RoomSpawned->GetActorLocation() + RC->SpawnLocations[rand];
            AActor* ItemSpawned = GetWorld()->SpawnActor<AActor>(PickupItem, location, FRotator::ZeroRotator);
        //}
    }

    for (FVector space : RC->gridSpaces)
    {
        int32 i = CurrentI + static_cast<int32>(space.X);
        int32 j = CurrentJ + static_cast<int32>(space.Y);


        if (i >= 0 && i < Height && j >= 0 && j < Width)
        {
            Grid[j].BoolValues[i] = true;

            FVector Place(j * CellSize, i * CellSize, 400.0f);
            //DrawDebugSphere(GetWorld(), Place, 50.0f, 12, FColor::Red, true, -1.0f, 0, 2.0f);
        }
        else
        {

            UE_LOG(LogTemp, Warning, TEXT("Out of bounds"));
        }
    }

    return RC;
}

void ALevelGenerator::SpawnPath(int32 LastDoor, FRoom StartRoom, int32 CurrentI, int32 CurrentJ, bool start)
{
    int32 NextI = 0;
    int32 NextJ = 0;
    rooms = 0;
    TSubclassOf<AActor> ActorToSpawnNext = nullptr;
    URoomComponent* RC = nullptr;
    
    addPath++;
    LastActor = StartRoom;
    RC = SpawnRoom(CurrentI, CurrentJ, LastActor.Actor, true);
    rooms += 1;

    for (int i = 0; i < 50; i++)
    {
        bool bFoundRoom = false;

        if (LastActor.Doors.Num() > 2) {
            for (int j = 2; j < LastActor.Doors.Num(); j++) {
                FPlace NewPlace;
                NewPlace.Add(LastActor, CurrentI, CurrentJ, j);
                if (!Doorways.Contains(NewPlace)) {
                    Doorways.Add(NewPlace);
                }
            }
        }

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

            if (NextRC && CanPlaceRoom(NextI, NextJ, NextRC))
            {
                RC = SpawnRoom(NextI, NextJ, ActorToSpawnNext, true);
                rooms += 1;
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
            break;
        }

        
    }

    number = Doorways.Num();
    if (Doorways.Num() > 0) {
        for (int n = 0; n < number; n++) {
            SpawnAnotherPath(Doorways[n]);
        }
    }
}

void ALevelGenerator::SpawnAnotherPath(FPlace place) 
{
    UE_LOG(LogTemp, Error, TEXT("Reached"));
    int32 NextI = 0;
    int32 NextJ = 0;
    int32 CurrentI = place.currentI;
    int32 CurrentJ = place.currentJ;
    int32 LastDoor = place.lastDoor;
    rooms = 0;
    TSubclassOf<AActor> ActorToSpawnNext = nullptr;
    URoomComponent* RC = nullptr;

    LastActor = place.Room;
    AActor* spawned = GetWorld()->SpawnActor<AActor>(place.Room.Actor);
    RC = spawned->GetComponentByClass<URoomComponent>();
    spawned->Destroy();


    for (int i = 0; i < 50; i++)
    {
        bool bFoundRoom = false;

        if (LastActor.Doors.Num() > 2) {
            for (int j = 2; j < LastActor.Doors.Num(); j++) {
                FPlace NewPlace;
                NewPlace.Add(LastActor, CurrentI, CurrentJ, j);
                if (!Doorways.Contains(NewPlace)) {
                    Doorways.Add(NewPlace);
                    number++;
                }
            }
        }

        if (i != 0) {
            LastDoor = 1;
        }

        if (LastActor.Doors[LastDoor] == "X") {
            NextI = CurrentI + RC->xDoors[LastDoor-1].Y;
            NextJ = CurrentJ + RC->xDoors[LastDoor-1].X;
        }
        else if (LastActor.Doors[LastDoor] == "Y") {
            NextI = CurrentI + RC->yDoors[LastDoor-1].Y;
            NextJ = CurrentJ + RC->yDoors[LastDoor-1].X;
        }

        UE_LOG(LogTemp, Warning, TEXT("Current LastDoor: %d"), LastDoor);
        UE_LOG(LogTemp, Warning, TEXT("NextI: %d, NextJ: %d"), NextI, NextJ);
        //FVector Place(NextJ * 500.0f, NextI * 500.0f, 400.0f);
        //DrawDebugSphere(GetWorld(), Place, 50.0f, 12, FColor::Green, true, -1.0f, 0, 2.0f);


        TArray<FRoom> options = (i % 2 == 0) ? Corridors : Rooms;

        while (options.Num() > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, options.Num() - 1);
            ActorToSpawnNext = options[RandomIndex].Actor;
            const FRoom& NextActor = options[RandomIndex];

            if (LastActor.Doors[LastDoor] != NextActor.Doors[0])
            {
                options.RemoveAt(RandomIndex);
                continue;
            }

            AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawnNext);
            URoomComponent* NextRC = SpawnedActor->GetComponentByClass<URoomComponent>();
            SpawnedActor->Destroy();

            if (NextRC && CanPlaceRoom(NextI, NextJ, NextRC))
            {
                RC = SpawnRoom(NextI, NextJ, ActorToSpawnNext, true);
                rooms += 1;
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
            UE_LOG(LogTemp, Error, TEXT("No rooms fit here second path"));
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

    


