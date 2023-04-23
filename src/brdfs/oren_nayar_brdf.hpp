#ifndef __RT_ISICG_BRDF_OREN_NAYAR__
#define __RT_ISICG_BRDF_OREN_NAYAR__

#include "defines.hpp"

namespace RT_ISICG
{
    class OrenNayarBRDF
    {
    public:
        OrenNayarBRDF(const Vec3f& p_kd,const float p_sigma) : _kd(p_kd),_sigma(p_sigma) { 
            std::cout << p_sigma << std::endl;
        _A = 1 - 0.5 * ((_sigma * _sigma) / (_sigma * _sigma + 0.33));
        _B = 0.45 * ((_sigma * _sigma) / (_sigma * _sigma + 0.09));
        };

        inline Vec3f evaluate(const Vec3f& p_wo, const Vec3f& p_wi, const Vec3f& p_n) const
        {
            const float cosThetaO = glm::dot(p_n, p_wo);
            const float cosThetaI = glm::dot(p_n, p_wi);

            const float thetaO = glm::acos(cosThetaO);
            const float thetaI = glm::acos(cosThetaI);

            const float alpha = glm::max(thetaO, thetaI);
            const float beta = glm::min(thetaO, thetaI);
            const Vec3f woPhi = glm::normalize(p_wo - p_n * cosThetaO);
            const Vec3f wiPhi = glm::normalize(p_wi - p_n * cosThetaI);

            const float cosPhiIO = glm::dot(woPhi, wiPhi);
            const float gamma = glm::max(0.f, cosPhiIO);

            return _kd * INV_PIf * (_A + (_B * gamma * glm::sin(alpha) * glm::tan(beta)));
        }

        inline const Vec3f& getKd() const { return _kd; }

    private:
        Vec3f _kd = WHITE;
        float _sigma =0;
        float _A;
        float _B;
    };
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_LAMBERT__