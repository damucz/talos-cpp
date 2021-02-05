#ifdef TALOSVFX_AVAILABLE
#include "CGETalosVFX.h"
#include "TalosParticleEffectDescriptor.h"
#include "TalosParticleEffectInstance.h"
#include "serialization/TalosRapidJsonProvider.h"
#include "render/drawables/TalosTextureRegionDrawable.h"

#include "TalosIEmitter.h"
#include "TalosParticle.h"

#include "glm/gtc/type_ptr.hpp"

#include <IwDebug.h>

namespace CGE
{

	Talos::IFileProvider* TalosVFXFactory::CreateFileProvider(const char* fileName)
	{
		return new Talos::RapidJsonProvider(fileName);
	}

	Talos::TextureRegionDrawable* TalosVFXFactory::CreateTextureRegionDrawable(const char* region)
	{
		return new TalosTextureRegionDrawable(region);
	}

	Talos::PolylineDrawable* TalosVFXFactory::CreatePolylineDrawable(const char* region)
	{
		return new TalosPolylineDrawable(region);
	}

	Talos::ShadedDrawable* TalosVFXFactory::CreateShadedDrawable(const char* shdrFileName)
	{
		return new TalosShadedDrawable(shdrFileName);
	}

	TalosTextureRegionDrawable::TalosTextureRegionDrawable(const char* region)
		: Talos::TextureRegionDrawable(region)
	{
		Thor::String s;
		s = "talosvfx/";
		s += region;
		s += ".png";
		_texture = THOR_RESOURCEMANAGER.GetTexture(s.c_str());
	}

	void TalosTextureRegionDrawable::Draw(Talos::RenderContext* context, float x, float y, float width, float height,
		float rotation)
	{
#if 0
		region.setPosition(x - width / 2, y - height / 2);
		region.setSize(width, height);
		region.setOrigin(width / 2, height / 2);
		region.setRotation(rotation);
		region.draw(batch);
#endif
	}

	void TalosTextureRegionDrawable::Draw(Talos::RenderContext* context, Talos::Particle* particle, float r, float g,
	                                      float b, float a)
	{
		float rotation = particle->_rotation;
		float width = particle->_sizeX;
		float height = particle->_sizeY;
		float x = particle->GetX();
		float y = particle->GetY();

		auto* cgecontext = IwSafeCast<TalosRenderContext*>(context);
		auto* renderer = cgecontext->renderer;
		if (!_texture) return;


		//region.setColor(color);

		auto* uvs = renderer->GetRectUVs(Thor::Renderer::UVRotationType::UV_ROT_0);

		renderer->SetTexture(_texture);
		CIwFMat ptransform;
		ptransform.SetRotZ(rotation / 180.0f * (float)M_PI);
		CIwFMat pscale;
		pscale.SetIdentity();
		pscale.m[0][0] = width * 100.0f;// _texture->getWidth();
		pscale.m[1][1] = height * 100.0f;// _texture->getHeight();
		ptransform.PreMult(pscale);
		CIwFMat ptrans;
		ptrans.SetIdentity();
		ptrans.t.x = x * 100.0f;// _texture->getWidth();
		ptrans.t.y = -y * 100.0f;// _texture->getHeight();
		ptrans.t.z = 0;
		ptransform.PostMult(ptrans);
		ptransform.PostMult(cgecontext->animModelTransform);

		renderer->SetModelTransform(ptransform);

		//renderer->SetColor(*color);
		CIwFVec3 verts[4];
		verts[0].x = -0.5f;
		verts[0].y = -0.5f;
		verts[0].z = 0;

		verts[1].x = 0.5f;
		verts[1].y = -0.5f;
		verts[1].z = 0;

		verts[2].x = 0.5f;
		verts[2].y = 0.5f;
		verts[2].z = 0;

		verts[3].x = -0.5f;
		verts[3].y = 0.5f;
		verts[3].z = 0;
		renderer->DrawQuad(&verts[0], uvs);

		//draw(batch, x, y, width, height, rotation);
	}

	TalosPolylineDrawable::TalosPolylineDrawable(const char* region)
		: Talos::PolylineDrawable(region)
	{
		Thor::String s;
		s = "talosvfx/";
		s += region;
		s += ".png";
		_texture = THOR_RESOURCEMANAGER.GetTexture(s.c_str());
	}

	TalosPolylineDrawable::~TalosPolylineDrawable()
	{
		for (auto polyline : _polylineMap)
			delete polyline.second;
	}

