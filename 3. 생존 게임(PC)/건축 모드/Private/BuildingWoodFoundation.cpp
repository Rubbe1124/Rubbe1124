// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingWoodFoundation.h"

// Sets default values
ABuildingWoodFoundation::ABuildingWoodFoundation()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/NYB/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Foundation.SM_Stylized_Wood_Foundation'"));
	if (MeshAsset.Succeeded())
	{
    	UStaticMesh* Asset = MeshAsset.Object;
    	StaticMesh->SetStaticMesh(Asset);
		CalculationMesh->SetStaticMesh(Asset);
		CalculationMesh->SetWorldScale3D(FVector(0.9f, 0.9f, 0.9f));
		//CalculationMesh->SetVisibility(false);
	}

	CanBeBase=true;

    Corner1 = CreateDefaultSubobject<USceneComponent>(TEXT("Corner1"));
    Corner1->SetupAttachment(RootComponent);
	Corner1->SetRelativeLocation(FVector(150.0f, 0.0f, 235.0f));

    Corner2 = CreateDefaultSubobject<USceneComponent>(TEXT("Corner2"));
    Corner2->SetupAttachment(RootComponent);
	Corner2->SetRelativeLocation(FVector(0.0f, 150.0f, 235.0f));

    Corner3 = CreateDefaultSubobject<USceneComponent>(TEXT("Corner3"));
    Corner3->SetupAttachment(RootComponent);
	Corner3->SetRelativeLocation(FVector(-150.0f, 0.0f, 235.0f));

    Corner4 = CreateDefaultSubobject<USceneComponent>(TEXT("Corner4"));
    Corner4->SetupAttachment(RootComponent);
	Corner4->SetRelativeLocation(FVector(0.0f, -150.0f, 235.0f));

	InvisibleSM1=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvisibleSM1"));
	InvisibleSM1->SetupAttachment(Root);
	InvisibleSM1->SetRelativeLocation(FVector(300.0f, 0.0f, -10.0f));

	InvisibleSM2=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvisibleSM2"));
	InvisibleSM2->SetupAttachment(Root);
	InvisibleSM2->SetRelativeLocation(FVector(0.0f, 300.0f, -10.0f));

	InvisibleSM3=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvisibleSM3"));
	InvisibleSM3->SetupAttachment(Root);
	InvisibleSM3->SetRelativeLocation(FVector(-300.0f, 0.0f, -10.0f));

	InvisibleSM4=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvisibleSM4"));
	InvisibleSM4->SetupAttachment(Root);
	InvisibleSM4->SetRelativeLocation(FVector(0.0f, -300.0f, -10.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>InvisibleMeshAsset(TEXT("StaticMesh'/Game/NYB/EasyBuildingSystem/Meshes/Structures/Stylized/Wood/SM_Stylized_Wood_Ceiling.SM_Stylized_Wood_Ceiling'"));
	if (InvisibleMeshAsset.Succeeded())
	{
    	UStaticMesh* Asset = InvisibleMeshAsset.Object;
    	InvisibleSM1->SetStaticMesh(Asset);
		InvisibleSM2->SetStaticMesh(Asset);
		InvisibleSM3->SetStaticMesh(Asset);
		InvisibleSM4->SetStaticMesh(Asset);
	}
	/*
	InvisibleSM1->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
	InvisibleSM2->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
	InvisibleSM3->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
	InvisibleSM4->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
	*/
	InvisibleSM1->SetVisibility(false);
	InvisibleSM2->SetVisibility(false);
	InvisibleSM3->SetVisibility(false);
	InvisibleSM4->SetVisibility(false);

	CalculationMesh->SetVisibility(false);

	RecommendedLocation1 = CreateDefaultSubobject<USceneComponent>(TEXT("RecommendedLocation1"));
    RecommendedLocation1->SetupAttachment(RootComponent);
	RecommendedLocation1->SetRelativeLocation(FVector(300.0f, 0.0f, 0.0f));

    RecommendedLocation2 = CreateDefaultSubobject<USceneComponent>(TEXT("RecommendedLocation2"));
    RecommendedLocation2->SetupAttachment(RootComponent);
	RecommendedLocation2->SetRelativeLocation(FVector(0.0f, 300.0f, 0.0f));

    RecommendedLocation3 = CreateDefaultSubobject<USceneComponent>(TEXT("RecommendedLocation3"));
    RecommendedLocation3->SetupAttachment(RootComponent);
	RecommendedLocation3->SetRelativeLocation(FVector(-300.0f, 0.0f, 0.0f));

    RecommendedLocation4 = CreateDefaultSubobject<USceneComponent>(TEXT("RecommendedLocation4"));
    RecommendedLocation4->SetupAttachment(RootComponent);
	RecommendedLocation4->SetRelativeLocation(FVector(0.0f, -300.0f, 0.0f));
}

// Called when the game starts or when spawned
void ABuildingWoodFoundation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingWoodFoundation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform ABuildingWoodFoundation::FindNearestTransform(FVector HitLocation, AActor *HoldingObject) const
{
    //if(HoldingObject->GetClass()->GetName() == "BuildingWoodWall"||HoldingObject->GetClass()->GetName() == "BuildingWoodDoor"||HoldingObject->GetClass()->GetName() == "BuildingWoodWindow")
	if(HoldingObject->IsA(ABuildingWoodWall::StaticClass()))
    {
        return HoldingWall(HitLocation);
    }
	else if(HoldingObject->IsA(ABuildingWoodFoundation::StaticClass()))
	{
		return HoldingFoundation(HitLocation);
	}
	else if(HoldingObject->IsA(ABuildingWoodStair::StaticClass()))
	{
		return HoldingStair(HitLocation);
	}
    else
	{
		return GetTransform();
	}
}

FTransform ABuildingWoodFoundation::HoldingWall(FVector HitLocation) const
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

FTransform ABuildingWoodFoundation::HoldingFoundation(FVector HitLocation) const
{
    TArray<USceneComponent *> Components;
	GetComponents<USceneComponent>(Components, true);

    TArray<USceneComponent *> ConerComponents;

	for (USceneComponent *Component : Components)
	{
		FString ComponentName = Component->GetName();

		// "Corner"이라는 이름이 포함된 씬 컴포넌트를 찾음

		if (ComponentName.Contains(TEXT("RecommendedLocation")))
		{
			//UE_LOG(LogTemp, Log, TEXT("코너 컴포넌트 발견: %s"), *ComponentName);
			FVector ComponentLocation = Component->GetComponentLocation();
			//UE_LOG(LogTemp, Log, TEXT("코너 컴포넌트의 월드로케이션: %s"), *ComponentLocation.ToString());
			ConerComponents.Add(Component);
		}
	}
    FTransform ReturnValue = GetTransform();
	FVector TempLocation=GetTransform().GetLocation();

	float TempValue = 99999;

	for (USceneComponent *Component : ConerComponents)
	{
		float Distance = FVector::Distance(HitLocation, Component->GetComponentLocation());
		//UE_LOG(LogTemp, Log, TEXT("거리: %f"), Distance);
		if (TempValue > Distance)
		{
			TempValue = Distance;
			TempLocation = Component->GetComponentLocation();
		}
	}

    ReturnValue.SetLocation(TempLocation);

	return ReturnValue;
}

FTransform ABuildingWoodFoundation::HoldingStair(FVector HitLocation) const
{
	FTransform ReturnValue = GetTransform();
	FVector TempLocation=GetTransform().GetLocation();
	TempLocation.Z+=(85+160);
	ReturnValue.SetLocation(TempLocation);

	return ReturnValue;
}