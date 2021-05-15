#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform int indexColor;

out vec3 fcolor;

void main() {
    if (indexColor == 1) {
        fcolor = vec3 (0.2, 0.2, 0.2);
    }
    else if (indexColor == 0) {
        fcolor = vec3 (0.8, 0.8, 0.8);
    }
    else if (indexColor == 2) {
        fcolor = color;
    }
    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
