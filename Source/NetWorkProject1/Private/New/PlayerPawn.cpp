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
#include "New/Bullect.h"
#include "net/UnrealNetwork.h"

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
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);

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
	const FVector currentValue = Value.Get<FVector>();
	const double curX = currentValue.X;
	const double curY = currentValue.Y;

	//double cannon_Value_X=FMath::Clamp(curX,-20.f,20.f);
	//double cannon_Value_Y=FMath::Clamp(curY,5.f,0.f);
	
	FRotator cannonRot =FRotator(curY*cannonOffest_y,curX*cannonOffest_x,0);
	
	FVector widgetDir = FVector(0.f,curX*crossHairOffest,curY*crossHairOffest);
	
	if (Controller)
	{
		//UE_LOG(LogTemp, Log, TEXT("input Vector :: %s"), *currentValue.ToString());
		
		// (1) 키보드의 입력값을 대포 , 크로스헤어 둘다 받기
		// (2)대포는 입력값을 회전에만 받기 => 회전을 연속적으로 받게하기
		// (3)크로스 헤어는 Z,Y 축만 이동 받기
		// (4) 대포의 회전값 은 Value 보다 적게 그리고 제한을 해주기
		// (5) 크로스헤어의 이동은 Value 보다 많게 조절하기 그리고 제한을 주기
		/*if(cannonPivot->GetRelativeRotation().Pitch <=5.f && cannonPivot->GetRelativeRotation().Pitch >=0.f)
		{
			cannonPivot->AddRelativeRotation(cannonRot);
		}
		if(cannonPivot->GetRelativeRotation().Pitch >5.f && curY<0.f)
		{
		 cannonPivot->AddRelativeRotation(cannonRot);
		}*/
		cannonPivot->AddRelativeRotation(cannonRot);
		crossHairWidget->AddRelativeLocation(widgetDir);
		
	}
}

void APlayerPawn::InputFire(const FInputActionValue& Value)
{
	if (Controller && Value.Get<bool>() == true) //콘텐츠 외적인 . 입력계 
	{
		spawnBullect();
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
	FTransform firePosition;
	//FVector arrowVec = ArrowComp->loca
	
	firePosition.SetLocation(ArrowComp->GetComponentLocation());
	
	FVector fireDir = crossHairWidget->GetComponentLocation() - ArrowComp->GetComponentLocation();

	UE_LOG(LogTemp,Warning,TEXT("%s") , *ArrowComp->GetComponentLocation().ToString());
	
	//Rotator fireRot =  UKismetMathLibrary::MakeRotFromX(fireDir);
	FRotator fireRot = fireDir.Rotation();
	//크로스헤어와 fireposition 의 각도 구하기
	
	firePosition.SetRotation(fireRot.Quaternion());
	// 위치와 각도 에 맞춰 스폰 하기 
	ABullect* spawnbullect = GetWorld()->SpawnActor<ABullect>(bullectFactory, firePosition);
}


