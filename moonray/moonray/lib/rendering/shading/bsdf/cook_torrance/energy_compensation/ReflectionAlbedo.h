// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

///
/// @file ReflectionAlbedo.h
/// $Id$
///

/*
 * This file encodes the average directional albedo and
 * the normalization factor required for the Cook-Torrance Microfacet BRDF.
 * As of 10/14/19, we provide tables for Beckmann and GGX NDFs.
 * This class provides accessors to encapsulate the
 * indexing into the albedo table. Values can be looked up as:
 * E(cosThetaO, roughness)
 * oneMinusEavg(roughness)
 * These tables have been calculated using moonshine_devtools/compute_albedo
 * Ref:
 * A Microfacet Based Coupled Specular-Matte BRDF Model, Kelemen'01
 *
 */

#include <scene_rdl2/common/math/MathUtil.h>
#include <moonray/rendering/shading/ispc/BsdfComponent_ispc_stubs.h>

#pragma once

namespace moonray {
namespace shading {

class ReflectionAlbedo
{
public:
    static float E(ispc::MicrofacetDistribution type,
                   float cosTheta,
                   float roughness)
    {
        float w;
        size_t iLow, iHigh;
        index(cosTheta, roughness,
              w, iLow, iHigh);
        float e = 0.0f;
        switch (type) {
        case ispc::MICROFACET_DISTRIBUTION_BECKMANN:
            e = scene_rdl2::math::lerp(mBeckmannE[iLow],
                     mBeckmannE[iHigh],
                     w);
            break;
        case ispc::MICROFACET_DISTRIBUTION_GGX:
            e = scene_rdl2::math::lerp(mGGXE[iLow],
                     mGGXE[iHigh],
                     w);
            break;
        }
        return e;
    }

    static float oneMinusAvg(ispc::MicrofacetDistribution type,
                             float roughness)
    {
        float w;
        const size_t iLow = rIndex(roughness, w);
        const size_t iHigh = scene_rdl2::math::min(iLow+1, COMP-1);
        float e = 0.0f;
        switch (type) {
        case ispc::MICROFACET_DISTRIBUTION_BECKMANN:
            e = scene_rdl2::math::lerp(mBeckmannOneMinusEavg[iLow],
                     mBeckmannOneMinusEavg[iHigh],
                     w);
            break;
        case ispc::MICROFACET_DISTRIBUTION_GGX:
            e = scene_rdl2::math::lerp(mGGXOneMinusEavg[iLow],
                     mGGXOneMinusEavg[iHigh],
                     w);
            break;
        }
        return e;
    }

private:
    static void index(float cosTheta,
                        float roughness,
                        float& w,
                        size_t& iLow,
                        size_t& iHigh)
    {
        const size_t lIndex = rIndex(roughness, w);
        const size_t hIndex = scene_rdl2::math::min(lIndex+1, COMP*COMP-1);
        const float if1 = lIndex * COMP +
                  cosTheta * (COMP - 1);
        const float if2 = hIndex * COMP +
                  cosTheta * (COMP - 1);
        // Take cosTheta's ceiling for the index
        iLow = static_cast<size_t>(ceil(if1));
        iHigh = static_cast<size_t>(ceil(if2));
    }

