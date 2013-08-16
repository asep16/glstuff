#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <cstring>


int main() {
	//initialize sf context
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 3;
	settings.minorVersion = 1;

	//initialize sf window with above settings
	sf::Window window( sf::VideoMode( 800, 600 ), "OpenGL", sf::Style::Default, settings );
	window.setVerticalSyncEnabled( true );

	//init glew
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	GLuint VBO, IBO;

	//initialise vertex and index buffer
	float Vertices[] = { -0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f,
		0.8f, -0.8f, 0.0f };
	unsigned int Indices[] = { 0, 1, 2 };

	//vertex Buffer
	glGenBuffers( 1, &VBO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Vertices ), Vertices, GL_STATIC_DRAW );


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
		//glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
		//glDrawElements( GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0 );

		window.display();

	}
	return 1;
}
