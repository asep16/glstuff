#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <cstring>


const char* VertexText = " \n\
#version 140 \n\
\n\
in vec3 in_Position; \n\
\n\
void main() { \n\
	gl_Position = vec4( in_Position, 1.0 );\n\
}\n";

const char* FragmentText = " \n\
#version 140\n\
\n\
in vec4 ex_Color; \n\
out vec4 FragColor;\n\
\n\
void main() {\n\
	FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );\n\
}\n";


void addShader( GLuint shaderProgram, const char* source, GLenum shaderType ) {
	//create the shader object identifier
	GLuint shaderObj = glCreateShader( shaderType );
	//check for error
	if ( shaderObj == 0 ) {
		fprintf( stderr, "Error creating shader: %d\n", shaderType );
	}

	//get and set the source
	const GLchar* code[ 1 ];
	code[ 0 ] = source;
	GLint lengths[ 1 ];
	lengths[ 0 ] = strlen( source );
	glShaderSource( shaderObj, 1, code, lengths );

	//compile shader and check for errors
	glCompileShader( shaderObj );
	GLint success;
	glGetShaderiv( shaderObj, GL_COMPILE_STATUS, &success );
	if ( !success ) {
		GLchar infoLog[ 1024 ];
		glGetShaderInfoLog( shaderObj, sizeof( infoLog ), NULL, infoLog );
	    fprintf( stderr, "Error compiling shader type %d: %s\n", shaderType, infoLog );
	}

	glAttachShader( shaderProgram, shaderObj );
	
}

void initShaders() {
	//create the shader program
	GLuint shaderProgram = glCreateProgram();

	if ( shaderProgram == 0 ) {
		fprintf( stderr, "Error creating shader program\n" );
	}

	//add vertex and fragment shader to program
	addShader( shaderProgram, VertexText, GL_VERTEX_SHADER );
	addShader( shaderProgram, FragmentText, GL_FRAGMENT_SHADER );

	//glBindAttribLocation( shaderProgram, 0, "in_Position" );
	//glBindFragDataLocation( shaderProgram, 0, "ex_Color" );
	//error vars
	GLint success;
	GLchar errorLog[ 1024 ];

	//link the program
	glLinkProgram( shaderProgram );
	glGetProgramiv( shaderProgram, GL_LINK_STATUS, &success );
	if ( success == 0 ) {
		glGetProgramInfoLog( shaderProgram, sizeof( errorLog ), NULL, errorLog );
		fprintf( stderr, "Error linking shader program: %s\n", errorLog );
	}

	
    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
        exit(1);
    }

	//enable program for use
    glUseProgram(shaderProgram);

}


int main() {
	//initialize sf context
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 3;
	settings.minorVersion = 1;

	//initialize sf window with above settings
	sf::RenderWindow window( sf::VideoMode( 800, 600 ), "OpenGL", sf::Style::Default, settings );
	window.setVerticalSyncEnabled( true );

	//init glew
	glewExperimental = GL_TRUE;
	glewInit();

	initShaders();

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	GLuint VBO, VAO;

	//initialise vertex and index buffer
	float Vertices[] = { -0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f,
		0.8f, -0.8f, 0.0f };

	//vertex array object
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );
	//vertex Buffer
	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );

	glGenBuffers( 1, &VBO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Vertices ), Vertices, GL_STATIC_DRAW );

	sf::Font font;
	font.loadFromFile( "DejaVuSansMono.ttf" );

	sf::Text text;
	text.setFont( font );
	text.setString( "Hello World!" );
	text.setCharacterSize( 24 );
	text.setColor( sf::Color::Green );

	text.setStyle( sf::Text::Bold | sf::Text::Underlined );

	bool running = true;
	while ( running ) {

		sf::Event event;
		while( window.pollEvent( event ) ) {
			if ( event.type == sf::Event::Closed )
				running = false;
			else if ( event.type == sf::Event::Resized ) {
				glViewport( 0, 0, event.size.width, event.size.height );
			}
		}
		glClear( GL_COLOR_BUFFER_BIT );

		glEnableVertexAttribArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		glDisableVertexAttribArray( 0 );

		window.pushGLStates();
		window.draw( text );
		window.popGLStates();

		window.display();

	}
	return 1;
}
