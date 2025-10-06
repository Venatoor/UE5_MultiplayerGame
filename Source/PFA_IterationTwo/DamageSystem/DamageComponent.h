// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"


class UDamageTypeAbstract;
class UHealthComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PFA_ITERATIONTWO_API UDamageComponent : public UActorComponent
{

	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AActor* ownerActor;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float damageModifier;

	UFUNCTION()
		void ApplyDamage(AActor* damagedActor, float value, const class UDamageType* damageType, class AController* instigator, AActor* damagerActor);


private:

	UFUNCTION(Server, Reliable)
		void ServerApplyDamage(AActor* damagedActor, float value, const class UDamageType* damageType, class AController* instigator, AActor* damagerActor);

	void ApplyDamageEffect(UDamageTypeAbstract* damageType);

};
