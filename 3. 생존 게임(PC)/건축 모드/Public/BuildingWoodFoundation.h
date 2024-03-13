// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingBaseObject.h"
#include "BuildingWoodWall.h"
#include "BuildingWoodStair.h"
#include "BuildingWoodFoundation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_SRV_API ABuildingWoodFoundation : public ABuildingBaseObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABuildingWoodFoundation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
    USceneComponent* Corner1;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* Corner2;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* Corner3;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* Corner4;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* InvisibleSM1;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* InvisibleSM2;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* InvisibleSM3;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* InvisibleSM4;

	UPROPERTY(VisibleAnywhere)
    USceneComponent* RecommendedLocation1;
	UPROPERTY(VisibleAnywhere)
    USceneComponent* RecommendedLocation2;
	UPROPERTY(VisibleAnywhere)
    USceneComponent* RecommendedLocation3;
	UPROPERTY(VisibleAnywhere)
    USceneComponent* RecommendedLocation4;

	virtual FTransform FindNearestTransform(FVector HitLocation, AActor *HoldingObject) const override;
	
	FTransform HoldingWall(FVector HitLocation) const;
	FTransform HoldingFoundation(FVector HitLocation) const;
	FTransform HoldingStair(FVector HitLocation) const;
};
