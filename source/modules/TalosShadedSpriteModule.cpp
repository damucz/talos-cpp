#include "modules/TalosShadedSpriteModule.h"
#include "values/TalosDrawableValue.h"
#include "render/drawables/TalosShadedDrawable.h"
#include "TalosParticleEmitterDescriptor.h"
#include "TalosParticleEffectDescriptor.h"
#include "TalosIFactory.h"

namespace Talos
{
	
	ShadedSpriteModule::~ShadedSpriteModule()
	{
		delete _shdrFileName;
		delete _userDrawable->GetDrawable();
		delete _userDrawable;
	}

	void ShadedSpriteModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);

		const char* shdrFileName = moduleData->GetString("shdrAssetName");
		_shdrFileName = new char[strlen(shdrFileName) + 1];
		strcpy(_shdrFileName, shdrFileName);
	}

	void ShadedSpriteModule::SetModuleGraph(ParticleEmitterDescriptor* graph)
	{
		AbstractModule::SetModuleGraph(graph);

		auto* factory = graph->GetEffectDescriptor()->GetFactory();
		_userDrawable->SetDrawable(factory->CreateShadedDrawable(_shdrFileName));

		UpdateShader();
	}

	void ShadedSpriteModule::DefineSlots()
	{
		_outputValue = (DrawableValue*)CreateOutputSlot(OUTPUT, new DrawableValue());
		_userDrawable = new DrawableValue();
		_userDrawable->SetEmpty(true);
	}

	void ShadedSpriteModule::ProcessValues()
	{
		_outputValue->Set(_userDrawable);
	}

	void ShadedSpriteModule::UpdateShader()
	{
#if 0
		this.shaderDescriptor = shaderDescriptor;

		if (shaderDescriptor != null) {
			ShadedDrawable drawable = (ShadedDrawable)outputValue.getDrawable();
			drawable.setShader(shaderDescriptor.getFragCode());

			for (String uniformName : shaderDescriptor.getUniformMap().keys()) {
				ShaderDescriptor.UniformData data = shaderDescriptor.getUniformMap().get(uniformName);

				if (data.type == ShaderDescriptor.Type.TEXTURE) {
					TextureRegion textureRegion = graph.getEffectDescriptor().getAssetProvider().findAsset(data.payload, TextureRegion.class);
					textureMap.put(uniformName, textureRegion.getTexture());
				}
			}

			drawable.setTextures(textureMap);
		}
#endif
	}
	
}
