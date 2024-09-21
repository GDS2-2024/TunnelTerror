// Fill out your copyright notice in the Description page of Project Settings.


#include "Stalactite.h"
#include "Components/BoxComponent.h"

AStalactite::AStalactite() 
{
	FallTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FallTrigger"));
	FallTrigger->SetupAttachment(GetRootComponent());
}

void AStalactite::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		FallTrigger->OnComponentBeginOverlap.AddDynamic(this, &AStalactite::OnFallTriggerOverlap);
	}
}

void AStalactite::OnFallTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	Fall();
}

void AStalactite::Fall_Implementation()
{
	OnFall();
}

void AStalactite::OnFall_Implementation()
{
	Killbox->SetSimulatePhysics(true);
}