	void TalosPolylineDrawable::Draw(Talos::RenderContext* context, float x, float y, float width, float height, float rotation)
	{
		Polyline* polyline = GetPolyline();
		polyline->Set(width, rotation);
		polyline->Draw(IwSafeCast<TalosRenderContext*>(context)->renderer, _texture, x, y);

		for (auto it = _polylineMap.begin(); it != _polylineMap.end(); )
		{
			if (it->first->_alpha == 1.0f)
			{
				delete it->second;
				it = _polylineMap.erase(it);
			}
			else
				++it;
		}
	}

	void TalosPolylineDrawable::Draw(Talos::RenderContext* context, Talos::Particle* particle, float r, float g, float b, float a)
	{
		float rotation = particle->_rotation;
		float width = particle->_sizeX;
		float height = particle->_sizeY;
		float x = particle->GetX();
		float y = particle->GetY();

		CIwFMat ptransform;
		ptransform.SetRotZ(rotation / 180.0f * (float)M_PI);
		CIwFMat pscale;
		pscale.SetIdentity();
		pscale.m[0][0] = width * 10.0f;// *_texture->getWidth();
		pscale.m[1][1] = height * 10.0f;// *_texture->getHeight();
		ptransform.PreMult(pscale);
		CIwFMat ptrans;
		ptrans.SetIdentity();
		ptrans.t.x = x * 10.0f;// *_texture->getWidth();
		ptrans.t.y = -y * 10.0f;// *_texture->getHeight();
		ptrans.t.z = 0;
		ptransform.PostMult(ptrans);
		ptransform.PostMult(IwSafeCast<TalosRenderContext*>(context)->animModelTransform);

		IwSafeCast<TalosRenderContext*>(context)->renderer->SetModelTransform(ptransform);

		Draw(context, x, y, width, height, rotation);
	}

	TalosPolylineDrawable::Polyline::~Polyline()
	{
		delete[] _vertices;
		delete[] _indices;
		delete[] _uvs;
		delete[] _colors;
	}

	void TalosPolylineDrawable::Polyline::Reset()
	{
		for (PointData& pointData : _points)
		{
			pointData.positionX = 0;
			pointData.positionY = 0;
			pointData.offsetX = 0;
			pointData.offsetY = 0;
			pointData.thickness = 0;
		}
	}

	void TalosPolylineDrawable::Polyline::Set(float size, float rotation)
	{
		_leftPointX = -size / 2.0f;
		_leftPointY = 0;
		_rightPointX = size / 2.0f;
		_rightPointY = 0;
		_rotation = rotation;
	}

	void TalosPolylineDrawable::Polyline::InitPoints(int interpolationPoints, float x, float y)
	{
		if ((int)_points.size() != interpolationPoints + 2)
		{
			_points.clear();
			for (int i = 0; i < interpolationPoints + 2; i++)
				_points.push_back(PointData());

			InitVertices();
		} // else we reuse them

		for (auto& point : _points)
		{
			point.positionX = x;
			point.positionY = y;
		}
	}

	void TalosPolylineDrawable::Polyline::SetPointData(int index, float offsetX, float offsetY, float thickness,
		float r, float g, float b, float a)
	{
		auto& point = _points[index];
		point.r = r;
		point.g = g;
		point.b = b;
		point.a = a;
		point.offsetX = offsetX;
		point.offsetY = offsetY;
		point.thickness = thickness;
	}

	const std::vector<TalosPolylineDrawable::Polyline::PointData>& TalosPolylineDrawable::Polyline::GetPoints() const
	{
		return _points;
	}

	void TalosPolylineDrawable::Polyline::Draw(Thor::Renderer* renderer, Thor::Texture* region)
	{
		for (int i = 0; i < (int)_points.size() - 1; i++)
		{
			// extrude each point
			ExtrudePoint(region, i, 0);
			ExtrudePoint(region, i, 1);

			// creating indexes
			_indices[i * 6] = (uint16)(i * 4);
			_indices[i * 6 + 1] = (uint16)(i * 4 + 1);
			_indices[i * 6 + 2] = (uint16)(i * 4 + 3);
			_indices[i * 6 + 3] = (uint16)(i * 4);
			_indices[i * 6 + 4] = (uint16)(i * 4 + 3);
			_indices[i * 6 + 5] = (uint16)(i * 4 + 2);
		}

		// do the actual drawing
		renderer->SetTexture(region);
		renderer->DrawPrimitive(Thor::Video::ptTriangles, _verticesLength, _vertices, _uvs, _colors, false, _indicesLength, _indices);
	}

