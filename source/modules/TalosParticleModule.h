#pragma once

#include "modules/TalosAbstractModule.h"
#include "values/TalosDrawableValue.h"

namespace Talos
{

    class Particle;

	class ParticleModule : public AbstractModule
	{
	public:
        enum T
        {
            ID = 0,
            TAG = 1,
            DRAWABLE = 2,
            OFFSET = 3,
        	LIFE = 4,

        	VELOCITY = 5,
        	GRAVITY = 6,
        	ROTATION = 7,
        	TARGET = 8,
        	COLOR = 9,
        	TRANSPARENCY = 10,
        	ANGLE = 11,
        	MASS = 12,
        	SIZE = 13,
        	POSITION = 14,
        };

        void ProcessValues() override;

        void UpdateScopeData(Particle* particle);

        IParticleDrawable* GetDrawable();
        float GetTransparency();
        float GetLife();
        float GetAngle();
        float GetVelocity();
        float GetRotation();
        void GetSize(float& x, float& y);
        void GetColor(float& r, float& g, float& b, float& a);
        void GetStartPosition(float& x, float& y);
        bool GetTarget(float& x, float& y);
        bool GetPosition(float& x, float& y);
        void SetDefaultDrawable(IParticleDrawable* defaultDrawable);

        bool IsParticleModule() const override;
		
	protected:
        DrawableValue* _drawable = nullptr;
        NumericalValue* _offset = nullptr;
        NumericalValue* _life = nullptr;
        NumericalValue* _velocity = nullptr;
        NumericalValue* _gravity = nullptr;
        NumericalValue* _rotation = nullptr;
        NumericalValue* _target = nullptr;
        NumericalValue* _color = nullptr;
        NumericalValue* _transparency = nullptr;
        NumericalValue* _angle = nullptr;
        NumericalValue* _mass = nullptr;
        NumericalValue* _size = nullptr;
        NumericalValue* _position = nullptr;

        float _tmpColorR = 0, _tmpColorG = 0, _tmpColorB = 0, _tmpColorA = 0;
        float _tmpVecX = 0, _tmpVecY = 0;
        IParticleDrawable* _defaultDrawable = nullptr;

        void DefineSlots() override;
	};
	
}
