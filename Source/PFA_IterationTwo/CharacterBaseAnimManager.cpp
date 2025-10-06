// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseAnimManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterBaseAnimManager::NativeInitializeAnimation() {

	Super::NativeInitializeAnimation();

	characterInstance = Cast<ACharacterBase>(TryGetPawnOwner());
}

void UCharacterBaseAnimManager::NativeUpdateAnimation(float deltaTime) {

	Super::NativeUpdateAnimation(deltaTime);
	
	if (characterInstance == nullptr) {

		characterInstance = Cast<ACharacterBase>(TryGetPawnOwner());

	}

	if (characterInstance == nullptr) {
		
		return;
	}

	FVector velocity = characterInstance->GetVelocity();
	velocity.Z = 0;
	speed = velocity.Size();


	isInAir = characterInstance->GetCharacterMovement()->IsFalling();
	isAccelerating = characterInstance->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	isCrouching = characterInstance->bIsCrouched;
	isWeaponEquiped = characterInstance->IsWeaponEquiped();
	bAiming = characterInstance->IsAiming();
	
	FRotator aimRotation = characterInstance->GetBaseAimRotation();
	FRotator movementRotation = UKismetMathLibrary::MakeRotFromX(characterInstance->GetVelocity());
	yawOffset = UKismetMathLibrary::NormalizedDeltaRotator(movementRotation, aimRotation).Yaw;

	turningInPlace = characterInstance->GetTurningInPlace();

	characterRotationLastFrame = characterRotation;
	characterRotation = characterInstance->GetActorRotation();

	const FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(characterRotation, characterRotationLastFrame);
	const float target = delta.Yaw / deltaTime;
	const float interp = FMath::FInterpTo(lean, target, deltaTime, 0.6f);
	lean = FMath::Clamp(interp, -90.f, 90.f);

	AO_Yaw = characterInstance->GetAO_Yaw();
	AO_Pitch = characterInstance->GetAO_Pitch();

}

