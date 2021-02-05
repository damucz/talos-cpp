#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class MathModule : public AbstractModule
	{
	public:
		enum T
		{
			A = 0,
			B = 1,
			OUTPUT = 0,
		};
		enum Expression
		{
			SUBSTRACT,
			COS,
			SUM,
			SIN,
			MULTIPLY,
			ABS,
			POW,
			DIVIDE,
		};

		void Load(IFileProvider::ModuleData* moduleData) override;

        void SetExpression(Expression expression);
        Expression GetExpression() const;

        NumericalValue* GetOutputValue() const;

        void SetA(float a);
        void SetB(float b);
        float GetDefaultA() const;
        float GetDefaultB() const;
		
	protected:
        NumericalValue* _a = nullptr;
        NumericalValue* _b = nullptr;

        float _defaultA = 0, _defaultB = 0;

        NumericalValue* _output = nullptr;

        Expression _currentExpression = SUM;

        void DefineSlots() override;
        void ProcessValues() override;

        static void Express(Expression expression, NumericalValue* a, NumericalValue* b, NumericalValue* output);
		static Expression GetExpressionForName(const char* expr);
	};
	
}