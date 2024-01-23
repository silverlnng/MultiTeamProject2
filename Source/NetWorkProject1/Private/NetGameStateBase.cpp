// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameStateBase.h"
#include "GameFramework/PlayerState.h"

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
	UE_LOG(LogTemp, Warning, TEXT("%s(%d) WinTeam : %s "), *FString(__FUNCTION__), __LINE__,*winTeamName);
}

//winTeamName 과 플레이어스테이트 비교해서 같은 플레이어들에게는 위젯띄우기 명령 하기
// winTeamName = TeamB 이면 ps->bTeamB =true 인 플레이어들에게 승리 위젯 
