#include "point_light.hpp"

namespace RT_ISICG
{
	LightSample PointLight::sample(const Vec3f& p_point) const
	{
		Vec3f direction = glm::normalize( _position - p_point );
		float distance = glm::distance(p_point,_position);
		Vec3f radiance = (_color * _power) * (1.f / (distance*distance));
		return LightSample(direction,distance,radiance,1.f);
	}
} // namespace RT_ISICG
