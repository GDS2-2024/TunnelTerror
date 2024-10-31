// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleSpawner.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASampleSpawner::ASampleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASampleSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())  // Ensure this only runs on the server
	{
		SpawnNewPickup();
		bTimerActive = false;
	}
}

// Called every frame
void ASampleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Only execute the following code on the server
	if (!HasAuthority())
	{
		return;
	}
	
	if (CurrentSample == nullptr && !bTimerActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current sample is null, starting the 1-minute timer"));
		// Start a 1-minute timer to spawn a new pickup
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASampleSpawner::SpawnNewPickup, 60.0f, false);
		bTimerActive = true;
	} 
}

void ASampleSpawner::SpawnNewPickup_Implementation()
{
	// Ensure no duplicate pickups
	if (CurrentSample == nullptr && HasAuthority())  
	{
		CurrentSample = GetWorld()->SpawnActor<AItemPickup>(SampleClass);
		if (CurrentSample)
		{
			CurrentSample->SetActorTransform(GetActorTransform());
			// Bind OnDestroyed event
			CurrentSample->OnDestroyed.AddDynamic(this, &ASampleSpawner::OnSampleDestroyed);
			UE_LOG(LogTemp, Warning, TEXT("Spawning New Sample and binding OnDestroyed delegate"));
		}

		// Reset timer state
		bTimerActive = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentSample is not null during SpawnNewPickup, skipping spawn"));
	}
}


void ASampleSpawner::OnSampleDestroyed(AActor* DestroyedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSampleDestroyed called for: %s"), *DestroyedActor->GetName());

	if (HasAuthority() && DestroyedActor == CurrentSample)
	{
		UE_LOG(LogTemp, Warning, TEXT("Authority confirmed, setting CurrentSample to nullptr"));
		CurrentSample = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnSampleDestroyed called, but authority check or actor match failed"));
	}
}

void ASampleSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASampleSpawner, CurrentSample);
}
