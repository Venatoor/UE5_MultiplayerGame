// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PFA_IterationTwo/DamageSystem/Damageable.h"
#include "GameFramework/Character.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = collisionBox;

	collisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	collisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	collisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	movementComponent->bRotationFollowsVelocity = true;



}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (tracer) {

		tracerComponent = UGameplayStatics::SpawnEmitterAttached(tracer, collisionBox, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
	}


	if (HasAuthority()) {

		collisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectile::Destroyed() {

	if (impactParticles) {

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactParticles, GetActorTransform());
	}
}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	ACharacter* ownerCharacter = Cast<ACharacter>(GetOwner());
	if (ownerCharacter) {

		AController* ownerController = ownerCharacter->Controller;

			bool bImplementsInterface = OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass());
			if (bImplementsInterface) {

				UGameplayStatics::ApplyDamage(OtherActor, damage, ownerController, this, UDamageType::StaticClass());
			}

	}
	
	Destroy();
}
