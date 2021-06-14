// A good quality *thread-safe* Mersenne Twister random number generator.
//#include <random>
//std::random_device rd;
//std::mt19937_64 RNGen(rd());
//std::uniform_real_distribution<> myrandom(0.0, 1.0);

#include "rand.h"
#include "material.h"
#include "shape.h"

# define M_PI           3.14159265358979323846f  /* pi */

Vector3f Material::SampleBrdf(const Vector3f& N) const
{
    float xi1 = getARandomFloat();
    float xi2 = getARandomFloat();

    //float xi1 = myrandom(RNGen);
    //float xi2 = myrandom(RNGen);

    return SampleLobe(N, sqrt(xi1), 2.0f * M_PI * xi2);
}

Vector3f Material::SampleBrdf(const Vector3f& omegao, const Vector3f& N) const
{
    // Choose diffuse or reflective via a random number xi
    float xi = getARandomFloat();
    // do this here or after the choice?
    float xi1 = getARandomFloat();
    float xi2 = getARandomFloat();

    // choose diffuse
    if (xi < Calcpd())
    {
        return SampleLobe(N, sqrt(xi1), 2.0f * M_PI * xi2);
    }
    // choose reflection
    else if(xi < (Calcpd() + Calcpr()))
    {
        // phong
        float costhetam = pow(xi1, 1.0f / (alpha + 1.0f));


        // ggx
        //float alphag = ConvertAlphaFromPhong(alpha);
        //float costhetam = cos(atan(alphag * sqrt(xi1) / sqrt(1.0f - xi1)));

        // beckman
        //float alphab = ConvertAlphaFromPhong(alpha);
        //float costhetam = cos(atan(sqrt(-(alphab * alphab) * log(1.0f - xi1))));




        Vector3f m = SampleLobe(N, costhetam, 2.0f * M_PI * xi2);
        m.normalize();

        return 2.0f * abs(omegao.dot(m)) * m - omegao;
    }
    // choose transmission
    else
    {
        // phong
        float costhetam = pow(xi1, 1.0f / (alpha + 1.0f));
        // use same BRDF choice as reflection
        // ggx
        //float alphag = ConvertAlphaFromPhong(alpha);
        //float costhetam = cos(atan(alphag * sqrt(xi1) / sqrt(1.0f - xi1)));

        Vector3f m = SampleLobe(N, costhetam, 2.0f * M_PI * xi2);
        m.normalize();

        float etain = 1.0f;
        float etaout = 1.0f;

        float eta = CalcIndexofRefraction(omegao, N, etain, etaout);

        float r = TestRadicand(eta, omegao, m);
        // if negative, declare total internal reflection and
        if (r < 0.0f)
        {
            return 2.0f * abs(omegao.dot(m)) * m - omegao;
        }
        else
        {
            float omegaoDotM = omegao.dot(m);
            float omegaoDotN = omegao.dot(N);

            return (eta * omegaoDotM - sign(omegaoDotN) * sqrt(r)) * m - eta * omegao;
        }
    }
}

float Material::PdfBrdf(const Vector3f& N, const Vector3f& omegai) const
{
    return abs(N.dot(omegai))/ M_PI;
}

