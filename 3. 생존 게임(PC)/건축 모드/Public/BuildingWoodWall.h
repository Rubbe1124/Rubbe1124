// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBaseObject.h"
#include "BuildingWoodCeiling.h"
#include "BuildingWoodHalfWall.h"
#include "BuildingWoodWall.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_SRV_API ABuildingWoodWall : public ABuildingBaseObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingWoodWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
    USceneComponent* Corner2;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* Corner4;

	UPROPERTY(VisibleAnywhere)
    USceneComponent* Top;

	UPROPERTY(VisibleAnywhere)
    USceneComponent* Bottom;

	virtual FTransform FindNearestTransform(FVector HitLocation, AActor *HoldingObject) const override;

	
	FTransform HoldingCeiling(FVector HitLocation) const;
	FTransform HoldingWall(FVector HitLocation) const;
};
