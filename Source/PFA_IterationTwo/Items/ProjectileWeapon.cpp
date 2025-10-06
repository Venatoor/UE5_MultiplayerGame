// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "PFA_IterationTwo/Projectiles/Projectile.h"
#include "Engine/SkeletalMeshSocket.h"

void AProjectileWeapon::Fire(const FVector& hitTarget) {

	
	Super::Fire(hitTarget);
	

	APawn* instigator = Cast<APawn>(GetOwner());

		

		FVector toTarget = hitTarget - weaponShootLocation->GetComponentLocation();
		FRotator rotation = toTarget.Rotation();

		if (projectileClass && instigator) {

			FActorSpawnParameters spawnParameters;
			spawnParameters.Owner = GetOwner();
			spawnParameters.Instigator = instigator;
			UWorld* world = GetWorld();
			if (world) {

				world->SpawnActor<AProjectile>(projectileClass, weaponShootLocation->GetComponentLocation() , rotation, spawnParameters);
				UE_LOG(LogTemp, Warning, TEXT("THE SPAWN IS SUCCESFUL 2"));

			}

		}


}