float Material::PdfBrdf(const Vector3f& omegao, Vector3f& N, Vector3f& omegai) const
{
    // Given pd and pr as the probabilities of choice made in SampleBrdf
    // and Pd and Pr as the probabilities of the vector omegai being calculated
    // for the diffuse and reflection directions repectively
    //  return the combined probability
    
    // The diffuse probability calculation
    float Pd = abs(N.dot(omegai)) / M_PI;
    // The (specular) reflection probability calculation:
    Vector3f m = (omegao + omegai).normalized();
    
    float Pr = CalcDterm(m, N) * abs(m.dot(N)) * 1.0f / (4.0f * abs(omegai.dot(m)));
    //float Pr = CalcDtermGGX(m, N) * abs(m.dot(N)) * 1.0f / (4.0f * abs(omegai.dot(m)));
    //float Pr = CalcDtermBeckman(m, N) * abs(m.dot(N)) * 1.0f / (4.0f * abs(omegai.dot(m)));

    // transmission
    // get eta, etain ,eta out
    float etain = 1.0f;
    float etaout = 1.0f;
    float eta = CalcIndexofRefraction(omegao, N, etain, etaout);
    // calc mt
    Vector3f mt = -(etaout * omegai + etain * omegao).normalized();
    // test radicand
    float r = TestRadicand(eta, omegao, mt);
    // if negative, declare total internal reflection, and calculate the reflection term
    float Pt;
    if (r < 0.0f)
    {
        //Pt = CalcDtermGGX(m, N) * abs(m.dot(N)) * 1.0f / (4.0f * abs(omegai.dot(m)));
        Pt = CalcDterm(m, N) * abs(m.dot(N)) * 1.0f / (4.0f * abs(omegai.dot(m)));
    }
    else
    {
        float omegaoDotMt = omegao.dot(mt);
        float omegaiDotMt = omegai.dot(mt);
        float denom = etaout * omegaiDotMt + etain * omegaoDotMt;
        
        Pt = CalcDterm(mt, N) * abs(mt.dot(N)) * etaout * etaout * abs(omegaiDotMt) / (denom * denom);
        //Pt = CalcDtermGGX(mt, N) * abs(mt.dot(N)) * etaout * etaout * abs(omegaiDotMt) / (denom * denom);
        

        //float omegaoDotM = omegao.dot(m);
        //float omegaiDotM = omegai.dot(m);
        //float denom = etaout * omegaiDotM + etain * omegaoDotM;
        //
        ////Pt = CalcDtermGGX(m, N) * abs(m.dot(N)) * etaout * etaout * abs(omegaiDotM) / (denom * denom);
        //Pt = CalcDterm(m, N) * abs(m.dot(N)) * etaout * etaout * abs(omegaiDotM) / (denom * denom);
    }

    return Calcpd() * Pd + Calcpr() * Pr + Calcpt() * Pt;
}

Color Material::EvalScattering(const Vector3f& N, const Vector3f& omegai) const
{
    return abs(N.dot(omegai)) * Kd/ M_PI;
}

