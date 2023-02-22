#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;
uniform vec3 car_position;

// Output value to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
out vec2 tex_coord;
flat out int setColor;


void main()
{
    if (object_color.x != -1) {
        frag_color = object_color;
        setColor = 1;
    }
    else {
        frag_color = v_color;
        setColor = 0;
    }

    frag_normal = v_normal;
    tex_coord = v_texture_coord;
    vec4 newPosition = Model * vec4(v_position, 1.0);
    float positionX = newPosition.x;
    float positionY = newPosition.y - distance(car_position, vec3(newPosition.x, newPosition.y, newPosition.z)) * distance(car_position, vec3(newPosition.x, newPosition.y, newPosition.z)) * 0.02;
    float positionZ = newPosition.z;
    gl_Position = Projection * View * vec4(positionX, positionY, positionZ, 1.0);
}