// Project Oracle - Cozy Open World AAA

#include "Input/OracleInputDefaults.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "Input/OracleInputConfig.h"

namespace
{
	UInputAction* MakeAction(UObject* Outer, FName Name, EInputActionValueType ValueType)
	{
		UInputAction* Action = NewObject<UInputAction>(Outer, Name);
		Action->ValueType = ValueType;
		return Action;
	}
}

void FOracleInputDefaults::Build(UObject* Outer, TObjectPtr<UOracleInputConfig>& OutConfig,
                                 TObjectPtr<UInputMappingContext>& OutContext)
{
	UOracleInputConfig* Config = NewObject<UOracleInputConfig>(Outer, TEXT("IC_Default_Runtime"));
	UInputMappingContext* Context = NewObject<UInputMappingContext>(Outer, TEXT("IMC_Default_Runtime"));

	Config->Move       = MakeAction(Outer, TEXT("IA_Move"),       EInputActionValueType::Axis2D);
	Config->Look       = MakeAction(Outer, TEXT("IA_Look"),       EInputActionValueType::Axis2D);
	Config->ZoomCamera = MakeAction(Outer, TEXT("IA_ZoomCamera"), EInputActionValueType::Axis1D);
	Config->Jump       = MakeAction(Outer, TEXT("IA_Jump"),       EInputActionValueType::Boolean);
	Config->Sprint     = MakeAction(Outer, TEXT("IA_Sprint"),     EInputActionValueType::Boolean);
	Config->WalkToggle = MakeAction(Outer, TEXT("IA_WalkToggle"), EInputActionValueType::Boolean);

	// --- Move (WASD -> Axis2D, X = direita, Y = frente) ---
	// W: frente. O eixo do teclado chega em X; Swizzle YXZ o envia para Y.
	{
		FEnhancedActionKeyMapping& M = Context->MapKey(Config->Move, EKeys::W);
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Context);
		Swizzle->Order = EInputAxisSwizzle::YXZ;
		M.Modifiers.Add(Swizzle);
	}
	// S: trás (negado + swizzle).
	{
		FEnhancedActionKeyMapping& M = Context->MapKey(Config->Move, EKeys::S);
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Context);
		Swizzle->Order = EInputAxisSwizzle::YXZ;
		M.Modifiers.Add(Swizzle);
		M.Modifiers.Add(NewObject<UInputModifierNegate>(Context));
	}
	// A: esquerda (negado).
	{
		FEnhancedActionKeyMapping& M = Context->MapKey(Config->Move, EKeys::A);
		M.Modifiers.Add(NewObject<UInputModifierNegate>(Context));
	}
	// D: direita (sem modificador).
	Context->MapKey(Config->Move, EKeys::D);

	// --- Look (mouse; Y invertido para pitch natural) ---
	{
		FEnhancedActionKeyMapping& M = Context->MapKey(Config->Look, EKeys::Mouse2D);
		UInputModifierNegate* NegateY = NewObject<UInputModifierNegate>(Context);
		NegateY->bX = false;
		NegateY->bY = true;
		NegateY->bZ = false;
		M.Modifiers.Add(NegateY);
	}

	// --- Zoom / Pulo / Sprint / Andar ---
	Context->MapKey(Config->ZoomCamera, EKeys::MouseWheelAxis);
	Context->MapKey(Config->Jump, EKeys::SpaceBar);
	Context->MapKey(Config->Sprint, EKeys::LeftShift);
	Context->MapKey(Config->WalkToggle, EKeys::LeftControl);

	// --- Interação / Decoração / Sistema ---
	Config->Interact    = MakeAction(Outer, TEXT("IA_Interact"),    EInputActionValueType::Boolean);
	Config->BuildToggle = MakeAction(Outer, TEXT("IA_BuildToggle"), EInputActionValueType::Boolean);
	Config->Place       = MakeAction(Outer, TEXT("IA_Place"),       EInputActionValueType::Boolean);
	Config->RotateProp  = MakeAction(Outer, TEXT("IA_RotateProp"),  EInputActionValueType::Boolean);
	Config->QuickSave   = MakeAction(Outer, TEXT("IA_QuickSave"),   EInputActionValueType::Boolean);
	Config->QuickLoad   = MakeAction(Outer, TEXT("IA_QuickLoad"),   EInputActionValueType::Boolean);

	Context->MapKey(Config->Interact, EKeys::E);
	Context->MapKey(Config->BuildToggle, EKeys::B);
	Context->MapKey(Config->Place, EKeys::LeftMouseButton);
	Context->MapKey(Config->RotateProp, EKeys::R);
	Context->MapKey(Config->QuickSave, EKeys::F5);
	Context->MapKey(Config->QuickLoad, EKeys::F9);

	OutConfig = Config;
	OutContext = Context;
}
