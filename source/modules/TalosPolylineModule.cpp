#include "modules/TalosPolylineModule.h"
#include "values/TalosNumericalValue.h"
#include "values/TalosDrawableValue.h"
#include "render/drawables/TalosPolylineDrawable.h"
#include "TalosScopePayload.h"
#include "TalosSlot.h"
#include "TalosIFactory.h"
#include "TalosParticleEmitterDescriptor.h"
#include "TalosParticleEffectDescriptor.h"

namespace Talos
{
	
	PolylineModule::~PolylineModule()
	{
        delete _regionName;
        delete _polylineDrawable;
	}

	void PolylineModule::Load(IFileProvider::ModuleData* moduleData)
	{
        AbstractModule::Load(moduleData);

        _pointCount = moduleData->GetInt("points") + 2;

        const char* region = moduleData->GetString("regionName");   // default = "fire"
        _regionName = new char[strlen(region) + 1];
        strcpy(_regionName, region);
	}

	void PolylineModule::FetchAllInputSlotValues()
	{
        float requester = GetScope()->Get(ScopePayload::REQUESTER_ID)->GetFloat();
        _polylineDrawable->SetCurrentParticle(GetScope()->CurrParticle());

        for (int i = 0; i < _pointCount; i++)
        {
            float pointAlpha = (float)i / (_pointCount - 1);
            GetScope()->Set(ScopePayload::SECONDARY_SEED, pointAlpha);
            GetScope()->Set(ScopePayload::REQUESTER_ID, requester + pointAlpha * 0.1f);

        	for (auto it = _inputSlots.begin(); it != _inputSlots.end(); ++it)
            {
                Slot* inputSlot = it->second;
                FetchInputSlotValue(inputSlot->GetIndex());
            }

            float transparencyVal = 1.0f;
            if (!_transparency->IsEmpty())
                transparencyVal = _transparency->GetFloat();

            float r, g, b, a;
            if (_color->IsEmpty())
            {
                r = 1.0f;
                g = 1.0f;
                b = 1.0f;
                a = transparencyVal;
            }
            else
            {
                r = _color->Get(0);
                g = _color->Get(1);
                b = _color->Get(2);
                a = transparencyVal;
            }

            float thicknessVal = 0.1f;
            if (!_thickness->IsEmpty())
                thicknessVal = _thickness->GetFloat();

            if (_offset->IsEmpty())
                _offset->Set(0.0f, 0.0f);

            _polylineDrawable->SetPointData(i, _offset->Get(0), _offset->Get(1), thicknessVal, r, g, b, a);
            _polylineDrawable->SetTangents(_leftTangent->Get(0), _leftTangent->Get(1), _rightTangent->Get(0), _rightTangent->Get(1));
        }
        GetScope()->Set(ScopePayload::REQUESTER_ID, requester);
	}

	void PolylineModule::SetModuleGraph(ParticleEmitterDescriptor* graph)
	{
        AbstractModule::SetModuleGraph(graph);
        auto* factory = graph->GetEffectDescriptor()->GetFactory();
        _polylineDrawable = factory->CreatePolylineDrawable(_regionName);
        _polylineDrawable->SetCount(_pointCount - 2);
	}

	void PolylineModule::SetInterpolationPoints(int count)
	{
        _pointCount = count + 2;
        _polylineDrawable->SetCount(count);
	}

	void PolylineModule::DefineSlots()
	{
        _offset = CreateInputSlot(OFFSET);
        _thickness = CreateInputSlot(THICKNESS);
        _color = CreateInputSlot(COLOR);
        _transparency = CreateInputSlot(TRANSPARENCY);

        _leftTangent = CreateInputSlot(LEFT_TANGENT);
        _rightTangent = CreateInputSlot(RIGHT_TANGENT);

        //_polylineDrawable = new PolylineDrawable();

        _outputValue = (DrawableValue*)CreateOutputSlot(OUTPUT, new DrawableValue());
        //_outputValue->SetDrawable(_polylineDrawable);
	}

	void PolylineModule::ProcessValues()
	{
		_outputValue->SetDrawable(_polylineDrawable);
	}
	
}
