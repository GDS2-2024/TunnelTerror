// Fill out your copyright notice in the Description page of Project Settings.


#include "BridgeHazard.h"

#include "Components/BoxComponent.h"

ABridgeHazard::ABridgeHazard()
{
	FallTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FallTrigger"));
	FallTrigger->SetupAttachment(GetRootComponent());
}

bool ABridgeHazard::IsSabotaged()
{
	return bIsSabotaged;
}

void ABridgeHazard::SetIsSabotaged(bool sabotaged)
{
	bIsSabotaged = sabotaged;
}

void ABridgeHazard::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		FallTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABridgeHazard::OnFallTriggerOverlap);
	}
}

void ABridgeHazard::OnFallTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	Fall();
}

void ABridgeHazard::Fall_Implementation()
{
	FallVisual();
}

void ABridgeHazard::FallVisual_Implementation()
{
	
}
