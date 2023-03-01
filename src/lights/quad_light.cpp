#include "quad_light.hpp"
#include <utils/random.hpp>
namespace RT_ISICG
{
	LightSample QuadLight::sample( const Vec3f & p_point ) const
	{
		Vec3f randomPoint = _position+( _u * randomFloat() ) + ( _v * randomFloat() );

		Vec3f direction = glm::normalize( randomPoint-p_point );
		float distance	= glm::length( randomPoint-p_point);
		float pdf		= ( ( 1 / _area ) * ( distance * distance ) ) / ( glm::dot( _n, direction ) );
		Vec3f radiance	= ( _color * _power )/pdf;
		return LightSample( direction, distance, radiance, pdf );
	}
} // namespace RT_ISICG
