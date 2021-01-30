#shader vertex
#version 330 core

layout(location = 0) in vec3 coords;

uniform mat4 mvp;
uniform sampler2D height;

out vec2 texCoord;
out vec4 col;

void main()
{
	texCoord = vec2(coords.x / 100, coords.z / 100);
	col = texture(height, texCoord);

	float y = (col.x + col.y + col.z) / 3.0 * 10.0;

	gl_Position = mvp * vec4(coords.x, y, coords.z, 1.0);
};


#shader fragment
#version 330 core

out vec4 color;
in vec2 texCoord;
in vec4 col;

uniform sampler2D height;

void main()
{
	//color = col;
	color = vec4(1.0, 0, 0, 1.0);
};

