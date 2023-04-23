#include "direct_lighting_integrator.hpp"
#include "utils/random.hpp"
namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li(const Scene& p_scene,
		const Ray& p_ray,
		const float   p_tMin,
		const float   p_tMax) const
	{
		HitRecord hitRecord;
		if (p_scene.intersect(p_ray, p_tMin, p_tMax, hitRecord))
		{
			/// TODO ! cos theta...
			return _directLighting(p_scene, p_ray,hitRecord._object->getMaterial()->getFlatColor(), hitRecord._point,hitRecord._normal,hitRecord);
		}
		else
		{
			return _backgroundColor;
		}
	}
	Vec3f DirectLightingIntegrator::_directLighting(const Scene& p_scene, const Ray& p_ray,Vec3f p_color, Vec3f p_point,Vec3f p_normal,HitRecord p_hitRecord) const
	{
		Vec3f li = VEC3F_ZERO;
		for (BaseLight *light : p_scene.getLights())
		{
			int nbOfSamples = 1;
			if (light->getIsSurface())
				nbOfSamples = _nbLightSamples;
			for ( int i = 0; i < nbOfSamples; i++ )
			{
				LightSample lightSample = light->sample(p_point);
				Ray			rayon		= Ray( p_point, lightSample._direction );
				rayon.offset( p_normal );
				float cos = glm::dot( lightSample._direction, p_normal );
				cos		  = glm::max( cos, 0.0f );
				if ( !p_scene.intersectAny( rayon, 0, lightSample._distance ) )
					li +=  lightSample._radiance*p_hitRecord._object->getMaterial()->shade(p_ray,p_hitRecord,lightSample) * cos;
			}
			li = li/(float)nbOfSamples;

		}
		return li;
	}
} // namespace RT_ISICG
