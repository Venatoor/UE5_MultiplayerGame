// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "PFA_IterationTwo/CharacterBase.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	

	// ...
	
}
void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, currentHealth);
}



// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::ReduceHealth(float value) {
	if (value <= 0) {
		return;
	}

	currentHealth = FMath::Clamp(currentHealth - value, 0.0f, maxHealth);

}

void UHealthComponent::IncreaseHealth(float value) {

	if (value <= 0) {
		return;
	}

	currentHealth = FMath::Clamp(currentHealth + value, 0.0f, maxHealth);

}

void UHealthComponent::IncreaseMaxHealth(float value) {

	maxHealth += value;

}

void UHealthComponent::ReduceMaxHealth(float value) {

	if (maxHealth - value <= 0) {

		maxHealth = 1.0f;
	}

	else

		maxHealth -= value;

}

void UHealthComponent::ReduceAllHealth() {

	maxHealth = 0.0f;
}

float UHealthComponent::GetMaxHealth() {
	return maxHealth;
}

float UHealthComponent::GetHealth() {
	return currentHealth;
}

void UHealthComponent::OnRep_CurrentHealth() {

	ACharacterBase* owner = Cast<ACharacterBase>(GetOwner());
	if (owner) {

		owner->UpdateHUDHealth();
	}

}


