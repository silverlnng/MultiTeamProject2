// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameModeBase.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "NetPlayerState.h"
#include "NetGameStateBase.h"
#include "New/NetPlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkGameInstance.h"

ANetworkGameModeBase::ANetworkGameModeBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//default PlayerController class �ʱ�ȭ
	PlayerStateClass = ANetPlayerState::StaticClass();
}

//// ������ ���� =>���Ӱ� ���Ӹ�� ���̽� ���� => ��� ���Ӱ� ������ !! (���ǿ� �ѹ����� ������ �ƴ� ) 
//// �Լ�ȣ�� => �÷��̾� �����Ҷ� ���� 
//// AController* player  �� ���ڷ� �޾Ƽ� �÷��̾ ������ �ؼ� �÷��̾��� �������� ������ �����ϴ� !
//AActor* ANetworkGameModeBase::ChoosePlayerStart_Implementation(AController* player)
//{
//	TArray<APlayerStart*> playerStartActors;
//	
//	playerStartActors.SetNum(2); 
//	
//	//for(TActorIterator<APlayerStart> playerStart(GetWorld()); playerStart; ++playerStart)
//	//{
//	//	APlayerStart* ps = *playerStart;
//	//	
//	//	if(ps->ActorHasTag(FName("Red")))
//	//	{
//	//		playerStartActors[0] = ps;
//	//	}
//	//	else
//	//	{
//	//		playerStartActors[1] = ps;	
//	//	}
//	//	//playerStartActors.Add(*playerStart);
//	//}
//	int32 num = index++ % playerStartActors.Num();
//	return playerStartActors[num];
//
//	
//}



void ANetworkGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	CheckReadyToStart();

	if (NewPlayer != nullptr)
	{
		uint8 NumTeamA = 0;
		uint8 NumTeamB = 0;
		
		ANetPlayerState* PS = Cast<ANetPlayerState>(NewPlayer->PlayerState);
		
		//ANetGameStateBase* Gamestate = GetGameState<ANetGameStateBase>();

		if (PS && GameState)
		{
			for(APlayerState * It : GameState->PlayerArray)
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


void ANetworkGameModeBase::CheckReadyToStart()
{

	// Get the number of player controllers
	int32 NumPlayers = UGameplayStatics::GetNumPlayerControllers(GetWorld());

	// Check if the number of players is 4
	if (NumPlayers == 4)
	{
		// Assume all players are ready and start the game
		ShowStartWidget();
	}

}



AActor* ANetworkGameModeBase::ChoosePlayerStart(AController* Player)
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


void ANetworkGameModeBase::ShowStartWidget()
{
	
	UE_LOG(LogTemp, Warning, TEXT("ShowStartWidget"));

	/*UNetworkGameInstance* GameInstance = Cast<UNetworkGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->ShowStartWidget();
	}*/


}
