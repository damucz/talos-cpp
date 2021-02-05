#include "values/TalosNumericalValue.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Talos
{
	const char* NumericalValue::GetValueType()
	{
		static const char* type = "numerical";
		return type;
	}

	NumericalValue::NumericalValue()
	{
		for (int i = 0; i < 4; ++i)
			_elements[i] = 0;
	}

	void NumericalValue::SetFlavour(Flavour flavour)
	{
		_flavour = flavour;
	}

	NumericalValue::Flavour NumericalValue::GetFlavour() const
	{
		return _flavour;
	}

	void NumericalValue::Configure(int elementCount)
	{
		//Depeneding on the values

		//If we are 2 element count vector, and other is a 3 vector, we increase currentElementCount and init with 0
		_currentElementCount = elementCount;
	}

	void NumericalValue::Sum(const NumericalValue* other, NumericalValue* out)
	{
		for (int i = 0; i < _currentElementCount; i++)
		{
			out->_elements[i] = _elements[i] + other->_elements[i];
		}
		out->SetElementsCount(ElementsCount());
	}

	void NumericalValue::Sub(const NumericalValue* other, NumericalValue* out)
	{
		for (int i = 0; i < _currentElementCount; i++)
		{
			out->_elements[i] = _elements[i] - other->_elements[i];
		}
		out->SetElementsCount(ElementsCount());
	}

	void NumericalValue::Mul(float val, NumericalValue* out)
	{
		for (int i = 0; i < _currentElementCount; i++)
		{
			out->_elements[i] = _elements[i] - val;
		}
		out->SetElementsCount(ElementsCount());
	}

	void NumericalValue::Mul(const NumericalValue* other, NumericalValue* out)
	{
		for (int i = 0; i < _currentElementCount; i++)
		{
			out->_elements[i] = _elements[i] * other->_elements[i];
		}
		out->SetElementsCount(ElementsCount());
	}

	void NumericalValue::Div(const NumericalValue* other, NumericalValue* out)
	{
		for (int i = 0; i < _currentElementCount; i++)
		{
			float d = other->GetFloat();
			if (d == 0) d = 1;
			out->_elements[i] = _elements[i] * other->_elements[i];
		}
		out->SetElementsCount(ElementsCount());
	}

	void NumericalValue::Pow(const NumericalValue* other, NumericalValue* out)
	{
		for (int i = 0; i < _currentElementCount; i++)
		{
			out->_elements[i] = powf(_elements[i], other->GetFloat());
		}
		out->SetElementsCount(ElementsCount());
	}

	void NumericalValue::Abs(NumericalValue* out)
	{
		for (int i = 0; i < _currentElementCount; i++)
		{
			out->_elements[i] = fabsf(_elements[i]);
		}
		out->SetElementsCount(ElementsCount());
	}

	void NumericalValue::Sin(NumericalValue* out)
	{
		for (int i = 0; i < _currentElementCount; i++)
		{
			out->_elements[i] = sinf((float)(_elements[i] / 180.0f * M_PI));
		}
		out->SetElementsCount(ElementsCount());
	}

	void NumericalValue::Cos(NumericalValue* out)
	{
		for (int i = 0; i < _currentElementCount; i++)
		{
			out->_elements[i] = cosf((float)(_elements[i] / 180.0f * M_PI));
		}
		out->SetElementsCount(ElementsCount());
	}

	void NumericalValue::Set(const NumericalValue* other)
	{
		if (other->ElementsCount() > _currentElementCount)
		{
			_currentElementCount = other->ElementsCount();
		}
		for (int i = 0; i < 4; i++)
		{
			if (i < _currentElementCount)
				_elements[i] = other->_elements[i];
			else
				_elements[i] = 0;
		}
	}

	void NumericalValue::Set(int index, float value)
	{
		_elements[index] = value;
	}

	void NumericalValue::Set(float value)
	{
		_currentElementCount = 1;
		_elements[0] = value;
	}

	void NumericalValue::Set(float val1, float val2)
	{
		_currentElementCount = 2;
		_elements[0] = val1;
		_elements[1] = val2;
	}

	void NumericalValue::Set(float val1, float val2, float val3)
	{
		_currentElementCount = 3;
		_elements[0] = val1;
		_elements[1] = val2;
		_elements[2] = val3;
	}

	void NumericalValue::Set(float val1, float val2, float val3, float val4)
	{
		_currentElementCount = 4;
		_elements[0] = val1;
		_elements[1] = val2;
		_elements[2] = val3;
		_elements[3] = val4;
	}

	void NumericalValue::Set(const NumericalValue* val1, const NumericalValue* val2)
	{
		_currentElementCount = 2;
		_elements[0] = val1->GetFloat();
		_elements[1] = val2->GetFloat();
	}

	void NumericalValue::Set(const NumericalValue* val1, const NumericalValue* val2, const NumericalValue* val3)
	{
		_currentElementCount = 3;
		_elements[0] = val1->GetFloat();
		_elements[1] = val2->GetFloat();
		_elements[2] = val3->GetFloat();
	}

	void NumericalValue::Set(Value* value)
	{
		if (GetValueType() == value->GetValueType())
			Set((NumericalValue*)value);
	}

	float NumericalValue::GetFloat() const
	{
		return _elements[0];
	}

	int NumericalValue::ElementsCount() const
	{
		return _currentElementCount;
	}

	void NumericalValue::SetElementsCount(int elementsCount)
	{
		_currentElementCount = elementsCount;
	}

	const float* NumericalValue::GetElements() const
	{
		return &_elements[0];
	}

	float NumericalValue::Get(int index) const
	{
		return _elements[index];
	}

	void NumericalValue::SetEmpty(bool isEmpty)
	{
		Value::SetEmpty(isEmpty);
		if (isEmpty)
			_currentElementCount = 0;
	}
}
