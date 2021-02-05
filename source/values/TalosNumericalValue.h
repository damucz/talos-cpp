#pragma once

#include "values/TalosValue.h"

namespace Talos
{

	class NumericalValue : public Value
	{
	public:
		const char* GetValueType() override;
		
		enum Flavour
		{
			REGULAR,
			ANGLE,
			NORMALIZED,
		};

		NumericalValue();

		void SetFlavour(Flavour flavour);
		Flavour GetFlavour() const;

		void Configure(int elementCount);

		void Sum(const NumericalValue* other, NumericalValue* out);
		void Sub(const NumericalValue* other, NumericalValue* out);
		void Mul(float val, NumericalValue* out);
		void Mul(const NumericalValue* other, NumericalValue* out);
		void Div(const NumericalValue* other, NumericalValue* out);
		void Pow(const NumericalValue* other, NumericalValue* out);
		void Abs(NumericalValue* out);
		void Sin(NumericalValue* out);
		void Cos(NumericalValue* out);

		void Set(const NumericalValue* other);
		void Set(int index, float value);
		void Set(float value);
		void Set(float val1, float val2);
		void Set(float val1, float val2, float val3);
		void Set(float val1, float val2, float val3, float val4);
		void Set(const NumericalValue* val1, const NumericalValue* val2);
		void Set(const NumericalValue* val1, const NumericalValue* val2, const NumericalValue* val3);
		void Set(Value* value) override;

		float GetFloat() const;

		int ElementsCount() const;
		void SetElementsCount(int elementsCount);

		const float* GetElements() const;
		float Get(int index) const;

		void SetEmpty(bool isEmpty) override;
		
	protected:
		float _elements[4];
		int _currentElementCount = 0;
		Flavour _flavour = REGULAR;
	};

}
