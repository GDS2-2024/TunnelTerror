// Copyright Epic Games, Inc. All Rights Reserved.

#include "TunnelTerrorCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include <TunnelTerrorPlayerState.h>

#include "ElevatorEscape.h"
#include "Hazards/TorchHazard.h"
#include "InfectionTrap.h"
#include "Hazards/BridgeSabotager.h"


//////////////////////////////////////////////////////////////////////////
// ATunnelTerrorCharacter

ATunnelTerrorCharacter::ATunnelTerrorCharacter()
{
	bReplicates = true;
	
	// Character is not infected at the start
	bIsInSafeZone = false;
	health = 100.0f;

	trapCD = 30.0f;
	trapCDCurrent = 0.0f;

	samples = 0;
	money = 0;

	sporeInfectTime = 10.0f;
	sporeInfectCurrent = 0.0f;

	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	// Create Inventory Component
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Player Inventory"));
	Inventory->SetIsReplicated(true);
}

void ATunnelTerrorCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(InventoryMappingContext, 0);
		}

		if(IsLocallyControlled())
		{
			PlayerHUD = CreateWidget<UPlayerHUD>(PlayerController, PlayerHUDClass);
			PlayerHUD->AddToPlayerScreen();
		}
	}
	
}

void ATunnelTerrorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (trapCDCurrent > 0.0f)
	{
		trapCDCurrent -= DeltaTime;
		if (trapCDCurrent < 0.0f)
		{
			trapCDCurrent = 0.0f;
		}
	}

	if (bSporesInfecting)
	{
		sporeInfectCurrent -= DeltaTime;
		if (sporeInfectCurrent <= 0.0f)
		{
			this->DecreaseHealth(100.0f);
		}
	}
	else
	{
		sporeInfectCurrent = sporeInfectTime - 0.1f;
	}
	if (CollidedPickup)
	{
		if (CollidedPickup->PickupName == "CrystalPickup")
        {
        	if (Inventory->GetPlayersPickaxe())
        	{
        		if (Inventory->GetPlayersPickaxe()->HasReachedMiningThreshold())
        		{
        			ServerRemoveCrystals();
        			ClientAddMoney();
        		}
        	}
        }
	}
	
}

void ATunnelTerrorCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATunnelTerrorCharacter, Inventory);
	DOREPLIFETIME(ATunnelTerrorCharacter, bIsRagdolled);
	DOREPLIFETIME(ATunnelTerrorCharacter, CollidedPickup);
}

//////////////////////////////////////////////////////////////////////////// Input

void ATunnelTerrorCharacter::OnRagdoll_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Eyy that's nice"));
}

void ATunnelTerrorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::Look);

		//Inventory Selection
		EnhancedInputComponent->BindAction(Slot1, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot1);
		EnhancedInputComponent->BindAction(Slot2, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot2);
		EnhancedInputComponent->BindAction(Slot3, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot3);
		EnhancedInputComponent->BindAction(Slot4, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot4);
		EnhancedInputComponent->BindAction(Slot5, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::SelectSlot5);
		EnhancedInputComponent->BindAction(Scroll, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::ScrollSlots);
		//Item
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Started, this, &ATunnelTerrorCharacter::PressedUseItem);
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Completed, this, &ATunnelTerrorCharacter::ReleasedUseItem);
		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Triggered, this, &ATunnelTerrorCharacter::DropItem);
		//Interactions
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATunnelTerrorCharacter::Interact);
		//PlaceTrap
		EnhancedInputComponent->BindAction(PlaceTrapAction, ETriggerEvent::Started, this, &ATunnelTerrorCharacter::PlaceTrap);
	}
}

void ATunnelTerrorCharacter::MulticastEquipPickaxeAnim_Implementation(bool bEquip)
{
	EquipPickaxeAnim(bEquip);
}

void ATunnelTerrorCharacter::MulticastEquipCompassAnim_Implementation(bool bEquip)
{
	EquipCompassAnim(bEquip);
}

void ATunnelTerrorCharacter::MulticastEquipTorchAnim_Implementation(bool bEquip)
{
	EquipTorchAnim(bEquip);
}

