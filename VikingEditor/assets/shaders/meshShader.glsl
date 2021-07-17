// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

layout(std140, binding = 1) uniform Mesh
{
	mat4 u_Model;
};

struct VertexOutput
{
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	Output.Normal = a_Normal;
	Output.Tangent = a_Tangent;
	Output.Binormal = a_Binormal;
	Output.TexCoord = a_TexCoord;

	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

struct VertexOutput
{
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoord;
};

layout(location = 0) out vec4 color;
layout(location = 1) out int entityId;

layout (location = 0) in VertexOutput Input;

layout(std140, binding = 4) uniform Entity
{
	int EntityId;
} u_EntityUniforms;

layout(push_constant) uniform Material
{
	layout(offset = 64) vec3 AlbedoColor;
} u_MaterialUniforms;

void main()
{
	color = vec4(u_MaterialUniforms.AlbedoColor, 1.0);
    entityId = u_EntityUniforms.EntityId;
}