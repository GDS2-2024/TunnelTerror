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
