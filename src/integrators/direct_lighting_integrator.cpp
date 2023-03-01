#include "direct_lighting_integrator.hpp"

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
			float cos = glm::dot(hitRecord._normal, -p_ray.getDirection());
			cos = glm::max(cos, 0.0f);
			return _directLighting(p_scene, hitRecord._object->getMaterial()->getFlatColor(), hitRecord._point,hitRecord._normal);
		}
		else
		{
			return _backgroundColor;
		}
	}
	Vec3f DirectLightingIntegrator::_directLighting(const Scene& p_scene, Vec3f p_color, Vec3f p_point,Vec3f p_normal) const
	{
		Vec3f li = VEC3F_ZERO;
		for (BaseLight *light : p_scene.getLights())
		{
			LightSample lightSample = light->sample(p_point);
			Ray rayon=Ray(p_point, -lightSample._direction);
			rayon.offset(p_normal);
			float cos = glm::dot(-lightSample._direction, p_normal);
			cos = glm::max(cos, 0.0f);
			if (!p_scene.intersectAny(rayon, 0, lightSample._distance))
				li += p_color * lightSample._radiance * cos;
		}
		return li;
	}
} // namespace RT_ISICG
