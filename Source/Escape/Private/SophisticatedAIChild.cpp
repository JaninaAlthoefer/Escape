// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "SophisticatedAIChild.h"


// Sets default values
ASophisticatedAIChild::ASophisticatedAIChild()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASophisticatedAIChild::BeginPlay()
{
	Super::BeginPlay();
	risktaking = FMath::FRandRange(0.f, 100.f);
}

void ASophisticatedAIChild::PostInitProperties()
{
	Super::PostInitProperties();

	//risktaking = FMath::FRandRange(0.f, 100.f);
}

// Called every frame
void ASophisticatedAIChild::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASophisticatedAIChild::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

