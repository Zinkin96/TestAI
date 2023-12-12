// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/Test_Ability_MeleeAttack.h"
#include "AbilitySystemComponent.h"
#include "Test_GeneralPawnInterface.h"
#include "AbilitySystem/Test_AttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Math/Vector.h"

UTest_Ability_MeleeAttack::UTest_Ability_MeleeAttack()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack_Melee")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
}

void UTest_Ability_MeleeAttack::DealDamage(FGameplayEventData Payload)
{
	if (!CheckDistance(GetOwningActorFromActorInfo()))
	{
		PlayMontageAndWait->OnCompleted.RemoveDynamic(this, &UTest_Ability_MeleeAttack::K2_EndAbility);
		WaitGameplayEvent->EventReceived.RemoveDynamic(this, &UTest_Ability_MeleeAttack::DealDamage);
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}
	AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	const UTest_AttributeSet* AttributeSet = AbilitySystemComponent->GetSet<UTest_AttributeSet>();
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	MeleeAttackHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	MeleeAttackHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Values.HealthChange")), -1.0f * AttributeSet->GetMeleeDamageAttribute().GetNumericValue(AttributeSet));;

	FGameplayAbilityTargetData_ActorArray* TargetData = new FGameplayAbilityTargetData_ActorArray();
	TargetData->TargetActorArray.Add(ITest_GeneralPawnInterface::Execute_GetTarget(GetOwningActorFromActorInfo()));
	FGameplayAbilityTargetDataHandle TargetDataHandle = FGameplayAbilityTargetDataHandle(TargetData);

	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, MeleeAttackHandle, TargetDataHandle);
	WaitGameplayEvent->EventReceived.RemoveDynamic(this, &UTest_Ability_MeleeAttack::DealDamage);
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	return;
}

void UTest_Ability_MeleeAttack::EndAbilityAndUnbind()
{
	PlayMontageAndWait->OnCompleted.RemoveDynamic(this, &UTest_Ability_MeleeAttack::EndAbilityAndUnbind);
	PlayMontageAndWait->OnBlendOut.RemoveDynamic(this, &UTest_Ability_MeleeAttack::EndAbilityAndUnbind);
	PlayMontageAndWait->OnInterrupted.RemoveDynamic(this, &UTest_Ability_MeleeAttack::EndAbilityAndUnbind);
	PlayMontageAndWait->OnCancelled.RemoveDynamic(this, &UTest_Ability_MeleeAttack::EndAbilityAndUnbind);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

bool UTest_Ability_MeleeAttack::CheckDistance(AActor* const AvatarActor) const
{
	if (!IsValid(AvatarActor))
	{
		CurrentActorInfo->OwnerActor.Get();
		return false;
	}

	AActor* TargetActor = ITest_GeneralPawnInterface::Execute_GetTarget(AvatarActor);
	if (!IsValid(TargetActor))
	{
		return false;
	}

	FVector Vector1 = AvatarActor->GetActorLocation();
	FVector Vector2 = TargetActor->GetActorLocation();
	float Distance = FVector::Distance(Vector1, Vector2);

	if(Distance <= AttackRange)
	{
		return true;
	}
	return false;
}

void UTest_Ability_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AnimMontageAsset);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &UTest_Ability_MeleeAttack::EndAbilityAndUnbind);
	PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UTest_Ability_MeleeAttack::EndAbilityAndUnbind);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UTest_Ability_MeleeAttack::EndAbilityAndUnbind);
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &UTest_Ability_MeleeAttack::EndAbilityAndUnbind);
	PlayMontageAndWait->Activate();

	WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Effect.ApplyHealthChange")), NULL, true);
	WaitGameplayEvent->EventReceived.AddDynamic(this, &UTest_Ability_MeleeAttack::DealDamage);	
	WaitGameplayEvent->Activate();
}

bool UTest_Ability_MeleeAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	AActor* const AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	if (!AvatarActor)
	{
		return false;
	}
	if (!CheckDistance(AvatarActor))
	{
		return false;
	}
	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags,OptionalRelevantTags);
	return true;
}
