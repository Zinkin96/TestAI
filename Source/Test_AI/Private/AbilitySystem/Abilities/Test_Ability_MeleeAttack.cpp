// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Ability/Test_Ability_MeleeAttack.h"
#include "AbilitySystemComponent.h"
#include "Test_GeneralPawnInterface.h"
#include "AbilitySystem/Test_AttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UTest_Ability_MeleeAttack::UTest_Ability_MeleeAttack()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack_Melee")));
}

void UTest_Ability_MeleeAttack::DealDamage(FGameplayEventData Payload)
{
	AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	const UTest_AttributeSet* AttributeSet = AbilitySystemComponent->GetSet<UTest_AttributeSet>();
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	MeleeAttackHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	MeleeAttackHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Values.MeleeAttackDamage")), -1.0f * AttributeSet->GetMeleeDamageAttribute().GetNumericValue(AttributeSet));;

	FGameplayAbilityTargetData_ActorArray* TargetData = new FGameplayAbilityTargetData_ActorArray();
	TargetData->TargetActorArray.Add(ITest_GeneralPawnInterface::Execute_GetTarget(GetOwningActorFromActorInfo()));
	FGameplayAbilityTargetDataHandle TargetDataHandle = FGameplayAbilityTargetDataHandle(TargetData);

	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, MeleeAttackHandle, TargetDataHandle);
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}

void UTest_Ability_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AnimMontageAsset);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &UTest_Ability_MeleeAttack::K2_EndAbility);
	PlayMontageAndWait->Activate();

	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Effect.ApplyMeleeDamage")), NULL, true);
	WaitGameplayEvent->EventReceived.AddDynamic(this, &UTest_Ability_MeleeAttack::DealDamage);	
	WaitGameplayEvent->Activate();
}