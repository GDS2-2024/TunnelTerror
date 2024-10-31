// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Public/TunnelTerrorGameState.h"
#include "TunnelTerrorGameMode.generated.h"

UCLASS(minimalapi)
class ATunnelTerrorGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATunnelTerrorGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;
};



