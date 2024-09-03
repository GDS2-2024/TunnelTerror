// Fill out your copyright notice in the Description page of Project Settings.


#include "TunnelTerrorPlayerState.h"
#include "Net/UnrealNetwork.h"
#include <TunnelTerror/TunnelTerrorCharacter.h>

void ATunnelTerrorPlayerState::SetIsInfected(const bool bNewInfected)
{
    //MARK_PROPERTY_DIRTY_FROM_NAME(ATunnelTerrorPlayerState, bIsInfected, this); // errored, so i commented it out
    bIsInfected = bNewInfected;
}

void ATunnelTerrorPlayerState::OnRep_bIsInfected()
{

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