void ATunnelTerrorCharacter::Die()
{
	if (!HasAuthority()) return;
	if (ATunnelTerrorPlayerState* playerState = GetPlayerState<ATunnelTerrorPlayerState>()) {
		playerState->SetIsInfected(true);
		SetIsRagdolled(true);

		FTimerHandle timerHandle = FTimerHandle();
		GetWorldTimerManager().SetTimer(timerHandle, this, &ATunnelTerrorCharacter::Reanimate, 2);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Couldn't cast to ATunnelTerrorPlayerState!"));
	}
}

void ATunnelTerrorCharacter::Reanimate()
{
	if (!HasAuthority()) return;
	SetIsRagdolled(false);
}

void ATunnelTerrorCharacter::SetIsRagdolled(const bool bNewRagdolled)
{
	if (!HasAuthority()) {
		UE_LOG(LogTemp, Error, TEXT("This should only be called from server!"));
		return;
	}
	
	bIsRagdolled = bNewRagdolled;
	OnRagdoll(); // call it directly on the server, clients can just use ReplicatedUsing
}

void ATunnelTerrorCharacter::OnRep_CollidedPickup()
{
	if (IsLocallyControlled())
	{
		if (CollidedPickup)
		{
			CollidedPickup->ShowPrompt(true);
			PreviousPickup = CollidedPickup;
		}
		else
		{
			if (PreviousPickup)
			{
				PreviousPickup->ShowPrompt(false);
				PreviousPickup = nullptr;
			}
		}
	}
}

void ATunnelTerrorCharacter::ServerInteractWithElevator_Implementation(AElevatorEscape* Elevator, int32 Samples)
{
	if (Elevator)
	{
		Elevator->ServerAddSample(Samples);
		RemoveSamplesFromInventory();
	}
}

void ATunnelTerrorCharacter::ServerInteractWithTorch_Implementation(ATorchHazard* Torch)
{
	Torch->OnInteract();
}

void ATunnelTerrorCharacter::ServerInteractWithBridge_Implementation(ABridgeSabotager* BridgeSabotager)
{
	BridgeSabotager->OnInteract();
}

void ATunnelTerrorCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATunnelTerrorCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATunnelTerrorCharacter::SelectSlot1(const FInputActionValue& Value)
{
	PlayerHUD->SetSlotSelection(0); // HUD Changes on client
	Inventory->ServerSetSelectedSlot(0); //Mesh visibility changes on server
}

void ATunnelTerrorCharacter::SelectSlot2(const FInputActionValue& Value)
{
	PlayerHUD->SetSlotSelection(1); // HUD Changes on client
	Inventory->ServerSetSelectedSlot(1); //Mesh visibility changes on server
}

void ATunnelTerrorCharacter::SelectSlot3(const FInputActionValue& Value)
{
	PlayerHUD->SetSlotSelection(2); // HUD Changes on client
	Inventory->ServerSetSelectedSlot(2); //Mesh visibility changes on server
}

void ATunnelTerrorCharacter::SelectSlot4(const FInputActionValue& Value)
{
	PlayerHUD->SetSlotSelection(3); // HUD Changes on client
	Inventory->ServerSetSelectedSlot(3); //Mesh visibility changes on server
}

void ATunnelTerrorCharacter::SelectSlot5(const FInputActionValue& Value)
{
	PlayerHUD->SetSlotSelection(4); // HUD Changes on client
	Inventory->ServerSetSelectedSlot(4); //Mesh visibility changes on server
}

void ATunnelTerrorCharacter::ScrollSlots(const FInputActionValue& Value)
{
	if (Value.Get<float>() < 0) //Scroll Down
	{
		//Increase by 1, wrap around to start
		if (Inventory->SelectedSlotIndex < Inventory->GetMaxSlots()-1)
		{
			PlayerHUD->SetSlotSelection(Inventory->SelectedSlotIndex + 1); //HUD Change on client. Does NOT change SelectedSlotIndex, only references it
			Inventory->ServerSetSelectedSlot(Inventory->SelectedSlotIndex + 1); //Does change SelectedSlotIndex 
		} else
		{
			PlayerHUD->SetSlotSelection(0);  //HUD Change on client. Does NOT change SelectedSlotIndex, only references it
			Inventory->ServerSetSelectedSlot(0); //Does change SelectedSlotIndex
		}
	} else //Scroll Up
	{
		//Decrease by 1, wrap around to end
		if (Inventory->SelectedSlotIndex > 0)
		{
			PlayerHUD->SetSlotSelection(Inventory->SelectedSlotIndex - 1);  //HUD Change on client. Does NOT change SelectedSlotIndex, only references it
			Inventory->ServerSetSelectedSlot(Inventory->SelectedSlotIndex - 1); //Does change SelectedSlotIndex 
		} else
		{
			PlayerHUD->SetSlotSelection(4);  //HUD Change on client. Does NOT change SelectedSlotIndex, only references it
			Inventory->ServerSetSelectedSlot(4); //Does change SelectedSlotIndex 
		}
	}
}

