#include "implicit_surface.hpp"

namespace RT_ISICG
{
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		float t = p_tMin;

		while ( t <= p_tMax && t >= p_tMin )
		{
			if ( abs( _sdf( p_ray.pointAtT( t ) ) ) < _minDistance )
			{
				p_hitRecord._distance = t;
				p_hitRecord._point	  = p_ray.pointAtT( t );
				p_hitRecord._normal	  = _evaluateNormal( p_ray.pointAtT( t ) );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._object = this;
				return true;
			}
			t += abs( _sdf( p_ray.pointAtT( t ) ) );
		}

		return false;

	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float t = p_tMin;

		while ( t <= p_tMax && t >= p_tMin )
		{
			if ( abs( _sdf( p_ray.pointAtT( t ) ) ) < _minDistance ) { return true; }
			t += abs( _sdf( p_ray.pointAtT( t ) ) );
		}

		return false;

	}
} // namespace RT_ISICG
