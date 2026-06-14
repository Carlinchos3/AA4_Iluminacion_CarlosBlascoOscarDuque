#version 440 core

uniform vec2 windowSize;
uniform sampler2D textureSampler;

in vec2 uvsFragmentShader;
in vec3 normalsFragmentShader;
in vec4 primitivePosition;
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

    // Ambient: transiciona de azul oscuro (noche) a amarillento (dia)
    vec3 result = ambientColor * baseColor;

    // Sol: fade suave en el horizonte para evitar cortes bruscos
    float sunDot = dot(normalsFragmentShader, normalize(sunDir));
    float sunHorizonFade = smoothstep(-0.2, 0.2, sunDir.y);
    float sunDiffuse = max(sunDot, 0.15);
    result += baseColor * sunDiffuse * sunHorizonFade;

    // Luna: misma logica pero con intensidad reducida (es luz reflejada)
    float moonDot = dot(normalsFragmentShader, normalize(-moonDir));
    float moonHorizonFade = smoothstep(-0.2, 0.2, moonDir.y);
    float moonDiffuse = max(moonDot, 0.15) * 0.3;
    result += baseColor * moonDiffuse * moonHorizonFade;

    // Linterna: spotlight con inner/outer cone y atenuacion por distancia
    if (flashlightOn) 
    {
        vec3 toFragment = worldPosition4FragmentShader.xyz - flashlightPos;
        float dist = length(toFragment);
        if (dist < flashlightMaxDist) 
        {
            vec3 toFragDir = normalize(toFragment);
            float angle = dot(toFragDir, normalize(flashlightDir));
            float intensity = smoothstep(flashlightOuterCone, flashlightInnerCone, angle);
            float attenuation = 1.0 - pow(dist / flashlightMaxDist, 2.0); // cuadratica, cae menos fuerte al inicio
            result += baseColor * intensity * attenuation * 3.0; // boost de intensidad
        }
    }

    fragColor = vec4(result, 1.0);
}