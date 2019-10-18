// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "SophisticatedAIEnemy.h"
#include "GlobalDefinitions.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

// Sets default values
ASophisticatedAIEnemy::ASophisticatedAIEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASophisticatedAIEnemy::BeginPlay()
{
	Super::BeginPlay();

	this->startingPoint = this->GetActorLocation();
	taskIsUpdated = false;
	waitTime = 1.f;

	this->fovRadius = FMath::FRandRange(50.f, 120.f);
	this->compassion = FMath::FRandRange(30.f, 100.f);
	this->maxAreaRadius = FMath::FRandRange(200.f, 600.f);
	this->laziness = FMath::FRandRange(25.f, 90.f);
	this->hearingDistance = FMath::FRandRange(50.f, 300.f);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASophisticatedAIEnemy::StaticClass(), found);

	for (AActor* var : found)
	{
		ASophisticatedAIEnemy* c = Cast<ASophisticatedAIEnemy>(var);

		if (c != nullptr && c != this)
			others.Add((ASophisticatedAIEnemy*)var);
	}

	//found.Empty();
	bool test = IsCompassionate();
	FString text = FString::SanitizeFloat(this->compassion) + LINE_TERMINATOR + FString::SanitizeFloat(test); // + LINE_TERMINATOR + FString::SanitizeFloat(laziness) + LINE_TERMINATOR + FString::SanitizeFloat(maxWaitTime * laziness * 0.01f) + LINE_TERMINATOR + FString::SanitizeFloat(seenRate);

	if (debug)
		GEngine->AddOnScreenDebugMessage(-1, 315.0f, FColor::Red, text);
}

// Called every frame
void ASophisticatedAIEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (isSeen)
	{
		seenRate += (seenRaiseRate * DeltaTime);
		
		//checkSeenRate();
		if (debug)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::SanitizeFloat(seenRate));
	}
	else
	{
		seenRate -= (seenDecayRate * DeltaTime);
		
		if (seenRate < 0.f)
			seenRate = 0.f;

		if (debug)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::SanitizeFloat(seenRate));
	}

	waitTime -= waitTimeDecayRate * DeltaTime;

	if (waitTime <= 0.f)
	{
		//rng for next task
		float rng = FMath::FRandRange(0.f, 100.f);

		float threshold = laziness * getTaskRate();

		//switch on next task, use multithread
		if (rng <= threshold)
		{
			planGuardTask();

			addTaskToList(availableTasks::GUARD);
		}
		else
		{
			planPatrolTask();

			addTaskToList(availableTasks::PATROL);
		}
		
		taskIsUpdated = true;
		waitTime = maxWaitTime * laziness * 0.01f;
		if (waitTime <= 5.f)
			waitTime = 5.f;
	}

}

// Called to bind functionality to input
void ASophisticatedAIEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASophisticatedAIEnemy::PostInitProperties()
{
	Super::PostInitProperties();

	//fovRadius = FMath::FRandRange(0.f, 120.f);
	//compassion = FMath::FRandRange(0.f, 100.f);
	//maxAreaRadius = FMath::FRandRange(300.f, 1500.f);
	//laziness = FMath::FRandRange(0.f, 100.f);
	//hearingDistance = FMath::FRandRange(50.f, 300.f);


	/*TArray<AActor*> found;
	UGameplayStatics::GetAllActorsOfClass(this, ASophisticatedAIEnemy::StaticClass(), found);

	for (AActor* var : found)
	{
		others.Add((ASophisticatedAIEnemy*) var);
	}
	
	found.Empty();*/
}

float ASophisticatedAIEnemy::getFovRadius()
{
	return this->fovRadius;
}

void ASophisticatedAIEnemy::setFovRadius(float val)
{
	this->fovRadius = val;
}

float ASophisticatedAIEnemy::getCompassion()
{
	return this->compassion;
}

void ASophisticatedAIEnemy::setCompassion(float val)
{
	this->compassion = val;
}

float ASophisticatedAIEnemy::getMaxAreaRadius()
{
	return this->maxAreaRadius;
}

void ASophisticatedAIEnemy::setMaxAreaRadius(float val)
{
	this->maxAreaRadius = val;
}

float ASophisticatedAIEnemy::getLaziness()
{
	return this->laziness;
}

void ASophisticatedAIEnemy::setLaziness(float val)
{
	this->laziness = val;
}

float ASophisticatedAIEnemy::getHearingDistance()
{
	return this->hearingDistance;
}

void ASophisticatedAIEnemy::setHearingDistance(float val)
{
	this->hearingDistance = val;
}

