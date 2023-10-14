// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN_SendTagToActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"

void UAN_SendTagToActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	TObjectPtr<AActor> Owner = MeshComp->GetOwner();
	FGameplayEventData GameplayEventData;
	GameplayEventData.Instigator = Owner;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, GameplayTag, GameplayEventData);
}
