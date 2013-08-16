#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include "tetrahedron.hpp"

Tetrahedron::Tetrahedron() {
	//initialise vertex and index buffer
	float Vertices[] = { -0.8f, -0.8f, -0.4f,
						 0.0f, 0.0f, 0.4f,
						 0.8f, -0.8f, -0.4f,
						 0.0f, 1.13f, -0.4f }; /*,
						 0.0f, 0.0f, 0.0f }; */
	unsigned int Indices[] = { 0, 3, 1,
							   1, 3, 2,
							   2, 3, 0,
							   0, 2, 1 }; /*,
							   1, 2, 4,
							   4, 3, 0	}; */

	//vertex Buffer
	glGenBuffers( 1, &VBO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Vertices ), Vertices, GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	//index buffer
	glGenBuffers( 1, &IBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( Indices ), Indices, GL_STATIC_DRAW );

	

}

void Tetrahedron::render() {
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
	glDrawElements( GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0 );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

