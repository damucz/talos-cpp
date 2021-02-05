#include "modules/TalosTextureModule.h"
#include "values/TalosDrawableValue.h"
#include "TalosParticleEmitterDescriptor.h"
#include "TalosParticleEffectDescriptor.h"
#include "TalosIFactory.h"
#include "render/drawables/TalosTextureRegionDrawable.h"

namespace Talos
{
	
	TextureModule::~TextureModule()
	{
		delete _regionName;
		delete _userDrawable->GetDrawable();
		delete _userDrawable;
	}

	void TextureModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		const char* region = moduleData->GetString("regionName");
		_regionName = new char[strlen(region) + 1];
		strcpy(_regionName, region);
	}

	void TextureModule::ProcessValues()
	{
		_outputValue->Set(_userDrawable);
	}

	void TextureModule::SetModuleGraph(ParticleEmitterDescriptor* graph)
	{
		AbstractModule::SetModuleGraph(graph);
		
		auto* factory = graph->GetEffectDescriptor()->GetFactory();
		_userDrawable->SetDrawable(factory->CreateTextureRegionDrawable(_regionName));
	}

	void TextureModule::DefineSlots()
	{
		_outputValue = (DrawableValue*)CreateOutputSlot(OUTPUT, new DrawableValue());
		_userDrawable = new DrawableValue();
		_userDrawable->SetEmpty(true);
	}
	
}
