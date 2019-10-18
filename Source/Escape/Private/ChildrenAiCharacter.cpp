// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "ChildrenAiCharacter.h"


// Sets default values
AChildrenAiCharacter::AChildrenAiCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChildrenAiCharacter::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
	
}

void AChildrenAiCharacter::PostInitProperties()
{
	Super::PostInitProperties();

	risktaking = FMath::FRandRange(0.f, 100.f);
}


// Called every frame
void AChildrenAiCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AChildrenAiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

