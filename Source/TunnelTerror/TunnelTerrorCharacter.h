// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Inventory/InventoryItem.h"
#include "PlayerHUD.h"
#include "Inventory/ItemPickup.h"
#include "ElevatorEscape.h"
#include "TunnelTerrorCharacter.generated.h"

class ABridgeSabotager;
class ATorchHazard;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UInventoryComponent;
class AElevatorEscape;
class AInfectionTrap;

UCLASS(config=Game)
class ATunnelTerrorCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Inventory Input */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputMappingContext* InventoryMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Place Trap Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PlaceTrapAction;

	/** Use Item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UseItemAction;

	/** Drop Item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropItemAction;
	
	/** Inventory Input Actions */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot1;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot2;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot3;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot4;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Slot5;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* Scroll;
	
public:
	ATunnelTerrorCharacter();

protected:
	virtual void BeginPlay();

public:

	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY()
	bool bIsInSafeZone;

	UPROPERTY(BlueprintReadWrite)
	FString causeOfDeath;

	UPROPERTY(blueprintReadWrite)
	float timeAlive;

	/** Player Inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	UInventoryComponent* Inventory;

	UFUNCTION()
	void EquipToInventory(AInventoryItem* NewItem);

	UFUNCTION(BlueprintImplementableEvent)
	void ChangePlayerMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION(Server,Reliable)
	void ServerSpawnItem(TSubclassOf<AInventoryItem> ItemClass);

	UFUNCTION(Server, Reliable)
	void ServerSpawnPickup(FName PickupName);
	
	UFUNCTION(Server, Reliable)
	void ServerRemoveItem();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPickupSound();
	
	UFUNCTION(Server, Reliable)
	void ServerRemoveCrystals();

	UFUNCTION(Client, Reliable)
	void ClientAddMoney();

	UFUNCTION(Client, Reliable)
	void ClientRemoveMoney(int32 amount);
	
	UFUNCTION(Server, Reliable)
	void ServerEquipToInventory(AInventoryItem* InventoryItem);

	void PlaceTrapImplementation();

	UFUNCTION(Server, Reliable)
	void ServerPlaceTrap();

	UFUNCTION(Server, Reliable)
	void MulticastPlaceTrap();

	UFUNCTION(Client, Reliable)
	void ClientAddInventoryUI(AInventoryItem* NewItem, int32 SlotIndex);

	UFUNCTION(Client, Reliable)
	void ClientRemoveInventoryUI(int32 SlotIndex);

	UPROPERTY(VisibleAnywhere)
	float health;

	UPROPERTY(VisibleAnywhere)
	
	float sporeInfectTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float sporeInfectCurrent;

	UPROPERTY(VisibleAnywhere)
	bool bSporesInfecting;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 money;

	UFUNCTION(BlueprintCallable)
	void SetMoneyUI(int32 amount);
	
	/// <summary>
	/// Ragdolls and infects the player
	/// </summary>
	void Die();

	/// <summary>
	/// Unragdolls them after they die
	/// </summary>
	void Reanimate();

	UFUNCTION(BlueprintGetter)
	bool IsRagdolled() const
	{
		return bIsRagdolled;
	}

	void SetIsRagdolled(const bool bNewRagdolled);

	// Die() should be called instead!
	/*UFUNCTION(BlueprintCallable)
	void SetIsInfected(bool bIsNowInfected);*/

	UFUNCTION(BlueprintCallable)
	bool GetIsInfected();

	UFUNCTION()
	void SetIsInSafeZone(bool bNewIsInSafeZone);

	UFUNCTION()
	void DecreaseHealth(float damage, FString newCauseOfDeath);

	UFUNCTION()
	void StartSporeInfection();

	UFUNCTION()
	void EndSporeInfection();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayItemPickupSFX();
	
	// Variable is set by an item pickup
	// Used to reference pickup object when player presses 'E' to interact
	UPROPERTY(ReplicatedUsing = OnRep_CollidedPickup, VisibleAnywhere)
	AItemPickup* CollidedPickup;
	// The previous collided pickup
	UPROPERTY()
	AItemPickup* PreviousPickup;

	UFUNCTION()
	void OnRep_CollidedPickup();

	// Character Mesh Picker
	UPROPERTY(Replicated, BlueprintReadWrite)
	AActor* CollidedCharacterPicker;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CharacterPickerClass;
	UPROPERTY()
	UUserWidget* CharacterPickerUI;
	void ShowCharacterPickerUI();
	UFUNCTION(BlueprintCallable)
	void HideCharacterPickerUI();
	
	UFUNCTION(Server, Reliable)
	void ServerInteractWithElevator(AElevatorEscape* Elevator, int32 Samples);
	UFUNCTION(Server, Reliable)
	void ServerInteractWithTorch(ATorchHazard* Torch);
	UFUNCTION(Server, Reliable)
	void ServerInteractWithBridge(ABridgeSabotager* BridgeSabotager);

	UFUNCTION(Server, Reliable)
	void KillPlant(AInfectionTrap* trap);
	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void PressedUseItem(const FInputActionValue& Value);
	void ReleasedUseItem(const FInputActionValue& Value);
	
	/** Called for inventory input */
	void SelectSlot1(const FInputActionValue& Value);
	void SelectSlot2(const FInputActionValue& Value);
	void SelectSlot3(const FInputActionValue& Value);
	void SelectSlot4(const FInputActionValue& Value);
	void SelectSlot5(const FInputActionValue& Value);
	void ScrollSlots(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void PlaceTrap(const FInputActionValue& Value);

	void DropItem(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRagdoll();
	
	void RemoveSamplesFromInventory();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	AElevatorEscape* ElevatorEscape;

	UPROPERTY()
	UPlayerHUD* PlayerHUD;

	UFUNCTION(BlueprintImplementableEvent)
	void EquipTorchAnim(bool bEquip);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipTorchAnim(bool bEquip);

	UFUNCTION(BlueprintImplementableEvent)
	void EquipWeedKillerAnim(bool bEquip);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipWeedKillerAnim(bool bEquip);
	
	UFUNCTION(BlueprintImplementableEvent)
	void EquipCompassAnim(bool bEquip);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipCompassAnim(bool bEquip);
	
	UFUNCTION(BlueprintImplementableEvent)
	void EquipPickaxeAnim(bool bEquip);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipPickaxeAnim(bool bEquip);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SwingPickaxeAnim(bool bEquip);

	UFUNCTION(Server, Reliable)
	void ServerSwingAnim(bool swing);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSwingAnim(bool swing);
	
private:
	UPROPERTY(ReplicatedUsing = OnRagdoll, BlueprintGetter = IsRagdolled, Replicated)
	bool bIsRagdolled = false;

	UPROPERTY(VisibleAnywhere)
	int samples;

	UPROPERTY(VisibleAnywhere)
	float trapCD;

	UPROPERTY(VisibleAnywhere)
	float trapCDCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AInfectionTrap> TrapBlueprint;

	// Item Pickup classes to spawn pickups
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickups", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AItemPickup> TorchPickupClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickups", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AItemPickup> WeedKillerPickupClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickups", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AItemPickup> CompassPickupClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickups", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AItemPickup> PickaxePickupClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Pickups", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AItemPickup> PlantPickupClass;
	
};

