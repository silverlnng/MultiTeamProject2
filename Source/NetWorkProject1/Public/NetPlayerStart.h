// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "NetPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT1_API ANetPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "MySettins")
	bool bTeamB;
};
