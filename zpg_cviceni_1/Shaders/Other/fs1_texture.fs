#version 330
out vec4 frag_colour;
in vec2 uv;

uniform sampler2D textureUnitID;

void main ()
{
     frag_colour = texture(textureUnitID, uv);
}