// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterBasePlayerController.generated.h"


/**
 * 
 */
UCLASS()
class PFA_ITERATIONTWO_API ACharacterBasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void SetHUDHealth(float health, float maxHealth);

protected:

	virtual void BeginPlay() override;

private:

	class ACharacterBaseHUD* characterHUD;

};
