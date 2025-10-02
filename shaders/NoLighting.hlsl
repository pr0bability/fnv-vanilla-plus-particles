// Soft particle shader.
// VS
// NOLIGHT000 - TEX
// NOLIGHT001 - VC
// NOLIGHT002 - TEX, VC
// NOLIGHT003 - Base
// NOLIGHT004 - TEX, FALLOFF
// NOLIGHT005 - VC, FALLOFF
// NOLIGHT006 - TEX, VC, FALLOFF
// NOLIGHT007 - FALLOFF
// NOLIGHT008 - TEX, SKIN
// NOLIGHT009 - VC, SKIN
// NOLIGHT010 - TEX, VC, SKIN
// NOLIGHT011 - SKIN
// NOLIGHT012 - TEX, FALLOFF, SKIN
// NOLIGHT013 - VC, FALLOFF, SKIN
// NOLIGHT014 - TEX, VC, FALLOFF, SKIN
// NOLIGHT015 - FALLOFF, SKIN
// NOLIGHT016 - PSYS
// NOLIGHT017 - PSYS, SUBTEX
// NOLIGHT018 - PSYS, FALLOFF
// NOLIGHT019 - PSYS, SUBTEX, FALLOFF
//
// PS
// NOLIGHTTEX - TEX
// NOLIGHTVCONLY - VC
// NOLIGHTTEXVC - TEX, VC
// NOLIGHTTEXVCPMA - TEX, VC, PREMULT_ALPHA
// NOLIGHTMAT - Base
// NOLIGHTTEXSOFT - TEX, SOFT
// NOLIGHTVCONLYSOFT - VC, SOFT
// NOLIGHTTEXVCSOFT - TEX, VC, SOFT
// NOLIGHTTEXVCPMASOFT - TEX, VC, PREMULT_ALPHA, SOFT
// NOLIGHTMATSOFT - SOFT

#ifdef __INTELLISENSE__
    #define PS
#endif

#ifdef PSYS
    #define TEX
    #define VC
#endif

#if defined(SKIN)
    #include "includes/Skinned.hlsli"
#endif

// Shader inputs and outputs.

struct VertexData {
    float4 position : POSITION;
#if defined(TEX) || defined(SUBTEX)
    float4 uv : TEXCOORD0;
#endif
#if defined(VC)
    float4 vertexColor : COLOR0;
#endif
#if defined(PSYS)
    #if defined(SUBTEX)
        float2 sizeAndSubtexIndex : TEXCOORD1;
    #else
        float size : TEXCOORD1;
    #endif
#endif
#if defined(FALLOFF)
    float4 normal : NORMAL;
#endif
#if defined(SKIN)
    float3 blendWeight : BLENDWEIGHT;
    float4 blendIndices : BLENDINDICES;
#endif
};

struct VertexOutput {
    float4 positionSS : POSITION;
    float4 uvAndFog : TEXCOORD0;
#if defined(VC)
    float4 vertexColor : COLOR0;
#endif
    float2 depthAndSize : TEXCOORD1;
};

struct PixelInput {
    float4 screenUV : VPOS;
    float4 uvAndFog : TEXCOORD0;
    float4 vertexColor : COLOR0;
    float2 depthAndSize : TEXCOORD1;
};

struct PixelOutput {
    float4 shadedPixel : COLOR0;
};

#ifdef VS

row_major float4x4 WorldViewProjMatrix : register(c0);
row_major float3x3 TexCoordTranform : register(c4);
row_major float4x4 WorldViewMatrix : register(c36);

float4 FogParam : register(c13);
float4 SubTexOffsets[16] : register(c15);
float4 Falloff : register(c40);

#if !defined(PSYS)
    float Size : register(c41);
#endif

#if defined(SKIN)
    float4 Bones[54] : register(c64);
#endif

