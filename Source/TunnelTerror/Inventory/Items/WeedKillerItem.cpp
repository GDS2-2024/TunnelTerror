// Fill out your copyright notice in the Description page of Project Settings.


#include "WeedKillerItem.h"
#include "InfectionTrap.h"
#include "NiagaraComponent.h"
#include "TunnelTerror/TunnelTerrorCharacter.h"

AWeedKillerItem::AWeedKillerItem()
{
}

void AWeedKillerItem::UseItem()
{
	Super::UseItem();
	bIsActivated = true;
	OnUseItemVisual();
}

void AWeedKillerItem::ReleaseUseItem()
{
	Super::ReleaseUseItem();
	bIsActivated = false;
	OnReleaseItemVisual();
}

void AWeedKillerItem::BeginPlay()
{
	Super::BeginPlay();
}

void AWeedKillerItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActivated)
	{
		TArray<AActor*> Results;
		GetOverlappingActors(Results,AInfectionTrap::StaticClass());

		for (AActor* trap : Results)
		{
			AInfectionTrap* trap2 = Cast<AInfectionTrap>(trap);
			ATunnelTerrorCharacter* character = Cast<ATunnelTerrorCharacter>(Player);
			character->KillPlant(trap2);
		}
	}
}

void AWeedKillerItem::OnUseItemVisual_Implementation()
{
	OnUseItem();
}

void AWeedKillerItem::OnReleaseItemVisual_Implementation()
{
	OnReleaseUseItem();
}