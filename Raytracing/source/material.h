#pragma once
#include "geom.h"
#include "intersection.h"
////////////////////////////////////////////////////////////////////////
// Material: encapsulates a BRDF and communication with a shader.
////////////////////////////////////////////////////////////////////////
class Material
{
public:
    // added Kt for as4
    Vector3f Kd, Ks, Kt;
    float alpha;
    unsigned int texid;
    // added ior for as4
    float ior;

    virtual bool isLight() { return false; }

    // modified ctors for as4
    Material() : Kd(Vector3f(1.0, 0.5, 0.0)), Ks(Vector3f(1, 1, 1)), Kt(Vector3f(0.0f, 0.0f, 0.0f)), alpha(1.0), texid(0), ior(1.0f) {}
    Material(const Vector3f d, const Vector3f s, const float a)
        : Kd(d), Ks(s), Kt(Vector3f(0.0f, 0.0f, 0.0f)), alpha(a), texid(0), ior(1.0f) {}
    Material(const Material& o) { Kd = o.Kd;  Ks = o.Ks;  Kt = o.Kt, alpha = o.alpha;  texid = o.texid, ior = o.ior; }

    // added for as4
    Material(const Vector3f d, const Vector3f s, const float a, const Vector3f t, const float ior)
        : Kd(d), Ks(s), Kt(t), alpha(a), texid(0), ior(ior) {}

    void setTexture(const std::string path);
    //virtual void apply(const unsigned int program);

    // assignment 2
    Vector3f SampleBrdf(const Vector3f& N) const;
    // assignment 3
    Vector3f SampleBrdf(const Vector3f& omegao, const Vector3f& N) const;

    // assignment 2
    float PdfBrdf(const Vector3f& N, const Vector3f& omegai) const;
    // assignment 3
    float PdfBrdf(const Vector3f& omegao, Vector3f& N, Vector3f& omegai) const;


    // assignment 2 simple diffuse
    Color EvalScattering(const Vector3f& N, const Vector3f& omegai) const;
    // assignment 3 full brdf
    Color EvalScattering(const Vector3f& omegao, const Vector3f& N, const Vector3f& omegai) const;
    Color EvalScattering(const Vector3f& omegao, const Vector3f& N, const Vector3f& omegai, float t) const;



private:
    Vector3f SampleLobe(const Vector3f& N, float c, float phi) const;

    // Characteristic factor function
    float GetCharacteristicFactor(float d) const;

    // F is the Fresnel (reflection) is usually approximated by Schlick as
    // F(d) = Ks + (1 - Ks)(1 - d) ^ 5
    Vector3f CalcFterm(float d) const;

    // D is the micro-facet distribution
    // tan(thetam) = sqrt(1.0 - (m dot N) ^2) / (m dot N) and
    // tan(thetav) = sqrt(1.0 - (v dot N) ^2) / (v dot N)
    float CalcDterm(const Vector3f& m, const Vector3f& N) const;

    float CalcDtermGGX(const Vector3f& m, const Vector3f& N) const;

    float CalcDtermBeckman(const Vector3f& m, const Vector3f& N) const;

    // G1(...) function
    float CalcG1(const Vector3f& v, const Vector3f& m, const Vector3f& N) const;
    float CalcG1GGX(const Vector3f& v, const Vector3f& m, const Vector3f& N) const;
    float CalcG1Beckman(const Vector3f& v, const Vector3f& m, const Vector3f& N) const;

    float CalcGterm(const Vector3f& omegai, const Vector3f& omegao, const Vector3f& m, const Vector3f& N) const;
    float CalcGtermGGX(const Vector3f& omegai, const Vector3f& omegao, const Vector3f& m, const Vector3f& N) const;
    float CalcGtermBeckman(const Vector3f& omegai, const Vector3f& omegao, const Vector3f& m, const Vector3f& N) const;

    // modified for as4
    float Calcpd() const;
    float Calcpr() const;
    // added for as4
    float Calcpt() const;
    float ConvertAlphaFromPhong(float a) const;
    // return etai/etao
    float CalcIndexofRefraction(const Vector3f& omegao, const Vector3f& N, float& etai, float& etao) const;
    // added for as4
    float sign(float x) const;
    // added for as4
    float TestRadicand(float eta, const Vector3f& omegao, const Vector3f& m) const;
    Color CalcA(const Vector3f& omegao, const Vector3f& N, float t) const;
};

////////////////////////////////////////////////////////////////////////
// Light: encapsulates a light and communiction with a shader.
////////////////////////////////////////////////////////////////////////
class Light : public Material
{
public:

    Light(const Vector3f e) : Material() { Kd = e; }
    virtual bool isLight() { return true; }

    
    //virtual void apply(const unsigned int program);
};

Color EvalRadiance(const Intersection& Q);

Color CheckColorValue(const Color& c);