// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleWidget.h"

#include "NetGameStateBase.h"
#include "NetworkGameInstance.h"
#include "NetworkPlayerController.h"
#include "Components//TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/GameStateBase.h"
#include "NetWorkProject1/NetWorkProject1Character.h"
#include "GameFramework/PlayerState.h"
#include "Components/HorizontalBox.h"


void UBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	player = GetOwningPlayerPawn<ANetWorkProject1Character>();
	//Ä³½ºÆÃÇØ¼­ °¡Á®¿À±â
	text_ammo->SetText(FText::AsNumber(0));
	btn_exitSession->OnClicked.AddDynamic(this,&UBattleWidget::OnexitSession);
	btn_Retry->OnClicked.AddDynamic(this,&UBattleWidget::OnRetry);
	
	text_PlayerList->SetText(FText::FromString(FString(TEXT("")))); //°ø¹éÀ¸·Î ÃÊ±âÈ­

	currentTime=spectatorTime;
}

void UBattleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(player!=nullptr)
	{
		int32 ammoCount = player->GetAmmo();
		text_ammo->SetText(FText::AsNumber(ammoCount));
		
		TArray<APlayerState*> players = GetWorld()->GetGameState<ANetGameStateBase>()->GetMyPlayerList();
		//¿ùµå¿¡¼­ °ÔÀÓ½ºÅ×ÀÌÆ® °¡Á®¿Í¼­ ¹è¿­¿¡ ³Ö°í
		playerList="";
		for(APlayerState* ps :players)
		{
			AddPlayerList(ps->GetPlayerName(),ps->GetScore());
		}
	}
	if(bProcessTimer)
	{
		if(currentTime>0)
		{
			currentTime-=InDeltaTime;
		}
		else
		{
			bProcessTimer=false;
			currentTime=spectatorTime;
			text_RespawnTimer->SetVisibility(ESlateVisibility::Hidden);
		}
		text_RespawnTimer->SetText(FText::AsNumber((int32)currentTime));
	}
}

void UBattleWidget::PlayHitAnimation()
{
	PlayAnimationForward(hitAnim);
	//Á¤¹æÇâ , ¿ª¹æÇâ ¾Ö´Ï¸ÞÀÌ¼Ç ½ÇÇà °¡´É
}

void UBattleWidget::ShowButtons()
{
	//btn_exitSession->SetVisibility(ESlateVisibility::Visible);
	//SetVisibility °¡ UWidget ¿¡ ¼±¾ð µÇ¾îÀÖÀ½
	hb_menuButtons->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//visible À» ÇÏ¸é °¡Àå »óÀ§¿¡ ÀÖ´Â hb_menuButtons ¿¡¼­ ÀÎÅÍ·º¼ÇÀ» ¸·À½
	//SelfHitTestInvisible À¸·Î ÀÚ½ÅÀÇ ÀÎÅÍ·º¼ÇÀº ¸·°í , ÀÚ½ÄÀÇ ÀÎÅÍ·º¼ÇÀ» È°¼ºÈ­ ÇÏ±â 
}

void UBattleWidget::AddPlayerList(FString playerName,float score)
{
	playerList.Append(FString::Printf(TEXT("%s score: %d \n"),*playerName,(int32)score));
	// ÀÌ¸§ Ãß°¡ÇÏ°í ÇÑÁÙ ¶ç·ç°í °è¼Ó Ãß°¡ ...
	text_PlayerList->SetText(FText::FromString(FString(playerList)));
	
}

void UBattleWidget::OnexitSession()
{
	GetGameInstance<UNetworkGameInstance>()->ExitMySession();
}


void UBattleWidget::OnRetry()
{
	UE_LOG(LogTemp, Warning, TEXT("%s(%d) : %s"), *FString(__FUNCTION__), __LINE__,*FString("OnRetryClicked"));
	
	//¹öÆ°À» ´©¸£´Â »ç¶÷Àº Å¬¶óÀÌ¾ðÆ® µÉ¼öµµ ÀÖ¾î¼­ ChangeCharToSpectator À» rpcÇÔ¼ö·Î ¸¸µé±â  
	ANetworkPlayerController* pc =player->GetController<ANetworkPlayerController>();


	pc->ChangeCharToSpectator();
	//ÀÔ·Âµµ
	pc->SetShowMouseCursor(false);
	pc->SetInputMode(FInputModeGameOnly());
	hb_menuButtons->SetVisibility(ESlateVisibility::Hidden);

	text_RespawnTimer->SetVisibility(ESlateVisibility::Visible);
	bProcessTimer=true;

}