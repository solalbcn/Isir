#include "aabb.hpp"

namespace RT_ISICG
{
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
        double tx1 = (_min.x - p_ray.getOrigin().x) * (1/p_ray.getDirection().x);
        double tx2 = (_max.x - p_ray.getOrigin().x) * (1/p_ray.getDirection().x);

        double tmin = glm::min(tx1, tx2);
        double tmax = glm::max(tx1, tx2);

        double ty1 = (_min.y - p_ray.getOrigin().y) * (1 / p_ray.getDirection().y);
        double ty2 = (_max.y - p_ray.getOrigin().y) * (1 / p_ray.getDirection().y);

        tmin = glm::max(tmin, glm::min(ty1, ty2));
        tmax = glm::min(tmax, glm::max(ty1, ty2));

        double tz1 = (_min.z - p_ray.getOrigin().z) * (1 / p_ray.getDirection().z);
        double tz2 = (_max.z - p_ray.getOrigin().z) * (1 / p_ray.getDirection().z);

        tmin = glm::max(tmin, glm::min(tz1, tz2));
        tmax = glm::min(tmax, glm::max(tz1, tz2));

        return tmax >= tmin;
	}
} // namespace RT_ISICG
