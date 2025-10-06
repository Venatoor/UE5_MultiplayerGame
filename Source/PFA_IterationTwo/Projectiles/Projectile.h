// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class PFA_ITERATIONTWO_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()

		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) ;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* tracer;
	class UParticleSystemComponent* tracerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAcces = "private"));
	class UBoxComponent* collisionBox;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* movementComponent;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* impactParticles;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	float damage;

};
