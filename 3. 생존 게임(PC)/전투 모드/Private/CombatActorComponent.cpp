// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatActorComponent.h"

// UE_LOG(LogTemp, Log, TEXT("집고있는거 :%s"), *HoldingObject->GetClass()->GetName());
// UE_LOG(LogTemp, Log, TEXT("같네"));
UCombatActorComponent::UCombatActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCombatActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	MaxHP = 0;
	HP = 0;
	MaxStamina = 0;
	Stamina = 0;
	StaminaRecoverAmount = 5;
	StaminaRecoverDelay = 3.0f;

	canAction = true;

	canRolling = true;

	isAttacking = false;
	saveAttack = false;
	AttackCount = 0;

	// 게임 시작 후 StaminaRecoverDelay 간격마다 StaminaRecoverAmount만큼 스테미나 회복
	GetWorld()->GetTimerManager().SetTimer(StaminaRecoverTimerHandle, this, &UCombatActorComponent::StaminaRecover, StaminaRecoverDelay, true);
}

void UCombatActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// BeginPlay로 실행되서 일정 간격으로 스테미나 회복하는 기능
void UCombatActorComponent::StaminaRecover()
{
	Stamina += StaminaRecoverAmount;
	if (Stamina > MaxStamina)
		Stamina = MaxStamina;
}

// 수치와 몽타주 설정.
// 해당 액터컴포넌트를 부착하게 될 캐릭터의 BeginPlay에서 호출하는것이 바람직함.
void UCombatActorComponent::EssentialSetting(
	USkeletalMeshComponent *MeshInput,
	UAnimMontage *RollingMontageInput, int RollingStaminaCostInput,
	UAnimMontage *AttackMontageInput1, int AttackStaminaCostInput1,
	UAnimMontage *AttackMontageInput2, int AttackStaminaCostInput2,
	UAnimMontage *AttackMontageInput3, int AttackStaminaCostInput3,
	UAnimMontage *AttackMontageInput4, int AttackStaminaCostInput4,
	int MaxHPInput,
	int MaxStaminaInput,
	int StaminaRecoverAmountInput,
	float StaminaRecoverDelayInput)
{
	Mesh = MeshInput;
	MaxHP = MaxHPInput;
	HP = MaxHPInput;
	MaxStamina = MaxStaminaInput;
	Stamina = MaxStaminaInput;
	StaminaRecoverAmount = StaminaRecoverAmountInput;
	StaminaRecoverDelay = StaminaRecoverDelayInput;

	RollingMontage = RollingMontageInput;
	ActionMapData.Add(RollingMontage, RollingStaminaCostInput);

	AttackMontageArray.Add(AttackMontageInput1);
	ActionMapData.Add(AttackMontageInput1, AttackStaminaCostInput1);
	AttackMontageArray.Add(AttackMontageInput2);
	ActionMapData.Add(AttackMontageInput2, AttackStaminaCostInput2);
	AttackMontageArray.Add(AttackMontageInput3);
	ActionMapData.Add(AttackMontageInput3, AttackStaminaCostInput3);
	AttackMontageArray.Add(AttackMontageInput4);
	ActionMapData.Add(AttackMontageInput4, AttackStaminaCostInput4);
}

// 몽타주-스테미나 값을 다루는 맵 자료구조를 사용해서 입력으로 들어온 액션을 가지고 스테미나 값을 불러오고,
// 해당 값을 현재 스테미나 값과 비교해서 액션을 수행할 스테미나를 가지고 있는지 판별하는 함수.
bool UCombatActorComponent::CanAction(UAnimMontage *Action)
{
	if (Stamina < ActionMapData[Action])
		return false;
	else
		return true;
}

// 입력으로 들어온 액션을 수행하는 함수
// SectionName이 채워져서 들어오면 몽타주에서 지정된 해당 구역부터 재생됌.
void UCombatActorComponent::PlayMontageSection(UAnimMontage *Montage, FName SectionName)
{
	Stamina -= ActionMapData[Montage];
	UAnimInstance *AnimInstance = Mesh->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("메시 설정이 안되어 있습니다. 혹은 애님 인스턴스가 없습니다."));
		return;
	}
	if (AnimInstance && Montage)
	{
		canAction=false;
		AnimInstance->Montage_Play(Montage);
		if (SectionName == "")
			return;
		else
			AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

// 구르기 액션 함수
UAnimMontage *UCombatActorComponent::CharacterRolling(float Cooltime)
{
	if(!canAction)
	{
		UE_LOG(LogTemp, Log, TEXT("다른 액션이 수행중입니다."));
		return nullptr;
	}
	if (RollingMontage == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("구르기 몽타주가 설정되지않았습니다."));
		return nullptr;
	}
	if (CanAction(RollingMontage) && canRolling)
	{
		PlayMontageSection(RollingMontage, "");
		canRolling = false;
		GetWorld()->GetTimerManager().SetTimer(RollingTimerHandle, this, &UCombatActorComponent::RollingDelay, Cooltime, false);
		return RollingMontage;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("스테미나가 부족하거나 구르기가 판정 변수가 False입니다."));
		return nullptr;
	}
}

// 구르기 딜레이 함수
void UCombatActorComponent::RollingDelay()
{
	canRolling = true;
	canAction=true;
}

UAnimMontage *UCombatActorComponent::CharacterAttack()
{
	if (AttackMontageArray[AttackCount] == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("공격 몽타주가 설정되지않았습니다."));
		return nullptr;
	}
	if(isAttacking)
	{
		saveAttack=true;
		return nullptr;
	}
	if(!canAction)
	{
		UE_LOG(LogTemp, Log, TEXT("다른 액션이 수행중입니다."));
		return nullptr;
	}
	if (CanAction(AttackMontageArray[AttackCount]) && !isAttacking)
	{
		PlayMontageSection(AttackMontageArray[AttackCount], "");
		isAttacking = true;
		return AttackMontageArray[AttackCount];
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("스테미나가 부족하거나 공격 판정 변수가 true입니다."));
		return nullptr;
	}
}

UAnimMontage *UCombatActorComponent::ComboAttack()
{
	if(!saveAttack)
		return nullptr;
	
	saveAttack=false;
	if (AttackCount+1>=AttackMontageArray.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("공격 몽타주 배열의 범위를 벗어났습니다."));
		return nullptr;
	}
	if (AttackMontageArray[AttackCount+1] == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("다음 공격 몽타주가 설정되지않았습니다."));
		return nullptr;
	}
	if (CanAction(AttackMontageArray[AttackCount+1]))
	{
		PlayMontageSection(AttackMontageArray[AttackCount+1], "");
		AttackCount+=1;
		return AttackMontageArray[AttackCount];
	}
	return nullptr;
}

void UCombatActorComponent::OnAttackMontageEnded()
{
	isAttacking = false;
	saveAttack = false;
	AttackCount = 0;
	canAction=true;
}

void UCombatActorComponent::TakeDamage(int Damage)
{
	HP-=Damage;
	if(HP<0)
		HP=0;
}