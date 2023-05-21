#ifndef __RT_ISICG_METAL_MATERIAL__
#define __RT_ISICG_METAL_MATERIAL__

#include "base_material.hpp"
#include "brdfs/oren_nayar_brdf.hpp"
#include "brdfs/cook_torrance_brdf.hpp"

namespace RT_ISICG
{
	class MetalMaterial : public BaseMaterial
	{
	  public:
		MetalMaterial( const std::string & p_name,
					   const Vec3f &	   p_diffuse,
					   const Vec3f &	   p_specular,
					   const Vec3f &	   p_f0,
					   const float		   p_s,
					   const float		   p_metalness)
			: BaseMaterial( p_name ), _brdf_oren_nayar( p_diffuse, p_s ), _brdf_cook_torrance( p_specular, p_f0, p_s ),
			  _metalness(p_metalness)
		{
		}

		virtual ~MetalMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return ( 1 - _metalness )* _brdf_oren_nayar.evaluate(-p_ray.getDirection(), p_lightSample._direction, p_hitRecord._normal ) + _metalness * _brdf_cook_torrance.evaluate(-p_ray.getDirection(), p_lightSample._direction, p_hitRecord._normal );
		}

		inline const Vec3f & getFlatColor() const override { return _brdf_oren_nayar.getKd(); }

	  protected:
		OrenNayarBRDF _brdf_oren_nayar;
		CookTorranceBRDF _brdf_cook_torrance;
		float			 _metalness;

	};

} // namespace RT_ISICG

#endif // __RT_ISICG_LAMBERT_MATERIAL__
