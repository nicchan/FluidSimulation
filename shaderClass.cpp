#include"ShaderClass.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructor that builds the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	// Checks if Shader compiled successfully
	compileErrors(vertexShader, "VERTEX");

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Fragment Shader into machine code
	glCompileShader(fragmentShader);
	// Checks if Shader compiled successfully
	if (compileErrors(fragmentShader, "FRAGMENT"))
	{
		std::cout << "SHADER_COMPILATION_ERROR for: " << fragmentFile << "\n" << std::endl;
	}
	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	// Checks if Shaders linked successfully
	if (compileErrors(ID, "PROGRAM"))
	{
		std::cout << "SHADER_LINKING_ERROR for: " << vertexFile << " and " << fragmentFile << "\n" << std::endl;
	}
	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	std::cout << "Shader Program ID " << ID << ": " << vertexFile << " , " << fragmentFile << "\n" << std::endl;
	// Init vertScale if applicable
	glUseProgram(ID);
	GLuint uniID = glGetUniformLocation(ID, "vertScale");
	if (uniID != -1)
	{
		glUniform1f(uniID, 0.0f);
	}
}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}


// Assigns uniform values
void Shader::setUniform1f(float value, const char* uniformName)
{
	// Activates Shader Program
	glUseProgram(ID);
	// Assigns scale value
	GLuint uniID = glGetUniformLocation(ID, uniformName);
	if (uniID == -1)
	{
		std::cout << uniformName << " does not correspond to an active uniform variable in Shader Program " << ID << std::endl;
		return;
	}
	glUniform1f(uniID, value);
}

void Shader::setUniform2f(float value0, float value1, const char* uniformName)
{
	// Activates Shader Program
	glUseProgram(ID);
	// Assigns scale value
	GLuint uniID = glGetUniformLocation(ID, uniformName);
	if (uniID == -1)
	{
		std::cout << uniformName << " does not correspond to an active uniform variable in Shader Program " << ID << std::endl;
		return;
	}
	glUniform2f(uniID, value0, value1);
}

void Shader::setUniform3f(float value0, float value1, float value2, const char* uniformName)
{
	// Activates Shader Program
	glUseProgram(ID);
	// Assigns scale value
	GLuint uniID = glGetUniformLocation(ID, uniformName);
	if (uniID == -1)
	{
		std::cout << uniformName << " does not correspond to an active uniform variable in Shader Program " << ID << std::endl;
		return;
	}
	glUniform3f(uniID, value0, value1, value2);
}

void Shader::setUniform4f(float value0, float value1, float value2, float value3, const char* uniformName)
{
	// Activates Shader Program
	glUseProgram(ID);
	// Assigns scale value
	GLuint uniID = glGetUniformLocation(ID, uniformName);
	if (uniID == -1)
	{
		std::cout << uniformName << " does not correspond to an active uniform variable in Shader Program " << ID << std::endl;
		return;
	}
	//GLfloat vec[4] = { value0, value1, value2, value3 };
	//glUniform4fv(uniID, 1, vec);
	glUniform4f(uniID, value0, value1, value2, value3);
}

// checks if the different shaders have compiled properly
bool Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	bool hasError = false;
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
			hasError = true;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
			hasError = true;
		}
	}
	return hasError;
}
