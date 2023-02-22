#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 frag_color;
in vec3 frag_normal;
in vec2 tex_coord;
flat in int setColor;

uniform sampler2D u_texture_0;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    if (setColor == 1){
        out_color = vec4(frag_color, 1);
    }
    else {
        // I used the the approach from the Simple shader used in the simple_scene.
        out_color = texture(u_texture_0, tex_coord);
        if(out_color.a < 0.9)
        {
            discard;
        }
    }
}
