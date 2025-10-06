// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "Items/Weapon.h"
#include "CharacterBaseAnimManager.h"
#include "Items/EquipmentComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PFA_IterationTwo/Controllers/CharacterBasePlayerController.h"
#include "PFA_IterationTwo/Commands/SlideCommand.h"
#include "PFA_IterationTwo/DamageSystem/DamageComponent.h"
#include "DamageSystem/Damageable.h"
// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	GetMesh()->SetCollisionResponseToChannel(
		ECollisionChannel::ECC_Camera,
		ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->AirControl = 0.40f;


	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArmComponent->SetupAttachment(RootComponent);
	springArmComponent->TargetArmLength = 400.f;
	springArmComponent->bUsePawnControlRotation = true;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	cameraComponent->SetupAttachment(springArmComponent);
	cameraComponent->bUsePawnControlRotation = false;

	equipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment Component"));
	equipmentComponent->SetIsReplicated(true);

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	healthComponent->SetIsReplicated(true);

	turningInPlace = ETurningInPlace::ETIP_NotTurning;

	//TEMP
	slideCommand = CreateDefaultSubobject<USlideCommand>(TEXT("Slide Command"));
	//TEMP


}

void ACharacterBase::OnRep_OverlappingWeapon(AWeapon* lastWeapon)
{
	if (OverlappingWeapon) {

		OverlappingWeapon->ShowPickUpWidget(true);
	}

	if (lastWeapon) {

		lastWeapon->ShowPickUpWidget(false);
	}
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACharacterBase, OverlappingWeapon, COND_OwnerOnly);
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (equipmentComponent) {

		equipmentComponent->character = this;
	}
}

void ACharacterBase::PlayFireMontage()
{

	if (equipmentComponent == nullptr || equipmentComponent->equippedWeapon == nullptr) return;

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && fireWeaponMontage) {
		
		animInstance->Montage_Play(fireWeaponMontage);
	}
}

void ACharacterBase::PlaySlideMontage() 
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && slideMontage) {
		animInstance->Montage_Play(slideMontage);
	}
}


// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	
	characterController = Cast<ACharacterBasePlayerController>(Controller);
	if (characterController) {

		characterController->SetHUDHealth(GetHealthComponent()->GetHealth(), GetHealthComponent()->GetMaxHealth());
	}

	if (healthComponent && damageComponent) {
		if (HasAuthority()) {
			OnTakeAnyDamage.AddDynamic(this, &ACharacterBase::TakeDamage);
		}
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent) {

		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::Jump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterBase::StopJumping);
		PlayerInputComponent->BindAction("EquipWeapon", IE_Pressed, this, &ACharacterBase::EquipButtonPressed);
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACharacterBase::CrouchButtonPressed);
		PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACharacterBase::CrouchButtonPressed);
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterBase::FireButtonPressed);
		//PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACharacterBase::FireButtonReleased);


		PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ACharacterBase::AimButtonPressed);
		PlayerInputComponent->BindAction("Aim", IE_Released, this, &ACharacterBase::AimButtonReleased);

		PlayerInputComponent->BindAxis("Move Forward", this, &ACharacterBase::MoveForward);
		PlayerInputComponent->BindAxis("Move Right", this, &ACharacterBase::MoveRight);
		PlayerInputComponent->BindAxis("Look Up", this, &ACharacterBase::LookUp);
		PlayerInputComponent->BindAxis("Look Right", this, &ACharacterBase::LookRight);

		//temp

		PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &ACharacterBase::DoSlide);
		PlayerInputComponent->BindAction("Debug", IE_Pressed, this, &ACharacterBase::DebugSlide);
	}

}

void ACharacterBase::DoSlide() {

	slideCommand->ExecuteSlideLogic(1.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()/2);
}

void ACharacterBase::DebugSlide() {

	slideCommand->DebugClass();
}

