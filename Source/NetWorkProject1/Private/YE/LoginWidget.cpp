// Fill out your copyright notice in the Description page of Project Settings.


#include "YE/LoginWidget.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "NetworkGameInstance.h"
#include "Components/WidgetSwitcher.h"
#include "YE/SessionslotUserWidget.h"
#include "Components/ScrollBox.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	sl_maxPlayers->SetMinValue(2.0f);
	sl_maxPlayers->SetMaxValue(10.0f);
	sl_maxPlayers->SetStepSize(1.0f);

	text_maxPlayers->SetText(FText::AsNumber(2));

	// ��ư�� �Լ� ����(Bind)
	btn_create->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedCreateButton);
	btn_back->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedBackButton);
	btn_MoveToCreatePanel->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedMoveToCreate);
	btn_MoveToFindPanel->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedMoveToFind);
	btn_back2->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedBackButton);
	btn_findSessions->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedFindSessionsButton);

	gi = GetGameInstance<UNetworkGameInstance>();

	if (gi != nullptr)
	{
		// ���� ã�� ��������Ʈ�� �Լ��� �����Ѵ�.
		gi->onCreateSlot.AddDynamic(this, &ULoginWidget::OnSlotCreated);
		gi->onNewSearchComplete.AddDynamic(this, &ULoginWidget::OnClearScrollBox);
		gi->onFindButtonToggle.AddDynamic(this, &ULoginWidget::FindButtonOnOff);
	}

	// �����̴� ���� ����� ������ ����� �Լ� ����
	sl_maxPlayers->OnValueChanged.AddDynamic(this, &ULoginWidget::OnSliderMoved);
}

void ULoginWidget::OnClickedCreateButton()
{
	if (gi != nullptr)
	{
		gi->CreateSession(eText_roomName->GetText().ToString(), eText_hostName->GetText().ToString(), (int32)sl_maxPlayers->GetValue());
	}
}

void ULoginWidget::OnClickedBackButton()
{
	ws_widgetSwitcher->SetActiveWidgetIndex(0);
}

void ULoginWidget::OnSliderMoved(float value)
{
	text_maxPlayers->SetText(FText::AsNumber((int32)value));
}

void ULoginWidget::OnClickedMoveToCreate()
{
	ws_widgetSwitcher->SetActiveWidgetIndex(1);
}

void ULoginWidget::OnClickedMoveToFind()
{
	ws_widgetSwitcher->SetActiveWidgetIndex(2);
	if (gi != nullptr)
	{
		gi->FindSession();
	}
}

void ULoginWidget::OnClickedFindSessionsButton()
{
	//btn_findSessions->SetIsEnabled(false);
	if (gi != nullptr)
	{
		gi->FindSession();
	}
}

void ULoginWidget::OnSlotCreated(FString roomName, FString hostName, int32 currentPlayers, int32 maxPlayers, int32 ping, int32 sessionIdx)
{
	// �����κ��� ���� ������ ���� ������ ���� �߰��Ѵ�.
	if (slotWidget != nullptr)
	{
		if (USessionslotUserWidget* slot_UI = CreateWidget<USessionslotUserWidget>(GetWorld(), slotWidget))
		{
			slot_UI->SetSessionInfo(roomName, hostName, currentPlayers, maxPlayers, ping, sessionIdx);
			sb_roomList->AddChild(slot_UI);
		}
	}
}

void ULoginWidget::OnClearScrollBox()
{
	// ���� ���� ������ ��� �����
	sb_roomList->ClearChildren();
}

void ULoginWidget::FindButtonOnOff(bool on)
{
	btn_findSessions->SetIsEnabled(on);
}

void ULoginWidget::SetUserNameAndNext()
{
	// ���� �ν��Ͻ��� �Է��� �̸��� �����Ѵ�.
	//gi->SetSessionName(editText_userName->GetText().ToString());

	ws_widgetSwitcher->SetActiveWidgetIndex(1);
}
