// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera boom");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraBoom->SetupAttachment(GetMesh());
	Camera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);

	//SpringArm Default Settings
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->TargetArmLength = 200;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	bRunning = false;
	bSprinting = false;
	bCrouching = false;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(PlayerInputComponent)
	{
		PlayerInputComponent->BindAction("ToggleRun", IE_Pressed, this, &AMainCharacter::ToggleRun);
		PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::Sprint);
		PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::Sprint);
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJumping);
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::Crouching);
		PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::Crouching);
		PlayerInputComponent->BindAction("Combat", IE_Pressed, this, &AMainCharacter::Combat);
		PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::Aim);
		PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::Aim);
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::Fire);
		PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::Fire);

		PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveLeft", this, &AMainCharacter::MoveLeft);
		PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	}
}

void AMainCharacter::MoveForward(float Value)
{
	if(Value && !bAim && !bFire)
	{	
		AddMovementInput(GetActorForwardVector() * Value);
	}
}

void AMainCharacter::MoveLeft(float Value)
{
	if(Value && !bAim && !bFire)
	{
		//Left-Right of the Pawn
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::ToggleRun()
{
	if(!bRunning && !bSprinting && !bAim && !bFire)
	{
		bRunning = true;
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	else if(!bSprinting && !bAim && !bFire)
	{
		bRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = 300;
	}
}

void AMainCharacter::Sprint()
{
	if(!bSprinting && !bAim && !bFire)
	{
		bSprinting = true;
		CurWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed = 800;
	}
	else
	{
		bSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = CurWalkSpeed;
	}
}

void AMainCharacter::Crouching()
{
	if(bCrouching == false){
		bCrouching = true;
		GetCharacterMovement()->MaxWalkSpeed = 150;
		CameraBoom->TargetArmLength = 100;
		FVector CameraBoomLocation = CameraBoom->GetRelativeLocation();
		CameraBoomLocation.Z = CameraBoomLocation.Z/2;
		CameraBoom->SetRelativeLocation(CameraBoomLocation);
	}
	else
	{
		bCrouching =false;
		GetCharacterMovement()->MaxWalkSpeed = 300;
		CameraBoom->TargetArmLength = 200;
		FVector CameraBoomLocation = CameraBoom->GetRelativeLocation();
		CameraBoomLocation.Z = CameraBoomLocation.Z*2;
		CameraBoom->SetRelativeLocation(CameraBoomLocation);
	}
}

void AMainCharacter::Combat()
{
	if(bCombatMode == false)
	{
		bCombatMode = true;
	}
	else
	{
		bCombatMode = false;
	}
}

void AMainCharacter::Aim()
{
	if(bCombatMode && !bAim)
	{
		bAim = true;
	}
	else
	{
		bAim = false;
	}
	
}

void AMainCharacter::Fire()
{
	if(bCombatMode && !bFire)
	{
		bFire = true;
	}
	else
	{
		bFire = false;
	}
}

FString const AMainCharacter::GetCameraLocation()
{
	return Camera->GetName();
}

