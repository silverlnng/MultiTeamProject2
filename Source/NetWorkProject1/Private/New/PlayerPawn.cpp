// Fill out your copyright notice in the Description page of Project Settings.


#include "New/PlayerPawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include  "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "New/Bullect.h"
#include "net/UnrealNetwork.h"
#include "New/CamUserWidget.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	myBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MyBoxComponent"));
	SetRootComponent(myBoxComp);

	cannonPivot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	cannonPivot->SetupAttachment(myBoxComp);
	
	cannonComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshCompo"));
	cannonComp->SetupAttachment(cannonPivot);
	
	crossHairWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetCompo"));
	crossHairWidget->SetupAttachment(myBoxComp);
	
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowCompo"));
	ArrowComp->SetupAttachment(cannonPivot);

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	pc = GetController<APlayerController>();
	
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);

		}
	}
	
	if (TwoCamRenderwidget != nullptr && GetController() != nullptr && GetController()->IsLocalController())
	{
		TwoCamRenderUI = CreateWidget<UCamUserWidget>(GetWorld(), TwoCamRenderwidget);
		if (TwoCamRenderUI != nullptr)
		{
			TwoCamRenderUI->AddToViewport(); //UI 는 자신의 UI 만 띄우기 
		}
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UE_LOG(LogTemp,Warning,TEXT("%d") , cannonPivot->GetRelativeRotation().Yaw);

	/*auto Rotation = cannonPivot->GetComponentRotation();
	Rotation.Pitch = FMath::Clamp(Rotation.Pitch, -20, 20);
	cannonPivot->SetRelativeRotation(Rotation);*/
	
	//FMath::Clamp(cannonPivot->GetRelativeRotation().Yaw,-20,20);
	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveIA, ETriggerEvent::Triggered, this, &APlayerPawn::Move);

		EnhancedInputComponent->BindAction(FireIA, ETriggerEvent::Started, this, &APlayerPawn::InputFire);
	}
	
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	currentValue = Value.Get<FVector>();
	ServerMove(currentValue);
}

void APlayerPawn::ServerMove_Implementation(FVector Value)
{
	
	//UE_LOG(LogTemp, Log, TEXT(" %s(%d) input Vector :: %s"),*FString(__FUNCTION__), __LINE__,*Value.ToString());
	const double curX = Value.X;
	const double curY = Value.Y;
	
	cannonRot =FRotator(curY*cannonOffest_y,curX*cannonOffest_x,0);
	
	widgetDir = FVector(0.f,curX*crossHairOffest,curY*crossHairOffest);
	
	cannonPivot->AddRelativeRotation(cannonRot);
	crossHairWidget->AddRelativeLocation(widgetDir);
	
	MulticastMove(currentValue);
	
}

void APlayerPawn::MulticastMove_Implementation(FVector Value)
{
	const double curX = currentValue.X;
	const double curY = currentValue.Y;
	//UE_LOG(LogTemp, Log, TEXT(" %s(%d) input Vector :: %s"),*FString(__FUNCTION__), __LINE__,*currentValue.ToString());
	
	cannonRot =FRotator(curY*cannonOffest_y,curX*cannonOffest_x,0);
	
	widgetDir = FVector(0.f,curX*crossHairOffest,curY*crossHairOffest);
	cannonPivot->AddRelativeRotation(cannonRot);
	crossHairWidget->AddRelativeLocation(widgetDir);
}



void APlayerPawn::InputFire(const FInputActionValue& Value)
{
	if (Controller && Value.Get<bool>() == true) //콘텐츠 외적인 . 입력계 
	{
		ServerFire();
		
		// 총알 스폰하기

		// 1) 총알 BP 만들기
		// 2) 총알을 projectile으로 할껀지 , physics으로 할건지 선택하기 
		// 3) 캐논의 입구 ->  crossHairWidget 의 위치를 향해서 발사하기
		//spawnacto
		// 월드 (어느 월드-), 소리낼 사운드 , 소리 날 위치 
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, myArrowComp->GetComponentLocation());
	}
}



void APlayerPawn::spawnBullect()
{
	if(HasAuthority())
	{
		FTransform firePosition;
		//FVector arrowVec = ArrowComp->loca
		firePosition.SetLocation(ArrowComp->GetComponentLocation());
	
		FVector fireDir = crossHairWidget->GetComponentLocation() - ArrowComp->GetComponentLocation();

		//UE_LOG(LogTemp,Warning,TEXT("%s") , *ArrowComp->GetComponentLocation().ToString());
	
		//Rotator fireRot =  UKismetMathLibrary::MakeRotFromX(fireDir);
		FRotator fireRot = fireDir.Rotation();
		//크로스헤어와 fireposition 의 각도 구하기
	
		firePosition.SetRotation(fireRot.Quaternion());
		// 위치와 각도 에 맞춰 스폰 하기 
		ABullect* spawnbullect = GetWorld()->SpawnActor<ABullect>(bullectFactory, firePosition);
	
		spawnbullect->SetOwner(this);
	}
	
}



void APlayerPawn::ServerFire_Implementation()
{
	//위치를 동기화 시키기 
	spawnBullect();
}

void APlayerPawn::MulticastFire_Implementation()
{
}


void APlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); //부모에서 오버라이드 된 것들을 실행

	// DOREPLIFETIME(ANetWorkProject1Character,elapsedTime);
	// 서버만의 동기화 tick 에 변화값을 계속 주는 것
	// c :클래스 v : 변수 =>UPROPERTY(Replicated) 가 되있어야 서버에서 인식 
	DOREPLIFETIME(APlayerPawn, cannonRot);
	DOREPLIFETIME(APlayerPawn, widgetDir);
	//DOREPLIFETIME(APlayerPawn, currentValue);
	
}