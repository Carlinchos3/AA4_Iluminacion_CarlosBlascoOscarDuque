#version 440 core

uniform vec2 windowSize;
uniform sampler2D textureSampler;

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec4 primitivePosition;

in vec4 worldPosition4FragmentShader;

//Cosas de linterna
uniform vec3 flashlightPos;
uniform vec3 flashlightDir;
uniform float flashlightInnerCone;
uniform float flashlightOuterCone;
uniform float flashlightMaxDist;
uniform bool flashlightOn;

//Ciclo dia y noche
uniform vec3 sunDir;
uniform vec3 moonDir;
uniform vec3 ambientColor;
uniform bool isLightSource;

out vec4 fragColor;

void main() 
{
    vec2 adjustedTexCoord = vec2(uvsFragmentShader.x, 1.0 - uvsFragmentShader.y);


    if (isLightSource) 
    {
       fragColor = vec4(texture(textureSampler, adjustedTexCoord).rgb, 1.0);
        return;
    }

    vec3 baseColor = texture(textureSampler, adjustedTexCoord).rgb;

    // Ambient
    vec3 result = ambientColor * baseColor;

    // Sol
    float sunAngle = dot(normalsFragmentShader, normalize(-sunDir));
    if (sunDir.y > 0.0)
        result += baseColor * max(sunAngle, 0.15);

    // Luna
    float moonAngle = dot(normalsFragmentShader, normalize(moonDir));
    if (moonDir.y > 0.0)
        result += baseColor * max(moonAngle * 0.3, 0.15);

    // Linterna
    if (flashlightOn) 
    {
        vec3 toFragment = worldPosition4FragmentShader.xyz - flashlightPos;
        float dist = length(toFragment);
        if (dist < flashlightMaxDist) 
        {
            vec3 toFragDir = normalize(toFragment);
            float angle = dot(toFragDir, normalize(flashlightDir));
            float intensity = smoothstep(flashlightOuterCone, flashlightInnerCone, angle);
            float attenuation = 1.0 - (dist / flashlightMaxDist);
            result += baseColor * intensity * attenuation;
        }
    }

    fragColor = vec4(result, 1.0);
}
