// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingWoodWall.h"

ABuildingWoodWall::ABuildingWoodWall()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/NYB/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Wall.SM_Stylized_Wood_Wall'"));
	if (MeshAsset.Succeeded())
	{
    	UStaticMesh* Asset = MeshAsset.Object;
    	StaticMesh->SetStaticMesh(Asset);
		CalculationMesh->SetStaticMesh(Asset);
		CalculationMesh->SetWorldScale3D(FVector(0.9f, 0.9f, 0.9f));
		//CalculationMesh->SetVisibility(false);
	}

    Corner2 = CreateDefaultSubobject<USceneComponent>(TEXT("Corner2"));
    Corner2->SetupAttachment(RootComponent);
	Corner2->SetRelativeLocation(FVector(0.0f, 150.0f, 150.0f));

    Corner4 = CreateDefaultSubobject<USceneComponent>(TEXT("Corner4"));
    Corner4->SetupAttachment(RootComponent);
	Corner4->SetRelativeLocation(FVector(0.0f, -150.0f, 150.0f));
	
	Top = CreateDefaultSubobject<USceneComponent>(TEXT("Top"));
    Top->SetupAttachment(RootComponent);
	Top->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));

	Bottom = CreateDefaultSubobject<USceneComponent>(TEXT("Bottom"));
    Bottom->SetupAttachment(RootComponent);
	Bottom->SetRelativeLocation(FVector(0.0f, 0.0f, -300.0f));

	CalculationMesh->SetVisibility(false);

}

// Called when the game starts or when spawned
void ABuildingWoodWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingWoodWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform ABuildingWoodWall::FindNearestTransform(FVector HitLocation, AActor *HoldingObject) const
{
    if(HoldingObject->IsA(ABuildingWoodCeiling::StaticClass()))
    {
        return HoldingCeiling(HitLocation);
    }
	else if(HoldingObject->IsA(ABuildingWoodWall::StaticClass())||HoldingObject->IsA(ABuildingWoodHalfWall::StaticClass()))
	{
		return HoldingWall(HitLocation);
	}
    else
    {
        return GetTransform();
    }
    return GetTransform();
}

FTransform ABuildingWoodWall::HoldingCeiling(FVector HitLocation) const
{
    TArray<USceneComponent *> Components;
	GetComponents<USceneComponent>(Components, true);

    TArray<USceneComponent *> ConerComponents;

	for (USceneComponent *Component : Components)
	{
		FString ComponentName = Component->GetName();

		// "Corner"이라는 이름이 포함된 씬 컴포넌트를 찾음

		if (ComponentName.Contains(TEXT("Corner")))
		{
			//UE_LOG(LogTemp, Log, TEXT("코너 컴포넌트 발견: %s"), *ComponentName);
			FVector ComponentLocation = Component->GetComponentLocation();
			//UE_LOG(LogTemp, Log, TEXT("코너 컴포넌트의 월드로케이션: %s"), *ComponentLocation.ToString());
			ConerComponents.Add(Component);
		}
	}
    FTransform ReturnValue = GetTransform();
	FVector TempLocation=GetTransform().GetLocation();
    FRotator TempRotation = FRotator(0.0f, 0.0f, 0.0f);

	float TempValue = 99999;

	for (USceneComponent *Component : ConerComponents)
	{
		float Distance = FVector::Distance(HitLocation, Component->GetComponentLocation());
		//UE_LOG(LogTemp, Log, TEXT("거리: %f"), Distance);
		if (TempValue > Distance)
		{
			TempValue = Distance;
			TempLocation = Component->GetComponentLocation();

			// 현재 회전값을 얻어옵니다.
			FRotator CurrentRotation = GetTransform().Rotator();

			double temp = CurrentRotation.Yaw;
			//UE_LOG(LogTemp, Log, TEXT("변경됨"));
			if (Component->GetName().Contains(TEXT("Corner1")))
			{
				TempRotation = FRotator(0.0f, 270.0f + temp, 0.0f);
			}
			else if (Component->GetName().Contains(TEXT("Corner2")))
			{
				TempRotation = FRotator(0.0f, 0.0f + temp, 0.0f);
			}
			else if (Component->GetName().Contains(TEXT("Corner3")))
			{
				TempRotation = FRotator(0.0f, 90.0f + temp, 0.0f);
			}
			else if (Component->GetName().Contains(TEXT("Corner4")))
			{
				TempRotation = FRotator(0.0f, 180.0f + temp, 0.0f);
			}
		}
	}

    ReturnValue.SetLocation(TempLocation);
    ReturnValue.SetRotation(FQuat(TempRotation));

	return ReturnValue;
}

FTransform ABuildingWoodWall::HoldingWall(FVector HitLocation) const
{
	TArray<USceneComponent *> ConerComponents;
	ConerComponents.Add(Top);
	//ConerComponents.Add(Bottom);

	FTransform ReturnValue = GetTransform();
	FVector TempLocation=GetTransform().GetLocation();
    FRotator TempRotation = FRotator(0.0f, 0.0f, 0.0f);

	float TempValue = 99999;

	for (USceneComponent *Component : ConerComponents)
	{
		float Distance = FVector::Distance(HitLocation, Component->GetComponentLocation());
		//UE_LOG(LogTemp, Log, TEXT("거리: %f"), Distance);
		if (TempValue > Distance)
		{
			TempValue = Distance;
			TempLocation = Component->GetComponentLocation();

			TempRotation = GetTransform().Rotator();
		}
	}

    ReturnValue.SetLocation(TempLocation);
    ReturnValue.SetRotation(FQuat(TempRotation));

	return ReturnValue;
}