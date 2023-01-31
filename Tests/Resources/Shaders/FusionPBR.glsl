#version 450

#ifdef FUSION_COMPILE_VERTEX

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;

layout(push_constant) uniform TransformData
{
    mat4x4 ViewProjectionMatrix;
    mat4x4 TransformMatrix;
} InTransformData;

struct VertexOutput
{
    vec3 WorldNormal;
    vec3 VertexPos;
    vec3 Normal;
};

layout(location = 0) out VertexOutput Out;

void main()
{
    gl_Position = InTransformData.ViewProjectionMatrix * InTransformData.TransformMatrix * vec4(InPosition, 1.0);

    Out.VertexPos = InPosition;
	Out.Normal = InNormal;
    Out.WorldNormal = (InTransformData.TransformMatrix * vec4(InNormal, 0.0)).xyz;
}

#endif

#ifdef FUSION_COMPILE_PIXEL

struct PixelInput
{
    vec3 WorldNormal;
    vec3 VertexPos;
    vec3 Normal;
};

layout(location = 0) in PixelInput InData;

layout(location = 0) out vec4 OutColor;

vec3 GetColorFromPositionAndNormal(in vec3 InWorldPosition, in vec3 InNormal)
{
    const float PI = 3.141519;

    vec3 ScaledPos = InWorldPosition * PI * 2.0;
    vec3 ScaledPos2 = InWorldPosition * PI * 2.0 / 10.0 + vec3( PI / 4.0 );
    float S = cos(ScaledPos2.x) * cos(ScaledPos2.y) * cos(ScaledPos2.z);  // [-1,1] range
    float T = cos(ScaledPos.x) * cos(ScaledPos.y) * cos(ScaledPos.z);     // [-1,1] range

    vec3 ColorMultiplier = vec3( 0.5, 0.5, 1 );
    if (abs(InNormal.x) > abs(InNormal.y) && abs(InNormal.x) > abs(InNormal.z))
    {
        ColorMultiplier = vec3( 1, 0.5, 0.5 );
    }
    else if (abs(InNormal.y) > abs(InNormal.x) && abs(InNormal.y) > abs(InNormal.z))
    {
        ColorMultiplier = vec3( 0.5, 1, 0.5 );
    }

    T = ceil( T * 0.9 );
    S = ( ceil( S * 0.9 ) + 3.0 ) * 0.25;
    vec3 ColorB = vec3( 0.85, 0.85, 0.85 );
    vec3 ColorA = vec3( 1, 1, 1 );
    vec3 FinalColor = mix( ColorA, ColorB, T ) * S;

    return ColorMultiplier * FinalColor;
}

void main()
{
    vec3 DirToLight = normalize( vec3( 1, 1, 1 ) );

    float DX = 0.25;
    float DY = 0.25;
    vec3 ColorMultiplier = vec3( 0.0, 0.0, 0.0 );
    for (float y = 0.0; y < 1.0; y += DY)
    {
        for (float x = 0.0; x < 1.0; x += DX)
        {
            vec3 SamplePos = InData.VertexPos + dFdx(InData.VertexPos) * x + dFdy(InData.VertexPos) * y;
            ColorMultiplier += GetColorFromPositionAndNormal(SamplePos, InData.Normal) * DX * DY;
        }
    }
    
    float Ambient = 0.5;
    float Flux = clamp(dot(InData.WorldNormal, DirToLight ), 0.0, 1.0 - Ambient) + Ambient;
    OutColor = vec4(ColorMultiplier * Flux, 1.0);
}

#endif