	void TalosPolylineDrawable::Polyline::Draw(Thor::Renderer* renderer, Thor::Texture* region, float x, float y)
	{
		UpdatePointPositions(x, y);
		Draw(renderer, region);
	}

	void TalosPolylineDrawable::Polyline::InitVertices()
	{
		int pointCount = _points.size();
		int vertexCount = (pointCount - 1) * 4;
		int trisCount = (pointCount - 1) * 2;

		if (_vertices == nullptr || _verticesLength != vertexCount)
		{
			delete[] _vertices;
			delete[] _indices;
			delete[] _uvs;
			delete[] _colors;
			_verticesLength = vertexCount;
			_indicesLength = trisCount * 3;
			_vertices = new CIwFVec3[vertexCount];
			_indices = new uint16[_indicesLength];
			_uvs = new CIwFVec2[vertexCount];
			_colors = new CIwColour[vertexCount];
		}
	}

	void TalosPolylineDrawable::Polyline::UpdatePointPositions(float x, float y)
	{
		/** Cubic Bezier curve
		 * @param out The {@link Vector} to set to the result.
		 * @param t The location (ranging 0..1) on the curve.
		 * @param p0 The first bezier point.
		 * @param p1 The second bezier point.
		 * @param p2 The third bezier point.
		 * @param p3 The fourth bezier point.
		 * @param tmp A temporary vector to be used by the calculation.
		 * @return The value specified by out for chaining */
		auto BezierCubic = [](float& outx, float& outy, float t, float p0x, float p0y, float p1x, float p1y,
			float p2x, float p2y, float p3x, float p3y)
		{
			// B3(t) = (1-t) * (1-t) * (1-t) * p0 + 3 * (1-t) * (1-t) * t * p1 + 3 * (1-t) * t * t * p2 + t * t * t * p3
			float dt = 1.0f - t;
			float dt2 = dt * dt;
			float t2 = t * t;
			outx = p0x * (dt2 * dt) + p1x * (3 * dt2 * t) + p2x * (3 * dt * t2) + p3x * (t2 * t);
			outy = p0y * (dt2 * dt) + p1y * (3 * dt2 * t) + p2y * (3 * dt * t2) + p3y * (t2 * t);
		};
		
		int i = 0;
		for (auto& point : _points)
		{
			float alpha = (float)i / (_points.size() - 1);
			BezierCubic(point.positionX, point.positionY, alpha, _leftPointX, _leftPointY,
				_leftPointX + _leftTangentX, _leftPointY + _leftTangentY, _rightPointX + _rightTangentX, _rightPointY + _rightTangentY,
				_rightPointX, _rightPointY);

			// ad the offsets
			point.positionX += point.offsetX;
			point.positionY += point.offsetY;

			// apply rotation while origin is at 0
			float cos = cosf(_rotation * (float)M_PI / 180.0f);
			float sin = sinf(_rotation * (float)M_PI / 180.0f);
			float newx = point.positionX * cos - point.positionY * sin;
			float newy = point.positionX * sin + point.positionY * cos;
			point.positionX = newx;
			point.positionY = newy;

			//apply origin position
			point.positionX += x;
			point.positionY += y;

			++i;
		}
	}

