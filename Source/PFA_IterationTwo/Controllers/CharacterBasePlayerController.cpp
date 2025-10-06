// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBasePlayerController.h"
#include "PFA_IterationTwo/HUD/UCharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PFA_IterationTwo/HUD/CharacterBaseHUD.h"


void ACharacterBasePlayerController::BeginPlay() {

	Super::BeginPlay();

	characterHUD = Cast<ACharacterBaseHUD>(GetHUD());



}

void ACharacterBasePlayerController::SetHUDHealth(float health, float maxHealth) {
	characterHUD = characterHUD == nullptr ? Cast<ACharacterBaseHUD>(GetHUD()) : characterHUD;

	bool HUDHealthValid = characterHUD && characterHUD->characterOverlay && characterHUD->characterOverlay->healthBar
		&& characterHUD->characterOverlay->healthText;

	if (HUDHealthValid) {

		const float healthPercent = health / maxHealth;
		characterHUD->characterOverlay->healthBar->SetPercent(healthPercent);

		FString healthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(health), FMath::CeilToInt(maxHealth));
		characterHUD->characterOverlay->healthText->SetText(FText::FromString(healthText));
	}
}
