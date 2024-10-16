// Fill out your copyright notice in the Description page of Project Settings.


#include "TorchHazard.h"
#include <Components/PointLightComponent.h>
#include <TunnelTerror/TunnelTerrorCharacter.h>
#include "EngineUtils.h"

ATorchHazard::ATorchHazard()
{
	PointLight2 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight2"));
	PointLight2->SetupAttachment(GetRootComponent());
}

void ATorchHazard::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	if (ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(OtherActor))
		if (!Player->GetIsInfected()) return;
	if (bIsOff) return;
	
	Super::OnPickupBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, HitInfo);
}

void ATorchHazard::OnPlayerInfected()
{
	if (bIsOff) return;
	SetSabotagableVisual(true);
}

void ATorchHazard::TurnOff_Implementation()
{
	bIsOff = true;
	TurnOffBP();
	SetSabotagableVisual(false);
}

void ATorchHazard::OnInteract()
{
	if (bIsOff) return;

	// server function
	TurnOff();

	for (TActorIterator<ATorchHazard> It(GetWorld()); It; ++It)
	{
		ATorchHazard* torch = *It;
		if ((torch->GetActorLocation() - GetActorLocation()).Length() <= SabotageRange) {
			torch->TurnOff();
		}
	}
}

