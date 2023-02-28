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
			return _directLighting(p_scene, hitRecord._object->getMaterial()->getFlatColor(), cos, hitRecord._point);
		}
		else
		{
			return _backgroundColor;
		}
	}
	Vec3f DirectLightingIntegrator::_directLighting(const Scene& p_scene, Vec3f p_color, float p_cosTheta, Vec3f p_point) const
	{
		Vec3f li = VEC3F_ZERO;
		for (BaseLight *light : p_scene.getLights())
		{

			li += p_color * light->sample(p_point)._radiance * p_cosTheta;
		}
		return li;
	}
} // namespace RT_ISICG
