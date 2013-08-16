#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <cstring>
//project headers
#include "globals.hpp"
#include "tetrahedron.hpp"
#include "quadcopter.hpp"

const char* VertexText = " \n\
#version 140 \n\
\n\
in vec3 in_Position; \n\
uniform mat4 gScale;\n\
\n\
void main() { \n\
	gl_Position = gScale * vec4( in_Position, 1.0 );\n\
}\n";

const char* FragmentText = " \n\
#version 140\n\
\n\
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

	//get uniform location
	uniformLocation = glGetUniformLocation( shaderProgram, "gScale" );
	assert( uniformLocation != -1 );
	
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

sf::ContextSettings setupSettings() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 3;
	settings.minorVersion = 1;

	return settings;
}

void testDraw( float x, float y, float z ) {
	GLuint test;
	GLuint testIBO;
	float Vertices[] = { x, y, z };
	unsigned int Indices[] = { 0 };
	glGenBuffers( 1, &test );
	glBindBuffer( GL_ARRAY_BUFFER, test );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Vertices ), Vertices, GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, test );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	//glDrawArrays( GL_POINTS, 0, 1 );
	
	//index buffer
	glGenBuffers( 1, &testIBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, testIBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( Indices ), Indices, GL_STATIC_DRAW );

	//draw using index buffer
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, testIBO );
	glDrawElements( GL_POINTS, 1, GL_UNSIGNED_INT, 0 );

	//cleanup
	glDisableVertexAttribArray( 0 );
	glDeleteBuffers( 1, &test );
	glDeleteBuffers( 1, &testIBO );

}


int main() {
	//set up OpenGL context
	sf::ContextSettings settings = setupSettings();

	//set up sfml window bound to OpenGL
	sf::Window window( sf::VideoMode( 800, 600 ), "OpenGL", sf::Style::Default, settings );
	window.setVerticalSyncEnabled( true );

	//init opengl/glew
	glewInit();
	glewExperimental = GL_TRUE;
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f  );

	Tetrahedron tetra;
	Quadcopter quad;

	initShaders();


	sf::Clock clock;
	bool running = true;

	//test vars
	int height = 800;
	int width = 600;
	glm::mat4 scalingMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 srt;
	glm::mat4 viewMatrix = glm::translate( 0.0f, 0.0f, 1.0f );
	glm::mat4 cameraMatrix = glm::lookAt( glm::vec3( 10.0f, 0.0f, 10.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0, 1.0f, 0.0f ) );
	glm::mat4 projectionMatrix = glm::perspective( 70.0f, (float)width/height, 0.1f, 100.0f );
	glm::mat4 modelMatrix = glm::mat4( 1.0f );
	glm::mat4 mvp;
	float scale;
	viewMatrix = cameraMatrix;
	int count;
	sf::Clock timer;
	sf::Clock simTime;
	while( running ) {
		if ( clock.getElapsedTime() < sf::milliseconds( 16 ) )
			continue;

		count++;
		if ( count >= 60 ) {
			count = 0;
			//std::cout << timer.getElapsedTime().asSeconds() << std::endl;
			timer.restart();
		}
		clock.restart();

		quad.update( simTime.getElapsedTime().asSeconds() );
		scale += 0.010f;
		glm::mat4 projectionMatrix = glm::perspective( 70.0f, (float)width/height, 0.1f, 100.0f );
		cameraMatrix = glm::lookAt( glm::vec3(  quad.xPos +1.0f, quad.yPos +1.0f,  quad.zPos -3.0f  ), glm::vec3( quad.xPos, quad.yPos, quad.zPos ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
		translationMatrix = glm::translate( 0.0f, 0.0f , 0.0f  );
		rotationMatrix = glm::rotate( 100 * scale , glm::vec3( 1.0, 1.0, 1.0 ) );
		scalingMatrix = glm::scale( 1.0, 1.0, 1.0 );
		srt = translationMatrix * rotationMatrix * scalingMatrix;
		mvp = projectionMatrix * cameraMatrix * srt;
		glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &mvp[0][0] );

		sf::Event event;
		while( window.pollEvent( event ) ) {
			if ( event.type == sf::Event::Closed )
				running = false;
			else if ( event.type == sf::Event::Resized ) {
				glViewport( 0, 0, event.size.width, event.size.height );
				height = event.size.height;
				width = event.size.width;
			}
		}

		glClear( GL_COLOR_BUFFER_BIT );


		srt = quad.getModelMatrix();
		mvp = projectionMatrix * cameraMatrix * srt;
		glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &mvp[0][0] );
		quad.render();

		srt = quad.path.getModelMatrix();
		mvp = projectionMatrix * cameraMatrix * srt;
		glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &mvp[0][0] );
		quad.path.render();
		//gluCylinder( gluNewQuadric(), 2, 2, 2, 32, 32 );
		//swap buffers, displaying the backbuffer
		window.display();
	} 
	return 1;
}

