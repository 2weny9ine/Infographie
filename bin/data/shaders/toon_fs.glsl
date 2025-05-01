#version 330

in vec3 surface_position;
in vec3 surface_normal;

out vec4 fragment_color;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;

#define MAX_LIGHTS 4
uniform int num_active_lights;
uniform vec3 light_positions[MAX_LIGHTS];

float calculateToonShading(vec3 N, vec3 L) {
    float diff = max(dot(N, L), 0.0);

    if(diff > 0.73)
        return 1.0;
    else if(diff > 0.42)
        return 0.67;
    else if(diff > 0.18)
        return 0.35;
    else
        return 0.15;
}

void main() {
    vec3 normal = normalize(surface_normal);
    vec3 finalColor = color_ambient;

    for(int i = 0; i < num_active_lights && i < MAX_LIGHTS; i++) {
        vec3 lightDir = normalize(light_positions[i] - surface_position);
        float toonShade = calculateToonShading(normal, lightDir);
        finalColor += toonShade * color_diffuse;
    }

    finalColor = clamp(finalColor, 0.0, 1.0);
    fragment_color = vec4(finalColor, 1.0);
}
