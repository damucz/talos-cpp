#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

    class DrawableValue;

	class ShadedSpriteModule : public AbstractModule
	{
	public:
		enum T
		{
			OUTPUT = 0,
		};

        ~ShadedSpriteModule();

		void Load(IFileProvider::ModuleData* moduleData) override;

		void SetModuleGraph(ParticleEmitterDescriptor* graph) override;
		
	protected:
        DrawableValue* _outputValue = nullptr;
		DrawableValue* _userDrawable = nullptr;
        char* _shdrFileName = nullptr;

        void DefineSlots() override;
		void ProcessValues() override;

        void UpdateShader();
	};
	
}