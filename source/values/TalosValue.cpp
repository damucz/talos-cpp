#include "TalosValue.h"

namespace Talos
{
	const char* Value::GetValueType()
	{
		static const char* type = "base";
		return type;
	}

	Value::~Value()
	{
	}

	void Value::SetEmpty(bool isEmpty)
	{
		_isEmpty = isEmpty;
	}

	bool Value::IsEmpty() const
	{
		return _isEmpty;
	}

}