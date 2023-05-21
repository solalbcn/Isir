#ifndef __RT_ISICG_BRDF_COOK_TORRANCE__
#define __RT_ISICG_BRDF_COOK_TORRANCE__

#include "defines.hpp"

namespace RT_ISICG
{
	class CookTorranceBRDF
	{
	  public:
		CookTorranceBRDF( const Vec3f & p_kd, const Vec3f & p_f0, const float p_sigma )
			: _kd( p_kd ), _sigma( p_sigma ), _f0(p_f0)
		{

		};

		inline Vec3f evaluate( const Vec3f & p_wo, const Vec3f & p_wi, const Vec3f & p_n ) const
		{

			float D, G, k;
			Vec3f F;

			const float gwo = glm::dot( p_n, p_wo );
			const float gwi = glm::dot( p_n, p_wi );
			Vec3f		h	= glm::normalize( p_wo + p_wi );
			float		pi	= glm::pi<float>();
			k				= glm::pow( ( _sigma + 1 ), 2 ) / 8;

			G = ( ( gwo )/(gwo*(1-k)+k))* (( gwi ) /(gwi*(1-k)+k));

			D = glm::pow( _sigma, 4 ) / ( pi * glm::pow( ( glm::pow( glm::dot( p_n, h ), 2 ) ) * (glm::pow(_sigma,4)-1) + 1, 2 ) );
			F = _f0 + ( 1.f - _f0 ) * (float)(glm::pow( 1.f - glm::dot( h, p_wo ), 5.f ) );

			return ( D * F * G ) / ( 4*( glm::dot(p_wo,p_n)*glm::dot(p_wi,p_n) ));
		}

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd	 = WHITE;
		float _sigma = 0;
		Vec3f _f0;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_LAMBERT__