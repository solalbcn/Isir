#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"
#include <algorithm>

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();
		_root = new BVHNode();
		_buildRec( _root, 0, p_triangles->size(), 0 );
		/// TODO
		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{

		/// TODO
		return _intersectRec(_root, p_ray, p_tMin, p_tMax, p_hitRecord);
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		/// TODO
		return _intersectAnyRec(_root,p_ray, p_tMin, p_tMax);
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		for (size_t  i = p_firstTriangleId; i<p_lastTriangleId;i++)
		{
			p_node->_aabb.extend( _triangles->at(i)._aabb );
		}
		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId = p_lastTriangleId;
		if (!((p_depth >= _maxDepth) || ((p_lastTriangleId - p_firstTriangleId) <= _maxTrianglesPerLeaf))) 
		{
			p_node->_left = new BVHNode();
			p_node->_right = new BVHNode();
			float axePartition = p_node->_aabb.largestAxis();
			float centre = p_node->_aabb.centroid()[axePartition];
			std::vector<TriangleMeshGeometry>::iterator idPartition;
			idPartition = std::partition(_triangles->begin()+ p_firstTriangleId, _triangles->begin() + p_lastTriangleId, [axePartition, centre](const TriangleMeshGeometry	& triangle)
				{
					return triangle._aabb.centroid()[axePartition] < centre;
				});
			int index = idPartition - _triangles->begin();
			_buildRec( p_node->_left, p_firstTriangleId, index, p_depth + 1 );
			_buildRec( p_node->_right, index, p_lastTriangleId, p_depth + 1 );

		}
		/// TODO
	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		bool left=false, right = false;
		if (!p_node->isLeaf())
		{
			if (p_node->_left->_aabb.intersect(p_ray, p_tMin, p_tMax))
			{
				left=_intersectRec(p_node->_left, p_ray, p_tMin, p_tMax, p_hitRecord);
			}
			if (p_node->_right->_aabb.intersect(p_ray, p_tMin, p_tMax))
			{
				right=_intersectRec(p_node->_right, p_ray, p_tMin, p_tMax, p_hitRecord);
			}
			return left||right;
		}
		else
		{
			float  tClosest = p_tMax;			 // Hit distance.

			if (p_hitRecord._object != nullptr)
				tClosest = p_hitRecord._distance;

			size_t hitTri = _triangles->size(); // Hit triangle id.
			Vec3f normal;
			if (p_node->_aabb.intersect(p_ray, p_tMin, p_tMax))
			{
				for (size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++)
				{
					float t;
					Vec3f n;
					if (_triangles->at( i).intersect(p_ray, t, n))
					{
						if (t >= p_tMin && t <= tClosest)
						{
							tClosest = t;
							hitTri = i;
							normal = n;
						}
					}
				}
				if (hitTri != _triangles->size()) // Intersection found.
				{
					p_hitRecord._point = p_ray.pointAtT(tClosest);
					p_hitRecord._normal = normal;
					p_hitRecord.faceNormal(p_ray.getDirection());
					p_hitRecord._distance = tClosest;
					p_hitRecord._object = reinterpret_cast<BaseObject*>(_triangles->at(hitTri).getRefMesh());
					return true;
				}
			}

			return false;
		}
		

	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if (!p_node->_aabb.intersect(p_ray, p_tMin, p_tMax))
			return false;
		bool left = false, right = false;
		if (!p_node->isLeaf())
		{
			if (p_node->_left->_aabb.intersect(p_ray, p_tMin, p_tMax))
			{
				left= _intersectAnyRec(p_node->_left, p_ray, p_tMin, p_tMax);
			}
			if (p_node->_right->_aabb.intersect(p_ray, p_tMin, p_tMax))
			{
				right= _intersectAnyRec(p_node->_right, p_ray, p_tMin, p_tMax);
			}
			return left||right;
		}
		else
		{
			Vec3f normal;
			for (size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId ; i++)
			{
				float t;
				if (_triangles->at(i).intersect(p_ray, t, normal))
				{
					if (t >= p_tMin && t <= p_tMax) return true; // No need to search for the nearest.
				}
			}
			return false;
		}
		return false;
	}
} // namespace RT_ISICG
