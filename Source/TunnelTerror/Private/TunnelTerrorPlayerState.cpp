// Fill out your copyright notice in the Description page of Project Settings.


#include "TunnelTerrorPlayerState.h"
#include "Net/UnrealNetwork.h"
#include <TunnelTerror/TunnelTerrorCharacter.h>

#include "EngineUtils.h"
#include "TunnelTerror/TunnelTerror.h"
#include "TunnelTerror/Hazards/TorchHazard.h"
#include "TunnelTerror/Hazards/BridgeHazard.h"

void ATunnelTerrorPlayerState::SetIsInfected(const bool bNewInfected)
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Error, TEXT("ATunnelTerrorPlayerState::SetIsInfected should only be called from the server!"));
        return;
    }
    //MARK_PROPERTY_DIRTY_FROM_NAME(ATunnelTerrorPlayerState, bIsInfected, this); // errored, so i commented it out
    bIsInfected = bNewInfected;
    OnRep_bIsInfected();
}

// Gets called on the server and client whenever they get infected
void ATunnelTerrorPlayerState::OnRep_bIsInfected()
{
    if (GetPawn()->IsLocallyControlled())
    {
        // make bridges and torches appear sabotagable
        for (TActorIterator<ATorchHazard> It(GetWorld()); It; ++It)
        {
            ATorchHazard* torch = *It;
            torch->OnPlayerInfected();
        }
		for (TActorIterator<ABridgeHazard> It(GetWorld()); It; ++It)
        {
            ABridgeHazard* bridge = *It;
            bridge->OnPlayerInfected();
        }
    }

    if (ATunnelTerrorCharacter* Character = Cast<ATunnelTerrorCharacter>(GetPawn()))
    {
        Character->OnInfected();
    }
}

void ATunnelTerrorPlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    if (IsValid(PlayerState))
    {
        ATunnelTerrorPlayerState* TunnelTerrorPlayerState = Cast<ATunnelTerrorPlayerState>(PlayerState);
        if (IsValid(TunnelTerrorPlayerState))
        {
            TunnelTerrorPlayerState->bIsInfected = bIsInfected;
        }
    }
}

void ATunnelTerrorPlayerState::OverrideWith(class APlayerState* PlayerState)
{
    Super::OverrideWith(PlayerState);

    if (IsValid(PlayerState))
    {
        ATunnelTerrorPlayerState* TunnelTerrorPlayerState = Cast<ATunnelTerrorPlayerState>(PlayerState);
        if (IsValid(TunnelTerrorPlayerState))
        {
            bIsInfected = TunnelTerrorPlayerState->IsInfected();
        }
    }
}

void ATunnelTerrorPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    FDoRepLifetimeParams SharedParams;
    SharedParams.bIsPushBased = true;

    DOREPLIFETIME_WITH_PARAMS_FAST(ATunnelTerrorPlayerState, bIsInfected, SharedParams);
}