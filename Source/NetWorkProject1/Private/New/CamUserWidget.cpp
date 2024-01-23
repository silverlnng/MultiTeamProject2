// Fill out your copyright notice in the Description page of Project Settings.


#include "New/CamUserWidget.h"

#include "Components/TextBlock.h"

void UCamUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	text_Win->SetVisibility(ESlateVisibility::Hidden);
	text_Lose->SetVisibility(ESlateVisibility::Hidden);
	
}

void UCamUserWidget::ShowtextWin()
{
	text_Win->SetVisibility(ESlateVisibility::Visible);
}

void UCamUserWidget::ShowtextLose()
{
	text_Lose->SetVisibility(ESlateVisibility::Visible);
}

