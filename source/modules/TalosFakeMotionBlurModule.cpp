#include "modules/TalosFakeMotionBlurModule.h"
#include "values/TalosNumericalValue.h"

namespace Talos
{
	
	void FakeMotionBlurModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		_velocityMin = moduleData->GetFloat("velocityMin");
		_velocityMax = moduleData->GetFloat("velocityMax");
		_sizeMin = moduleData->GetFloat("sizeMin");
		_sizeMax = moduleData->GetFloat("sizeMax");
	}

	float FakeMotionBlurModule::GetVelocityMin() const
	{
		return _velocityMin;
	}

	void FakeMotionBlurModule::SetVelocityMin(float velocityMin)
	{
		_velocityMin = velocityMin;
	}

	float FakeMotionBlurModule::GetVelocityMax() const
	{
		return _velocityMax;
	}

	void FakeMotionBlurModule::SetVelocityMax(float velocityMax)
	{
		_velocityMax = velocityMax;
	}

	float FakeMotionBlurModule::GetSizeMin() const
	{
		return _sizeMin;
	}

	void FakeMotionBlurModule::SetSizeMin(float sizeMin)
	{
		_sizeMin = sizeMin;
	}

	float FakeMotionBlurModule::GetSizeMax() const
	{
		return _sizeMax;
	}

	void FakeMotionBlurModule::SetSizeMax(float sizeMax)
	{
		_sizeMax = sizeMax;
	}

	void FakeMotionBlurModule::DefineSlots()
	{
		_velocity = CreateInputSlot(VELOCITY);
		_size = CreateOutputSlot(SIZE);
	}

	void FakeMotionBlurModule::ProcessValues()
	{
		float velocity = _velocity->GetFloat();
		// clamp
		if (velocity < _velocityMin) velocity = _velocityMin;
		if (velocity > _velocityMax) velocity = _velocityMax;
		
		float normVel = (velocity - _velocityMin) / (_velocityMax - _velocityMin); // 0..1
		float sizeVal = _sizeMin + (_sizeMax - _sizeMin) * normVel;

		_size->Set(sizeVal);
	}
	
}
