// Fill out your copyright notice in the Description page of Project Settings.


#include "New/CamUserWidget.h"
#include "Components/TextBlock.h"

void UCamUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	text_TeamB->SetVisibility(ESlateVisibility::Hidden);
	text_TeamA->SetVisibility(ESlateVisibility::Hidden);
	
}

void UCamUserWidget::ShowTextTeamB(FText value)
{
	text_TeamB->SetText(value);
	text_TeamB->SetVisibility(ESlateVisibility::Visible);
}

void UCamUserWidget::ShowTextTeamA(FText value)
{
	text_TeamA->SetText(value);
	text_TeamA->SetVisibility(ESlateVisibility::Visible);
}

