// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Weapon.h"
#include "PFA_IterationTwo/CharacterBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "PFA_IterationTwo/Controllers/CharacterBasePlayerController.h"
#include "PFA_IterationTwo/HUD/CharacterBaseHUD.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (character) {

		if (character->GetCamera()) {

			defaultFOV = character ->GetCamera()->FieldOfView;
			currentFOV = defaultFOV;
		}
	}
	// ...
	
}


void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEquipmentComponent, equippedWeapon);
	DOREPLIFETIME(UEquipmentComponent, bAiming);

}

void UEquipmentComponent::EquipWeapon(AWeapon* weapon)
{
	if (character == nullptr || weapon == nullptr ) {
		return;
	}
	equippedWeapon = weapon;
	equippedWeapon->SetWeaponState(EWeaponState::EWS_Equiped);

	

	const USkeletalMeshSocket* handSocket = character->GetMesh()->GetSocketByName(FName("weapon_rSocket"));
	if (handSocket) {

		handSocket->AttachActor(equippedWeapon, character->GetMesh());

	}

	equippedWeapon->SetOwner(character);

	equippedWeapon->ShowPickUpWidget(false);
	

	if (equippedWeapon) {
		
		UE_LOG(LogTemp, Warning, TEXT("IS THIS SHIT DEBUGGED ?"));
		character->GetCharacterMovement()->bOrientRotationToMovement = false;
		character->bUseControllerRotationYaw = true;

	}

}

void UEquipmentComponent::TraceUnderCrosshair(FHitResult& hitResult) {
	
	FVector2D viewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(viewportSize);
	}

	FVector2D crosshairLocation = FVector2D(viewportSize.X / 2, viewportSize.Y / 2);
	FVector crosshairWorldDirection;
	FVector crosshairWorldLocation;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		crosshairLocation, 
		crosshairWorldLocation,
		crosshairWorldDirection);

	if (bScreenToWorld) {
		
		FVector start = crosshairWorldLocation;
		FVector end = start + crosshairWorldDirection * TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility);

		if (hitResult.GetActor() && hitResult.GetActor()->Implements<UInteractWithCrosshair>()) {

			HUDPackage.crosshairColor = FLinearColor::Red;
		}

		else {

			HUDPackage.crosshairColor = FLinearColor::White;
		}

		
	}
}





// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (character && character->IsLocallyControlled()) {
		SetCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}

	// ...
}

void UEquipmentComponent::SetCrosshairs(float deltaTime) {

	if (character == nullptr || character->Controller == nullptr) return;

	controller = controller == nullptr ? Cast<ACharacterBasePlayerController>(character->Controller) : controller;
	if (controller) {
		HUD = HUD == nullptr ? Cast<ACharacterBaseHUD>(controller->GetHUD()) : HUD;
		if (HUD) {

			
			if (equippedWeapon) {

				HUDPackage.crosshairsLeft = equippedWeapon->crosshairLeft;
				HUDPackage.crosshairsRight = equippedWeapon->crosshairRight;
				HUDPackage.crosshairsTop = equippedWeapon->crosshairTop;
				HUDPackage.crosshairsBottom = equippedWeapon->crosshairBottom;
				HUDPackage.crosshairsCenter = equippedWeapon->crosshairCenters;

			}
			else {
				HUDPackage.crosshairsLeft = nullptr;
				HUDPackage.crosshairsRight = nullptr;
				HUDPackage.crosshairsTop = nullptr;
				HUDPackage.crosshairsBottom = nullptr;
				HUDPackage.crosshairsCenter = nullptr;

			}

			FVector2D walkSpeedRange(0.f, character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D velocityMultiplier(0.f, 1.f);
			FVector Velocity = character->GetVelocity();
			Velocity.Z = 0.f;

			crosshairVelocityFactor = FMath::GetMappedRangeValueClamped(walkSpeedRange, velocityMultiplier, Velocity.Size());
			if (character->GetCharacterMovement()->IsFalling()) {

				crosshairInAirFactor = FMath::FInterpTo(crosshairInAirFactor, 2.25f, deltaTime, 2.25f);

			}
			else {
				crosshairInAirFactor = FMath::FInterpTo(crosshairInAirFactor, 0.f, deltaTime, 30.f);

			}

			if (bAiming) {

				crosshairAimFactor = FMath::FInterpTo(crosshairAimFactor, 0.58f, deltaTime, 30.f);

			}
			else {
				
				crosshairAimFactor = FMath::FInterpTo(crosshairAimFactor, 0.f, deltaTime, 30.f);
			}

			crosshairShootingFactor = FMath::FInterpTo(crosshairShootingFactor, 0.f, deltaTime, 40.f);

			HUDPackage.crosshairSpread = 0.5f + crosshairInAirFactor + crosshairVelocityFactor - crosshairAimFactor + crosshairShootingFactor;

			HUD->SetHUDPackage(HUDPackage);
		}

		

	}

}


void UEquipmentComponent::FireButtonPressed(bool bPressed) {

	bFireButtonPressed = bPressed;
	if (bFireButtonPressed) {
		
		FHitResult hitResult;
		TraceUnderCrosshair(hitResult);
		ServerFire(hitResult.ImpactPoint);

		if (equippedWeapon) {

			crosshairShootingFactor = 0.75f;
		}
	}
}

void UEquipmentComponent::ServerFire_Implementation(const FVector_NetQuantize& traceHitTarget)
{
	MulticastFire(traceHitTarget);
}

void UEquipmentComponent::MulticastFire_Implementation(const FVector_NetQuantize& traceHitTarget)
{


	if (equippedWeapon == nullptr) { return; }
	if (character) {

		character->PlayFireMontage();
		equippedWeapon->Fire(traceHitTarget);
	}
}

void UEquipmentComponent::OnRep_EquipedWeapon() {

	if (equippedWeapon && character) {

		character->GetCharacterMovement()->bOrientRotationToMovement = false;
		character->bUseControllerRotationYaw = true;
	}
}



void UEquipmentComponent::SetAiming(bool IsAiming) {
	bAiming = IsAiming;
	ServerSetAiming(IsAiming);
}


void UEquipmentComponent::ServerSetAiming_Implementation(bool bIsAiming) {

	bAiming = bIsAiming;

}

void UEquipmentComponent::InterpFOV(float deltaTime) {

	if (equippedWeapon == nullptr) { return; }

	if (bAiming) {

		UE_LOG(LogTemp, Warning, TEXT("this shit is aimed"));
		currentFOV = FMath::FInterpTo(currentFOV, equippedWeapon->GetZoomedFOV(), deltaTime, equippedWeapon->GetZoomInterpSpeed());
	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("this is not aimed"));
		currentFOV = FMath::FInterpTo(currentFOV, defaultFOV, deltaTime, zoomInterpSpeed);
	}
	if (character && character->GetCamera()) {

		character->GetCamera()->SetFieldOfView(currentFOV);
	}
}