void ATunnelTerrorCharacter::EquipToInventory(AInventoryItem* NewItem)
{
	if (HasAuthority())
	{
		if (NewItem)
		{
			Inventory->ServerAddItem(NewItem);
			ClientAddInventoryUI(NewItem, Inventory->NewestItemSlotIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to add item to inventory because NewItem is null."));
		}
	}
}

void ATunnelTerrorCharacter::ServerSpawnPickup_Implementation(FName PickupName)
{
	AItemPickup* ItemPickup = nullptr;
	int offset = 0.0f;
	
	if (PickupName == "Torch")
	{
		ItemPickup = GetWorld()->SpawnActor<AItemPickup>(TorchPickupClass);
		offset = 10.0f;
	} else if (PickupName == "Compass")
	{
		ItemPickup = GetWorld()->SpawnActor<AItemPickup>(CompassPickupClass);
		offset = -90.0f;
	} else if (PickupName == "Pickaxe")
	{
		ItemPickup = GetWorld()->SpawnActor<AItemPickup>(PickaxePickupClass);
		offset = 50.0f;
	} else if (PickupName == "Plant Sample")
	{
		ItemPickup = GetWorld()->SpawnActor<AItemPickup>(PlantPickupClass);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("No item pickup with given name"))
		return;
	}

	// If the item was spawned successfully
	if (ItemPickup)
	{
		// Set the initial spawn location at the character's location
		FVector SpawnLocation = GetActorLocation();
		ItemPickup->SetActorLocation(SpawnLocation);

		// Perform a line trace down to find the ground
		FVector TraceStart = SpawnLocation;
		FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 10000.0f);
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,ECC_Visibility, TraceParams);
		if (bHit)
		{
			FVector GroundLocation = HitResult.ImpactPoint;
			GroundLocation.Z += offset;
			ItemPickup->SetActorLocation(GroundLocation);
		}
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 5.0f, 0, 5.0f);
	}
}

void ATunnelTerrorCharacter::ServerRemoveItem_Implementation()
{
	// Destroy the Item
	if (!Inventory->InventorySlots[Inventory->SelectedSlotIndex].IsEmpty())
	{
		Inventory->InventorySlots[Inventory->SelectedSlotIndex].Item->Destroy();
	}
	// Remove from inventory
	Inventory->ServerRemoveItem(Inventory->SelectedSlotIndex);
	//Updates anims
	Inventory->ServerSetSelectedSlot(Inventory->SelectedSlotIndex);
	//Update HUD
	if (IsLocallyControlled())
	{
		Inventory->OnRep_InventorySlots();
	}
}

void ATunnelTerrorCharacter::ServerSpawnItem_Implementation(TSubclassOf<AInventoryItem> ItemClass)
{
	AInventoryItem* InventoryItem = GetWorld()->SpawnActor<AInventoryItem>(ItemClass);
	if (InventoryItem)
	{
		InventoryItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "hand_r_socket");
		if (InventoryItem->ItemName.ToString() == "Torch")
		{
			FRotator DesiredRotation(0.0f, -90.0f, 0.0f);
			InventoryItem->SetActorRelativeRotation(DesiredRotation);
		}
		if (InventoryItem->ItemName.ToString() == "Compass")
		{
			FRotator DesiredRotation(180.0f, 0.0f, 90.0f);
			FVector DesiredPos(2.0f,3.0f,0.0f);
			InventoryItem->SetActorRelativeLocation(DesiredPos);
			InventoryItem->SetActorRelativeRotation(DesiredRotation);
			EquipCompassAnim(true);
			MulticastEquipCompassAnim(true);
		}
		if (InventoryItem->ItemName.ToString() == "Pickaxe")
		{
			EquipPickaxeAnim(true);
			MulticastEquipPickaxeAnim(true);
		}
		if (InventoryItem->ItemName.ToString() == "Torch")
		{
			FRotator DesiredRotation(0.0f, -90.0f, 15.0f);
			FVector DesiredPos(1.0f,3.0f,1.0f);
			InventoryItem->SetActorRelativeLocation(DesiredPos);
			InventoryItem->SetActorRelativeRotation(DesiredRotation);
			EquipTorchAnim(true);
			MulticastEquipTorchAnim(true);
		}
		if (InventoryItem->ItemName.ToString() == "Plant Sample") // Plant uses the same anim as compass
		{
			FVector DesiredPos(5.0f,3.0f,-8.0f);
			FRotator DesiredRotation(30.0f, 0.0f, 0.0f);
			InventoryItem->SetActorRelativeLocation(DesiredPos);
			InventoryItem->SetActorRelativeRotation(DesiredRotation);
			EquipCompassAnim(true); // Plant uses the same anim as compass
			MulticastEquipCompassAnim(true); // Plant uses the same anim as compass
		}
		ServerEquipToInventory(InventoryItem);
		if (CollidedPickup)
		{
			CollidedPickup->Destroy();
		}
	}
}