Color Material::EvalScattering(const Vector3f& omegao, const Vector3f& N, const Vector3f& omegai) const
{
    // diffuse
    Vector3f Ed = Kd / M_PI;
 
    // half vector
    Vector3f m = (omegao + omegai).normalized();

    float absomegaIdotN = abs(omegai.dot(N));
    float absomegaOdotN = abs(omegao.dot(N));

    //if (isnan(4.0f * absomegaidotN * abs(omegao.dot(N))))
        //std::cout << "bugev" << std::endl;

    //if (isinf(4.0f * absomegaidotN * abs(omegao.dot(N))))
        //std::cout << "bugev" << std::endl;

    // specular
    Vector3f Er = (CalcDterm(m, N) * CalcGterm(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaIdotN * absomegaOdotN);
    //Vector3f Er = (CalcDtermGGX(m, N) * CalcGtermGGX(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaidotN * abs(omegao.dot(N)));
    //Vector3f Er = (CalcDtermBeckman(m, N) * CalcGtermBeckman(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaidotN * abs(omegao.dot(N)));

    // transmission
    // determine eta in and eta out by calculating omegao dot  N
    float etain = 1.0f;
    float etaout = 1.0f;

    //float eta = CalcIndexofRefraction(omegao, N, etain, etaout);
    float eta = 1.0f;

    float omegaoDotN = omegao.dot(N);
   
    if (omegaoDotN > 0.0f)
   {
       etain = 1.0f;
       etaout = ior;
   }
   else if (omegaoDotN < 0.0f)
   {
       etain = ior;
       etaout = 1.0f;
   }
   
   eta = etain / etaout;


    // calculate mt
    Vector3f mt = (-(etaout * omegai + etain * omegao).normalized());
    float omegaoDotMt = omegao.dot(mt);
    // test radicand
    float r = 1.0f - (eta * eta) * (1.0f - omegaoDotMt * omegaoDotMt);
    //float r = TestRadicand(eta, omegao, mt);
    Vector3f Et = Vector3f(0.0f, 0.0f, 0.0f);
    // if negative, declare total internal reflection, and calculate the reflection term
    if (r < 0.0f)
    {
        //Et = (CalcDtermGGX(m, N) * CalcGtermGGX(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaidotN * abs(omegao.dot(N)));
        Et = (CalcDterm(m, N) * CalcGterm(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaIdotN * absomegaOdotN);

    }
    else
    {
        float omegaiDotMt = omegai.dot(mt);
        float omegaoDotMt = omegao.dot(mt);
        float EtsecondPartDenom = etaout * omegaiDotMt + etain * omegaoDotMt;
        float EtsecondPart = (abs(omegaiDotMt) * abs(omegaoDotMt) * etaout * etaout) / (EtsecondPartDenom * EtsecondPartDenom);
        // calculate the transmission term
        //Et = (CalcDtermGGX(mt, N) * CalcGtermGGX(omegai, omegao, mt, N) *  (Vector3f(1.0f, 1.0f, 1.0f) - CalcFterm(omegai.dot(mt))) ) / (absomegaidotN * abs(omegao.dot(N))) * EtsecondPart;
        Et = (CalcDterm(mt, N) * CalcGterm(omegai, omegao, mt, N) *  (Vector3f(1.0f, 1.0f, 1.0f) - CalcFterm(omegai.dot(mt))) ) / (absomegaIdotN * abs(omegao.dot(N))) * EtsecondPart;
        //second part
        
        
        
        //float omegaIDotM = omegai.dot(m);
        //float omegaODotM = omegao.dot(m);
        //
        //
        //Vector3f EtfirstPart = (CalcDterm(m, N) * CalcGterm(omegai, omegao, m, N) * (Vector3f(1.0f, 1.0f, 1.0f) - CalcFterm(omegai.dot(m)))) / (absomegaIdotN * absomegaOdotN);        
        //float EtsecondPartDenom = etaout * omegaIDotM + etain * omegaODotM;
        //float EtsecondPart = (abs(omegaIDotM) * abs(omegaODotM) * etaout * etaout) / (EtsecondPartDenom * EtsecondPartDenom);
        //
        //// calculate the transmission term
        ////Et = (CalcDtermGGX(m, N) * CalcGtermGGX(omegai, omegao, m, N) * (Vector3f(1.0f, 1.0f, 1.0f) - CalcFterm(omegai.dot(m)))) / (absomegaidotN * abs(omegao.dot(N))) * EtsecondPart;
        ////Et = (CalcDterm(m, N) * CalcGterm(omegai, omegao, m, N) * (Vector3f(1.0f, 1.0f, 1.0f) - CalcFterm(omegai.dot(m))) ) / (absomegaidotN * abs(omegao.dot(N))) * EtsecondPart;
        //Et = EtfirstPart * EtsecondPart;
        
        // second part
        
    
    }



    //Vector3f result = absomegaidotN * (Ed + Er);
    Vector3f result = absomegaIdotN * (Ed + Er + Et);

    if (isnan(result.x()))
    {   //std::cout << "bugxn" << std::endl;
        result.x() = 0.0f;
        result.y() = 0.0f;
        result.z() = 0.0f;
    }
    if (isinf(result.x()))
    {
        std::cout << "bugxinf" << std::endl;
        result.x() = 0.0f;
        result.y() = 0.0f;
        result.z() = 0.0f;
    }
    if (isnan(result.y()))
    {   //std::cout << "bugyn" << std::endl;
        result.x() = 0.0f;
        result.y() = 0.0f;
        result.z() = 0.0f;
    }
    if (isinf(result.y()))
    {
        std::cout << "bugyinf" << std::endl;
        result.x() = 0.0f;
        result.y() = 0.0f;
        result.z() = 0.0f;
    }
    if (isnan(result.z()))
    {   //std::cout << "bugzn" << std::endl;
        result.x() = 0.0f;
        result.y() = 0.0f;
        result.z() = 0.0f;
    }
    if (isinf(result.z()))
    {
        std::cout << "bugzinf" << std::endl;
        result.x() = 0.0f;
        result.y() = 0.0f;
        result.z() = 0.0f;
    }

    return result;
}

Color Material::EvalScattering(const Vector3f& omegao, const Vector3f& N, const Vector3f& omegai, float t) const
{
    // diffuse
    Color Ed = Kd / M_PI;

    Vector3f m = (omegao + omegai).normalized();

    float absomegaidotN = abs(omegai.dot(N));
    float absomegaOdotN = abs(omegao.dot(N));
    float omegaIdotM = omegai.dot(m);

    //if (isnan(4.0f * absomegaidotN * abs(omegao.dot(N))))
        //std::cout << "bugev" << std::endl;

    //if (isinf(4.0f * absomegaidotN * abs(omegao.dot(N))))
        //std::cout << "bugev" << std::endl;

    // specular
    Color Er = (CalcDterm(m, N) * CalcGterm(omegai, omegao, m, N) * CalcFterm(omegaIdotM)) / (4.0f * absomegaidotN * absomegaOdotN);
    //Vector3f Er = (CalcDtermGGX(m, N) * CalcGtermGGX(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaidotN * abs(omegao.dot(N)));
    //Vector3f Er = (CalcDtermBeckman(m, N) * CalcGtermBeckman(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaidotN * abs(omegao.dot(N)));

    // transmission
    // determine eta in and eta out by calculating omegao dot  N
    Color At = CalcA(omegao, N, t);
    float etain = 1.0f;
    float etaout = 1.0f;

    float eta = CalcIndexofRefraction(omegao, N, etain, etaout);

    float omegaoDotN = omegao.dot(N);
    //if (omegaoDotN > 0.0f)
    //{
    //    etain = 1.0f;
    //    etaout = ior;
    //}
    //else if (omegaoDotN < 0.0f)
    //{
    //    etain = ior;
    //    etaout = 1.0f;
    //}
    //
    //eta = etain / etaout;


    // calculate mt
    Vector3f mt = -((etaout * omegai + etain * omegao).normalized());
    //float omegaoDotMt = omegao.dot(mt);
    // test radicand
    //float r = 1.0f - (eta * eta) * (1.0f - omegaoDotMt * omegaoDotMt);
    float r = TestRadicand(eta, omegao, mt);
    Color Et = Vector3f(0.0f, 0.0f, 0.0f);
    // if negative, declare total internal reflection, and calculate the reflection term
    if (r < 0.0f)
    {
        Et = (CalcDterm(m, N) * CalcGterm(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaidotN * absomegaOdotN);
        //Et = (CalcDtermGGX(m, N) * CalcGtermGGX(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaidotN * abs(omegao.dot(N)));
        //Et = Vector3f(Et.x() * At.x(), Et.y() * At.y(), Et.z() * At.z());
        Et = Et * At;
        //Et = (CalcDtermGGX(m, N) * CalcGtermGGX(omegai, omegao, m, N) * CalcFterm(omegai.dot(m))) / (4.0f * absomegaidotN * abs(omegao.dot(N)));
    }
    else
    {
        
        float omegaiDotMt = omegai.dot(mt);
        float omegaoDotMt = omegao.dot(mt);
        float EtsecondPartDenom = etaout * omegaiDotMt + etain * omegaoDotMt;
        float EtsecondPart = (abs(omegaiDotMt) * abs(omegaoDotMt) * etaout * etaout) / (EtsecondPartDenom * EtsecondPartDenom);
        // calculate the transmission term
        //Et = (CalcDtermGGX(mt, N) * CalcGtermGGX(omegai, omegao, mt, N) *  (Vector3f(1.0f, 1.0f, 1.0f) - CalcFterm(omegai.dot(mt))) ) / (absomegaidotN * abs(omegao.dot(N))) * EtsecondPart;
        Et = (CalcDterm(mt, N) * CalcGterm(omegai, omegao, mt, N) * (Vector3f(1.0f, 1.0f, 1.0f) - CalcFterm(omegaiDotMt))) / (absomegaidotN * absomegaOdotN) * EtsecondPart;
        Et = Et * At;
    
    
        //float omegaiDotM = omegai.dot(m);
        //float omegaoDotM = omegao.dot(m);
        //float EtsecondPartDenom = etaout * omegaiDotM + etain * omegaoDotM;
        //float EtsecondPart = (abs(omegaiDotM) * abs(omegaoDotM) * etaout * etaout) / (EtsecondPartDenom * EtsecondPartDenom);
        //// calculate the transmission term
        ////Et = At *(CalcDtermGGX(m, N) * CalcGtermGGX(omegai, omegao, m, N) * (Vector3f(1.0f, 1.0f, 1.0f) - CalcFterm(omegai.dot(m)))) / (absomegaidotN * abs(omegao.dot(N))) * EtsecondPart;
        //Et = (CalcDtermGGX(m, N) * CalcGtermGGX(omegai, omegao, m, N) * (Vector3f(1.0f, 1.0f, 1.0f) - CalcFterm(omegai.dot(m)))) / (absomegaidotN * abs(omegao.dot(N))) * EtsecondPart;
        //Et = Vector3f(Et.x() * At.x(), Et.y() * At.y(), Et.z() * At.z());
        // second part
    
    
    }



    //Vector3f result = absomegaidotN * (Ed + Er);
    Color result = absomegaidotN * (Ed + Er + Et);

    //if (isnan(result.x()))
    //{   //std::cout << "bugxn" << std::endl;
    //    result.x() = 0.0f;
    //    result.y() = 0.0f;
    //    result.z() = 0.0f;
    //}
    //if (isinf(result.x()))
    //    std::cout << "bugxinf" << std::endl;
    //
    //if (isnan(result.y()))
    //{   //std::cout << "bugyn" << std::endl;
    //    result.x() = 0.0f;
    //    result.y() = 0.0f;
    //    result.z() = 0.0f;
    //}
    //if (isinf(result.y()))
    //    std::cout << "bugyinf" << std::endl;
    //
    //if (isnan(result.z()))
    //{   //std::cout << "bugzn" << std::endl;
    //    result.x() = 0.0f;
    //    result.y() = 0.0f;
    //    result.z() = 0.0f;
    //}
    //if (isinf(result.z()))
    //    std::cout << "bugzinf" << std::endl;
    //

    //return absomegaidotN * (Ed);
    return result;
}


Vector3f Material::SampleLobe(const Vector3f& N, float c, float phi) const
{
    float s = sqrt(1.0f - c * c);

    Vector3f K = Vector3f(s * cos(phi), s * sin(phi), c);

    Quaternionf q = Quaternionf::FromTwoVectors(Vector3f::UnitZ(), N);

    return q._transformVector(K);
}

float Material::GetCharacteristicFactor(float d) const
{
    float result = 0.0f;

    if (d > 0.0f)
        result = 1.0f;
    else
        result = 0.0f;


    return result;
}

Vector3f Material::CalcFterm(float d) const
{
    return Ks + (Vector3f(1.0f, 1.0f, 1.0f) - Ks) * pow((1.0f - abs(d)), 5);
    //return Ks + (Vector3f(1.0f, 1.0f, 1.0f) - Ks) * pow((1.0f - d), 5);
    //return Ks + (Vector3f(1.0f - Ks.x(), 1.0f - Ks.y(), 1.0f - Ks.z())) * pow((1.0f - d), 5);
}

float Material::CalcDterm(const Vector3f& m, const Vector3f& N) const
{
    float mdotN = m.dot(N);

    if (mdotN <= 0.0f)
        return 0.0f;

    //return GetCharacteristicFactor(mdotN) * (alpha + 2.0f) / (2.0f * M_PI) * pow(mdotN, alpha);
    return (alpha + 2.0f) / (2.0f * M_PI) * pow(mdotN, alpha);
}

float Material::CalcDtermGGX(const Vector3f& m, const Vector3f& N) const
{
    float mdotN = m.dot(N);

    if (mdotN <= 0.0f)
        return 0.0f;

    float tanthetam = sqrt((1.0f - mdotN * mdotN)) / mdotN;

    float alphag = ConvertAlphaFromPhong(alpha);

    float ag_squared = alphag * alphag;

    float mdotN_quaded = mdotN * mdotN * mdotN * mdotN;

    float tantm_squared = tanthetam * tanthetam;

    float denom = M_PI * mdotN_quaded * (ag_squared + tantm_squared) * (ag_squared + tantm_squared);

    //if (isnan(denom))
        //std::cout << "bugden" << std::endl;

    //if (isinf(denom))
        //std::cout << "bugden" << std::endl;


    if (isnan(ag_squared / denom))
        return 1.0f;

    if (isinf(ag_squared / denom))
        return 1.0f;


    return ag_squared / denom;

    //return alphag * alphag / (M_PI * pow(mdotN, 4) *(alphag* alphag + tanthetam * tanthetam)  * (alphag * alphag + tanthetam * tanthetam));
}

float Material::CalcDtermBeckman(const Vector3f& m, const Vector3f& N) const
{
    float mdotN = m.dot(N);

    if (mdotN <= 0.0f)
        return 0.0f;

    float tanthetam = sqrt((1.0f - mdotN * mdotN)) / mdotN;

    float alphab = ConvertAlphaFromPhong(alpha);

    float ab_squared = alphab * alphab;
    float mdotN_quaded = mdotN * mdotN * mdotN * mdotN;
    float tantm_squared = tanthetam * tanthetam;

    float denom = M_PI * ab_squared * mdotN_quaded;

    return exp(-tantm_squared / ab_squared)/denom;
}

float Material::CalcG1(const Vector3f& v, const Vector3f& m, const Vector3f& N) const
{
    float vdotm = v.dot(m);
    float vdotN = v.dot(N);

    if ((vdotm / vdotN) <= 0.0f)
        return 0.0f;

    if (vdotN > 1.0f)
        return 1.0f;

    float tanthetav = sqrt((1.0f) - (vdotN * vdotN)) / (vdotN);

    // may use epsilon?
    if (tanthetav == 0.0f)
    //if (abs(tanthetav) < 0.0001f)
        return 1.0f;



    // phong
    float a = sqrt(alpha / (2.0f) + 1.0f) / tanthetav;

    float afactor = 1.0f;

    if (a < 1.6f)
    {
        afactor = (3.535f * a + 2.181f * a * a) / (1.0f + 2.276f * a + 2.577f * a * a);
    }



    //return GetCharacteristicFactor(vdotm/vdotN) * afactor;
    return afactor;
}

float Material::CalcG1GGX(const Vector3f& v, const Vector3f& m, const Vector3f& N) const
{
    float vdotm = v.dot(m);
    float vdotN = v.dot(N);

    if (vdotN > 1.0f)
        return 1.0f;

    float tanthetav = sqrt((1.0f) - (vdotN * vdotN)) / (vdotN);
    
    //if (tanthetav == 0.0f)
        if (abs(tanthetav) < 0.0001f)
        return 1.0f;

    if ((vdotm / vdotN) <= 0.0f)
        return 0.0f;

    float alphag = ConvertAlphaFromPhong(alpha);


    if (isnan(2.0f / (1.0f + sqrt(1.0f + alphag * alphag * tanthetav * tanthetav))))
        std::cout << "bug" << std::endl;

    if (isinf(2.0f / (1.0f + sqrt(1.0f + alphag * alphag * tanthetav * tanthetav))))
        std::cout << "bug" << std::endl;


    return 2.0f/(1.0f + sqrt(1.0f + alphag * alphag * tanthetav * tanthetav));
}

float Material::CalcG1Beckman(const Vector3f& v, const Vector3f& m, const Vector3f& N) const
{
    float vdotm = v.dot(m);
    float vdotN = v.dot(N);

    if (vdotN > 1.0f)
        return 1.0f;

    float tanthetav = sqrt((1.0f) - (vdotN * vdotN)) / (vdotN);

    // may use epsilon?
    //if (tanthetav == 0.0f)
        if (abs(tanthetav) < 0.0001f)
        return 1.0f;

    // beckman
    float alphab = ConvertAlphaFromPhong(alpha);


    float a = 1.0f/(alphab * tanthetav);

    float afactor = 1.0f;

    if (a < 1.6f)
    {
        afactor = (3.535f * a + 2.181f * a * a) / (1.0f + 2.276f * a + 2.577f * a * a);
    }

    if ((vdotm / vdotN) <= 0.0f)
        return 0.0f;

    //return GetCharacteristicFactor(vdotm/vdotN) * afactor;
    return afactor;
}

float Material::CalcGterm(const Vector3f& omegai, const Vector3f& omegao, const Vector3f& m, const Vector3f& N) const
{
    return CalcG1(omegai, m, N) * CalcG1(omegao, m, N);
}

float Material::CalcGtermGGX(const Vector3f& omegai, const Vector3f& omegao, const Vector3f& m, const Vector3f& N) const
{
    return CalcG1GGX(omegai, m, N) * CalcG1GGX(omegao, m, N);
}

float Material::CalcGtermBeckman(const Vector3f& omegai, const Vector3f& omegao, const Vector3f& m, const Vector3f& N) const
{
    return CalcG1Beckman(omegai, m, N) * CalcG1Beckman(omegao, m, N);
}

float Material::Calcpd() const
{
    return Kd.norm() / (Kd.norm() + Ks.norm() + Kt.norm());
    //return 0.9f;
}

float Material::Calcpr() const
{
    return Ks.norm() / (Kd.norm() + Ks.norm() + Kt.norm());
    //return 0.09f;
}

float Material::Calcpt() const
{
    return Kt.norm() / (Kd.norm() + Ks.norm() + Kt.norm());
    //return 0.01f;
}

float Material::ConvertAlphaFromPhong(float a) const
{
    return sqrt(2.0f/(alpha + 2.0f));
}

float Material::CalcIndexofRefraction(const Vector3f& omegao, const Vector3f& N, float& etai, float& etao) const
{
    float omegaoDotN = omegao.dot(N);

    if (omegaoDotN > 0.0f)
    {
        etai = 1.0f;
        etao = ior;

        return etai / etao;
    }
    else if (omegaoDotN < 0.0f)
    {
        etai = ior;
        etao = 1.0f;

        return etai / etao;
    }

    return etai/etao;
}

float Material::sign(float x) const
{
    return (x >= 0.0f) ? 1.0f : -1.0f;
}

float Material::TestRadicand(float eta, const Vector3f& omegao, const Vector3f& m) const
{
    float omegaoDotM = omegao.dot(m);

    return 1.0f - (eta * eta) * (1.0f - omegaoDotM * omegaoDotM);
}

Color Material::CalcA(const Vector3f& omegao, const Vector3f& N, float t) const
{
    float Ktr = Kt.x();
    float Ktg = Kt.y();
    float Ktb = Kt.z();



    if (omegao.dot(N) < 0.0f)
    {
        return Vector3f(exp(t * log(Ktr)), exp(t * log(Ktg)), exp(t * log(Ktb)));
    }
    else
    {
        return Vector3f(1.0f, 1.0f, 1.0f);
    }
}

Color EvalRadiance(const Intersection& Q)
{
    return Q._pObject->_pMaterial->Kd;
}

Color CheckColorValue(const Color& c)
{
    if (isnan(c.x()))
        return Color(0.0f, 0.0f, 0.0f);
    if (isinf(c.x()))
        return Color(0.0f, 0.0f, 0.0f);
    if (isnan(c.y()))
        return Color(0.0f, 0.0f, 0.0f);
    if (isinf(c.y()))
        return Color(0.0f, 0.0f, 0.0f);
    if (isnan(c.z()))
        return Color(0.0f, 0.0f, 0.0f);
    if (isinf(c.z()))
        return Color(0.0f, 0.0f, 0.0f);

    return c;
}