	void TalosPolylineDrawable::Polyline::ExtrudePoint(Thor::Texture* region, int index, int pos)
	{
		int i = index + pos;
		float v = (float)(i) / (_points.size() - 1);

		float thickness = _points[i].thickness;

		if (i > 0 && i < (int)_points.size() - 1)
		{
			float p1x = _points[i - 1].positionX;
			float p1y = _points[i - 1].positionY;
			float p2x = _points[i].positionX;
			float p2y = _points[i].positionY;
			float p3x = _points[i + 1].positionX;
			float p3y = _points[i + 1].positionY;

			//Left hand side normal of first edge
			float tmpx = p2x - p1x;
			float tmpy = p2y - p1y;
			//nor
			float len = sqrtf(tmpx * tmpx + tmpy * tmpy);
			if (len != 0) { tmpx /= len; tmpy /= len; }
			//rotate90(1)
			float tmp = tmpx; tmpx = -tmpy; tmpy = tmp;
			//nor?
			len = sqrtf(tmpx * tmpx + tmpy * tmpy);
			
			//Left hand side normal of second edge
			float tmp2x = p3x - p2x;
			float tmp2y = p3y - p2y;
			//nor
			len = sqrtf(tmp2x * tmp2x + tmp2y * tmp2y);
			if (len != 0) { tmp2x /= len; tmp2y /= len; }
			//rotate90(1)
			tmp = tmp2x; tmp2x = -tmp2y; tmp2y = tmp;
			//nor?

			//Bisector
			float tmp3x = tmpx + tmp2x;
			float tmp3y = tmpy + tmp2y;
			//nor
			len = sqrtf(tmp3x * tmp3x + tmp3y * tmp3y);
			if (len != 0) { tmp3x /= len; tmp3y /= len; }
			
			tmp3x *= thickness / 2.0f;
			tmp3y *= thickness / 2.0f;

			tmpx = tmp3x + p2x;
			tmpy = tmp3y + p2y;
			tmp2x = -tmp3x + p2x;
			tmp2y = -tmp3y + p2y;

			// left extension vertex
			auto& pointa = _points[i];
			PackVertex(region, _vertices, _uvs, _colors, index * 4 + 1 + pos * 2, tmpx, tmpy, pointa.r, pointa.g, pointa.b, pointa.a, 0, v);
			// right extension vertex
			auto& pointb = _points[i + 1];
			PackVertex(region, _vertices, _uvs, _colors, index * 4 + pos * 2, tmp2x, tmp2y, pointb.r, pointb.g, pointb.b, pointb.a, 1, v);
		}
		else
		{
			if (i == 0)
			{
				float p1x = _points[i].positionX;
				float p1y = _points[i].positionY;
				float p2x = _points[i + 1].positionX;
				float p2y = _points[i + 1].positionY;

				float tmpx = p2x - p1x;
				float tmpy = p2y - p1y;
				//nor
				float len = sqrtf(tmpx * tmpx + tmpy * tmpy);
				if (len != 0) { tmpx /= len; tmpy /= len; }

				float tmp2x = tmpx;
				float tmp2y = tmpy;
				//rotate90(1)
				float tmp = tmp2x; tmp2x = -tmp2y; tmp2y = tmp;
				//nor?

				tmp2x *= thickness / 2.0f;
				tmp2y *= thickness / 2.0f;

				float tmp3x = tmpx;
				float tmp3y = tmpy;
				//rotate90(-1)
				tmp = tmp3x; tmp3x = tmp3y; tmp3y = -tmp;
				//nor?

				tmp3x *= thickness / 2.0f;
				tmp3y *= thickness / 2.0f;

				tmpx = tmp2x + p1x;
				tmpy = tmp2y + p1y;

				tmp2x = tmp3x + p1x;
				tmp2y = tmp3y + p1y;

				// left extension vertex
				auto& pointa = _points[i];
				PackVertex(region, _vertices, _uvs, _colors, index * 4 + 1 + pos * 2, tmpx, tmpy, pointa.r, pointa.g, pointa.b, pointa.a, 0, v);
				// right extension vertex
				auto& pointb = _points[i + 1];
				PackVertex(region, _vertices, _uvs, _colors, index * 4 + pos * 2, tmp2x, tmp2y, pointb.r, pointb.g, pointb.b, pointb.a, 1, v);
			}
			if (i == (int)_points.size() - 1)
			{
				float p1x = _points[i - 1].positionX;
				float p1y = _points[i - 1].positionY;
				float p2x = _points[i].positionX;
				float p2y = _points[i].positionY;

				float tmpx = p2x - p1x;
				float tmpy = p2y - p1y;
				//nor
				float len = sqrtf(tmpx * tmpx + tmpy * tmpy);
				if (len != 0) { tmpx /= len; tmpy /= len; }

				float tmp2x = tmpx;
				float tmp2y = tmpy;
				//rotate90(1)
				float tmp = tmp2x; tmp2x = -tmp2y; tmp2y = tmp;
				//nor?

				tmp2x *= thickness / 2.0f;
				tmp2y *= thickness / 2.0f;

				float tmp3x = tmpx;
				float tmp3y = tmpy;
				//rotate90(-1)
				tmp = tmp3x; tmp3x = tmp3y; tmp3y = -tmp;
				//nor?

				tmp3x *= thickness / 2.0f;
				tmp3y *= thickness / 2.0f;

				tmpx = tmp2x + p2x;
				tmpy = tmp2y + p2y;

				tmp2x = tmp3x + p2x;
				tmp2y = tmp3y + p2y;

				// left extension vertex
				auto& pointa = _points[i - 1];
				PackVertex(region, _vertices, _uvs, _colors, index * 4 + 1 + pos * 2, tmpx, tmpy, pointa.r, pointa.g, pointa.b, pointa.a, 0, v);
				// right extension vertex
				auto& pointb = _points[i];
				PackVertex(region, _vertices, _uvs, _colors, index * 4 + pos * 2, tmp2x, tmp2y, pointb.r, pointb.g, pointb.b, pointb.a, 1, v);
			}
		}
	}