void ATunnelTerrorCharacter::ServerRemoveCrystals_Implementation()
{
	if (CollidedPickup)
	{
		CollidedPickup->Destroy();
	}
}

void ATunnelTerrorCharacter::ClientAddMoney_Implementation() {
	money = money + 3;
	PlayerHUD->SetCurrencyUI(money); 
	UE_LOG(LogTemp, Warning, TEXT("money = %d"), money);
}

void ATunnelTerrorCharacter::ServerEquipToInventory_Implementation(AInventoryItem* InventoryItem)
{
	EquipToInventory(InventoryItem);
}

void ATunnelTerrorCharacter::ClientAddInventoryUI_Implementation(AInventoryItem* NewItem, int32 SlotIndex)
{
	PlayItemPickupSFX();
	if (!PlayerHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerHUD is null in ClientAddInventoryUI"));
		return;
	}

	if (!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is null in ClientAddInventoryUI"));
		return;
	}

	if (NewItem)
	{
		PlayerHUD->SetSlotIcon(SlotIndex, NewItem->InventoryIcon);	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NewItem is null in ClientAddInventoryUI"));
	}
}

void ATunnelTerrorCharacter::ClientRemoveInventoryUI_Implementation(int32 SlotIndex)
{
	if (!PlayerHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerHUD is null in ClientAddInventoryUI"));
		return;
	}

	if (!Inventory)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is null in ClientAddInventoryUI"));
		return;
	}
	
	PlayerHUD->ClearSlotIcon(SlotIndex);
	
}

void ATunnelTerrorCharacter::PressedUseItem(const FInputActionValue& Value)
{
	if (Inventory)
	{
		AInventoryItem* SelectedItem = Inventory->GetSelectedItem();
		if (SelectedItem)
		{
			SelectedItem->UseItem();
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("No selected item in inventory!"));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("Inventory is null!"));
	}	
}

void ATunnelTerrorCharacter::ReleasedUseItem(const FInputActionValue& Value)
{
	if (Inventory)
	{
		AInventoryItem* SelectedItem = Inventory->GetSelectedItem();
		if (SelectedItem)
		{
			SelectedItem->ReleaseUseItem();
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("No selected item in inventory!"));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("Inventory is null!"));
	}	
}

void ATunnelTerrorCharacter::Interact(const FInputActionValue& Value)
{
	// Player Presses 'E' on (Client) while currently colliding with a pickup (Server)
	if (CollidedPickup)
	{
		if (CollidedPickup->CorrespondingItemClass)
		{
			if (Inventory->HasEmptySlot())
			{
				if (CollidedPickup->PickupName == "SamplePickup")
				{
					samples++;
					// Spawn an instance of the item (sample) on the server
					UE_LOG(LogTemp, Log, TEXT("Telling Server to spawn inventory item (sample)"))
						ServerSpawnItem(CollidedPickup->CorrespondingItemClass);
				}
				else {
					// Spawn an instance of the inventory item on the server
					UE_LOG(LogTemp, Log, TEXT("Telling Server to spawn inventory item"))
						ServerSpawnItem(CollidedPickup->CorrespondingItemClass);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't pickup item as inventory is full"));
			}
		}
		else
		{
			if (CollidedPickup->PickupName == "TorchHazard") {
				ATorchHazard* torchHazard = Cast<ATorchHazard>(CollidedPickup);
				ServerInteractWithTorch(torchHazard);
			}
			else if (CollidedPickup->PickupName == "BridgeSabotager")
			{
				ABridgeSabotager* BridgeSabotager = Cast<ABridgeSabotager>(CollidedPickup);
				ServerInteractWithBridge(BridgeSabotager);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("CorrespondingItemClass is NULL"));
			}
		}
	}
	if (ElevatorEscape != nullptr)
	{
		if (!GetIsInfected() && samples > 0)
		{
			ServerInteractWithElevator(ElevatorEscape, samples);
			samples = 0;
		}
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("Move to Elevator to interact"));
	}
}

