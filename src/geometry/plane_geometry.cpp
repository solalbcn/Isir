#include "plane_geometry.hpp"

namespace RT_ISICG
{
	bool PlaneGeometry::intersect(const Ray& p_ray, float& p_t1) const
	{
		p_t1 = -1;
		// On suppose les vecteurs normalises
		float den = glm::dot(_normal, p_ray.getDirection());
		// Si le plan et le rayon ne sont pas parallèles
		if (den != 0) { // Attention à la comparaison avec 0
			Vec3f po = _point - p_ray.getOrigin();
			p_t1 = glm::dot(po, _normal) / den;
			return true;
		}
		// Sinon, pas d’intersection
		return false;

	}

} // namespace RT_ISICG
