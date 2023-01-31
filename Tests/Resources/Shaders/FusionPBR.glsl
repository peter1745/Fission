#version 450

#ifdef FUSION_COMPILE_VERTEX

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InTexCoord;

layout(push_constant) uniform TransformData
{
    mat4x4 ViewProjectionMatrix;
    mat4x4 TransformMatrix;
} InTransformData;

struct VertexOutput
{
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoord;
};

layout(location = 0) out VertexOutput Out;

void main()
{
    gl_Position = InTransformData.ViewProjectionMatrix * InTransformData.TransformMatrix * vec4(InPosition, 1.0);
    Out.FragPosition = vec3(InTransformData.TransformMatrix * vec4(InPosition, 1.0));
    Out.Normal = InNormal;
    Out.TexCoord = InTexCoord;
}

#endif

#ifdef FUSION_COMPILE_PIXEL

struct PixelInput
{
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoord;
};

layout(location = 0) in PixelInput InData;

layout(location = 0) out vec4 OutColor;

void main()
{
    const vec3 LightPosition = vec3(0.0, 50.0, 50.0);
    const vec3 LightColor = vec3(1, 1, 1);

    vec3 N = normalize(InData.Normal);
    vec3 LightDirection = normalize(LightPosition - InData.FragPosition);
    float Diff = max(dot(N, LightDirection), 0.0);
    vec3 Diffuse = Diff * LightColor;

    vec3 Color = Diffuse * vec3(1.0, 0.0, 0.0);
    OutColor = vec4(Color, 1.0);
}

#endif
