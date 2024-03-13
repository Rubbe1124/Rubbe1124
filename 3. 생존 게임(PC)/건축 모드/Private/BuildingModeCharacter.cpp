// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingModeCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

#include "BuildingWoodFoundation.h"
#include "BuildingWoodWall.h"
#include "BuildingWoodCeiling.h"

// Sets default values
ABuildingModeCharacter::ABuildingModeCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 경로에 해당하는 머티리얼을 찾기
	static ConstructorHelpers::FObjectFinder<UMaterial> DebugMaterialFinder(TEXT("/Game/NYB/Material/M_FieldVolumePreview1.M_FieldVolumePreview1"));

	// 머티리얼이 있는지 확인
	if (DebugMaterialFinder.Succeeded())
	{
		// 머티리얼을 얻어옴
		CheckFailMaterial = DebugMaterialFinder.Object;
	}
}

// Called when the game starts or when spawned
void ABuildingModeCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = FindComponentByClass<USpringArmComponent>();
	if (SpringArm == nullptr)
		return;

	UWorld *World = GetWorld();
	if (World == nullptr)
		return;
	APlayerController *PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr))
		return;

	PlayerController->bShowMouseCursor = true;

	CanBeBuilt = false;

	IsLeftMouseDown = false;
}

// Called every frame
void ABuildingModeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult OutHit;
	bool isHit = IsHitLineTraceMousePositionToWorld(&OutHit); // 마우스 좌표를 월드상 좌표로 변환 후 해당 위치로 라인트레이스 발사
	if (isHit)
	{
		FVector HitLocation = OutHit.Location;
		if (IsLeftMouseDown)
		{
			BuildOneObjectOnMousePosition(HitLocation); // 클릭시 라인트레이스 위치로 오브젝트 건설
		}

		if (HoldingObject != nullptr)
		{
			SetTransformBasedOnTarget(HitLocation); // 라인트레이스가 BuildingBaseObject에 맞을 때 컴포넌트 확인 후 가장 가까운 컴포넌트의 위치로 홀딩 오브젝트 위치 변경
			SetFloorNumBasedOnTarget();				// 홀딩 오브젝트의 층 결정
			CanBeBuilt = IsValidToBuild(HitLocation);
			ChangeMaterial(CanBeBuilt);
		}
	}
	else
	{
		HoldingObjectOnAir(); // 라인트레이스로 맞춘 타겟이 없다면 공중에 홀딩오브젝트 띄워놓음
		ChangeMaterial(false);
		CanBeBuilt = false;
	}
}

// Called to bind functionality to input
void ABuildingModeCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABuildingModeCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABuildingModeCharacter::MoveRight);

	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed, this, &ABuildingModeCharacter::LeftMousePressed);
	PlayerInputComponent->BindAction("LeftMouse", IE_Released, this, &ABuildingModeCharacter::LeftMouseReleased);

	PlayerInputComponent->BindAction("RightMouse", IE_Pressed, this, &ABuildingModeCharacter::RightMousePressed);
	PlayerInputComponent->BindAction("RightMouse", IE_Released, this, &ABuildingModeCharacter::RightMouseReleased);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABuildingModeCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABuildingModeCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &ABuildingModeCharacter::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &ABuildingModeCharacter::ZoomOut);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Num1"), EInputEvent::IE_Pressed, this, &ABuildingModeCharacter::PressNum1);
	PlayerInputComponent->BindAction(TEXT("Num2"), EInputEvent::IE_Pressed, this, &ABuildingModeCharacter::PressNum2);
	PlayerInputComponent->BindAction(TEXT("Num3"), EInputEvent::IE_Pressed, this, &ABuildingModeCharacter::PressNum3);

	PlayerInputComponent->BindAction(TEXT("Rotation"), EInputEvent::IE_Pressed, this, &ABuildingModeCharacter::SetRotationMode);
	PlayerInputComponent->BindAction(TEXT("Rotation"), EInputEvent::IE_Released, this, &ABuildingModeCharacter::ReleaseRotationMode);

	PlayerInputComponent->BindAction(TEXT("PageUp"), EInputEvent::IE_Pressed, this, &ABuildingModeCharacter::PageUp);
	PlayerInputComponent->BindAction(TEXT("PageDown"), EInputEvent::IE_Pressed, this, &ABuildingModeCharacter::PageDown);

	PlayerInputComponent->BindAction(TEXT("Tab"), EInputEvent::IE_Released, this, &ABuildingModeCharacter::PressTab);

	PlayerInputComponent->BindAction(TEXT("Esc"), EInputEvent::IE_Released, this, &ABuildingModeCharacter::PressEsc);

	PlayerInputComponent->BindAction(TEXT("Delete"), EInputEvent::IE_Released, this, &ABuildingModeCharacter::PressDelete);
}

void ABuildingModeCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ABuildingModeCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void ABuildingModeCharacter::LeftMousePressed()
{
	if (HoldingObject == nullptr)
		return;
	// IsLeftMouseDown = true;
}

void ABuildingModeCharacter::LeftMouseReleased()
{
	if (HoldingObject == nullptr)
		return;
	IsLeftMouseDown = true;
}

void ABuildingModeCharacter::RightMousePressed()
{
	IsRightMouseDown = true;
	// Your logic for right mouse button press
}

void ABuildingModeCharacter::RightMouseReleased()
{
	IsRightMouseDown = false;
}

void ABuildingModeCharacter::LookUp(float AxisValue)
{
	if (IsRightMouseDown)
		AddControllerPitchInput(AxisValue);
}

void ABuildingModeCharacter::Turn(float AxisValue)
{
	if (IsRightMouseDown)
		AddControllerYawInput(AxisValue);
}

void ABuildingModeCharacter::ZoomIn()
{
	if (!Rotation)
		SpringArm->TargetArmLength = FMath::Clamp<float>(SpringArm->TargetArmLength - 30.0f, 150.0f, 3000.0f);
	else
	{
		if (HoldingObject == nullptr)
			return;
		FTransform CurrentTransform = HoldingObject->GetActorTransform();

		// 현재 회전값을 얻어옵니다.
		FRotator CurrentRotation = CurrentTransform.Rotator();

		double temp = CurrentRotation.Yaw;
		temp += 15;

		// Z축만 90도 회전한 값을 설정합니다.
		FRotator NewRotation = FRotator(CurrentRotation.Pitch, temp, CurrentRotation.Roll);

		// 새로운 트랜스폼을 생성하고 액터에 적용합니다.
		FTransform NewTransform = FTransform(NewRotation, CurrentTransform.GetTranslation(), CurrentTransform.GetScale3D());
		HoldingObject->SetActorTransform(NewTransform);
	}
}
void ABuildingModeCharacter::ZoomOut()
{
	if (!Rotation)
		SpringArm->TargetArmLength = FMath::Clamp<float>(SpringArm->TargetArmLength + 30.0f, 150.0f, 3000.0f);
	else
	{
		if (HoldingObject == nullptr)
			return;
		FTransform CurrentTransform = HoldingObject->GetActorTransform();

		// 현재 회전값을 얻어옵니다.
		FRotator CurrentRotation = CurrentTransform.Rotator();

		double temp = CurrentRotation.Yaw;
		temp -= 15;

		// Z축만 90도 회전한 값을 설정합니다.
		FRotator NewRotation = FRotator(CurrentRotation.Pitch, temp, CurrentRotation.Roll);

		// 새로운 트랜스폼을 생성하고 액터에 적용합니다.
		FTransform NewTransform = FTransform(NewRotation, CurrentTransform.GetTranslation(), CurrentTransform.GetScale3D());
		HoldingObject->SetActorTransform(NewTransform);
	}
}

bool ABuildingModeCharacter::IsHitLineTraceMousePositionToWorld(FHitResult *OutHit)
{
	// FHitResult OutHit;
	FVector Start, End;
	Cast<APlayerController>(GetController())->DeprojectMousePositionToWorld(Start, End);
	End = End * 3000.0f + Start;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (HoldingObject != nullptr)
	{
		CollisionParams.AddIgnoredActor(HoldingObject);
	}
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.05f);

	bool isHit = GetWorld()->LineTraceSingleByChannel(*OutHit, Start, End, ECC_Visibility, CollisionParams);

	if (isHit)
	{
		TargetObject = OutHit->GetActor();
	}
	return isHit;
}