//uses Waypoint1 for location & Waypoint2 for rotation to look 
void ASophisticatedAIEnemy::planGuardTask()
{
	FVector avoid;

	getAvoidanceVector(&avoid);
	avoid.Normalize();

	//set up Waypoint 1 & direction for Waypoint2 from avoiding others and old waypoints
	float range = FMath::FRandRange(100.f, this->maxAreaRadius);
	FVector newWP = this->startingPoint - (range * avoid);
	FVector direction = this->Waypoint1 - this->GetActorLocation();

	//check if Waypoint is valid
	switch (isValidVector(newWP, this->GetActorLocation()))
	{
	case vectorValidReturns::OUTOFRANGE:
		newWP = this->startingPoint - (this->maxAreaRadius * avoid);
		break;
	case vectorValidReturns::TOOLARGE:
		newWP.X = maxXDirection - 10.f;
		direction = -direction;
		break;
	case vectorValidReturns::TOOSMALL:
		newWP.X = minXDirection + 10.f;
		direction = -direction;
		break;
	case vectorValidReturns::BLOCKED:break;
	case vectorValidReturns::VALID:break;
	}

	addWPtoList(newWP);

	this->Waypoint1 = newWP;
	//FVector direction = this->Waypoint1 - this->GetActorLocation();
	direction.Normalize();
	
	newWP = this->Waypoint1 + 200.f*direction;

	//check if Waypoint is valid
	float rng = FMath::FRandRange(0.f, 100.f);

	switch (isValidVector(newWP, this->Waypoint1))
	{
	case vectorValidReturns::OUTOFRANGE:break;
	case vectorValidReturns::TOOLARGE:break;
	case vectorValidReturns::TOOSMALL:break;
	case vectorValidReturns::BLOCKED:
		if (rng < 50.f)
			direction.RotateAngleAxis(90.f, FVector::UpVector);
		else
			direction.RotateAngleAxis(270.f, FVector::UpVector);
		break;
	case vectorValidReturns::VALID:break;
	}

	this->Waypoint2 = this->Waypoint1 + 10.f*direction;
	this->Waypoint3 = FVector::ZeroVector;

	//addWPtoList(this->Waypoint2);

	if (debug)
		GEngine->AddOnScreenDebugMessage(-1, 315.0f, FColor::Red, TEXT("Guarding"));

}

//sets Waypoint1 as current location and Waypoint2 & Waypoint3 angled from avoiding others  and old waypoints
void ASophisticatedAIEnemy::planPatrolTask()
{
	FVector avoid;

	getAvoidanceVector(&avoid);
	avoid.Normalize();

	//Set Waypoint1
	this->Waypoint1 = this->GetActorLocation();
	addWPtoList(this->Waypoint1);

	//Compute Waypoint2
	FVector rotated = avoid;
	float angle = FMath::FRandRange(25.f, 90.f);

	rotated.RotateAngleAxis(angle, FVector::UpVector);
	rotated.Normalize();

	float trackSize = FMath::FRandRange(500.f, this->maxAreaRadius);
	FVector newWP = this->Waypoint1 + (trackSize * rotated);

	//check if Waypoint is valid
	switch (isValidVector(newWP, this->Waypoint1))
	{
	case vectorValidReturns::OUTOFRANGE:
		this->Waypoint1 + (this->maxAreaRadius * rotated);
		break;
	case vectorValidReturns::TOOLARGE:
		newWP.X = maxXDirection - 10.f;
		break;
	case vectorValidReturns::TOOSMALL:
		newWP.X = minXDirection + 10.f;
		break;
	case vectorValidReturns::BLOCKED:
		newWP = this->Waypoint1 + ((blockedDistance - 25.f) * rotated);
		break;
	case vectorValidReturns::VALID:break;
	}

	this->Waypoint2 = newWP;
	addWPtoList(this->Waypoint2);


	//Compute Waypoint3
	trackSize = FMath::FRandRange(200.f, this->maxAreaRadius);
	rotated = avoid;
	rotated.RotateAngleAxis(angle, FVector::UpVector);
	rotated.Normalize();

	newWP = this->Waypoint1 + (trackSize * rotated);

	//check if Waypoint is valid
	switch (isValidVector(newWP, this->Waypoint1))
	{
	case vectorValidReturns::OUTOFRANGE:
		this->Waypoint1 + (this->maxAreaRadius * rotated);
		break;
	case vectorValidReturns::TOOLARGE:
		newWP.X = maxXDirection - 10.f;
		break;
	case vectorValidReturns::TOOSMALL:
		newWP.X = minXDirection + 10.f;
		break;
	case vectorValidReturns::BLOCKED:
		newWP = this->Waypoint1 + ((blockedDistance - 25.f) * rotated);
		break;
	case vectorValidReturns::VALID:break;
	}

	this->Waypoint3 = newWP;
	addWPtoList(this->Waypoint3);

	if (debug)
		GEngine->AddOnScreenDebugMessage(-1, 315.0f, FColor::Red, TEXT("Patroling"));

}

//Computes Vector the Character will avoid - simple usage to ensure AI doesn't crowd on one spot
void ASophisticatedAIEnemy::getAvoidanceVector(FVector *vec)
{
	FVector avoidOthers = FVector::ZeroVector;
	FVector avoidWPs = FVector::ZeroVector;

	for (ASophisticatedAIEnemy* a : others)
	{
		FVector avoidVec = a->GetActorLocation() - this->GetActorLocation();
		avoidVec.Normalize();

		avoidOthers += (avoidVec);
	}

	for (FVector v : oldWPs)
	{
		v.Normalize();
		avoidWPs += v;
	}

	*vec = avoidOthers + avoidWPs;
}

