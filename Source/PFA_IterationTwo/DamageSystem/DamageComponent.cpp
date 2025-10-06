// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include "PFA_IterationTwo/Stats/HealthComponent.h"
#include "DamageTypeAbstract.h"

// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// CAN ADD DAMAGE TYPE RESISTANCES ETC SINCE THE TRUE DAMAGE SYSTEM IS IMPLEMENTED IN DAMAGEABLE
void UDamageComponent::ApplyDamage(AActor* damagedActor, float value, const UDamageType* damageType, AController* instigator, AActor* damagerActor)
{
	if (GetOwnerRole() < ROLE_Authority) {
		ServerApplyDamage(damagedActor, value, damageType, instigator, damagerActor);
		return;
	}

	UHealthComponent* healthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	if (healthComponent) {




			healthComponent->ReduceHealth(value * damageModifier);

	}
}

void UDamageComponent::ApplyDamageEffect(UDamageTypeAbstract* damageType)
{
	
}

void UDamageComponent::ServerApplyDamage_Implementation(AActor* damagedActor, float value, const UDamageType* damageType, AController* instigator, AActor* damagerActor)
{
	ApplyDamage(  damagedActor, value,  damageType,  instigator, damagerActor);
}