void ABuildingModeCharacter::BuildOneObjectOnMousePosition(FVector HitLocation)
{
	if (HoldingObject == nullptr || !CanBeBuilt)
	{
		IsLeftMouseDown = false;
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor *TempActor = GetWorld()->SpawnActor<AActor>(HoldingObject->GetClass(), HoldingObject->GetActorLocation(), HoldingObject->GetActorRotation(), SpawnParams);

	HoldingObject->SetActorEnableCollision(true);

	ABuildingBaseObject *HoldingBuilding = Cast<ABuildingBaseObject>(HoldingObject);

	UStaticMeshComponent *MyStaticMeshComponent = HoldingBuilding->CalculationMesh;

	FName DesiredCollisionProfile = FName("BlockAllDynamic");

	MyStaticMeshComponent->SetCollisionProfileName(DesiredCollisionProfile);
	HoldingObject = nullptr;

	// 건설 후 지우지않고 남겨놓기

	HoldingObject = TempActor;

	HoldingBuilding = Cast<ABuildingBaseObject>(HoldingObject);

	MyStaticMeshComponent = HoldingBuilding->CalculationMesh;

	DesiredCollisionProfile = FName("OverlapAllDynamic");

	MyStaticMeshComponent->SetCollisionProfileName(DesiredCollisionProfile);

	OriginalMaterialInterface = nullptr;

	IsLeftMouseDown = false;
}

void ABuildingModeCharacter::SettingHoldingObejct(ABuildingBaseObject *SeletedObject)
{
	UWorld *World = GetWorld();
	if (World)
	{
		if (HoldingObject != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("집고있는거 :%s"), *HoldingObject->GetClass()->GetName());
			UE_LOG(LogTemp, Log, TEXT("바꾸려는거 :%s"), *SeletedObject->GetClass()->GetName());
			if (HoldingObject->GetClass()->GetName() == SeletedObject->GetClass()->GetName())
			{
				UE_LOG(LogTemp, Log, TEXT("같네"));
				SeletedObject->Destroy();
				return;
			}
			AActor *TempObject = HoldingObject;
			HoldingObject = nullptr;
			TempObject->Destroy();
		}

		HoldingObject = SeletedObject;
		ABuildingBaseObject *HoldingBuilding = Cast<ABuildingBaseObject>(SeletedObject);
		// 특정 스태틱 메시 컴포넌트에 대한 포인터 가져오기 (예: UStaticMeshComponent* MyStaticMeshComponent)
		UStaticMeshComponent *MyStaticMeshComponent = HoldingBuilding->CalculationMesh; // 스태틱 메시 컴포넌트에 대한 포인터를 어떻게 얻을지에 대한 부분은 여러 가지 방법이 있습니다.

		// 원하는 콜리전 프리셋의 이름 설정
		FName DesiredCollisionProfile = FName("OverlapAllDynamic");

		// 콜리전 프리셋 설정
		MyStaticMeshComponent->SetCollisionProfileName(DesiredCollisionProfile);

		OriginalMaterialInterface = nullptr;
	}
}

void ABuildingModeCharacter::SetTransformBasedOnTarget(FVector HitLocation)
{
	if (TargetObject->IsA(ABuildingBaseObject::StaticClass()))	//타겟 오브젝트가 건물이라면...
	{
		ABuildingBaseObject *TargetBuilding = Cast<ABuildingBaseObject>(TargetObject);
		if (TargetBuilding)
		{
			FTransform Transform = TargetBuilding->FindNearestTransform(HitLocation, HoldingObject);
			if (AreTransformsEqual(Transform, TargetObject->GetTransform(), 1.0f))
			{
				// UE_LOG(LogTemp, Log, TEXT("같네"));
				Transform = HoldingObject->GetTransform();
				Transform.SetLocation(HitLocation);
				HoldingObject->SetActorTransform(Transform);
			}
			else
			{
				// UE_LOG(LogTemp, Log, TEXT("다르네"));
				if (HoldingObject->IsA(ABuildingWoodStair::StaticClass()) || HoldingObject->IsA(ABuildingWoodHalfWall::StaticClass()))
				{
					Transform.SetRotation(HoldingObject->GetTransform().GetRotation());
				}
				HoldingObject->SetActorTransform(Transform);
			}
		}
	}
	else	///타겟 오브젝트가 건물이 아니라면... (바닥이나 기타 오브젝트)
	{
		if (HoldingObject->IsA(ABuildingWoodFoundation::StaticClass()))	
		{
			FTransform Transform = HoldingObject->GetTransform();
			FVector Location = HitLocation;
			Location.Z += 85;
			Transform.SetLocation(Location);
			HoldingObject->SetActorTransform(Transform);
		}
		else
		{
			FTransform Transform = HoldingObject->GetTransform();
			Transform.SetLocation(HitLocation);
			HoldingObject->SetActorTransform(Transform);
		}
	}
}

