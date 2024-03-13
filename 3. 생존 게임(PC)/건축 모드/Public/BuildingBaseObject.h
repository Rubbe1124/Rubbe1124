// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBaseObject.generated.h"

UCLASS()
class PROJECT_SRV_API ABuildingBaseObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingBaseObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USceneComponent *Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *CalculationMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *StaticMesh;

	int FloorNum;

	bool CanBeBase;

	virtual FTransform FindNearestTransform(FVector HitLocation, AActor *HoldingObject) const PURE_VIRTUAL(ABuildingBaseObject::FindNearestTransform, return FTransform(););
	/*
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent *OverlappedComp,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);
	
	UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	bool IsOverlapSomething;
	*/
};
