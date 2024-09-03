// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TunnelTerrorPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TUNNELTERROR_API ATunnelTerrorPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	UPROPERTY(ReplicatedUsing = OnRep_bIsInfected, Category = PlayerState, BlueprintGetter = IsInfected)
	bool bIsInfected;

public:
	void SetIsInfected(const bool bNewInfected);

	UFUNCTION(BlueprintGetter)
	bool IsInfected() const
	{
		return bIsInfected;
	}

	UFUNCTION()
	virtual void OnRep_bIsInfected();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	// Used to copy properties from the current PlayerState to the passed one
	virtual void CopyProperties(class APlayerState* PlayerState);

	// Used to override the current PlayerState with the properties of the passed one
	virtual void OverrideWith(class APlayerState* PlayerState);

};
