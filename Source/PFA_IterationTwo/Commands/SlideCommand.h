// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ICommand.h"
#include "TimerManager.h"
#include "SlideCommand.generated.h"

class UCharacterMovementComponent;
class UAnimInstance;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PFA_ITERATIONTWO_API USlideCommand : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlideCommand();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	

	void ExecuteSlideLogic(float slideDuration, float slideHeight);

	void ExecuteFinishedSlideLogic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
		UAnimMontage* slideMontage;

private:
	

	UPROPERTY(Transient, Replicated)
		bool bIsSliding;

	UPROPERTY(Transient, Replicated)
		float slideStartTime;

	UPROPERTY(Transient, Replicated)
		float slideDuration;

	UPROPERTY(Transient, Replicated)
		float originalCapsuleHeight;

	UPROPERTY(Transient, Replicated)
		float targetCapsuleHeight;


	FVector originalLocation;
	FTimerHandle timerSlideHandle;

	UPROPERTY()
		UCharacterMovementComponent* characterMovementComponent;

	UPROPERTY()
		UAnimInstance* AnimInstance;

public:

	UFUNCTION(BlueprintCallable, Category = "Slide")
		void SetIsSliding(bool bNewIsSliding) { bIsSliding = bNewIsSliding; }

	UFUNCTION(BlueprintCallable, Category = "Slide")
		void SetSlideStartTime(float NewSlideStartTime) { slideStartTime = NewSlideStartTime; }

	UFUNCTION(BlueprintCallable, Category = "Slide")
		void SetSlideDuration(float NewSlideDuration) { slideDuration = NewSlideDuration; }

	UFUNCTION(BlueprintCallable, Category = "Slide")
		void SetOriginalCapsuleHalfHeight(float NewOriginalCapsuleHalfHeight) { originalCapsuleHeight = NewOriginalCapsuleHalfHeight; }

	UFUNCTION(BlueprintCallable, Category = "Slide")
		void SetTargetCapsuleHalfHeight(float NewTargetCapsuleHalfHeight) { targetCapsuleHeight = NewTargetCapsuleHalfHeight; }


	UFUNCTION()
		void DebugClass();

private:

};
