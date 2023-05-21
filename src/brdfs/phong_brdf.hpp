#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"

namespace RT_ISICG
{
	class PhongBRDF
	{
	public:
		PhongBRDF(const Vec3f& p_kd, const float p_s, const bool p_blinnphong) : _kd(p_kd),_s(p_s),_blinnphong(p_blinnphong) {};

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate(const Vec3f& p_wo, const Vec3f& p_wi, const Vec3f& p_n) const 
		{
			float cos_alpha;
			const float cos_theta = glm::max(0.f, glm::dot(p_n, p_wi));
			if (_blinnphong)
			{
				const Vec3f h = normalize(p_wo + p_wi);
				cos_alpha = glm::max(0.f, glm::dot(p_n, h));
			}
			else
			{
				const Vec3f wr = glm::reflect(-p_wi, p_n);
				cos_alpha = glm::max(0.f, glm::dot(p_wo, wr));	
			}
				
			return _kd / cos_theta * glm::pow(cos_alpha, _s);
		}

		inline const Vec3f& getKd() const { return _kd; }

	private:
		Vec3f _kd = WHITE;
		bool _blinnphong = false;
		float _s = 1;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__
