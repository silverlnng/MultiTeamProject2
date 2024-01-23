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
			TwoCamRenderUI->AddToViewport(); //UI �� �ڽ��� UI �� ���� 
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
	
	/*const double curX = currentValue.X;
	const double curY = currentValue.Y;

	//double cannon_Value_X=FMath::Clamp(curX,-20.f,20.f);
	//double cannon_Value_Y=FMath::Clamp(curY,5.f,0.f);
	
	cannonRot =FRotator(curY*cannonOffest_y,curX*cannonOffest_x,0);
	
	widgetDir = FVector(0.f,curX*crossHairOffest,curY*crossHairOffest);
	
	if (Controller)
	{
		//UE_LOG(LogTemp, Log, TEXT("input Vector :: %s"), *currentValue.ToString());
		
		// (1) Ű������ �Է°��� ���� , ũ�ν���� �Ѵ� �ޱ�
		// (2)������ �Է°��� ȸ������ �ޱ� => ȸ���� ���������� �ް��ϱ�
		// (3)ũ�ν� ���� Z,Y �ุ �̵� �ޱ�
		// (4) ������ ȸ���� �� Value ���� ���� �׸��� ������ ���ֱ�
		// (5) ũ�ν������ �̵��� Value ���� ���� �����ϱ� �׸��� ������ �ֱ�


		
		cannonPivot->AddRelativeRotation(cannonRot);
		crossHairWidget->AddRelativeLocation(widgetDir);
		
	}*/
	//UE_LOG(LogTemp, Log, TEXT(" %s(%d) input Vector :: %s"),*FString(__FUNCTION__), __LINE__,*currentValue.ToString());
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
	if (Controller && Value.Get<bool>() == true) //������ ������ . �Է°� 
	{
		ServerFire();
		
		// �Ѿ� �����ϱ�

		// 1) �Ѿ� BP �����
		// 2) �Ѿ��� projectile���� �Ҳ��� , physics���� �Ұ��� �����ϱ� 
		// 3) ĳ���� �Ա� ->  crossHairWidget �� ��ġ�� ���ؼ� �߻��ϱ�
		//spawnacto
		// ���� (��� ����-), �Ҹ��� ���� , �Ҹ� �� ��ġ 
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
		//ũ�ν����� fireposition �� ���� ���ϱ�
	
		firePosition.SetRotation(fireRot.Quaternion());
		// ��ġ�� ���� �� ���� ���� �ϱ� 
		ABullect* spawnbullect = GetWorld()->SpawnActor<ABullect>(bullectFactory, firePosition);
	
		spawnbullect->SetOwner(this);
	}
	
}



void APlayerPawn::ServerFire_Implementation()
{
	//��ġ�� ����ȭ ��Ű�� 
	spawnBullect();
}

void APlayerPawn::MulticastFire_Implementation()
{
}


void APlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); //�θ𿡼� �������̵� �� �͵��� ����

	// DOREPLIFETIME(ANetWorkProject1Character,elapsedTime);
	// �������� ����ȭ tick �� ��ȭ���� ��� �ִ� ��
	// c :Ŭ���� v : ���� =>UPROPERTY(Replicated) �� ���־�� �������� �ν� 
	DOREPLIFETIME(APlayerPawn, cannonRot);
	DOREPLIFETIME(APlayerPawn, widgetDir);
	//DOREPLIFETIME(APlayerPawn, currentValue);
	
}