bool ABuildingModeCharacter::AreTransformsEqual(const FTransform &Transform1, const FTransform &Transform2, float Tolerance)
{
	return FMath::IsNearlyEqual(Transform1.GetTranslation().X, Transform2.GetTranslation().X, Tolerance) &&
		   FMath::IsNearlyEqual(Transform1.GetTranslation().Y, Transform2.GetTranslation().Y, Tolerance) &&
		   FMath::IsNearlyEqual(Transform1.GetTranslation().Z, Transform2.GetTranslation().Z, Tolerance);
}

void ABuildingModeCharacter::SetFloorNumBasedOnTarget()
{
	ABuildingBaseObject *TargetBuilding = Cast<ABuildingBaseObject>(TargetObject);
	ABuildingBaseObject *HoldingBuilding = Cast<ABuildingBaseObject>(HoldingObject);
	int TempFloorNum;
	if (TargetObject->IsA(ABuildingBaseObject::StaticClass()))
	{
		if (TargetBuilding)
		{
			TempFloorNum = TargetBuilding->FloorNum;
			if (TargetBuilding->CanBeBase)
			{
				HoldingBuilding->FloorNum = TempFloorNum;
			}
			else
			{

				HoldingBuilding->FloorNum = TempFloorNum + 1;
			}
		}
	}
	else
	{
		if (HoldingBuilding)
		{
			HoldingBuilding->FloorNum = 1;
		}
	}
}

bool ABuildingModeCharacter::IsValidToBuild(FVector HitLocation)
{
	if (HoldingObject->IsA(ABuildingWoodFoundation::StaticClass()))
	{
		if (TargetObject == nullptr)
			return false;
		// UE_LOG(LogTemp, Warning, TEXT("타겟: %s"), *TargetObject->GetClass()->GetName());
		if (TargetObject->GetClass()->GetName() != "Landscape" && !TargetObject->IsA(ABuildingWoodFoundation::StaticClass()))
		{
			return false;
		}
	}
	else
	{
		if (TargetObject != nullptr && !TargetObject->IsA(ABuildingBaseObject::StaticClass()))
		{
			return false;
		}
	}
	if (TargetObject != nullptr && TargetObject->IsA(ABuildingBaseObject::StaticClass()))
	{
		ABuildingBaseObject *TargetBuilding = Cast<ABuildingBaseObject>(TargetObject);
		FTransform Transform = TargetBuilding->FindNearestTransform(HitLocation, HoldingObject);
		if (AreTransformsEqual(Transform, TargetObject->GetTransform(), 1.0f))
		{
			return false;
		}
	}

	TArray<UPrimitiveComponent *> OverlappingComponents;

	ABuildingBaseObject *HoldingBuilding = Cast<ABuildingBaseObject>(HoldingObject);
	// 특정 스태틱 메시 컴포넌트에 대한 포인터 가져오기 (예: UStaticMeshComponent* MyStaticMeshComponent)
	UStaticMeshComponent *MyStaticMeshComponent = HoldingBuilding->CalculationMesh;

	// 오버랩된 컴포넌트들을 가져오기
	MyStaticMeshComponent->GetOverlappingComponents(OverlappingComponents);

	bool IsOverlapSomething = true;

	for (UPrimitiveComponent *OverlappingComponent : OverlappingComponents)
	{
		FString ComponentName = OverlappingComponent->GetName();
		if (OverlappingComponent->GetOwner() == HoldingObject)
		{
			// UE_LOG(LogTemp, Warning, TEXT("내꺼: %s"), *OverlappingComponent->GetName());
			// continue;
		}
		else if (OverlappingComponent->GetOwner() == this)
		{
			// UE_LOG(LogTemp, Warning, TEXT("그거 나임: %s"), *OverlappingComponent->GetName());
		}
		else if (!ComponentName.Contains(TEXT("InvisibleSM")))
		{
			UE_LOG(LogTemp, Warning, TEXT("OverlappedComp: %s"), *OverlappingComponent->GetName());
			IsOverlapSomething = false;
		}
		// UE_LOG(LogTemp, Warning, TEXT("OverlappedComp: %s"), *OverlappingComponent->GetName());
	}
	return IsOverlapSomething;
}

