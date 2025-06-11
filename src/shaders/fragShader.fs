#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixing;


void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1 - TexCoord.x, TexCoord.y)), mixing);
   // mirror with:
   //  FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1 - TexCoord.x, TexCoord.y)), 0.2) * vec4(ourColor, 1.0);

}