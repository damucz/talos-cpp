#include "modules/TalosMathModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosInterpolationModule.h"
#include <assert.h>

namespace Talos
{
	void MathModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		_defaultA = moduleData->GetFloat("a");
		_defaultB = moduleData->GetFloat("b");
		_currentExpression = GetExpressionForName(moduleData->GetString("mathExpression"));
	}

	void MathModule::SetExpression(Expression expression)
	{
		_currentExpression = expression;
	}

	MathModule::Expression MathModule::GetExpression() const
	{
		return _currentExpression;
	}

	NumericalValue* MathModule::GetOutputValue() const
	{
		return _output;
	}

	void MathModule::SetA(float a)
	{
		_defaultA = a;
	}

	void MathModule::SetB(float b)
	{
		_defaultB = b;
	}

	float MathModule::GetDefaultA() const
	{
		return _defaultA;
	}

	float MathModule::GetDefaultB() const
	{
		return _defaultB;
	}

	void MathModule::DefineSlots()
	{
		_a = CreateInputSlot(A);
		_b = CreateInputSlot(B);

		_output = CreateOutputSlot(OUTPUT);
	}

	void MathModule::ProcessValues()
	{
		if (_a->IsEmpty()) _a->Set(_defaultA);
		if (_b->IsEmpty()) _b->Set(_defaultB);

		Express(_currentExpression, _a, _b, _output);
	}

	void MathModule::Express(Expression expression, NumericalValue* a, NumericalValue* b, NumericalValue* output)
	{
		switch(expression)
		{
		case SUBSTRACT: a->Sub(b, output); break;
		case COS: a->Cos(output); break;
		case SUM: a->Sum(b, output); break;
		case SIN: a->Sin(output); break;
		case MULTIPLY: a->Mul(b, output); break;
		case ABS: a->Abs(output); break;
		case POW: a->Pow(b, output); break;
		case DIVIDE: a->Div(b, output); break;
		}
	}

	MathModule::Expression MathModule::GetExpressionForName(const char* expr)
	{
		struct { const char* name; Expression expr; } exprlist[] = {
			{ "substract", SUBSTRACT },
			{ "cos", COS },
			{ "sum", SUM },
			{ "sin", SIN },
			{ "multiply", MULTIPLY },
			{ "abs", ABS },
			{ "pow", POW },
			{ "divide", DIVIDE },
		};

		for (int i = 0; i < (int)(sizeof(exprlist) / sizeof(exprlist[0])); ++i)
		{
			if (strcmp(exprlist[i].name, expr) == 0)
				return exprlist[i].expr;
		}

		assert(false);
		return SUM;
	}
	
}