void ATunnelTerrorCharacter::PlaceTrap(const FInputActionValue& Value)
{
	if (HasAuthority())
	{
		PlaceTrapImplementation();
		MulticastPlaceTrap();
	}
	else
	{
		ServerPlaceTrap();
	}
}

void ATunnelTerrorCharacter::DropItem(const FInputActionValue& Value)
{
	// Spawn the dropped item as a pickup on the ground
	if (Inventory->GetSelectedItem())
	{
		ServerSpawnPickup(Inventory->GetSelectedItem()->ItemName);
	}
	// Remove item from inventory
	ServerRemoveItem();
}

void ATunnelTerrorCharacter::PlaceTrapImplementation()
{
	if (GetIsInfected() && trapCDCurrent == 0) {
		trapCDCurrent = trapCD;

		UE_LOG(LogTemp, Log, TEXT("trap cd set"));

		FVector PlayerLocation = GetActorLocation();
		PlayerLocation.Z = 0.0f;
		FVector ForwardVector = GetActorForwardVector();
		FVector TrapLocation = PlayerLocation + (ForwardVector * 200.0f);

		FRotator PlayerRotation = GetActorRotation();
		FRotator RotationOffset(0.0f, -90.0f, 0.0f);
		FRotator TrapRotation = PlayerRotation + RotationOffset;


		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		GetWorld()->SpawnActor<AInfectionTrap>(TrapBlueprint, TrapLocation, TrapRotation, SpawnParams);
	}
}

void ATunnelTerrorCharacter::ServerPlaceTrap_Implementation()
{
	PlaceTrapImplementation();
}

void ATunnelTerrorCharacter::MulticastPlaceTrap_Implementation()
{
	if (!HasAuthority())
	{
		PlaceTrapImplementation();
	}
}

void ATunnelTerrorCharacter::RemoveSamplesFromInventory()
{
	Inventory->RemoveSamples();
}

void ATunnelTerrorCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ATunnelTerrorCharacter::GetHasRifle()
{
	return bHasRifle;
}

bool ATunnelTerrorCharacter::GetIsInfected()
{
	if (ATunnelTerrorPlayerState* playerState = GetPlayerState<ATunnelTerrorPlayerState>()) {
		return playerState->IsInfected();
	}
	UE_LOG(LogTemp, Error, TEXT("Couldn't cast to ATunnelTerrorPlayerState!"));
	return false;
}

void ATunnelTerrorCharacter::DecreaseHealth(float damage)
{
	UE_LOG(LogTemp, Log, TEXT("Player health: %f"), health);
	if (bIsInSafeZone == false)
	{
		health -= damage;
	}
	UE_LOG(LogTemp, Log, TEXT("Player has taken damage: %f"), damage);

	if (health <= 0)
	{
		Die();
	}
}

void ATunnelTerrorCharacter::StartSporeInfection()
{
	bSporesInfecting = true;
	sporeInfectCurrent = sporeInfectTime;
	UE_LOG(LogTemp, Log, TEXT("Spores are infecting"));
}

void ATunnelTerrorCharacter::EndSporeInfection()
{
	bSporesInfecting = false;
	sporeInfectCurrent = 0.0f;
	UE_LOG(LogTemp, Log, TEXT("Spores have stopped infecting"));
}

void ATunnelTerrorCharacter::MulticastSwingAnim_Implementation(bool swing)
{
	SwingPickaxeAnim(swing);
}

void ATunnelTerrorCharacter::ServerSwingAnim_Implementation(bool swing)
{
	MulticastSwingAnim(swing);
}

void ATunnelTerrorCharacter::SetIsInSafeZone(bool bNewIsInSafeZone)
{
	bIsInSafeZone = bNewIsInSafeZone;
}