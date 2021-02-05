#if 0
#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class VectorFieldModule : public AbstractModule
	{
	public:
		enum T
		{
			SIZE_SCALE = 0,
			FORCE_SCALE = 1,
			POSITION = 2,
			ANGLE = 0,
			VELOCITY = 1,
		};
	protected:
        NumericalValue* _scale = nullptr;
        NumericalValue* _force = nullptr;
        NumericalValue* _position = nullptr;
        NumericalValue* _angle = nullptr;
        NumericalValue* _velocity = nullptr;

        void DefineSlots() override;

        Vector2 pos = new Vector2();
        Vector2 tmp = new Vector2();

        public String fgaFileName;
        VectorField vectorField;

        @Override
            public void processValues() {
            if (vectorField == null) return;
            float scaleVal = 1f;
            if (!scale.isEmpty()) {
                scaleVal = scale.getFloat();
            }

            float forceVal = 1f;
            if (!force.isEmpty()) {
                forceVal = force.getFloat();
            }

            NumericalValue posNumVal = getScope().get(ScopePayload.PARTICLE_POSITION);
            pos.set(posNumVal.get(0), posNumVal.get(1));

            vectorField.setScale(scaleVal);
            vectorField.setPosition(position.get(0), position.get(1));

            tmp = vectorField.getValue(pos, tmp);
            tmp.scl(forceVal);

            angle.set(tmp.angle());
            velocity.set(tmp.len());
        }

        @Override
            public void setModuleGraph(ParticleEmitterDescriptor graph) {
            super.setModuleGraph(graph);
            final AssetProvider assetProvider = graph.getEffectDescriptor().getAssetProvider();
            if (fgaFileName != null && !fgaFileName.isEmpty()) {
                setVectorField(assetProvider.findAsset(fgaFileName, VectorField.class), fgaFileName);
            }
        }

        @Override
            public void write(Json json) {
            super.write(json);
            json.writeValue("fgaAssetName", fgaFileName);
        }

        @Override
            public void read(Json json, JsonValue jsonData) {
            super.read(json, jsonData);
            fgaFileName = jsonData.getString("fgaAssetName", "");
        }

        public void setVectorField(VectorField vectorField, String fileName) {
            this.vectorField = vectorField;
            fgaFileName = fileName;
        }

        public VectorField getVectorField() {
            return vectorField;
        }
	};
	
}
#endif