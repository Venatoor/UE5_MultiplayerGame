// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Stats/HealthComponent.h"
#include "TurningInPlace.h"
#include "PFA_IterationTwo/DamageSystem/Damageable.h"
#include "Items/InteractWithCrosshair.h"
#include "CharacterBase.generated.h"


UCLASS()
class PFA_ITERATIONTWO_API ACharacterBase : public ACharacter, public IDamageable, public IInteractWithCrosshair
{
	GENERATED_BODY()

public:
	void DebugSlide();
	// Sets default values for this character's properties
	ACharacterBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAcces = "true"))
		class USpringArmComponent* springArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAcces = "true"))
		class UCameraComponent* cameraComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcces = "true"))
		class UEquipmentComponent* equipmentComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcces = "true"))
		class UHealthComponent* healthComponent;

	//TEMP
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcces = "true"))
		class USlideCommand* slideCommand;
	//TEMP

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon )
		class AWeapon* OverlappingWeapon;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcces = "true"))
		class ACharacterBasePlayerController* characterController;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcces = "true"))
		class UDamageComponent* damageComponent;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* lastWeapon);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const override;

	virtual void PostInitializeComponents() override;

	void PlayFireMontage();

	void PlaySlideMontage();

	void PlayHitReactMontage();

	void DoSlide();

	UFUNCTION(Server, Reliable)
		void ServerEquipButtonPressed();

	bool IsWeaponEquiped();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	void LookUp(float value);

	void LookRight(float value);

	// INPUTS 

	void EquipButtonPressed();

	void CrouchButtonPressed();

	void FireButtonPressed();

	void FireButtonReleased();

	void AimOffset(float deltaTime);

	void AimButtonPressed();

	void AimButtonReleased();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TakeDamage(AActor* damagedActor, float value, const UDamageType* damageType, AController* instigator, AActor* damagerActor) override;
protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public: 

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return springArmComponent; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return cameraComponent; }
	FORCEINLINE class UHealthComponent* GetHealthComponent() const { return healthComponent; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return turningInPlace;  }


	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	
	bool IsAiming();
	
	void SetOverlapingWeapon(AWeapon* weapon);

	void UpdateHUDHealth();

private:

	UPROPERTY(EditAnywhere, Category = "Combat")
		class UAnimMontage* fireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = "Movement")
		class UAnimMontage* slideMontage;

	float AO_Pitch;
	float AO_Yaw;
	FRotator startingAimRotation;

	ETurningInPlace turningInPlace;
	void TurnInPlace(float deltaTime);


};
