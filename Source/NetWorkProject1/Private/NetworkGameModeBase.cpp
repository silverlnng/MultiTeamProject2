// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameModeBase.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "NetPlayerState.h"
#include "NetGameStateBase.h"
#include "NetPlayerStart.h"

// ������ ���� =>���Ӱ� ���Ӹ�� ���̽� ���� => ��� ���Ӱ� ������ !! (���ǿ� �ѹ����� ������ �ƴ� ) 
// �Լ�ȣ�� => �÷��̾� �����Ҷ� ���� 
// AController* player  �� ���ڷ� �޾Ƽ� �÷��̾ ������ �ؼ� �÷��̾��� �������� ������ �����ϴ� !
AActor* ANetworkGameModeBase::ChoosePlayerStart_Implementation(AController* player)
{
	TArray<APlayerStart*> playerStartActors;
	
	playerStartActors.SetNum(2); 
	
	//for(TActorIterator<APlayerStart> playerStart(GetWorld()); playerStart; ++playerStart)
	//{
	//	APlayerStart* ps = *playerStart;
	//	
	//	if(ps->ActorHasTag(FName("Red")))
	//	{
	//		playerStartActors[0] = ps;
	//	}
	//	else
	//	{
	//		playerStartActors[1] = ps;	
	//	}
	//	//playerStartActors.Add(*playerStart);
	//}
	int32 num = index++ % playerStartActors.Num();
	return playerStartActors[num];

	PlayerStateClass = ANetPlayerState::StaticClass();
}

void ANetworkGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	

	if (NewPlayer != nullptr)
	{
		uint8 NumTeamA = 0;
		uint8 NumTeamB = 0;
		ANetPlayerState* PS = Cast<ANetPlayerState>(NewPlayer->PlayerState);
		
		//ANetGameStateBase* Gamestate = GetGameState<ANetGameStateBase>();

		if (PS && Gamestate)
		{
			for(ANetPlayerState* It : Gamestate->PlayerArray)
			{
				ANetPlayerState* OtherPS = Cast<ANetPlayerState>(It);
				if (OtherPS)
				{
					if (OtherPS->bTeamB)
					{
						NumTeamB++;
					}
					else
					{
						NumTeamA++;
					}
				}
			}

			if (NumTeamA > NumTeamB)
			{
				PS->bTeamB = true;
			}

		}
	}
}


AActor* ANetworkGameModeBase::ChoosePlayerStart_Implementation(AController* Player) override;
{
	if (Player)
	{
		ANetPlayerState* PS = Cast<ANetPlayerState>(Player->PlayerState);
		if (PS)
		{
			TArray<ANetPlayerStart*> Starts;
			for (TActorIterator<ANetPlayerStart> StartItr(GetWorld()); StartItr; ++StartItr)
			{
				if (StartItr->bTeamB == PS->bTeamB)
				{
					Starts.Add(*StartItr);
				}
			}

			return Starts[FMath::RandRange(0, Starts.Num() - 1)];
		}
	}

	return NULL;
}
