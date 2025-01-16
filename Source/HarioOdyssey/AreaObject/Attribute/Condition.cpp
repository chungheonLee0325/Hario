// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition.h"

bool UCondition::AddCondition(EConditionType Condition)
{
	if (HasCondition(Condition))
		return false;

	ConditionFlags |= static_cast<uint8>(Condition);
	return true;
}

bool UCondition::RemoveCondition(EConditionType Condition)
{
	if (HasCondition(Condition))
		return false;
	
	ConditionFlags &= ~static_cast<uint8>(Condition);
	return true;
}

bool UCondition::HasCondition(EConditionType Condition) const
{
	return (((ConditionFlags) & (static_cast<uint8>(Condition))) != 0);
}

bool UCondition::ExchangeDead()
{
	if (HasCondition(EConditionType::Dead))
		return false;

	return AddCondition(EConditionType::Dead);
}

void UCondition::Restart()
{
	ConditionFlags = 0;
}
