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
	if (GetLocalRole() == ROLE_Authority) // Only the server should handle the pickup
	{
		//UE_LOG(LogTemp, Log, TEXT("On Pickup BEGIN Overlap (SERVER)"))
		if (ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(OtherActor))
		{
			if (Player->GetIsInfected()) { // torches can only be interacted by infected
				Player->CollidedPickup = this;
				if (Player->IsLocallyControlled())
				{
					Player->OnRep_CollidedPickup();
				}
			}
		}
	}
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
		if ((torch->GetActorLocation() - GetActorLocation()).Length() <= 5000) {
			torch->TurnOff();
		}
	}
}

