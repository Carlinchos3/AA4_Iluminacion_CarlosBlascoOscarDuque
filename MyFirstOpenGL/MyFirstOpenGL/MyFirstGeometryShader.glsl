#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 uvsGeometryShader[];
in vec3 normalsGeometryShader[];

out vec2 uvsFragmentShader;
out vec3 normalsFragmentShader;

in vec4 worldPosition4GeometryShader[];
out vec4 worldPosition4FragmentShader;

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;
uniform mat4 view;
uniform mat4 projection;

void main(){

    mat4 model = translationMatrix * rotationMatrix * scaleMatrix;
	mat3 normalMatrix = transpose(inverse(mat3(model)));

	for(int i = 0; i < gl_in.length(); i++)
	{

		gl_Position = projection * view * model * gl_in[i].gl_Position;

		worldPosition4FragmentShader = model * worldPosition4GeometryShader[i];

		uvsFragmentShader = uvsGeometryShader[i];

		normalsFragmentShader = normalize(normalMatrix * normalsGeometryShader[i]);

		EmitVertex();
	}

	EndPrimitive();
}