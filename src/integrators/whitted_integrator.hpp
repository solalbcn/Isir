#ifndef __RT_ISICG_WHITTED_INTEGRATOR__
#define __RT_ISICG_WHITTED_INTEGRATOR__

#include "base_integrator.hpp"

namespace RT_ISICG
{
	class WhittedIntegrator : public BaseIntegrator
	{
	  public:
		WhittedIntegrator() : BaseIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::WHITTED; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		Vec3f _directLighting( const Scene & p_scene,
							   const Ray &	 p_ray,
							   Vec3f		 p_color,
							   Vec3f		 p_point,
							   Vec3f		 p_normal,
							   HitRecord	 p_hitRecord ) const;
		Vec3f _recursiveLighting( const Scene &						   p_scene,
								  const Ray &						   p_ray,
								  const float						   p_tMin,
								  const float						   p_tMax,
								  const int p_currentBounce,
								  const bool p_insde) const;

		int	  _nbBounces = 5;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_DIRECT_LIGHTING_INTEGRATOR