void ABuildingModeCharacter::ChangeMaterial(bool CanBuilt)
{
	if (HoldingObject == nullptr)
	{
		return;
	}
	ABuildingBaseObject *HoldingBuilding = Cast<ABuildingBaseObject>(HoldingObject);

	UStaticMeshComponent *MyStaticMeshComponent = HoldingBuilding->StaticMesh;

	if (!CanBeBuilt)
	{
		if (OriginalMaterialInterface == nullptr)
		{
			if (HoldingObject->IsA(ABuildingWoodCeiling::StaticClass()))
			{
				OriginalMaterialInterface = MyStaticMeshComponent->GetMaterial(1);
			}
			else
			{
				OriginalMaterialInterface = MyStaticMeshComponent->GetMaterial(0);
			}
		}

		if (HoldingObject->GetClass()->GetName() == "BuildingWoodCeiling")
		{
			//UE_LOG(LogTemp, Warning, TEXT("천장임"));
			MyStaticMeshComponent->SetMaterial(1, CheckFailMaterial);
		}
			MyStaticMeshComponent->SetMaterial(0, CheckFailMaterial);
	}
	else
	{
		if (OriginalMaterialInterface == nullptr)
		{
			// UE_LOG(LogTemp, Warning, TEXT("없네"));
			return;
		}
		// UE_LOG(LogTemp, Warning, TEXT("있네"));

		if (HoldingObject->GetClass()->GetName() == "BuildingWoodCeiling")
		{
			MyStaticMeshComponent->SetMaterial(1, OriginalMaterialInterface);
		}

			MyStaticMeshComponent->SetMaterial(0, OriginalMaterialInterface);

	}
}

void ABuildingModeCharacter::HoldingObjectOnAir()
{
	if (HoldingObject == nullptr)
		return;
	FVector Start, End;
	Cast<APlayerController>(GetController())->DeprojectMousePositionToWorld(Start, End);
	End = End * 3000.0f + Start;
	FTransform Transform = HoldingObject->GetTransform();
	Transform.SetLocation(End);
	HoldingObject->SetActorTransform(Transform);
}

void ABuildingModeCharacter::SetRotationMode()
{
	// UE_LOG(LogTemp, Log, TEXT("로테이션 모드"));
	Rotation = true;
}

void ABuildingModeCharacter::ReleaseRotationMode()
{
	// UE_LOG(LogTemp, Log, TEXT("로테이션 모드 해제"));
	Rotation = false;
}

void ABuildingModeCharacter::PageUp()
{
	FloorNum++;

	TArray<ABuildingBaseObject *> BuildingBaseObject;
	FindAllActors(GetWorld(), BuildingBaseObject);

	for (ABuildingBaseObject *Element : BuildingBaseObject)
	{
		if (Element->FloorNum == FloorNum)
		{
			Element->SetActorHiddenInGame(false);
		}
		else
		{
			Element->SetActorHiddenInGame(true);
		}
	}
}

void ABuildingModeCharacter::PageDown()
{
	if (FloorNum <= 0)
		return;

	FloorNum--;

	TArray<ABuildingBaseObject *> BuildingBaseObject;
	FindAllActors(GetWorld(), BuildingBaseObject);

	for (ABuildingBaseObject *Element : BuildingBaseObject)
	{
		if (FloorNum == 0)
		{
			Element->SetActorHiddenInGame(false);
			continue;
		}
		if (Element->FloorNum == FloorNum)
		{
			Element->SetActorHiddenInGame(false);
		}
		else
		{
			Element->SetActorHiddenInGame(true);
		}
	}
}

void ABuildingModeCharacter::PressEsc()
{
	if(HoldingObject!=nullptr)
	{
		HoldingObject->Destroy();
	}
	HoldingObject=nullptr;
}

void ABuildingModeCharacter::PressDelete()
{
	if(HoldingObject==nullptr&&TargetObject->IsA(ABuildingBaseObject::StaticClass()))
	{
		TargetObject->Destroy();
	}
	return;
}

template <typename T>
void ABuildingModeCharacter::FindAllActors(UWorld *World, TArray<T *> &Out)
{
	for (TActorIterator<T> It(World); It; ++It)
	{
		Out.Add(*It);
	}
}