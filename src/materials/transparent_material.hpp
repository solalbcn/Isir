#ifndef __RT_ISICG_TRANSPARENT_MATERIAL__
#define __RT_ISICG_TRANSPARENT_MATERIAL__

#include "base_material.hpp"

namespace RT_ISICG
{
	class TransparentMaterial : public BaseMaterial
	{
	  public:
		TransparentMaterial( const std::string & p_name ) : BaseMaterial( p_name ) {}

		virtual ~TransparentMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return BLACK;
		}

		inline const Vec3f & getFlatColor() const override { return BLACK; }

		const bool isTransparent() const { return true; }
		const float getIOR() const { return _ior; }

	  protected:
		float _ior = 1.3f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_LAMBERT_MATERIAL__