	void TalosPolylineDrawable::Polyline::PackVertex(Thor::Texture* region, CIwFVec3* vertices, CIwFVec2* uvs, CIwColour* colors,
		int index, float x, float y, float r, float g, float b, float a, float u, float v)
	{
		float insideOffset = 0.0f; // needed in case region has have some weird transparent edge. maybe.

		vertices[index].x = x;
		vertices[index].y = y;
		vertices[index].z = 0.0f;
		uvs[index].x = u;// region->getU() + u * (region->getDU() - region->getU() - insideOffset) + insideOffset;
		uvs[index].y = v;// region->getV() + v * (region->getDV() - region->getV() - insideOffset) + insideOffset;
		colors[index].Set((uint8)(r * 255.0f), (uint8)(g * 255.0f), (uint8)(b * 255.0f), (uint8)(a * 255.0f));
	}

	TalosPolylineDrawable::Polyline* TalosPolylineDrawable::GetPolyline()
	{
		auto it = _polylineMap.find(_currentParticle);
		if (it == _polylineMap.end())
		{
			Polyline* polyline = new Polyline;
			polyline->InitPoints(_interpolationPointCount, _currentParticle->GetX(), _currentParticle->GetY());
			_polylineMap[_currentParticle] = polyline;
			return polyline;
		}

		return it->second;
	}

	void TalosPolylineDrawable::SetCount(int count)
	{
		_interpolationPointCount = count;
		// reset all existing items from the pool
		for (auto it = _polylineMap.begin(); it != _polylineMap.end(); ++it)
			delete it->second;
		_polylineMap.clear();
	}

	void TalosPolylineDrawable::SetPointData(int pointIndex, float offsetX, float offsetY, float thickness, float r,
		float g, float b, float a)
	{
		Polyline* polyline = GetPolyline();
		polyline->SetPointData(pointIndex, offsetX, offsetY, thickness, r, g, b, a);
	}

	void TalosPolylineDrawable::SetTangents(float leftX, float leftY, float rightX, float rightY)
	{
		Polyline* polyline = GetPolyline();
		polyline->_leftTangentX = leftX;
		polyline->_leftTangentY = leftY;
		polyline->_rightTangentX = rightX;
		polyline->_rightTangentY = rightY;
	}

	TalosShadedDrawable::TalosShadedDrawable(const char* shdrFileName)
		: Talos::ShadedDrawable(shdrFileName)
	{
		Thor::String s;
		s = "talosvfx/";
		s += shdrFileName;
		s += ".png";
		
		_shader = _shaderMgr.CreateShader(0, "vs", "fs_portal");
	}

	void TalosShadedDrawable::Draw(Talos::RenderContext* context, float x, float y, float width, float height,
		float rotation)
	{
	}

