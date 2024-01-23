// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameStateBase.h"

#include "NetPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "New/CamUserWidget.h"
#include "New/PlayerPawn.h"

TArray<APlayerState*> ANetGameStateBase::GetMyPlayerList()
{
	//PlayerArray.Sort(ANetGameStateBase::AscendingByString);
	// Sort() :(퀵정렬) 안에 조건식을 넣어서

	// 1) static 조건식 함수를 연결하는 방식 
	PlayerArray.Sort(ANetGameStateBase::AscendingByString);

	// 2) 람다식으로 사용하는 방식 
	PlayerArray.Sort([](const APlayerState& ps1, const APlayerState& ps2)
	{
		return ps1.GetPlayerName() < ps2.GetPlayerName();
	});

	//점수순 정렬
	PlayerArray.Sort([](const APlayerState& ps1, const APlayerState& ps2)
	{
		return ps1.GetScore() >= ps2.GetScore();
	});
	
	return PlayerArray;
}

bool ANetGameStateBase::AscendingByString(const APlayerState& ps1, const APlayerState& ps2)
{
	return ps1.GetPlayerName() < ps2.GetPlayerName();

	//ps1->GetPlayerName() < ps2->GetPlayerName(); 아스키코드를 비교 :  오름차순 순서 => 오른쪽 값이 더크면 true를 반환
}

bool ANetGameStateBase::DescendingByString(const APlayerState& ps1, const APlayerState& ps2)
{
	return ps1.GetPlayerName() > ps2.GetPlayerName();
}

bool ANetGameStateBase::DescendingByScore(const APlayerState& ps1, const APlayerState& ps2)
{
	return ps1.GetScore() >= ps2.GetScore();
}

bool ANetGameStateBase::AscendingByScore(const APlayerState& ps1, const APlayerState& ps2)
{
	return ps1.GetScore() < ps2.GetScore();
}

void ANetGameStateBase::setWinTeam(FString value)
{
	winTeamName=value;
	
	ShowPlayerWidget();
}

void ANetGameStateBase::ShowPlayerWidget()
{
// winTeamName 과 플레이어스테이트 비교해서 같은 플레이어들에게는 위젯띄우기 명령 하기
// winTeamName = TeamB 이면 ps->bTeamB =true 인 플레이어들에게 승리 위젯
	
	for(APlayerState* It : PlayerArray)
	{
		//PlayerArray 중에서 로컬에 PlayerState만 찾아야함 
		ANetPlayerState* NetPS = Cast<ANetPlayerState>(It);
		
		APlayerPawn* playerPawn = NetPS->GetPawn<APlayerPawn>();

		AController* PC = playerPawn->GetController();
		
		UE_LOG(LogTemp, Warning, TEXT("%s(%d) WinTeam : %s "), *FString(__FUNCTION__), __LINE__,*winTeamName);
		
		if(NetPS && PC && PC->IsLocalController())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s(%d) WinTeam : %s "), *FString(__FUNCTION__), __LINE__,*winTeamName);
			if(winTeamName.Equals("TeamB"))
			{
				if(NetPS->bTeamB)
				{
					//로컬의 NetPS 에서 승리 위젯
					playerPawn->TwoCamRenderUI->ShowTextTeamB(FText::FromString("Win"));
					//pawn  에 접근해서 ui를 나오도록 하기 
				}
				else
				{
					playerPawn->TwoCamRenderUI->ShowTextTeamA(FText::FromString("Lose"));
					//로컬의 NetPS 에서 lose 위젯  
				}
			}
			else //승리팀이 A팀인경우
				{
				if(!(NetPS->bTeamB))
				{
					playerPawn->TwoCamRenderUI->ShowTextTeamA(FText::FromString("Win"));
					//로컬의 NetPS 에서 승리 위젯 
				}
				else
				{
					playerPawn->TwoCamRenderUI->ShowTextTeamB(FText::FromString("Lose"));
					//로컬의 NetPS 에서 lose 위젯  
				}
				}
		}
		
		
	}
}

