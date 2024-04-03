// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Components/ProgressBar.h"


bool UHealthBar::Initialize()
{
	bool Success = Super::Initialize();

	if (HealthBar)
	{
		HealthBar->PercentDelegate.BindUFunction(this, "SetHealthProgress");
	}
	return true;
}

float UHealthBar::SetHealthProgress()
{
	return 1;
}
