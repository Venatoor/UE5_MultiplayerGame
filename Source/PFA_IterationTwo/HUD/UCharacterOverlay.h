// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UCharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class PFA_ITERATIONTWO_API UUCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* healthBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* healthText;

};
