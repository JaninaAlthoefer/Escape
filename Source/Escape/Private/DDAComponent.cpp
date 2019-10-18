// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "DDAComponent.h"
#include "SophisticatedAIEnemy.h"
#include "GlobalDefinitions.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>


//extern float compMax, lazMax, areaMax, fovMax, hearMax;
//extern float compMin, lazMin, areaMin, fovMin, hearMin;


// Sets default values for this component's properties
UDDAComponent::UDDAComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDDAComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	for (int i = 0; i < populationSize; i++)
	{
		parents.Add(FEnemyAttributes());
	}
	
}


// Called every frame
void UDDAComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UDDAComponent::SetEnemyAttributes(TArray<FEnemyAttributes> &pop)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString("Updating enemies...") + LINE_TERMINATOR);

	int32 counter = 0;

	for (ASophisticatedAIEnemy* e : enemies)
	{
		e->setCompassion(pop[counter].compassion);
		e->setLaziness(pop[counter].laziness);
		e->setFovRadius(pop[counter].fovRadius);
		e->setMaxAreaRadius(pop[counter].maxAreaRadius);
		e->setHearingDistance(pop[counter].hearingDistance);
		
		counter++;
	}
}

void UDDAComponent::Evolve()
{
	//Update min and max if necessary
	//...

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Starting evolve function");

	for (int i = 0; i < eliteSize; i++)
	{
		result.Add(parents[i]);
	}


	for (int i = eliteSize; i < populationSize; i++)
	{
		FEnemyAttributes child;

		FEnemyAttributes par1 = GetParent();
		FEnemyAttributes par2 = GetParent();

		child = Breed(par1, par2);
		Mutate(&child);


		result.Add(child);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "end of evolve loop - inside");
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "end of evolve loop - outside");
}

FEnemyAttributes UDDAComponent::Breed(FEnemyAttributes p1, FEnemyAttributes p2)
{
	FEnemyAttributes res;

	int32 rng = FMath::RandRange(0, 100);

	if (rng < uniformRate)
		res.compassion = p1.compassion;
	else
		res.compassion = p2.compassion;

	rng = FMath::RandRange(0, 100);

	if (rng < uniformRate)
		res.laziness = p1.laziness;
	else
		res.laziness = p2.laziness;

	rng = FMath::RandRange(0, 100);

	if (rng < uniformRate)
		res.fovRadius = p1.fovRadius;
	else
		res.fovRadius = p2.fovRadius;

	rng = FMath::RandRange(0, 100);

	if (rng < uniformRate)
		res.maxAreaRadius = p1.maxAreaRadius;
	else
		res.maxAreaRadius = p2.maxAreaRadius;

	rng = FMath::RandRange(0, 100);

	if (rng < uniformRate)
		res.hearingDistance = p1.hearingDistance;
	else
		res.hearingDistance = p2.hearingDistance;


	return res;
}

void UDDAComponent::Mutate(FEnemyAttributes* c)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "starting mutate");
	int32 rng = FMath::RandRange(0, 100);

	if (rng < mutationRate)
		c->compassion = FMath::FRandRange(compMin, compMax);

	rng = FMath::RandRange(0, 100);

	if (rng < mutationRate)
		c->laziness = FMath::FRandRange(lazMin, lazMax);

	rng = FMath::RandRange(0, 100);

	if (rng < mutationRate)
		c->maxAreaRadius = FMath::FRandRange(areaMin, areaMax);

	rng = FMath::RandRange(0, 100);

	if (rng < mutationRate)
		c->fovRadius = FMath::FRandRange(fovMin, fovMax);

	rng = FMath::RandRange(0, 100);

	if (rng < mutationRate)
		c->hearingDistance = FMath::FRandRange(hearMin, hearMax);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "ending mutate");
}

FEnemyAttributes* UDDAComponent::GetFittest()
{
	parents.Sort();

	FEnemyAttributes *res = &parents[0];

	return res;
}

FEnemyAttributes UDDAComponent::GetParent()
{
	//int rng = FMath::RandRange(0, parents->size());
	//return parents->at(rng);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Get Parent function");

	FEnemyAttributes res;
	
	int32 rng = FMath::RandRange(0, (parents.Num()-1));
	res = parents[rng];
	
	return res;
}

void UDDAComponent::ClearWard()
{
	/*/not allocating anything since struct?! 

	for (FEnemyAttributes c : result) 
	{
		delete c;
	}*/
	parents = result;

	result.Empty();
}

void UDDAComponent::SetPopulationSize(int32 size)
{
	/*if (size > populationSize)
	{
		for (int i = populationSize - 1; i < size; i++)
		{
			parents.Add(FEnemyAttributes());
		}
	}
	else
	{
		for (int i = size; i < populationSize; i++)
		{
			parents.Pop();
		}
	}

	populationSize = size; */
}

void UDDAComponent::SetNumberIterations(int32 num)
{
	if (num > 5)
		numIterations = num;
}

void UDDAComponent::SetMutationRate(float val)
{
	mutationRate = val;
}

void UDDAComponent::SetEliteSize(int32 val)
{
	eliteSize = val;
}

void UDDAComponent::SetEnemyArray(UPARAM(ref) TArray<ASophisticatedAIEnemy*> enem)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString("Setting new values...") + LINE_TERMINATOR);

	if (enem.Num() > populationSize)
		SetPopulationSize(enem.Num()+1);

	int counter = 0;

	for (ASophisticatedAIEnemy* e : enem)
	{
		parents[counter].compassion = e->getCompassion();
		parents[counter].laziness = e->getLaziness();
		parents[counter].fovRadius = e->getFovRadius();
		parents[counter].maxAreaRadius = e->getMaxAreaRadius();
		parents[counter].hearingDistance = e->getHearingDistance();

		counter++;
	}

	enemies = enem;
	//enemies = &enem;
}

void UDDAComponent::SetGATargetValues(TArray<float> targetVals)
{

}

void UDDAComponent::printEnemyAttributes()
{
	UE_LOG(LogTemp, Error, TEXT("In DDA CPP"));

	for (ASophisticatedAIEnemy* e : enemies)
	{
		FString text = "Comapssion: " + FString::SanitizeFloat(e->getCompassion()) + LINE_TERMINATOR
			+ "Laziness: " + FString::SanitizeFloat(e->getLaziness()) + LINE_TERMINATOR
			+ "Hearing Distance: " + FString::SanitizeFloat(e->getHearingDistance()) + LINE_TERMINATOR
			+ "FOV Radius: " + FString::SanitizeFloat(e->getFovRadius()) + LINE_TERMINATOR
			+ "max Area Radius: " + FString::SanitizeFloat(e->getMaxAreaRadius()) + LINE_TERMINATOR;

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, text);
		UE_LOG(LogTemp, Error, TEXT("%s"), *text);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::SanitizeFloat(parents[10].compassion));
}

void UDDAComponent::RunGA()
{
	//SetGATargetValues
	FString text = FString("Starting GA..."); // +LINE_TERMINATOR;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, text);

	for (int i = 0; i < numIterations; i++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Cycle " + FString::FromInt(i)); // +LINE_TERMINATOR);

		Evolve();

		parents = result;

		parents.Sort();

		FEnemyAttributes *fittest = GetFittest();
		FString text2 = "Cycle " + FString::FromInt(i) + " Fittest has Fitness: " + FString::FromInt(fittest->getFitness());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, text2); // +LINE_TERMINATOR);

		UE_LOG(LogTemp, Error, TEXT("%s"), *text2);
	}

	SetEnemyAttributes(result);

	//*ClearWard();*/
}


