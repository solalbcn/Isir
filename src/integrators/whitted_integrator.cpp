#include "whitted_integrator.hpp"
#include "utils/random.hpp"
namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax ) const
	{
		return _recursiveLighting( p_scene, p_ray, p_tMin, p_tMax, 0, false);
	}

	Vec3f WhittedIntegrator::_recursiveLighting( const Scene & p_scene,
												 const Ray &   p_ray,
												 const float   p_tMin,
												 const float   p_tMax,
												 const int currentBounce,
												 const bool p_inside) const
	{
		if ( currentBounce >= _nbBounces ) return BLACK;
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if (hitRecord._object->getMaterial()->isMirror())
			{
				Ray reflectedRay= Ray( hitRecord._point, normalize( glm::reflect( p_ray.getDirection(), hitRecord._normal ) ) );
				reflectedRay.offset( hitRecord._normal );
				return _recursiveLighting( p_scene, reflectedRay, 0, p_tMax, currentBounce + 1,false);
			}
			else if (hitRecord._object->getMaterial()->isTransparent()) 
			{ 
				// Tentative de FRESNEL ===========================================
				Ray reflectedRay = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				reflectedRay.offset( hitRecord._normal );
				Vec3f reflectedColor= _recursiveLighting( p_scene, reflectedRay, 0, p_tMax, currentBounce + 1, p_inside );
				float n1 = 1;
				float n2		 = hitRecord._object->getMaterial()->getIOR();
				float eta;
				if ( p_inside )
					eta=n2/n1;
				else
					eta = n1 / n2;
				float cosI = glm::dot( -p_ray.getDirection(), hitRecord._normal );
				float sinI = glm::sqrt( 1.0 - cosI * cosI);
				float sinT = ( n1 / n2 ) * sinI;
				float cosT = glm::sqrt( 1.0 - sinT * sinT);
				cosI	   = glm::max( cosI, 0.f );
				cosT	   = glm::max( cosT, 0.f );

				float reflectionCoefficientP = glm::pow( ( n1 * cosI - n2 * cosT ) / ( n1 * cosI + n2 * cosT ), 2 );
				float reflectionCoefficientS = glm::pow( ( n1 * cosT - n2 * cosI ) / ( n1 * cosT + n2 * cosI ), 2 );

				float reflectedProportion = 0.5 * ( reflectionCoefficientP + reflectionCoefficientS );
				if ( reflectedProportion >= 1 ) 
					return reflectedColor;
				Ray refractedRay= Ray( hitRecord._point, glm::refract( p_ray.getDirection(), hitRecord._normal, eta ) );
				refractedRay.offset( -hitRecord._normal );
				Vec3f refractedColor= _recursiveLighting( p_scene, refractedRay, 0, p_tMax, currentBounce + 1, !p_inside );

				return reflectedProportion * reflectedColor + ( 1 - reflectedProportion ) * refractedColor;

			}
			else
			{
				return _directLighting( p_scene,
													p_ray,
													hitRecord._object->getMaterial()->getFlatColor(),
													hitRecord._point,
													hitRecord._normal,
													hitRecord );
			}
			
		}
		else { return _backgroundColor; }
	}

	Vec3f WhittedIntegrator::_directLighting( const Scene & p_scene,
													 const Ray &   p_ray,
													 Vec3f		   p_color,
													 Vec3f		   p_point,
													 Vec3f		   p_normal,
													 HitRecord	   p_hitRecord ) const
	{
		Vec3f li = VEC3F_ZERO;
		for ( BaseLight * light : p_scene.getLights() )
		{
			Vec3f currentColor = VEC3F_ZERO;
			int	  nbOfSamples  = 1;
			if ( light->getIsSurface() ) nbOfSamples = _nbLightSamples;
			for ( int i = 0; i < nbOfSamples; i++ )
			{
				LightSample lightSample = light->sample( p_point );
				Ray			rayon		= Ray( p_point, lightSample._direction );
				rayon.offset( p_normal );
				float cos = glm::dot( lightSample._direction, p_normal );
				cos		  = glm::max( cos, 0.0f );
				if ( !p_scene.intersectAny( rayon, 0, lightSample._distance ) )
					currentColor += lightSample._radiance
									* p_hitRecord._object->getMaterial()->shade( p_ray, p_hitRecord, lightSample )
									* cos;
			}
			li = currentColor / (float)nbOfSamples;
		}
		return li;
	}
} // namespace RT_ISICG
