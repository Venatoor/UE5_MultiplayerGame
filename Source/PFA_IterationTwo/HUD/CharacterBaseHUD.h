// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CharacterBaseHUD.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FHUDPackage {

	GENERATED_BODY()

public:

	class UTexture2D* crosshairsCenter;
	UTexture2D* crosshairsLeft;
	UTexture2D* crosshairsRight;
	UTexture2D* crosshairsTop;
	UTexture2D* crosshairsBottom;
	float crosshairSpread;
	FLinearColor crosshairColor;
};

UCLASS()
class PFA_ITERATIONTWO_API ACharacterBaseHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
		TSubclassOf<class UUserWidget> characterOverlayClass;

	class UUCharacterOverlay* characterOverlay;


protected:

	virtual void BeginPlay() override;
	void AddCharacterOverlay();

private:

	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* texture, FVector2D viewportCenter, FVector2D Spread, FLinearColor crosshairColor);


	UPROPERTY(EditAnywhere)
		float crosshairSpreadMax = 16.f;

public:

	FORCEINLINE void SetHUDPackage(const FHUDPackage& package) { HUDPackage = package; }

};
