#pragma once

#include "modules/TalosAbstractModule.h"
#include "TalosTypes.h"

namespace Talos
{

    class CurveModule : public AbstractModule
    {
    public:
        enum T
        {
            ALPHA = 0,
            OUTPUT = 0,
        };

        void Init() override;
        void Load(IFileProvider::ModuleData* moduleData) override;

        int CreatePoint(float x, float y);
        void RemovePoint(int index);

    protected:
        NumericalValue* _alpha = nullptr;
        NumericalValue* _output = nullptr;

        std::vector<Point> _points;

        void DefineSlots() override;
        void ResetPoints();
        void SortPoints();
        void ProcessValues() override;
        void ProcessAlphaDefaults();
        float Interpolate(float alpha);
        float Interpolate(float alpha, float x1, float y1, float x2, float y2);
    };
	
}
