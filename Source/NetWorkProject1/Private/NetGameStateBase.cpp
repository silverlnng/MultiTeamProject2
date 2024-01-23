// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameStateBase.h"

#include "NetPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "New/CamUserWidget.h"
#include "New/PlayerPawn.h"

TArray<APlayerState*> ANetGameStateBase::GetMyPlayerList()
{
	//PlayerArray.Sort(ANetGameStateBase::AscendingByString);
	// Sort() :(������) �ȿ� ���ǽ��� �־

	// 1) static ���ǽ� �Լ��� �����ϴ� ��� 
	PlayerArray.Sort(ANetGameStateBase::AscendingByString);

	// 2) ���ٽ����� ����ϴ� ��� 
	PlayerArray.Sort([](const APlayerState& ps1, const APlayerState& ps2)
	{
		return ps1.GetPlayerName() < ps2.GetPlayerName();
	});

	//������ ����
	PlayerArray.Sort([](const APlayerState& ps1, const APlayerState& ps2)
	{
		return ps1.GetScore() >= ps2.GetScore();
	});
	
	return PlayerArray;
}

bool ANetGameStateBase::AscendingByString(const APlayerState& ps1, const APlayerState& ps2)
{
	return ps1.GetPlayerName() < ps2.GetPlayerName();

	//ps1->GetPlayerName() < ps2->GetPlayerName(); �ƽ�Ű�ڵ带 �� :  �������� ���� => ������ ���� ��ũ�� true�� ��ȯ
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
// winTeamName �� �÷��̾����Ʈ ���ؼ� ���� �÷��̾�鿡�Դ� �������� ��� �ϱ�
// winTeamName = TeamB �̸� ps->bTeamB =true �� �÷��̾�鿡�� �¸� ����
	
	for(APlayerState* It : PlayerArray)
	{
		//PlayerArray �߿��� ���ÿ� PlayerState�� ã�ƾ��� 
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
					//������ NetPS ���� �¸� ����
					playerPawn->TwoCamRenderUI->ShowTextTeamB(FText::FromString("Win"));
					//pawn  �� �����ؼ� ui�� �������� �ϱ� 
				}
				else
				{
					playerPawn->TwoCamRenderUI->ShowTextTeamA(FText::FromString("Lose"));
					//������ NetPS ���� lose ����  
				}
			}
			else //�¸����� A���ΰ��
				{
				if(!(NetPS->bTeamB))
				{
					playerPawn->TwoCamRenderUI->ShowTextTeamA(FText::FromString("Win"));
					//������ NetPS ���� �¸� ���� 
				}
				else
				{
					playerPawn->TwoCamRenderUI->ShowTextTeamB(FText::FromString("Lose"));
					//������ NetPS ���� lose ����  
				}
				}
		}
		
		
	}
}

