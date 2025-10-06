// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PFA_IterationTwo/Controllers/CharacterBasePlayerController.h"
#include "PFA_IterationTwo/HUD/CharacterBaseHUD.h"
#include "EquipmentComponent.generated.h"


#define TRACE_LENGTH 80000.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PFA_ITERATIONTWO_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetAiming(bool IsAiming);

	UFUNCTION(Server, Reliable)
		void ServerSetAiming(bool IsAiming);

public:	
	void EquipWeapon(class AWeapon* weapon);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class ACharacterBase;

	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
		void ServerFire(const FVector_NetQuantize& traceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastFire(const FVector_NetQuantize& traceHitTarget);

	void TraceUnderCrosshair(FHitResult& traceHitResult);

	UFUNCTION()
		void OnRep_EquipedWeapon();


	void SetCrosshairs(float deltaTime);

private:

	// WEAPON + AIMING

	UPROPERTY(ReplicatedUsing = OnRep_EquipedWeapon)
	AWeapon* equippedWeapon;
	UPROPERTY(Replicated)
		bool bAiming;
	class ACharacterBase* character;


	FHUDPackage HUDPackage;

	//FOV

	float defaultFOV;

	UPROPERTY(EditAnywhere, Category = "FOV")
		float zoomedFOV = 30.f;

	float currentFOV;

	UPROPERTY(EditAnywhere, Category = "FOV")
		float zoomInterpSpeed = 20.f;

	void InterpFOV(float deltaTime);



	bool bFireButtonPressed;

	class ACharacterBaseHUD* HUD;
	class ACharacterBasePlayerController* controller;


	//HUD SECTION

	float crosshairVelocityFactor;
	float crosshairInAirFactor;
	float crosshairAimFactor;
	float crosshairShootingFactor;
		
};
