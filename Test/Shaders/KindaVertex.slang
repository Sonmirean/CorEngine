
module lol;

public class Material
{
    float3 color;
    float metallness;
    float roughness;
};

public namespace EssentialProcesses
{
// Classic fog formula. "distance" is length of (viewMat * camWorldPos).xyz
float4 fog(float distance, float density, float gradient)
{
    return exp(-pow(distance * density, gradient));
}
};

// Yeah, Slang looks much more convenient than GLSL.