//Checks if the Waypoint is reachable and useable for its purpose
vectorValidReturns ASophisticatedAIEnemy::isValidVector(FVector end, FVector start)
{
	if (end.X <= minXDirection)
	{
		if (debug)
			GEngine->AddOnScreenDebugMessage(-1, 315.0f, FColor::Green, TEXT("small"));
		return vectorValidReturns::TOOSMALL;
	}

	if (end.X >= maxXDirection)
	{
		if (debug)
			GEngine->AddOnScreenDebugMessage(-1, 315.0f, FColor::Green, TEXT("large"));
		return vectorValidReturns::TOOLARGE;
	}

	FVector way = end - start;

	if (way.Size() >= maxAreaRadius)
	{
		if (debug)
			GEngine->AddOnScreenDebugMessage(-1, 315.0f, FColor::Green, TEXT("range"));
		return vectorValidReturns::OUTOFRANGE;
	}

	FCollisionQueryParams params(FName(TEXT("isWPblocked?")), true, this);
	params.AddIgnoredActors(found);
	params.bTraceComplex = true;
	params.bTraceAsyncScene = true;
	params.bReturnPhysicalMaterial = false;

	FHitResult result(ForceInit);

	if (GetWorld()->LineTraceSingleByChannel(result, start, end, ECC_Pawn, params))
	{
		blockedDistance = result.Distance;

		if (debug)
			GEngine->AddOnScreenDebugMessage(-1, 315.0f, FColor::Green, TEXT("blocked"));
		return vectorValidReturns::BLOCKED;
	}
	
	if (debug)
		GEngine->AddOnScreenDebugMessage(-1, 315.0f, FColor::Green, TEXT("valid"));
	return vectorValidReturns::VALID;
}

//dummy function - implemented in Blueprints
void ASophisticatedAIEnemy::checkSeenRate()
{
	if (seenRate >= 100.f)
	{
		//do stuff here

		seenRate = 0.f;
	}
}

//Computes how often enemy has guarded to influence next task
float ASophisticatedAIEnemy::getTaskRate()
{
	int numOfTasks = oldTasks.Num();
	
	if (numOfTasks < 3)
		return 1.f;

	int amountPatrol=0;

	for (int i = 0; i < numOfTasks; i++)
	{
		if (oldTasks[i] == availableTasks::PATROL)
			amountPatrol++;
	}

	return (amountPatrol / numOfTasks);
}

//Adds to the list of latest tasks, max length is 5
void ASophisticatedAIEnemy::addTaskToList(availableTasks task)
{
	oldTasks.Insert(task, 0);

	if (oldTasks.Num() > 5)
		oldTasks.Pop();
}

//Adds to the list of latest Waypoints, max length is 5
void ASophisticatedAIEnemy::addWPtoList(FVector vector)
{
	oldWPs.Insert(vector, 0);

	if (oldWPs.Num() > 5)
		oldWPs.Pop();
}

void ASophisticatedAIEnemy::printAttributes()
{
	UE_LOG(LogTemp, Warning, TEXT("In ENEMY CPP"));

	FString text = "Comapssion: " + FString::SanitizeFloat(this->compassion) + LINE_TERMINATOR 
		         + "Laziness: " + FString::SanitizeFloat(this->laziness) + LINE_TERMINATOR 
		         + "Hearing Distance: " + FString::SanitizeFloat(this->hearingDistance) + LINE_TERMINATOR 
		         + "FOV Radius: " + FString::SanitizeFloat(this->fovRadius) + LINE_TERMINATOR
				 + "max Area Radius: " + FString::SanitizeFloat(this->maxAreaRadius) + LINE_TERMINATOR;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
	UE_LOG(LogTemp, Warning, TEXT("%s  "), *text);
}

//Used to artificially raise SeenRate - was supposed to be used for Touch events
void ASophisticatedAIEnemy::AddToSeenRate(float value)
{
	seenRate += value;

	checkSeenRate();
}

bool ASophisticatedAIEnemy::IsCompassionate()
{
	return (this->compassion >= compassionThreshold);
}

//Enables / Disables SeenRate raising through ticks
void ASophisticatedAIEnemy::SetIsSeen(bool seen)
{
	isSeen = seen;
}

//returns given Waypoint for Behaviour Tree
FVector ASophisticatedAIEnemy::GetWaypoint(int num)
{
	FVector result;

	switch (num)
	{
	case 1: result = Waypoint1; break;
	case 2: result = Waypoint2; break;
	case 3: result = Waypoint3; break;
	default: result = FVector::ZeroVector;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 315.0f, FColor::Red, FString::SanitizeFloat(result.X));
	
	return result;
}