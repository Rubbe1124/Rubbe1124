// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingBaseObject.h"

// Sets default values
ABuildingBaseObject::ABuildingBaseObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);

	CalculationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CalculationMesh"));
	CalculationMesh->SetupAttachment(Root);

	//StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABuildingBaseObject::OnOverlapBegin); // Bind overlap begin event
	//StaticMesh->OnComponentEndOverlap.AddDynamic(this, &ABuildingBaseObject::OnOverlapEnd);		// Bind overlap end event

	CanBeBase = false;
}

// Called when the game starts or when spawned
void ABuildingBaseObject::BeginPlay()
{
	Super::BeginPlay();

	FloorNum = 1;

	//IsOverlapSomething = true;
}

// Called every frame
void ABuildingBaseObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*

	TArray<UPrimitiveComponent *> OverlappingComponents;

	// 오버랩된 컴포넌트들을 가져오기
	StaticMesh->GetOverlappingComponents(OverlappingComponents);

	IsOverlapSomething = true;

	// 가져온 컴포넌트들에 대한 작업 수행
	for (UPrimitiveComponent *OverlappingComponent : OverlappingComponents)
	{
		// 가져온 컴포넌트에 대한 로직 수행
		// 예: GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Overlapping Component: %s"), *OverlappingComponent->GetName()));
		// UE_LOG(LogTemp, Warning, TEXT("OverlappedComp: %s"), *OverlappingComponent->GetName());
		FString ComponentName = OverlappingComponent->GetName();
		if (!ComponentName.Contains(TEXT("InvisibleSM")))
		{
			// UE_LOG(LogTemp, Warning, TEXT("OverlappedComp: %s"), *OverlappingComponent->GetName());
			IsOverlapSomething = false;
		}
	}
	*/
}
/*
void ABuildingBaseObject::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Code to execute when overlap begins
	// OtherActor is the actor that overlaps with this actor
	// UE_LOG(LogTemp, Warning, TEXT("OverlappedComp: %s"), *OverlappedComp->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s"), *OtherComp->GetName());

}

// Overlap end event handler
void ABuildingBaseObject::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	// Code to execute when overlap ends
	// UE_LOG(LogTemp, Warning, TEXT("하나 탈출함"));
}
*/