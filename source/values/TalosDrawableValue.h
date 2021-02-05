#pragma once

#include "values/TalosValue.h"
#include "TalosIParticleDrawable.h"

namespace Talos
{

    class IParticleDrawable;
	
	class DrawableValue : public Value
	{
	public:
		const char* GetValueType() override;
		
        DrawableValue();

        void Set(Value* value) override;
        IParticleDrawable* GetDrawable() const;
		void SetDrawable(IParticleDrawable* drawable);
		
	protected:
        IParticleDrawable* _drawable = nullptr;
	};
	
}