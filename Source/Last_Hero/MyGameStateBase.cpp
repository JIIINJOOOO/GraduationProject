// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

AMyGameStateBase::AMyGameStateBase()
{
	MonsterKillScore = 0;
	bGameCleared = false;

}


int32 AMyGameStateBase::GetMonsterKillScore() const
{
	return MonsterKillScore;
}

void AMyGameStateBase::SetGameCleared()
{
	bGameCleared = true;
}

bool AMyGameStateBase::IsGameCleared() const
{
	return bGameCleared;
}

