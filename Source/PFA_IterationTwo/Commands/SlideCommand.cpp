// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideCommand.h"
#include "PFA_IterationTwo/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

#define SLIDEVELOCITY 300.0f


// Sets default values for this component's properties
USlideCommand::USlideCommand()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
	slideStartTime = 0.0f;
	slideDuration = 0.0f;
	originalCapsuleHeight = 0.0f;
	targetCapsuleHeight = 0.0f;
	// ...
}


// Called when the game starts
void USlideCommand::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());
	if (character) {

		characterMovementComponent = character->GetCharacterMovement();
		AnimInstance = character->GetMesh()->GetAnimInstance();
	}

	// ...
	
}


// Called every frame
void USlideCommand::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsSliding && characterMovementComponent ) {

		const float slideTime = GetWorld()->GetTimeSeconds() - slideStartTime;
		const float interpolatedCapsuleHeight = FMath::Lerp(originalCapsuleHeight, targetCapsuleHeight, slideTime / 0.2f);

		//FVector CharacterLocation = originalLocation + GetOwner()->GetActorUpVector() * 100.f;
		//characterMovementComponent->GetOwner()->SetActorLocation(CharacterLocation);

		float CapsuleSizeDifference = targetCapsuleHeight - originalCapsuleHeight;
		FVector Offset = FVector(0.0f, 0.0f, 25.f);

		characterMovementComponent->GetOwner()->SetActorLocation(characterMovementComponent->GetOwner()->GetActorLocation() + Offset);

		characterMovementComponent->GetCharacterOwner()->GetCapsuleComponent()->SetCapsuleHalfHeight(interpolatedCapsuleHeight, true);

		if (slideTime >= slideDuration) {

			ExecuteFinishedSlideLogic();
		}
		else {
			FVector startLocation = GetOwner()->GetActorLocation();
			FVector endLocation =  startLocation + (GetOwner()->GetActorForwardVector() * SLIDEVELOCITY  * DeltaTime);
			characterMovementComponent->GetOwner()->SetActorLocation(endLocation);

			FHitResult hitResult;

			FCollisionQueryParams params;
			params.AddIgnoredActor(GetOwner());

			bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECC_Visibility, params);

		}
	}

	// ...
}

void USlideCommand::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USlideCommand, bIsSliding);
	DOREPLIFETIME(USlideCommand, slideDuration);
	DOREPLIFETIME(USlideCommand, slideStartTime);
	DOREPLIFETIME(USlideCommand, originalCapsuleHeight);
	DOREPLIFETIME(USlideCommand, targetCapsuleHeight);

}

void USlideCommand::ExecuteSlideLogic(float duration, float slideHeight)
{
	if (!bIsSliding && characterMovementComponent) {
		//characterMovementComponent->DisableMovement();

		this->slideDuration = duration;
		originalCapsuleHeight = characterMovementComponent->GetCharacterOwner()->GetCapsuleComponent()-> GetScaledCapsuleHalfHeight();
		targetCapsuleHeight = slideHeight;

		slideStartTime = GetWorld()->GetTimeSeconds();
		bIsSliding = true;

		SetIsSliding(bIsSliding);
		SetSlideStartTime(slideStartTime);
		SetSlideDuration(slideDuration);
		SetOriginalCapsuleHalfHeight(originalCapsuleHeight);
		SetTargetCapsuleHalfHeight(targetCapsuleHeight);

		if (AnimInstance && slideMontage) {

			AnimInstance->Montage_Play(slideMontage);
		}

	}
}

void USlideCommand::ExecuteFinishedSlideLogic()
{
	if (bIsSliding && characterMovementComponent) {

		characterMovementComponent->GetCharacterOwner()->GetCapsuleComponent()->SetCapsuleHalfHeight(originalCapsuleHeight);
		

		characterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		//FOR OPTIMISATION

		characterMovementComponent->SetComponentTickEnabled(true);
		bIsSliding = false;
		SetIsSliding(bIsSliding);


	}
}

void USlideCommand::DebugClass() {

	if (AnimInstance && slideMontage) {

		AnimInstance->Montage_Play(slideMontage);
	}
}