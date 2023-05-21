#ifndef __RT_ISICG_IMPLICIT_SURFACE__
#define __RT_ISICG_IMPLICIT_SURFACE__

#include "base_object.hpp"

namespace RT_ISICG
{
	class ImplicitSurface : public BaseObject
	{
	  public:
		ImplicitSurface()		   = delete;
		virtual ~ImplicitSurface() = default;

		ImplicitSurface( const std::string & p_name ) : BaseObject( p_name ) {}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		// Check for any intersection between p_tMin and p_tMax.
		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const = 0;

		// Evaluate normal by computing gradient at 'p_point'
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			float e = 0.0001f;
			return normalize( Vec3f( 1, -1, -1 ) * _sdf( p_point + Vec3f( e, -e, -e ) )
							  + Vec3f( -1, -1, 1 ) * _sdf( p_point + Vec3f( -e, -e, e ) )
							  + Vec3f( -1, 1, -1 ) * _sdf( p_point + Vec3f( -e, e, -e ) )
							  + Vec3f( 1, 1, 1 ) * _sdf( p_point + Vec3f( e, e, e ) ) );

			const Vec3f p = p_point;
			const float h = 0.0001f; // replace by an appropriate value
			const Vec2f k = Vec2f( 1.f, -1.f );

			return glm::normalize( Vec3f( k.x, k.y, k.y ) * _sdf( p + Vec3f( k.x, k.y, k.y ) * h )
								   + Vec3f( k.y, k.y, k.x ) * _sdf( p + Vec3f( k.y, k.y, k.x ) * h )
								   + Vec3f( k.y, k.x, k.y ) * _sdf( p + Vec3f( k.y, k.x, k.y ) * h )
								   + Vec3f( k.x, k.x, k.x ) * _sdf( p + Vec3f( k.x, k.x, k.x ) * h ) );

		}

	  private:
		const float _minDistance = 1e-4f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SURFACE__
