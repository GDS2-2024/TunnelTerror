// Fill out your copyright notice in the Description page of Project Settings.


#include "BridgeSabotager.h"

#include "BridgeHazard.h"
#include "EngineUtils.h"

void ABridgeSabotager::OnInteract_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Test 1"));
	if (bIsSabotaged)
		return;

	for (TActorIterator<ABridgeHazard> It(GetWorld()); It; ++It)
	{
		ABridgeHazard* Bridge = *It;
		if ((Bridge->GetActorLocation() - GetActorLocation()).Length() <= 2000) {
			Bridge->Fall();
		}
	}

	for (TActorIterator<ABridgeSabotager> It(GetWorld()); It; ++It)
	{
		ABridgeSabotager* BridgeSabotager = *It;
		if ((BridgeSabotager->GetActorLocation() - GetActorLocation()).Length() <= 2000) {
			BridgeSabotager->OnBridgeSabotaged();
		}
	}
}

void ABridgeSabotager::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo)
{
	if (bIsSabotaged) return;
	if (ATunnelTerrorCharacter* Player = Cast<ATunnelTerrorCharacter>(OtherActor))
		if (!Player->GetIsInfected()) return;
	
	Super::OnPickupBeginOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, HitInfo);
}

void ABridgeSabotager::OnBridgeSabotaged()
{
	bIsSabotaged = true;
}
