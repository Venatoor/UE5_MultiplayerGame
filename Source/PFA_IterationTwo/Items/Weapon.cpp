// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "PFA_IterationTwo/CharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// for the weapon to have authority on the server 
	bReplicates = true;

	weaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	SetRootComponent(weaponSkeletalMesh);


	weaponSkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	weaponSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	weaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	areaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Area Sphere"));
	areaSphere->SetupAttachment(RootComponent);

	areaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	areaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	overheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Overhead Widget"));
	overheadWidget->SetupAttachment(RootComponent);

	weaponShootLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	weaponShootLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {

		areaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		areaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		areaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		areaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}


	
	if (overheadWidget) {

		overheadWidget->SetVisibility(false);
	}
	
	
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	ACharacterBase* character = Cast<ACharacterBase>(otherActor);
	if (character ) {


		character->SetOverlapingWeapon(this);

	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{

	ACharacterBase* character = Cast<ACharacterBase>(otherActor);
	if (character) {

		character->SetOverlapingWeapon(nullptr);
	}

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::ShowPickUpWidget(bool bShowWidget) {

	if (overheadWidget) {

		overheadWidget->SetVisibility(bShowWidget);
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, weaponState);
}


void AWeapon::SetWeaponState(EWeaponState state) {
	
	this->weaponState = state;
	switch (weaponState) {
	case EWeaponState::EWS_Equiped:

		areaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ShowPickUpWidget(false);
		break;

	}
}

void AWeapon::OnRep_WeaponState() {

	switch (weaponState) {

	case EWeaponState::EWS_Equiped:
		ShowPickUpWidget(false);
		break;
	}
}


void AWeapon::Fire(const FVector& hitTarget) {

	if (weaponShootVFX )  {

		UWorld* world = GetWorld();
		const FTransform weaponLocation = weaponShootLocation->GetComponentTransform();
		if (world) {

			UParticleSystemComponent* muzzleVFX = UGameplayStatics::SpawnEmitterAtLocation(world, weaponShootVFX, weaponLocation );
		}

	}
}


