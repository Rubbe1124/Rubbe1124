// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include "BuildingWoodHalfWall.h"
#include "BuildingWoodStair.h"
#include "BuildingModeCharacter.generated.h"

UCLASS()
class PROJECT_SRV_API ABuildingModeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABuildingModeCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class USpringArmComponent *SpringArm;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SettingHoldingObejct(ABuildingBaseObject *SeletedObject);

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	AActor *HoldingObject;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	AActor *TargetObject;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	UFUNCTION()
	void LeftMousePressed();

	UFUNCTION()
	void LeftMouseReleased();

	UFUNCTION()
	void RightMousePressed();

	UFUNCTION()
	void RightMouseReleased();

	bool IsLeftMouseDown;

	bool IsRightMouseDown;

	void LookUp(float AxisValue);
	void Turn(float AxisValue);

	void ZoomIn();
	void ZoomOut();

	bool IsHitLineTraceMousePositionToWorld(FHitResult *OutHit);

	// UStaticMeshComponent* StaticMeshComponent;

	void BuildOneObjectOnMousePosition(FVector HitLocation);

	void SetTransformBasedOnTarget(FVector HitLocation);

	bool AreTransformsEqual(const FTransform& Transform1, const FTransform& Transform2, float Tolerance);

	void SetFloorNumBasedOnTarget();

	bool CanBeBuilt;

	bool IsValidToBuild(FVector HitLocation);

	void ChangeMaterial(bool CanBeBuilt);

	UMaterial* CheckFailMaterial;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* OriginalMaterialInterface;

	void HoldingObjectOnAir();

	void SetRotationMode();
	void ReleaseRotationMode();
	bool Rotation;

	int FloorNum;

	void PageUp();
	void PageDown();

	void PressEsc();
	
	void PressDelete();
	
	template <typename T>
	void FindAllActors(UWorld *World, TArray<T *> &Out);
};
