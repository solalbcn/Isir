#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{
		/// TODO !
		Vec3f oc = p_ray.getOrigin() - _center;
		p_t1 = -1.f;
		p_t2 = -1.f;
		float a, b, c;
		b= 2 * glm::dot( p_ray.getDirection(), oc);
		c= glm::dot( oc, oc) - _radius * _radius;
		float delta = b * b - 4 * c; // a = 1 car normalisé
		if (delta >= 0) { 
			p_t1 = ( -b - sqrt( delta ) ) / 2;
			p_t2 = p_t1;
			if ( p_t1 > 0 )
				return true;
			p_t2 = ( -b + sqrt( delta ) ) / 2;
			return true;
		}
		return false;
	}

} // namespace RT_ISICG
