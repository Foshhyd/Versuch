// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPawn.h"

// Sets default values
AMainPawn::AMainPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &AMainPawn::MoveForward);
		PlayerInputComponent->BindAxis("MoveLeft", this, &AMainPawn::MoveLeft);
	}

}

void AMainPawn::MoveForward(float Value)
{
	UE_LOG(LogTemp, Display, TEXT("Forward"));
	//Forward-Backward of the Pawn
	// const FRotator Rotation = Controller->GetControlRotation();
	// const FRotator YawRotation(0, Rotation.Yaw, 0);

	// const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(GetActorForwardVector() * Value);

}

void AMainPawn::MoveLeft(float Value)
{
	//Left-Right of the Pawn
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);

}

