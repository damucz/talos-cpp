#include "modules/TalosColorModule.h"
#include "values/TalosNumericalValue.h"

namespace Talos
{
	
	void ColorModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		_defaultR = moduleData->GetFloat("r");
		_defaultG = moduleData->GetFloat("g");
		_defaultB = moduleData->GetFloat("b");
	}

	void ColorModule::SetR(float r)
	{
		_defaultR = r;
	}

	void ColorModule::SetG(float g)
	{
		_defaultG = g;
	}

	void ColorModule::SetB(float b)
	{
		_defaultB = b;
	}

	void ColorModule::GetColor(float& r, float& g, float& b) const
	{
		r = _defaultR;
		g = _defaultG;
		b = _defaultB;
		//a = 1.0f;
	}

	void ColorModule::DefineSlots()
	{
		_r = CreateInputSlot(R);
		_g = CreateInputSlot(G);
		_b = CreateInputSlot(B);

		_output = CreateOutputSlot(OUTPUT);
	}

	void ColorModule::ProcessValues()
	{
		if (_r->IsEmpty()) _r->Set(_defaultR);
		if (_g->IsEmpty()) _g->Set(_defaultG);
		if (_b->IsEmpty()) _b->Set(_defaultB);

		_output->Set(_r, _g, _b);
	}
	
}
