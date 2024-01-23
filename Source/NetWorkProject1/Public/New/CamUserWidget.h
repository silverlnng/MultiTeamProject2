// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CamUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT1_API UCamUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual  void NativeConstruct() override;
	
public:	
	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_Win;
	
	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_Lose;

	void ShowtextWin();
	void ShowtextLose();
		
		
};
