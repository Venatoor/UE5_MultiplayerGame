// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCommandHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PFA_ITERATIONTWO_API UPlayerCommandHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerCommandHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame

	void SendSignal();

	void ExecuteCommand(FString commandString);


private:

	TMap<FString, class IICommand*> playerCommandsMap;

	FString buttonCTRL = FString("ButtonCTRL");
	class ACharacterBase* characterBase;

	bool mutex = false;
		
};
