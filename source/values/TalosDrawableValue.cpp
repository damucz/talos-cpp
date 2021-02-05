#include "values/TalosDrawableValue.h"

namespace Talos
{
	const char* DrawableValue::GetValueType()
	{
		static const char* type = "drawable";
		return type;
	}

	DrawableValue::DrawableValue()
	{
		SetEmpty(true);
	}

	void DrawableValue::Set(Value* value)
	{
		if (value->IsEmpty())
		{
			SetEmpty(true);
			_drawable = nullptr;
			return;
		}
		if (GetValueType() == value->GetValueType())
			_drawable = ((DrawableValue*)value)->GetDrawable();

		SetEmpty(_drawable == nullptr);
	}

	IParticleDrawable* DrawableValue::GetDrawable() const
	{
		return _drawable;
	}

	void DrawableValue::SetDrawable(IParticleDrawable* drawable)
	{
		_drawable = drawable;
		SetEmpty(_drawable == nullptr);
	}
	
}