	void TalosShadedDrawable::Draw(Talos::RenderContext* context, Talos::Particle* particle, float r, float g, float b,
		float a)
	{
		float rotation = particle->_rotation;
		float width = particle->_sizeX;
		float height = particle->_sizeY;
		float x = particle->GetX();
		float y = particle->GetY();

		auto* cgecontext = IwSafeCast<TalosRenderContext*>(context);
		auto* renderer = cgecontext->renderer;

		renderer->PushRenderState();
		auto oldShader = renderer->GetShader();

		bgfx::UniformHandle u_time_pack = _shader->FindUniformHandle("u_time_pack");
		if (isValid(u_time_pack))
		{
			glm::vec4 uTimePack(particle->_alpha * particle->_life, 0, 0, 0); // TODO this should be exposed as port later on 
			bgfx::setUniform(u_time_pack, glm::value_ptr(uTimePack));
		}

		renderer->SetTexture(nullptr);
		renderer->SetShader(_shader);


		auto uvs = renderer->GetRectUVs(Thor::Renderer::UVRotationType::UV_ROT_0);
		
		CIwFMat ptransform;
		ptransform.SetRotZ(rotation / 180.0f * (float)M_PI);
		CIwFMat pscale;
		pscale.SetIdentity();
		pscale.m[0][0] = width * 100.0f;// _texture->getWidth();
		pscale.m[1][1] = height * 100.0f;// _texture->getHeight();
		ptransform.PreMult(pscale);
		CIwFMat ptrans;
		ptrans.SetIdentity();
		ptrans.t.x = x * 100.0f;// _texture->getWidth();
		ptrans.t.y = -y * 100.0f;// _texture->getHeight();
		ptrans.t.z = 0;
		ptransform.PostMult(ptrans);
		ptransform.PostMult(cgecontext->animModelTransform);

		renderer->SetModelTransform(ptransform);

		//renderer->SetColor(*color);
		CIwFVec3 verts[4];
		verts[0].x = -0.5f;
		verts[0].y = -0.5f;
		verts[0].z = 0;

		verts[1].x = 0.5f;
		verts[1].y = -0.5f;
		verts[1].z = 0;

		verts[2].x = 0.5f;
		verts[2].y = 0.5f;
		verts[2].z = 0;

		verts[3].x = -0.5f;
		verts[3].y = 0.5f;
		verts[3].z = 0;

		renderer->DrawQuad(&verts[0], uvs);
		
		renderer->DrawBatch();
		renderer->SetShader(oldShader);

		renderer->PopRenderState();
	}

	TalosVFX::TalosVFX(const char* fileName)
	{
		_factory = new TalosVFXFactory;
		_particleEffectDescriptor = new Talos::ParticleEffectDescriptor(_factory, fileName);
		_particleEffectInstance = _particleEffectDescriptor->CreateEffectInstance();
	}

	TalosVFX::~TalosVFX()
	{
		delete _particleEffectInstance;
		delete _particleEffectDescriptor;
	}

	bool TalosVFX::Update(float dt)
	{
		_particleEffectInstance->Update(dt / 1000.0f);
		//IwTrace(TALOS, ("TalosVFX: nodecalls=%d  particles=%d", _particleEffectInstance->GetNodeCalls(), _particleEffectInstance->GetParticleCount()));
		return true;
	}

	void TalosVFX::DrawContent(Thor::Renderer* renderer)
	{
		TalosRenderContext context;
		context.renderer = renderer;
		context.animModelTransform = _animModelTransform;

		renderer->PushRenderState();
		//renderer->SetBlendMode(Thor::Video::bmAdd);		// alpha, one
		renderer->SetBlendMode(Thor::Video::bmPremultipliedAlphaAdd);		// alpha, one

		for (auto* particleEmitter : _particleEffectInstance->GetEmitters())
		{
			if (!particleEmitter->IsVisible()) continue;
			if (particleEmitter->IsBlendAdd())
				renderer->SetBlendMode(Thor::Video::bmPremultipliedAlpha);		// one, inv src alpha
			else
			{
				if (particleEmitter->IsAdditive())
					renderer->SetBlendMode(Thor::Video::bmPremultipliedAlphaAdd);		// alpha, one
				else
					renderer->SetBlendMode(Thor::Video::bmPremultipliedAlpha);	// alpha, inv src alpha
			}

			for (auto* particle : particleEmitter->GetActiveParticles())
				RenderParticle(&context, particle, _particleEffectInstance->GetAlpha());
		}
		
		renderer->PopRenderState();
	}

	void TalosVFX::RenderParticle(TalosRenderContext* context, Talos::Particle* particle, float parentAlpha)
	{
		float tintR, tintG, tintB, tintA;
		particle->GetEmitter()->GetTint(tintR, tintG, tintB, tintA);
		float r = particle->_colorR * tintR;
		float g = particle->_colorG * tintG;
		float b = particle->_colorB * tintB;
		float a = particle->_transparency * parentAlpha;

		context->renderer->PushRenderState();
		
		CIwColour c;
		c.Set((uint8)(r * 255.0f), (uint8)(g * 255.0f), (uint8)(b * 255.0f), (uint8)(a * 255.0f));
		//context->renderer->SetColorBlend(c);
		context->renderer->SetColor(c);

		if (particle->_drawable)
		{
			particle->_drawable->SetCurrentParticle(particle);
			particle->_drawable->Draw(context, particle, r, g, b, a);
		}
		context->renderer->PopRenderState();
	}
	
}

#endif
