// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <TunnelTerror/TunnelTerrorCharacter.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hazard.generated.h"

class UBoxComponent;

UCLASS()
class TUNNELTERROR_API AHazard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHazard();

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Killbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnHazardOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitInfo);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnKill(ATunnelTerrorCharacter* victim);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
