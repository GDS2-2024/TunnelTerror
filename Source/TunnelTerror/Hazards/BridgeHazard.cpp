// Fill out your copyright notice in the Description page of Project Settings.


#include "BridgeHazard.h"

#include "Components/BoxComponent.h"

ABridgeHazard::ABridgeHazard()
{
}

void ABridgeHazard::OnPlayerInfected()
{
	if (bIsSabotaged) return;
	SetSabotagableVisual(true);
}

void ABridgeHazard::BeginPlay()
{
	Super::BeginPlay();
}

void ABridgeHazard::Fall_Implementation()
{
	if (bIsSabotaged) return;
	bIsSabotaged = true;
	SetSabotagableVisual(false);
	
	FallVisual();
}