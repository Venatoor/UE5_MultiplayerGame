// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"



UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equiped UMETA(DisplayName = "Equiped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_Max UMETA(DisplayName = "DefaultMax")
};



UCLASS()
class PFA_ITERATIONTWO_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		class USceneComponent* weaponShootLocation;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const override;

	
	
	UFUNCTION()
		virtual void OnSphereOverlap(
			UPrimitiveComponent* overlappedComponent,
			AActor* otherActor,
			UPrimitiveComponent* otherComp,
			int32 otherBodyIndex,
			bool bFromSweep,
			const FHitResult& sweepResult

		);

	UFUNCTION()
		void OnSphereEndOverlap(
			UPrimitiveComponent* overlappedComponent,
			AActor* otherActor,
			UPrimitiveComponent* otherComp,
			int32 otherBodyIndex);

	UFUNCTION()
		void OnRep_WeaponState();


	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ShowPickUpWidget(bool bShowWidget);

private:

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		class USphereComponent* areaSphere;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		USkeletalMeshComponent* weaponSkeletalMesh;
	UPROPERTY(ReplicatedUsing = OnRep_WeaponState,VisibleAnywhere, Category = "Weapon Properties")
		EWeaponState weaponState;

	UPROPERTY(VisibleAnywhere, Category = "HUD")
		class UWidgetComponent* overheadWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Effects")
		class UParticleSystem* weaponShootVFX;




public:

	void SetWeaponState(EWeaponState state);

	FORCEINLINE USphereComponent* GetAreaSphere() const { return areaSphere;  }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return weaponSkeletalMesh; }
	FORCEINLINE float GetZoomedFOV() const { return zoomedFOV;  }
	FORCEINLINE float GetZoomInterpSpeed() const{ return zoomInterpSpeed;  }
	


	UFUNCTION()
		virtual void Fire(const FVector& hitTarget);


	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		class UTexture2D* crosshairCenters;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* crosshairLeft;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* crosshairRight;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* crosshairTop;

	UPROPERTY(EditAnywhere, Category = "Crosshairs")
		UTexture2D* crosshairBottom;

	//ZOOMED FOV WHILE AIMING

	UPROPERTY(EditAnywhere)
		float zoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
		float zoomInterpSpeed = 20.f;

};
