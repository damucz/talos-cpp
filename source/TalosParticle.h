#pragma once

namespace Talos
{

    class ParticleModule;
    class IEmitter;
    class IParticleDrawable;
	
	class Particle
	{
	public:
        float _spawnPositionX = 0, _spawnPositionY = 0;
        float _positionX = 0, _positionY = 0;
        float _life = 0;
        float _transparency = 0;
        float _rotation = 0;
        float _sizeX = 0;
        float _sizeY = 0;
        float _colorR = 0, _colorG = 0, _colorB = 0, _colorA = 0;
        float _alpha = 0;      // alpha position from 0 to 1 in it's lifetime cycle
        float _seed = 0;
        float _durationAtInit = 0;
        IParticleDrawable* _drawable = nullptr;

        void Init(IEmitter* emitterReference);
        void Init(IEmitter* emitterReference, float seed);
        void Update(float delta);
        void ApplyAlpha(float alpha, float delta);

        float GetX() const;
        float GetY() const;

        float GetEmitterAlpha() const;
        IEmitter* GetEmitter() const;

        void NotifyKill();

	protected:
        IEmitter* _emitterReference = nullptr;
	};
	
}
