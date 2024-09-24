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
	
	Super::OnPickupBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, HitInfo);
}

void ATorchHazard::TurnOff_Implementation()
{
	PointLight2->SetVisibility(false);
}
void ATorchHazard::OnInteract_Implementation()
{
	TurnOff();

	for (TActorIterator<ATorchHazard> It(GetWorld()); It; ++It)
	{
		ATorchHazard* torch = *It;
		if ((torch->GetActorLocation() - GetActorLocation()).Length() <= SabotageRange) {
			torch->TurnOff();
		}
	}
}

