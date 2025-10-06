// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBaseHUD.h"
#include "GameFramework/PlayerController.h"
#include "UCharacterOverlay.h"


void ACharacterBaseHUD::BeginPlay() {

	Super::BeginPlay();

	AddCharacterOverlay();
}

void ACharacterBaseHUD::DrawHUD() {

	Super::DrawHUD();
	FVector2D viewportSize;
	if (GEngine) {

		GEngine->GameViewport->GetViewportSize(viewportSize);
		const FVector2D viewportCenter(viewportSize.X / 2.f, viewportSize.Y / 2.f);

		float spreadScaled = crosshairSpreadMax * HUDPackage.crosshairSpread;

		if (HUDPackage.crosshairsBottom) {
			FVector2D spread(0.f, spreadScaled);
			DrawCrosshair(HUDPackage.crosshairsBottom, viewportCenter, spread, HUDPackage.crosshairColor);

		}

		if (HUDPackage.crosshairsCenter) {
			FVector2D spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.crosshairsCenter, viewportCenter, spread, HUDPackage.crosshairColor );

		}

		if (HUDPackage.crosshairsLeft) {
			FVector2D spread(-spreadScaled, 0.f);
			DrawCrosshair(HUDPackage.crosshairsLeft, viewportCenter, spread, HUDPackage.crosshairColor);

		}

		if (HUDPackage.crosshairsRight) {
			FVector2D spread(spreadScaled, 0.f);
			DrawCrosshair(HUDPackage.crosshairsRight, viewportCenter, spread, HUDPackage.crosshairColor);

		}

		if (HUDPackage.crosshairsTop) {
			FVector2D spread(0.f, -spreadScaled);
			DrawCrosshair(HUDPackage.crosshairsTop, viewportCenter, spread, HUDPackage.crosshairColor);

		}
	}
}


void ACharacterBaseHUD::DrawCrosshair(UTexture2D* texture, FVector2D viewportCenter, FVector2D spread, FLinearColor crosshairColor) {
	
	const float textureWidth = texture->GetSizeX();
	const float textureHeight = texture->GetSizeY();

	const FVector2D textureDrawPoint(viewportCenter.X - (textureWidth / 2.f) + spread.X, viewportCenter.Y - (textureHeight / 2.f) + spread.Y);

	DrawTexture(texture, textureDrawPoint.X, textureDrawPoint.Y, textureWidth, textureHeight, 0.f, 0.f, 1.f, 1.f, crosshairColor);
}


void ACharacterBaseHUD::AddCharacterOverlay() {

	APlayerController* playerController = GetOwningPlayerController();
	if (playerController && characterOverlayClass) {

		characterOverlay = CreateWidget<UUCharacterOverlay>(playerController, characterOverlayClass);
		characterOverlay->AddToViewport();
	}
}