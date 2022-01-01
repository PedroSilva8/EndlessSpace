#version 450 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec2 uv;

layout (location = 0) out vec3 localPos;

layout (binding = 0) uniform VP {
	mat4 view;
} vp;

void main() {
    localPos = pos;
    vec4 clipPos = vp.view * vec4(localPos, 1.0);
    gl_Position = clipPos.xyww;
}