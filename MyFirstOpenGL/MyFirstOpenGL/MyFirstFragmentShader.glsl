#version 440 core

//Para evitar Magic Numbers
#define MIN_DIFFUSE       0.15
#define MOON_INTENSITY         0.3
#define HORIZON_FADE_MIN      -0.2
#define HORIZON_FADE_MAX       0.2
#define FLASHLIGHT_ATTEN_EXP   2.0
#define FLASHLIGHT_INTENSITY  3.0

uniform vec2 windowSize;
uniform sampler2D textureSampler;

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec4 worldPosition4FragmentShader;

// Linterna
uniform vec3 flashlightPos;
uniform vec3 flashlightDir;
uniform float flashlightInnerCone;
uniform float flashlightOuterCone;
uniform float flashlightMaxDist;
uniform bool flashlightOn;

// Ciclo dia y noche
uniform vec3 sunDir;
uniform vec3 moonDir;
uniform vec3 ambientColor;
uniform bool isLightSource;

out vec4 fragColor;

void main() 
{
    vec2 adjustedTexCoord = vec2(uvsFragmentShader.x, 1.0 - uvsFragmentShader.y);

    // Las fuentes de luz se renderizan sin iluminacion
    if (isLightSource) 
    {
        fragColor = vec4(texture(textureSampler, adjustedTexCoord).rgb, 1.0);
        return;
    }

    vec3 baseColor = texture(textureSampler, adjustedTexCoord).rgb;

    // Ambient con transición
    vec3 result = ambientColor * baseColor;

    // Sol con un fade porque sino mete cortes burscos
    float sunDot = dot(normalsFragmentShader, normalize(sunDir));
    float sunHorizonFade = smoothstep(HORIZON_FADE_MIN, HORIZON_FADE_MAX, sunDir.y);
    float sunDiffuse = max(sunDot, MIN_DIFFUSE);
    result += baseColor * sunDiffuse * sunHorizonFade;

    // Luna igual que el sol pero menos intensa
    float moonDot = dot(normalsFragmentShader, normalize(-moonDir));
    float moonHorizonFade = smoothstep(HORIZON_FADE_MIN, HORIZON_FADE_MAX, moonDir.y);
    float moonDiffuse = max(moonDot, MIN_DIFFUSE) * MOON_INTENSITY;
    result += baseColor * moonDiffuse * moonHorizonFade;

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
            float attenuation = 1.0 - pow(dist / flashlightMaxDist, FLASHLIGHT_ATTEN_EXP);
            result += baseColor * intensity * attenuation * FLASHLIGHT_INTENSITY;
        }
    }

    fragColor = vec4(result, 1.0);
}