VertexOutput Main(VertexData vertex) {
    VertexOutput output;
    
    float4 position = vertex.position.xyzw;
    
    #ifdef SKIN
        float4 offset = vertex.blendIndices.zyxw * 765.01001;
        float4 blend = vertex.blendWeight.xyzz;
        blend.w = 1 - dot(vertex.blendWeight.xyz, 1.0f);
        
        position.xyz = BonesTransformPosition(Bones, offset, blend, position);
        position.w = 1.0f;
    #endif
    
    output.positionSS.xyzw = mul(WorldViewProjMatrix, position.xyzw);
    
    float3 fogPos = output.positionSS.xyz;
    fogPos.z = output.positionSS.w - fogPos.z;
    float fogStrength = 1 - saturate((FogParam.x - length(fogPos)) / FogParam.y);
    float fog = pow(fogStrength, FogParam.z);
    
    #if defined(FALLOFF)
        float3 positionVS = normalize(mul(WorldViewMatrix, float4(vertex.position.xyz, 1.0f)).xyz);
        float3 normalVS = normalize(mul((float3x3)WorldViewMatrix, vertex.normal.xyz).xyz);
        float PdotN = abs(dot(positionVS, normalVS));
        float falloffRaw = saturate((PdotN - Falloff.x) / (Falloff.y - Falloff.x));
        float falloff = (3.0f - 2.0f * falloffRaw) * (falloffRaw * falloffRaw) * (Falloff.w - Falloff.z) + Falloff.z;
    #else
        float falloff = 1.0f;
    #endif
    
    #if defined(SUBTEX)
        float index = vertex.sizeAndSubtexIndex.y;
        output.uvAndFog.xy = SubTexOffsets[index].yw * vertex.uv.xy + SubTexOffsets[index].xz;
    #elif defined(TEX)
        float3 transformedUV = mul(TexCoordTranform, float3(vertex.uv.xy, 1.0f));
    
        output.uvAndFog.x = transformedUV.x;
        output.uvAndFog.y = transformedUV.y;
    #else
        output.uvAndFog.x = 0.0f;
        output.uvAndFog.y = 0.0f;
    #endif
    
    output.uvAndFog.z = falloff;
    output.uvAndFog.w = fog;
    
    #ifdef VC
        output.vertexColor = vertex.vertexColor;
    #endif
    
    #ifdef PSYS
        #if defined(SUBTEX)
            output.depthAndSize = float2(output.positionSS.w, vertex.sizeAndSubtexIndex.x);
        #else
            output.depthAndSize = float2(output.positionSS.w, vertex.size);
        #endif
    #else
        output.depthAndSize = float2(output.positionSS.w, Size);
    #endif

    return output;
}

#else

#if defined(TEX)
sampler2D DiffuseMap : register(s0);
    sampler2D DepthMap : register(s1);
#else
    sampler2D DepthMap : register(s0);
#endif

float4 MaterialColor : register(c0);
float4 MaterialAlpha : register(c1);
float4 FogColor : register(c2);

#if defined(SOFT)
    float4 SoftScaleAndScreenSize : register(c8);
    float4 DepthConstants : register(c9);
#endif

PixelOutput Main(PixelInput input) {
    PixelOutput output;
    
    #if defined(TEX)
        float4 baseColor = tex2D(DiffuseMap, input.uvAndFog.xy);
    #else
        float4 baseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    #endif
    
    #if defined(VC)
        baseColor *= input.vertexColor;
    #endif
    
    baseColor *= MaterialColor;
    
    float alpha = baseColor.a;
    
    // Falloff.
    alpha *= input.uvAndFog.z;
    
    #if defined(SOFT)
        // Soft particles.
        float sceneDepth = tex2D(DepthMap, (input.screenUV.xy + 0.5f) * SoftScaleAndScreenSize.zw).r;
        float sceneDepthVS = DepthConstants.w / (sceneDepth * DepthConstants.z + DepthConstants.x);
        float softness = saturate((sceneDepthVS - input.depthAndSize.x) / (input.depthAndSize.y * SoftScaleAndScreenSize.x));
    
        alpha *= softness;
    #endif
    
    #if defined(PREMULT_ALPHA)
        float3 finalBlend = baseColor.rgb * (1 - input.uvAndFog.w) * alpha;
        alpha = 1.0f;
    #else
        float3 fogBlend = lerp(baseColor.rgb, FogColor.rgb, input.uvAndFog.w);
        float3 fadeBlend = lerp(fogBlend, baseColor.rgb * (1.0f - input.uvAndFog.w), MaterialAlpha.x);
        float3 finalBlend = lerp(fadeBlend, (saturate(input.uvAndFog.w * 1.5f) * (1.0f - baseColor.rgb)) + baseColor.rgb, MaterialAlpha.y);
    #endif
    
    output.shadedPixel.rgb = finalBlend;
    output.shadedPixel.a = alpha;
    
    return output;
}

#endif