    static size_t rIndex(float roughness,
                         float& weight)
    {
        const float i = roughness * (COMP - 1);
        const float indexI = floor(i);
        weight = (i-indexI);
        return static_cast<size_t>(indexI);
    }
    static const size_t COMP = 33;
    static const float mBeckmannE[COMP*COMP];
    static const float mGGXE[COMP*COMP];
    static const float mBeckmannOneMinusEavg[COMP];
    static const float mGGXOneMinusEavg[COMP];
};

// Table calculated using moonshine_devtools/compute_albedo
// For the microfacet reflection lobe using Beckmann and Smith
// no fresnel
const float ReflectionAlbedo::mBeckmannE[COMP*COMP] = {
    0.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 0.99780f, 1.00000f, 1.00000f, 1.00000f, 0.99920f, 0.99644f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 0.99789f, 1.00000f, 0.99721f, 0.99592f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.95384f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.92372f, 0.97904f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.91041f, 0.94333f, 0.98162f, 0.99952f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.91129f, 0.92556f, 0.95107f, 0.97892f, 0.99694f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.91444f, 0.91255f, 0.93227f, 0.95312f, 0.97481f, 0.98993f, 0.99849f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.92211f, 0.90971f, 0.92234f, 0.93333f, 0.95325f, 0.96955f, 0.98484f, 0.99377f, 0.99906f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.92105f, 0.90951f, 0.90438f, 0.91948f, 0.93266f, 0.95063f, 0.96493f, 0.97624f, 0.98773f, 0.99243f, 0.99771f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.93319f, 0.90727f, 0.90564f, 0.91429f, 0.92192f, 0.93309f, 0.94482f, 0.95808f, 0.96911f, 0.98118f, 0.98649f, 0.99368f, 0.99612f, 0.99970f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.93668f, 0.92183f, 0.90433f, 0.90694f, 0.91307f, 0.92097f, 0.92911f, 0.93944f, 0.94866f, 0.96369f, 0.97152f, 0.97981f, 0.98739f, 0.99202f, 0.99612f, 0.99842f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.94068f, 0.92066f, 0.90915f, 0.90752f, 0.90666f, 0.91034f, 0.91983f, 0.92714f, 0.93627f, 0.94461f, 0.95583f, 0.96388f, 0.97285f, 0.97897f, 0.98508f, 0.99061f, 0.99297f, 0.99718f, 0.99822f, 0.99968f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 0.99989f, 1.00000f,
    0.00000f, 0.93685f, 0.92307f, 0.91332f, 0.90610f, 0.90211f, 0.90634f, 0.90993f, 0.91704f, 0.92289f, 0.93010f, 0.93981f, 0.94592f, 0.95523f, 0.96451f, 0.97098f, 0.97817f, 0.98269f, 0.98884f, 0.99313f, 0.99535f, 0.99652f, 0.99917f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 0.99977f, 0.99991f, 1.00000f,
    0.00000f, 0.94354f, 0.93049f, 0.91650f, 0.90942f, 0.90444f, 0.90122f, 0.90528f, 0.90767f, 0.91449f, 0.91912f, 0.92378f, 0.93187f, 0.93836f, 0.95003f, 0.95643f, 0.96406f, 0.97037f, 0.97697f, 0.98229f, 0.98645f, 0.98919f, 0.99455f, 0.99548f, 0.99795f, 0.99904f, 0.99946f, 1.00000f, 1.00000f, 1.00000f, 0.99983f, 0.99983f, 1.00000f,
    0.00000f, 0.92864f, 0.92993f, 0.91421f, 0.90439f, 0.90278f, 0.89667f, 0.89745f, 0.90254f, 0.90401f, 0.91168f, 0.91447f, 0.91952f, 0.92479f, 0.93186f, 0.93970f, 0.94794f, 0.95461f, 0.96055f, 0.96701f, 0.97568f, 0.98037f, 0.98558f, 0.98906f, 0.99204f, 0.99525f, 0.99706f, 0.99882f, 0.99896f, 0.99982f, 0.99949f, 1.00000f, 1.00000f,
    0.00000f, 0.94134f, 0.93047f, 0.92460f, 0.91805f, 0.90357f, 0.89555f, 0.89539f, 0.90060f, 0.89829f, 0.89897f, 0.90588f, 0.90864f, 0.91491f, 0.91867f, 0.92284f, 0.92984f, 0.93915f, 0.94173f, 0.95283f, 0.96038f, 0.96478f, 0.97284f, 0.97816f, 0.98285f, 0.98730f, 0.99203f, 0.99438f, 0.99703f, 0.99840f, 0.99906f, 1.00000f, 1.00000f,
    0.00000f, 0.93970f, 0.93186f, 0.91736f, 0.91642f, 0.90312f, 0.90217f, 0.89881f, 0.88999f, 0.89109f, 0.89861f, 0.89683f, 0.90024f, 0.90476f, 0.90548f, 0.90946f, 0.91602f, 0.92163f, 0.92532f, 0.93427f, 0.94093f, 0.95076f, 0.95650f, 0.96516f, 0.96969f, 0.97509f, 0.98103f, 0.98638f, 0.99034f, 0.99415f, 0.99703f, 0.99893f, 0.99999f,
    0.00000f, 0.93786f, 0.93542f, 0.92989f, 0.91328f, 0.90596f, 0.90439f, 0.89262f, 0.89356f, 0.89447f, 0.88988f, 0.89292f, 0.89436f, 0.88992f, 0.89722f, 0.90024f, 0.90081f, 0.90797f, 0.91038f, 0.91756f, 0.92515f, 0.93182f, 0.94015f, 0.94533f, 0.95353f, 0.96023f, 0.96793f, 0.97464f, 0.98132f, 0.98622f, 0.99183f, 0.99613f, 0.99949f,
    0.00000f, 0.94443f, 0.93077f, 0.92609f, 0.91117f, 0.90976f, 0.90006f, 0.90093f, 0.88895f, 0.88555f, 0.88430f, 0.88049f, 0.88549f, 0.88704f, 0.88626f, 0.88682f, 0.89273f, 0.89344f, 0.89761f, 0.90479f, 0.90890f, 0.91386f, 0.91942f, 0.92718f, 0.93375f, 0.94233f, 0.94941f, 0.95794f, 0.96411f, 0.97350f, 0.98190f, 0.98942f, 0.99695f,
    0.00000f, 0.93508f, 0.93540f, 0.92171f, 0.91698f, 0.90640f, 0.89434f, 0.89166f, 0.89003f, 0.88416f, 0.88173f, 0.87728f, 0.88032f, 0.87905f, 0.88074f, 0.88275f, 0.88495f, 0.88196f, 0.88492f, 0.88593f, 0.88965f, 0.89366f, 0.90142f, 0.90646f, 0.91413f, 0.92338f, 0.92900f, 0.93720f, 0.94604f, 0.95660f, 0.96626f, 0.97628f, 0.98898f,
    0.00000f, 0.94417f, 0.93808f, 0.92522f, 0.91873f, 0.91110f, 0.89636f, 0.89362f, 0.89040f, 0.88969f, 0.88624f, 0.87711f, 0.87650f, 0.87487f, 0.87396f, 0.87030f, 0.86887f, 0.87312f, 0.87215f, 0.87263f, 0.87421f, 0.87836f, 0.88164f, 0.88744f, 0.89232f, 0.89919f, 0.90606f, 0.91468f, 0.92376f, 0.93345f, 0.94327f, 0.95530f, 0.97056f,
    0.00000f, 0.93061f, 0.93999f, 0.92784f, 0.91504f, 0.90673f, 0.89685f, 0.89212f, 0.88725f, 0.87726f, 0.87727f, 0.86730f, 0.86857f, 0.86801f, 0.86327f, 0.86186f, 0.86301f, 0.85704f, 0.85903f, 0.85987f, 0.86322f, 0.86245f, 0.86653f, 0.86854f, 0.87137f, 0.87570f, 0.88304f, 0.88836f, 0.89629f, 0.90842f, 0.91498f, 0.92872f, 0.94125f,
    0.00000f, 0.94020f, 0.92956f, 0.92790f, 0.91331f, 0.90878f, 0.89462f, 0.89373f, 0.88645f, 0.87900f, 0.88045f, 0.86844f, 0.86406f, 0.86277f, 0.85831f, 0.85669f, 0.85060f, 0.84845f, 0.84685f, 0.84478f, 0.84610f, 0.84417f, 0.84707f, 0.84441f, 0.84812f, 0.85063f, 0.85570f, 0.86002f, 0.86959f, 0.87359f, 0.87973f, 0.89003f, 0.90187f,
    0.00000f, 0.93565f, 0.92840f, 0.92265f, 0.91277f, 0.90805f, 0.90266f, 0.89373f, 0.88649f, 0.88348f, 0.86852f, 0.86962f, 0.86407f, 0.85354f, 0.85201f, 0.84430f, 0.84254f, 0.84003f, 0.83880f, 0.83396f, 0.83359f, 0.82894f, 0.82758f, 0.83039f, 0.82446f, 0.82401f, 0.82796f, 0.83160f, 0.83134f, 0.83877f, 0.84109f, 0.84834f, 0.85321f,
    0.00000f, 0.93583f, 0.92829f, 0.92043f, 0.91376f, 0.90027f, 0.90277f, 0.88562f, 0.87706f, 0.87212f, 0.87339f, 0.86195f, 0.85319f, 0.84522f, 0.84761f, 0.83767f, 0.83249f, 0.83167f, 0.82650f, 0.82197f, 0.81516f, 0.80997f, 0.81149f, 0.80556f, 0.80448f, 0.80116f, 0.79751f, 0.79893f, 0.79723f, 0.79686f, 0.79696f, 0.79587f, 0.80117f,
    0.00000f, 0.93485f, 0.92132f, 0.91838f, 0.91080f, 0.90168f, 0.89926f, 0.88129f, 0.88158f, 0.87115f, 0.86300f, 0.85717f, 0.84708f, 0.84031f, 0.83737f, 0.83160f, 0.82348f, 0.81770f, 0.81402f, 0.80412f, 0.80176f, 0.79349f, 0.79124f, 0.78569f, 0.77854f, 0.77212f, 0.76819f, 0.75935f, 0.75692f, 0.75453f, 0.74926f, 0.74598f, 0.74344f,
    0.00000f, 0.92745f, 0.92349f, 0.91308f, 0.90742f, 0.90013f, 0.88996f, 0.88032f, 0.87345f, 0.86447f, 0.85764f, 0.85116f, 0.84721f, 0.83343f, 0.82514f, 0.81702f, 0.81552f, 0.80746f, 0.79830f, 0.78783f, 0.77908f, 0.77524f, 0.76946f, 0.75850f, 0.75182f, 0.74300f, 0.73469f, 0.72574f, 0.71566f, 0.70887f, 0.69941f, 0.69119f, 0.68411f,
    0.00000f, 0.92318f, 0.91646f, 0.91232f, 0.89345f, 0.90016f, 0.89156f, 0.87970f, 0.86990f, 0.86053f, 0.85274f, 0.84124f, 0.83532f, 0.81901f, 0.81431f, 0.80664f, 0.80004f, 0.78883f, 0.78104f, 0.76692f, 0.76395f, 0.75487f, 0.74615f, 0.73551f, 0.72453f, 0.71371f, 0.69621f, 0.68780f, 0.67518f, 0.65975f, 0.64748f, 0.63431f, 0.62765f,
    0.00000f, 0.91507f, 0.91887f, 0.90499f, 0.90142f, 0.88454f, 0.88050f, 0.87180f, 0.86054f, 0.84918f, 0.84150f, 0.83514f, 0.82115f, 0.81438f, 0.80240f, 0.79440f, 0.78172f, 0.77451f, 0.76345f, 0.75091f, 0.74144f, 0.73153f, 0.71656f, 0.70717f, 0.69353f, 0.67631f, 0.65776f, 0.64283f, 0.63075f, 0.61418f, 0.59615f, 0.58340f, 0.57106f,
    0.00000f, 0.90244f, 0.89541f, 0.89880f, 0.88588f, 0.87825f, 0.87301f, 0.86399f, 0.85419f, 0.84005f, 0.82768f, 0.82092f, 0.80912f, 0.79819f, 0.78739f, 0.77739f, 0.76942f, 0.75860f, 0.74301f, 0.73097f, 0.71739f, 0.70185f, 0.68879f, 0.67060f, 0.66043f, 0.64048f, 0.62229f, 0.60280f, 0.58766f, 0.56803f, 0.54814f, 0.53070f, 0.51701f,
    0.00000f, 0.90428f, 0.89554f, 0.89357f, 0.88006f, 0.87192f, 0.85875f, 0.85599f, 0.83462f, 0.83299f, 0.81917f, 0.80572f, 0.79145f, 0.78616f, 0.77224f, 0.75349f, 0.74356f, 0.73141f, 0.71580f, 0.70483f, 0.69308f, 0.67259f, 0.65518f, 0.63796f, 0.62117f, 0.60444f, 0.58483f, 0.56250f, 0.54098f, 0.51956f, 0.50107f, 0.48495f, 0.46446f,
};

// Table calculated using moonshine_devtools/compute_albedo
// For the microfacet reflection lobe using Beckmann and Smith
// no fresnel
const float ReflectionAlbedo::mGGXE[COMP*COMP] = {
    0.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 0.99973f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.98761f, 0.99844f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.94614f, 0.98345f, 0.99383f, 0.99774f, 0.99862f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f,
    0.00000f, 0.88296f, 0.95154f, 0.97741f, 0.98698f, 0.99329f, 0.99560f, 0.99787f, 0.99856f, 0.99907f, 0.99912f, 0.99983f, 0.99985f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 1.00000f, 0.99998f, 1.00000f, 1.00000f, 0.99994f, 0.99995f, 1.00000f, 1.00000f,
    0.00000f, 0.84597f, 0.91149f, 0.94714f, 0.96925f, 0.97859f, 0.98610f, 0.98971f, 0.99274f, 0.99481f, 0.99585f, 0.99671f, 0.99732f, 0.99842f, 0.99883f, 0.99942f, 0.99962f, 1.00000f, 0.99968f, 0.99981f, 0.99973f, 0.99970f, 0.99984f, 1.00000f, 0.99986f, 1.00000f, 0.99993f, 1.00000f, 1.00000f, 0.99991f, 0.99991f, 1.00000f, 1.00000f,
    0.00000f, 0.82443f, 0.87043f, 0.91170f, 0.94271f, 0.95931f, 0.97118f, 0.97902f, 0.98358f, 0.98730f, 0.98961f, 0.99244f, 0.99369f, 0.99455f, 0.99565f, 0.99692f, 0.99741f, 0.99859f, 0.99847f, 0.99842f, 0.99846f, 0.99944f, 0.99950f, 0.99914f, 0.99989f, 0.99956f, 0.99948f, 0.99963f, 0.99980f, 0.99985f, 0.99987f, 0.99995f, 1.00000f,
    0.00000f, 0.81909f, 0.84686f, 0.87943f, 0.91015f, 0.93712f, 0.94963f, 0.96091f, 0.97019f, 0.97587f, 0.97949f, 0.98371f, 0.98665f, 0.98780f, 0.99133f, 0.99099f, 0.99312f, 0.99411f, 0.99548f, 0.99646f, 0.99642f, 0.99743f, 0.99795f, 0.99853f, 0.99851f, 0.99864f, 0.99908f, 0.99935f, 0.99960f, 0.99970f, 0.99966f, 0.99979f, 1.00000f,
    0.00000f, 0.81940f, 0.83133f, 0.85457f, 0.88055f, 0.90547f, 0.92506f, 0.94044f, 0.95087f, 0.96057f, 0.96824f, 0.97366f, 0.97621f, 0.98011f, 0.98389f, 0.98515f, 0.98709f, 0.98951f, 0.98994f, 0.99214f, 0.99377f, 0.99446f, 0.99472f, 0.99582f, 0.99662f, 0.99759f, 0.99793f, 0.99797f, 0.99862f, 0.99911f, 0.99936f, 0.99971f, 0.99999f,
    0.00000f, 0.81852f, 0.82207f, 0.84101f, 0.85853f, 0.88006f, 0.89833f, 0.91658f, 0.93062f, 0.94162f, 0.95122f, 0.95779f, 0.96298f, 0.96910f, 0.97237f, 0.97640f, 0.97846f, 0.98202f, 0.98347f, 0.98601f, 0.98847f, 0.98945f, 0.99104f, 0.99123f, 0.99235f, 0.99427f, 0.99492f, 0.99623f, 0.99635f, 0.99717f, 0.99845f, 0.99927f, 0.99987f,
    0.00000f, 0.81056f, 0.81921f, 0.82164f, 0.83990f, 0.85764f, 0.87696f, 0.89427f, 0.90621f, 0.92080f, 0.92926f, 0.93960f, 0.94700f, 0.95468f, 0.95994f, 0.96421f, 0.96772f, 0.97182f, 0.97591f, 0.97717f, 0.98062f, 0.98194f, 0.98596f, 0.98543f, 0.98798f, 0.98955f, 0.98995f, 0.99142f, 0.99316f, 0.99433f, 0.99572f, 0.99638f, 0.99888f,
    0.00000f, 0.81708f, 0.80955f, 0.82100f, 0.83394f, 0.84099f, 0.85746f, 0.87065f, 0.88423f, 0.89606f, 0.91007f, 0.91762f, 0.92957f, 0.93552f, 0.94409f, 0.95064f, 0.95448f, 0.95949f, 0.96415f, 0.96644f, 0.97060f, 0.97493f, 0.97686f, 0.98006f, 0.97990f, 0.98337f, 0.98545f, 0.98561f, 0.98822f, 0.98873f, 0.99055f, 0.99229f, 0.99330f,
    0.00000f, 0.81980f, 0.81689f, 0.81155f, 0.81990f, 0.82872f, 0.84109f, 0.85469f, 0.86425f, 0.87514f, 0.89072f, 0.89771f, 0.90658f, 0.91800f, 0.92481f, 0.93194f, 0.93736f, 0.94514f, 0.94726f, 0.95272f, 0.95793f, 0.96077f, 0.96522f, 0.96849f, 0.97014f, 0.97460f, 0.97418f, 0.97755f, 0.97983f, 0.98151f, 0.98301f, 0.98415f, 0.98555f,
    0.00000f, 0.81977f, 0.81467f, 0.80953f, 0.81807f, 0.81888f, 0.82653f, 0.83930f, 0.84579f, 0.85937f, 0.86825f, 0.87985f, 0.88718f, 0.89825f, 0.90482f, 0.91187f, 0.91933f, 0.92432f, 0.93264f, 0.93670f, 0.94186f, 0.94845f, 0.95070f, 0.95438f, 0.95833f, 0.96062f, 0.96456f, 0.96594f, 0.96792f, 0.96998f, 0.97067f, 0.97336f, 0.97642f,
    0.00000f, 0.81589f, 0.81562f, 0.81469f, 0.81091f, 0.81523f, 0.81912f, 0.82740f, 0.83571f, 0.84018f, 0.84979f, 0.86101f, 0.86549f, 0.87400f, 0.88495f, 0.89155f, 0.89880f, 0.90404f, 0.91182f, 0.91991f, 0.92380f, 0.92771f, 0.93334f, 0.93759f, 0.94271f, 0.94448f, 0.94692f, 0.95112f, 0.95267f, 0.95611f, 0.95690f, 0.95981f, 0.96251f,
    0.00000f, 0.80940f, 0.81600f, 0.80984f, 0.81076f, 0.81157f, 0.80872f, 0.81690f, 0.82149f, 0.82658f, 0.83418f, 0.84114f, 0.84700f, 0.85501f, 0.86712f, 0.87225f, 0.87875f, 0.88429f, 0.89045f, 0.89667f, 0.90018f, 0.90386f, 0.91380f, 0.91519f, 0.92033f, 0.92348f, 0.92717f, 0.93144f, 0.93494f, 0.93678f, 0.93970f, 0.94271f, 0.94510f,
    0.00000f, 0.79618f, 0.80937f, 0.80415f, 0.79721f, 0.80166f, 0.79920f, 0.80071f, 0.81039f, 0.80863f, 0.82129f, 0.82455f, 0.83292f, 0.83717f, 0.84267f, 0.84905f, 0.85740f, 0.86205f, 0.86603f, 0.87028f, 0.88032f, 0.88291f, 0.88816f, 0.89247f, 0.89554f, 0.90054f, 0.90267f, 0.90923f, 0.91183f, 0.91480f, 0.91784f, 0.92144f, 0.92473f,
    0.00000f, 0.80315f, 0.80604f, 0.80839f, 0.80267f, 0.79861f, 0.79089f, 0.79528f, 0.79913f, 0.80045f, 0.80207f, 0.80929f, 0.81108f, 0.81849f, 0.82287f, 0.82485f, 0.83176f, 0.83890f, 0.83847f, 0.84599f, 0.85266f, 0.85466f, 0.86113f, 0.86599f, 0.86901f, 0.87355f, 0.87945f, 0.88219f, 0.88649f, 0.88850f, 0.89240f, 0.89830f, 0.90000f,
    0.00000f, 0.80230f, 0.79895f, 0.79135f, 0.79422f, 0.78700f, 0.79023f, 0.79063f, 0.78080f, 0.78562f, 0.79283f, 0.79040f, 0.79416f, 0.79855f, 0.79707f, 0.80179f, 0.80706f, 0.80915f, 0.81104f, 0.81649f, 0.82002f, 0.82850f, 0.83133f, 0.83779f, 0.84002f, 0.84317f, 0.84668f, 0.85204f, 0.85574f, 0.85983f, 0.86406f, 0.86806f, 0.87235f,
    0.00000f, 0.79026f, 0.79314f, 0.79421f, 0.78381f, 0.77967f, 0.78322f, 0.77571f, 0.77575f, 0.77544f, 0.77409f, 0.77701f, 0.77621f, 0.77064f, 0.77857f, 0.77911f, 0.77844f, 0.78388f, 0.78345f, 0.78787f, 0.79328f, 0.79704f, 0.80288f, 0.80281f, 0.80888f, 0.81075f, 0.81566f, 0.82008f, 0.82339f, 0.82698f, 0.83140f, 0.83464f, 0.84101f,
    0.00000f, 0.79456f, 0.77825f, 0.78117f, 0.76967f, 0.77222f, 0.76681f, 0.76810f, 0.75915f, 0.75655f, 0.75593f, 0.75082f, 0.75475f, 0.75334f, 0.75260f, 0.75119f, 0.75658f, 0.75495f, 0.75839f, 0.76323f, 0.76352f, 0.76476f, 0.76800f, 0.77093f, 0.77293f, 0.77788f, 0.78104f, 0.78445f, 0.78457f, 0.79135f, 0.79406f, 0.79693f, 0.80436f,
    0.00000f, 0.77637f, 0.78047f, 0.76689f, 0.76454f, 0.75667f, 0.74993f, 0.74898f, 0.74611f, 0.74137f, 0.73801f, 0.73129f, 0.73211f, 0.73118f, 0.73116f, 0.73060f, 0.73121f, 0.72626f, 0.72914f, 0.72842f, 0.72898f, 0.73155f, 0.73517f, 0.73574f, 0.73872f, 0.74250f, 0.74372f, 0.74711f, 0.74888f, 0.75233f, 0.75765f, 0.76122f, 0.76365f,
    0.00000f, 0.76913f, 0.76589f, 0.75467f, 0.75029f, 0.74578f, 0.73398f, 0.73108f, 0.72921f, 0.72843f, 0.72415f, 0.71549f, 0.71225f, 0.70742f, 0.70646f, 0.70149f, 0.69833f, 0.69938f, 0.69923f, 0.69750f, 0.69774f, 0.69767f, 0.69843f, 0.70156f, 0.70247f, 0.70421f, 0.70648f, 0.70774f, 0.71180f, 0.71292f, 0.71446f, 0.71795f, 0.72179f,
    0.00000f, 0.74800f, 0.75308f, 0.74227f, 0.73256f, 0.72815f, 0.71876f, 0.71196f, 0.70905f, 0.69855f, 0.69791f, 0.68587f, 0.68442f, 0.68213f, 0.67788f, 0.67384f, 0.67382f, 0.66757f, 0.66716f, 0.66585f, 0.66758f, 0.66532f, 0.66780f, 0.66658f, 0.66625f, 0.66679f, 0.66683f, 0.66776f, 0.66835f, 0.67282f, 0.67216f, 0.67736f, 0.67853f,
    0.00000f, 0.74344f, 0.73417f, 0.72677f, 0.71498f, 0.71223f, 0.69738f, 0.69498f, 0.68638f, 0.68043f, 0.67752f, 0.66636f, 0.65939f, 0.65837f, 0.65347f, 0.64829f, 0.64052f, 0.63946f, 0.63489f, 0.63317f, 0.63236f, 0.63001f, 0.63092f, 0.62790f, 0.62696f, 0.62699f, 0.62868f, 0.62644f, 0.63019f, 0.62756f, 0.62844f, 0.62970f, 0.63288f,
    0.00000f, 0.72171f, 0.71312f, 0.70971f, 0.69773f, 0.69304f, 0.68404f, 0.67581f, 0.66822f, 0.66215f, 0.64643f, 0.64533f, 0.63952f, 0.62919f, 0.62571f, 0.61657f, 0.61287f, 0.61163f, 0.60659f, 0.60043f, 0.60056f, 0.59613f, 0.59314f, 0.59531f, 0.59097f, 0.58653f, 0.58887f, 0.58707f, 0.58312f, 0.58693f, 0.58313f, 0.58769f, 0.58768f,
    0.00000f, 0.70441f, 0.69851f, 0.68953f, 0.67907f, 0.66470f, 0.66519f, 0.64720f, 0.63870f, 0.62979f, 0.62773f, 0.61570f, 0.60607f, 0.60026f, 0.59848f, 0.59096f, 0.58486f, 0.58225f, 0.57697f, 0.57144f, 0.56465f, 0.56027f, 0.56087f, 0.55532f, 0.55602f, 0.55249f, 0.54832f, 0.54709f, 0.54598f, 0.54351f, 0.54184f, 0.54134f, 0.54273f,
    0.00000f, 0.68966f, 0.67571f, 0.66732f, 0.65594f, 0.64478f, 0.63961f, 0.62358f, 0.61918f, 0.60635f, 0.59775f, 0.59020f, 0.57982f, 0.57364f, 0.56813f, 0.56074f, 0.55415f, 0.54887f, 0.54446f, 0.53826f, 0.53434f, 0.52649f, 0.52505f, 0.52210f, 0.51579f, 0.51277f, 0.51195f, 0.50514f, 0.50502f, 0.50326f, 0.49879f, 0.49953f, 0.49877f,
    0.00000f, 0.66681f, 0.65937f, 0.64702f, 0.63533f, 0.62367f, 0.61175f, 0.59841f, 0.59082f, 0.57918f, 0.57191f, 0.56382f, 0.55842f, 0.54560f, 0.53646f, 0.52800f, 0.52479f, 0.52077f, 0.51320f, 0.50320f, 0.49712f, 0.49475f, 0.49148f, 0.48407f, 0.48096f, 0.47871f, 0.47474f, 0.46980f, 0.46506f, 0.46342f, 0.45971f, 0.45802f, 0.45606f,
    0.00000f, 0.64732f, 0.63242f, 0.62413f, 0.60357f, 0.60198f, 0.59127f, 0.57660f, 0.56489f, 0.55434f, 0.54392f, 0.53386f, 0.52663f, 0.51371f, 0.50742f, 0.49963f, 0.49469f, 0.48512f, 0.47991f, 0.47011f, 0.46808f, 0.46357f, 0.45838f, 0.45362f, 0.44792f, 0.44437f, 0.43621f, 0.43344f, 0.42984f, 0.42421f, 0.42137f, 0.41664f, 0.41707f,
    0.00000f, 0.62252f, 0.61498f, 0.59817f, 0.58823f, 0.56956f, 0.56126f, 0.54888f, 0.53585f, 0.52425f, 0.51400f, 0.50707f, 0.49569f, 0.48895f, 0.47918f, 0.47300f, 0.46269f, 0.45649f, 0.45106f, 0.44037f, 0.43635f, 0.43121f, 0.42357f, 0.41911f, 0.41321f, 0.40625f, 0.39953f, 0.39712f, 0.39427f, 0.38964f, 0.38337f, 0.38152f, 0.37943f,
    0.00000f, 0.59474f, 0.57969f, 0.57584f, 0.55757f, 0.54637f, 0.53605f, 0.52271f, 0.51127f, 0.49754f, 0.48528f, 0.47783f, 0.46801f, 0.45806f, 0.45006f, 0.44208f, 0.43486f, 0.42926f, 0.41969f, 0.41277f, 0.40500f, 0.39650f, 0.39280f, 0.38256f, 0.38112f, 0.37369f, 0.36920f, 0.36316f, 0.36168f, 0.35525f, 0.34964f, 0.34579f, 0.34303f,
    0.00000f, 0.57758f, 0.56217f, 0.55254f, 0.53528f, 0.52106f, 0.50656f, 0.49792f, 0.47830f, 0.47319f, 0.45925f, 0.44927f, 0.43787f, 0.43241f, 0.42233f, 0.41142f, 0.40254f, 0.39635f, 0.38720f, 0.38202f, 0.37763f, 0.36690f, 0.36002f, 0.35406f, 0.34894f, 0.34297f, 0.33854f, 0.33175f, 0.32604f, 0.32019f, 0.31625f, 0.31480f, 0.30903f,
};

// Data calculated using moonshine_devtools/compute_albedo
// For the microfacet reflection lobe using Beckmann and Smith
// no fresnel
const float ReflectionAlbedo::mBeckmannOneMinusEavg[COMP] = {
    0.00035f, 0.00000f, 0.00000f, 0.00000f, 0.00000f, 0.00013f, 0.00040f, 0.00082f, 0.00151f, 0.00244f, 0.00398f, 0.00585f, 0.00845f, 0.01183f, 0.01617f, 0.02135f, 0.02798f, 0.03557f, 0.04480f, 0.05489f, 0.06719f, 0.08088f, 0.09632f, 0.11434f, 0.13464f, 0.15585f, 0.17984f, 0.20522f, 0.23245f, 0.26100f, 0.29115f, 0.32208f, 0.35467f,
};

// Data calculated using moonshine_devtools/compute_albedo
// For the microfacet reflection lobe using Beckmann and Smith
// no fresnel
const float ReflectionAlbedo::mGGXOneMinusEavg[COMP] = {
    0.00000f, 0.00000f, 0.00000f, 0.00014f, 0.00075f, 0.00214f, 0.00443f, 0.00791f, 0.01247f, 0.01862f, 0.02647f, 0.03597f, 0.04817f, 0.06207f, 0.07844f, 0.09731f, 0.11854f, 0.14172f, 0.16804f, 0.19558f, 0.22590f, 0.25718f, 0.29005f, 0.32423f, 0.35949f, 0.39430f, 0.42952f, 0.46477f, 0.49911f, 0.53271f, 0.56573f, 0.59744f, 0.62837f,
};
//----------------------------------------------------------------------------

} // namespace shading
} // namespace moonray

