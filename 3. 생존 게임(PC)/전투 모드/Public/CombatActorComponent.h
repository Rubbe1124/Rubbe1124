// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatActorComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HORSESTARTERKIT_API UCombatActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	USkeletalMeshComponent* Mesh;

	//모든 액션에 스테미나가 소모된다면 몽타주마다 스테미나 값을 쌍으로 변수로 필요함.
	//헤더 파일에서 다루는 변수 양을 가시적으로 줄이기 위해 맵 자료구조로 키 값을 몽타주로 지정하고, 
	//몽타주를 통해 스테미나 값을 꺼내올 수 있도록 설계함.
	//같은 키(몽타주)가 들어갈 경우 TMap은 기존 키-값을 대체한다.
	TMap<UAnimMontage*, int> ActionMapData;

	bool canAction;
	bool canRolling;

	
	bool saveAttack;
	int AttackCount;

public:
	FTimerHandle StaminaRecoverTimerHandle;
	FTimerHandle RollingTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	int MaxHP;
	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	int HP;
	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	int MaxStamina;
	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	int Stamina;
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int StaminaRecoverAmount;
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float StaminaRecoverDelay;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
	UAnimMontage* RollingMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
	TArray<UAnimMontage*> AttackMontageArray;

	UPROPERTY(BlueprintReadOnly, Category = "Condition")
	bool isAttacking;

	UFUNCTION(BlueprintCallable)
	void EssentialSetting(
		USkeletalMeshComponent* MeshInput,
		UAnimMontage* RollingMontageInput, int RollingStaminaCostInput,
		UAnimMontage* AttackMontageInput1, int AttackStaminaCostInput1,
		UAnimMontage* AttackMontageInput2, int AttackStaminaCostInput2,
		UAnimMontage* AttackMontageInput3, int AttackStaminaCostInput3,
		UAnimMontage* AttackMontageInput4, int AttackStaminaCostInput4,
		int MaxHPInput=0,
		int MaxStaminaInput=0,
		int StaminaRecoverAmountInput=5,
		float StaminaRecoverDelayInput=3.0f);

	void StaminaRecover();

	bool CanAction(UAnimMontage* Action);

	void PlayMontageSection(UAnimMontage* Montage, FName SectionName);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* CharacterRolling(float Cooltime=1.0f);

	void RollingDelay();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* CharacterAttack();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* ComboAttack();

	UFUNCTION(BlueprintCallable)
	void OnAttackMontageEnded();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(int Damage);
};
