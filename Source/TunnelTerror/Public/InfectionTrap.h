// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InfectionTrap.generated.h"

class ATunnelTerrorCharacter;
class USphereComponent;

UCLASS()
class TUNNELTERROR_API AInfectionTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInfectionTrap();

	UFUNCTION(BlueprintCallable)
	void InfectPlayer(ATunnelTerrorCharacter* TargetCharacter);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlantDie();

	UFUNCTION(BlueprintImplementableEvent)
	void DeathAnim();

	UFUNCTION(BlueprintImplementableEvent)
	void Snap();

	UFUNCTION(BlueprintImplementableEvent)
	void StopSnap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	float despawnTimer;

	UPROPERTY()
	float activateTimer;

	UPROPERTY()
	float activate;

	UPROPERTY(BlueprintReadWrite)
	bool bCanTimeOut;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	ATunnelTerrorCharacter* Character;
};
