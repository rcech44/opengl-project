#pragma once

// Collection of all shaders

const char* vertex_shader_1 =
	"#version 330\n"
	"layout(location=0) in vec4 vp;"
	"layout(location=1) in vec4 vc;"
	"out vec4 color;\n"
	"void main () {"
	"     gl_Position = vec4 (vp);"
	"	  color = vc;\n"
	"}";

const char* fragment_shader_1 =
	"#version 330\n"
	"out vec4 frag_colour;"
	"in vec4 color;"
	"void main () {"
	"     frag_colour = vec4(color);"
	"}";

const char* vertex_shader_mat =
	"#version 330\n"
	"layout(location=0) in vec4 vp;"
	"layout(location=1) in vec4 vc;"
	"out vec4 color;\n"
	"uniform mat4 modelMatrix;\n"
	"void main () {"
	"     gl_Position = modelMatrix * vec4 (vp);\n"
	"	  color = vc;\n"
	"}";

const char* vertex_shader_camera =
	"#version 330\n"
	"layout(location=0) in vec4 vp;"
	"layout(location=1) in vec4 vc;"
	"out vec4 color;\n"
	"uniform mat4 modelMatrix;\n"
	"uniform mat4 viewMatrix;\n"
	"uniform mat4 projectionMatrix;\n"
	"uniform vec3 objectColor;\n"
	"void main () {"
	"     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vp);\n"
	"	  color = vec4(objectColor, 1.0);\n"
	"}";

const char* vertex_shader_camera_inverted =
	"#version 330\n"
	"layout(location=0) in vec4 vp;"
	"layout(location=1) in vec4 vc;"
	"out vec4 color;\n"
	"uniform mat4 modelMatrix;\n"
	"uniform mat4 viewMatrix;\n"
	"uniform mat4 projectionMatrix;\n"
	"void main () {"
	"     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (-vp.x, -vp.y, -vp.z, vp.w);\n"
	"	  color = vec4 (1-vc.x, 1-vc.y, 1-vc.z, vc.w);\n"
	"}";

const char* vertex_shader_camera_inverted_redify =
	"#version 330\n"
	"layout(location=0) in vec4 vp;"
	"layout(location=1) in vec4 vc;"
	"out vec4 color;\n"
	"uniform mat4 modelMatrix;\n"
	"uniform mat4 viewMatrix;\n"
	"uniform mat4 projectionMatrix;\n"
	"void main () {"
	"     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (-vp.x, -vp.y, -vp.z, vp.w);\n"
	"	  color = vec4 (vc.x, 0.2*vc.y, 0.2*vc.z, vc.w);\n"
	"}";

const char* vertex_shader_camera_moved =
	"#version 330\n"
	"layout(location=0) in vec4 vp;"
	"layout(location=1) in vec4 vc;"
	"out vec4 color;\n"
	"uniform mat4 modelMatrix;\n"
	"uniform mat4 viewMatrix;\n"
	"uniform mat4 projectionMatrix;\n"
	"void main () {"
	"     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (5+vp.x, 4+vp.y, vp.z - 5, vp.w);\n"
	"	  color = vec4 (1-vc.x, 1-vc.y, 1-vc.z, vc.w);\n"
	"}";

const char* vertex_shader_camera_light =
	"#version 330\n"

	"layout(location=0) in vec3 pos;"
	"layout(location=1) in vec3 nor;"

	"out vec3 normal;\n"
	"out vec3 FragPos;\n"

	"uniform mat4 modelMatrix;\n"
	"uniform mat4 viewMatrix;\n"
	"uniform mat4 projectionMatrix;\n"

	"void main () {"
	"	  FragPos = vec3(modelMatrix * vec4(pos, 1.0));"
	"     gl_Position = projectionMatrix * viewMatrix * vec4 (FragPos, 1.0);\n"
	"	  normal = mat3(transpose(inverse(modelMatrix))) * nor;\n"
	"}";

const char* fragment_shader_1_light =
	"#version 330\n"

	"out vec4 frag_colour;"

	"in vec3 normal;"
	"in vec3 FragPos;"
	"uniform vec3 lightPos;"
	"uniform vec3 lightColor;"
	"uniform vec3 objectColor;"

	"void main () {"
	"     float ambientStrength = 0.07;"
	"     vec3 ambient = ambientStrength * lightColor;"
	"     vec3 norm = normalize(normal);"
	"     vec3 lightDir = normalize(lightPos - FragPos);"
	"     float diff = max(dot(norm, lightDir), 0.0);"
	"     vec3 diffuse = diff * lightColor;"

	"     vec3 result = (ambient + diffuse) * objectColor;"
	"     frag_colour = vec4(result, 1.0);"
	"}";

const char* fragment_shader_1_light_specular =
	"#version 330\n"

	"out vec4 frag_colour;"

	"in vec3 normal;"
	"in vec3 FragPos;"
	"uniform vec3 lightPos;"
	"uniform vec3 lightColor;"
	"uniform vec3 objectColor;"
	"uniform vec3 viewPos;"

	"void main () {"
	"     float ambientStrength = 0.15;"
	"     float specularStrength = 1;"
	"     vec3 ambient = ambientStrength * lightColor;"
	"     vec3 viewDir = normalize(viewPos - FragPos);"
	"     vec3 norm = normalize(normal);"
	"     vec3 lightDir = normalize(lightPos - FragPos);"
	"     vec3 reflectDir = reflect(-lightDir, norm);"
	"     float diff = dot(norm, lightDir);"
	"     vec3 diffuse = max(diff, 0.0) * lightColor;"
	"     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);"
	"     vec3 specular = specularStrength * spec * lightColor; "
	"	  if ( diff < 0.0)"
	"	  {"
	"		specular = vec3(0.0, 0.0, 0.0);"
	"	  }"
	"     vec3 result = (ambient + diffuse + specular) * objectColor;"
	"     frag_colour = vec4(result, 1.0);"
	"}";