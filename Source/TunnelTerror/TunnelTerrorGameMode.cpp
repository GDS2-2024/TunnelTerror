// Copyright Epic Games, Inc. All Rights Reserved.

#include "TunnelTerrorGameMode.h"
#include "TunnelTerrorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATunnelTerrorGameMode::ATunnelTerrorGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void ATunnelTerrorGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (ATunnelTerrorGameState* CurrentGameState = GetGameState<ATunnelTerrorGameState>())
    {
        if (ATunnelTerrorCharacter* Character = Cast<ATunnelTerrorCharacter>(NewPlayer->GetPawn()))
        {
            CurrentGameState->AddPlayer(Character);
        }
    }
}

void ATunnelTerrorGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    if (ATunnelTerrorGameState* CurrentGameState = GetGameState<ATunnelTerrorGameState>())
    {
        if (ATunnelTerrorCharacter* Character = Cast<ATunnelTerrorCharacter>(Exiting->GetPawn()))
        {
            CurrentGameState->RemovePlayer(Character);
        }
    }
}
