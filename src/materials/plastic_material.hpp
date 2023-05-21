#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "base_material.hpp"
#include "brdfs/phong_brdf.hpp"
#include "brdfs/lambert_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	public:
		PlasticMaterial(const std::string& p_name, const Vec3f& p_diffuse, const Vec3f& p_specular, const float p_s, const bool p_blinnPhong)
			: BaseMaterial(p_name), _brdf_lambert(p_diffuse),_brdf_phong(p_specular,p_s,p_blinnPhong)
		{
		}

		virtual ~PlasticMaterial() = default;

		Vec3f shade(const Ray& p_ray,
			const HitRecord& p_hitRecord,
			const LightSample& p_lightSample) const override
		{
			return _brdf_lambert.evaluate()*0.7f + _brdf_phong.evaluate(-p_ray.getDirection(), p_lightSample._direction, p_hitRecord._normal)*0.3f;
		}

		inline const Vec3f& getFlatColor() const override { return _brdf_lambert.getKd(); }

	protected:
		LambertBRDF _brdf_lambert;
		PhongBRDF _brdf_phong;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_LAMBERT_MATERIAL__