void ACharacterBase::MoveForward(float value) {

	if (Controller && value != 0.0f) {

		const FRotator rotation  = Controller->GetControlRotation();
		const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);

		FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, value);


	}
}

void ACharacterBase::MoveRight(float value) {
	if (Controller && value != 0.0f) {

		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);

		FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value);


	}
}

void ACharacterBase::LookUp(float value) {

	AddControllerPitchInput(value);
}

void ACharacterBase::LookRight(float value) {

	AddControllerYawInput(value);
}

void ACharacterBase::SetOverlapingWeapon(AWeapon* weapon) {

	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickUpWidget(false);
		UE_LOG(LogTemp, Warning, TEXT("TESTING CASE OVERLAP"));
	}

	OverlappingWeapon = weapon;
	if (IsLocallyControlled()) {

		if (OverlappingWeapon) {

			OverlappingWeapon->ShowPickUpWidget(true);
		}
	}

}


void ACharacterBase::EquipButtonPressed() {

	if (equipmentComponent ) {

		if (HasAuthority()) {

			equipmentComponent->EquipWeapon(OverlappingWeapon);

		}
		else {
			ServerEquipButtonPressed();
		}
	}

}

void ACharacterBase::ServerEquipButtonPressed_Implementation()
{
	if (equipmentComponent) {
		equipmentComponent->EquipWeapon(OverlappingWeapon);
	}
}

void ACharacterBase::CrouchButtonPressed() {

	if (bIsCrouched) {
		UnCrouch();
	}
	else {
		Crouch();
	}
}

void ACharacterBase::FireButtonPressed()
{
	if (equipmentComponent) {

		equipmentComponent->FireButtonPressed(true);
	}
}

void ACharacterBase::FireButtonReleased()
{
}

void ACharacterBase::AimOffset(float deltaTime)
{
	if (equipmentComponent && equipmentComponent->equippedWeapon == nullptr ) {
		return;
	}
	FVector velocity = GetVelocity();
	velocity.Z = 0.f;
	float speed = velocity.Size();
	bool isInAir = GetCharacterMovement()->IsFalling();

	if (speed == 0.f && !isInAir) {

		FRotator currentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator deltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(currentAimRotation, startingAimRotation);
		AO_Yaw = deltaAimRotation.Yaw;
		bUseControllerRotationYaw = false;
		TurnInPlace(deltaTime);

	}

	if (speed > 0.f || isInAir) {
		
		startingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		turningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
	AO_Pitch = GetBaseAimRotation().Pitch;
	//FIX FOR NETWORKED MAPPING PROBLEM 
	if (AO_Pitch > 90.f && !IsLocallyControlled()) {

		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

bool ACharacterBase::IsWeaponEquiped() {

	return (equipmentComponent && equipmentComponent->equippedWeapon);
}

bool ACharacterBase::IsAiming() {

	return (equipmentComponent && equipmentComponent->bAiming);
}

void ACharacterBase::UpdateHUDHealth() {

	characterController = characterController == nullptr ? Cast<ACharacterBasePlayerController>(characterController) : characterController;
	if (characterController) {

		characterController->SetHUDHealth(healthComponent->GetHealth(), healthComponent->GetMaxHealth());
	}
}

void ACharacterBase::TakeDamage(AActor* damagedActor, float value, const UDamageType* damageType, AController* instigator, AActor* damagerActor) {

	if (damageComponent) {

		damageComponent->ApplyDamage(damagedActor, value,  damageType,  instigator, damagerActor);

	}
}


void ACharacterBase::TurnInPlace(float deltaTime) {

	if (AO_Yaw > 90.f) {

		turningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f) {

		turningInPlace = ETurningInPlace::ETIP_Left;
	}
}

void ACharacterBase::AimButtonPressed() {

	if (equipmentComponent) {

		equipmentComponent->SetAiming(true);
	}
}

void ACharacterBase::AimButtonReleased() {

	if (equipmentComponent) {

		equipmentComponent->SetAiming(false);
	}
}