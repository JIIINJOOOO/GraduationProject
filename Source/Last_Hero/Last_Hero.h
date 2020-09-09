// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG.h"
#include "UMGStyle.h"
#include "Slate/SObjectWidget.h"
#include "IUMGModule.h"
#include "Blueprint/UserWidget.h"
#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Last_Hero, Log, All);
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(Last_Hero, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(Last_Hero, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define ABCHECK(Expr, ...) {if(!(Expr)) {ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}


enum GOLEM_ANIM_MONTAGE
{
	CLOSE_RANGE_START = 0, NORMAL_ATTACK, HANDCLAP_ATTACK, SWEEP_ATTACK, PUNCH_ATTACK, STOMP_ATTACK, WALKING_ATTACK, RUSH_CLOSE, ICE_EXPLO, CLOSE_RANGE_END, // Close Range
	LONG_RANGE_START /*= 10*/, RUSH_ATTACK, THROW_STONE, THROW_SPEAR, LONG_RANGE_END,	// Long Range
	DOWN_START /*= 15*/, DOWN_ATTACK_1, DOWN_ATTACK_2, DOWN_END,		// Down State
	BROKENLEG_FALLING // Down Falling Anim 
};

