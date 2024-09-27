// Fill out your copyright notice in the Description page of Project Settings.


#include "TunnelTerrorGameState.h"

ATunnelTerrorGameState::ATunnelTerrorGameState()
{
	bGameTime = true;
	chaosTime = 30.0f;
	bChaosTime = false;
}

void ATunnelTerrorGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bChaosTime == true && HasAuthority())
	{
		chaosTime -= DeltaTime;
	}
}

void ATunnelTerrorGameState::StartChaosTimer()
{
	bChaosTime = true;
	bGameTime = false;
	UE_LOG(LogTemp, Log, TEXT("Chaos Timer Started"));
}