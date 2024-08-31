// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazard.h"

// Sets default values
AHazard::AHazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHazard::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("IT STARTED OMG"));
	if (UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(GetRootComponent()))
	{
		UE_LOG(LogTemp, Display, TEXT("BRUH"));
		mesh->OnComponentBeginOverlap.AddDynamic(this, &AHazard::OnHazardOverlap);
	}
}

void AHazard::OnHazardOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	//ATunnelTerrorCharacter* character;
}

// Called every frame
void AHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}