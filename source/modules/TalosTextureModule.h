#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

    class DrawableValue;
    class TextureRegionDrawable;

	class TextureModule : public AbstractModule
	{
	public:
        enum T
        {
            OUTPUT = 0,
		};

        ~TextureModule();

		void Load(IFileProvider::ModuleData* moduleData) override;

        void ProcessValues() override;
        void SetModuleGraph(ParticleEmitterDescriptor* graph) override;

	protected:
        DrawableValue* _userDrawable = nullptr;
        DrawableValue* _outputValue = nullptr;

        char* _regionName = nullptr;

		void DefineSlots() override;
	};
	
}
