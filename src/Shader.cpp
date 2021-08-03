#include <fstream>
#include <iostream>
#include "Shader.hpp"

Shader::Shader() {}

Shader::Shader(const std::string& fragPath, const std::string& vertPath) {
	init(fragPath, vertPath);
}	

void Shader::init(const std::string& fragPath, const std::string& vertPath) {
	m_program = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	std::string fragSrc = read(fragPath);
	std::string vertSrc = read(vertPath);
	compile(vertSrc, vertexShader);
	compile(fragSrc, fragmentShader);

	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);  
}

void Shader::bind() const{
	glUseProgram(m_program);
}

void Shader::unbind() const{
	glUseProgram(0);
}

std::string Shader::read(const std::string& shaderPath) {
	std::ifstream ifs(shaderPath);
  	std::string content((std::istreambuf_iterator<char>(ifs)),
                       (std::istreambuf_iterator<char>()));
	return content; 
}

void Shader::compile(const std::string& shaderSource, GLuint shader) {
	const char *shaderSrc = shaderSource.c_str();
	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Shader failed to compile:\n" << infoLog << std::endl;
	}
}

GLuint Shader::getUniformLocation(const std::string& name) {
	return glGetUniformLocation(m_program, name.c_str());
}

GLuint Shader::getProgram() {
	return m_program;
}

Shader::~Shader() {
	glDeleteProgram(m_program);
}