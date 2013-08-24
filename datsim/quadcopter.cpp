#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <math.h>
#include "quadcopter.hpp"
#include "globals.hpp"


Quadcopter::Quadcopter() {
	
	float XVertices[] = { 0.5f, 0.0f, 0.5f,
						  -0.5f, 0.0f, -0.5f,
						  -0.5f, 0.0f, 0.5f,
						  0.5f, 0.0f, -0.5f };
							
							//upper square
	float cubeVertices[] = { 0.1f, 0.1f, 0.1f,
							 -0.1f, 0.1f, 0.1f,
							 -0.1f, -0.1f, 0.1f,
							 0.1f, -0.1f, 0.1f,

							 //lower square
							 0.1f, 0.1f, -0.1f,
							 -0.1f, 0.1f, -0.1f,
							 -0.1f, -0.1f, -0.1f,
							 0.1f, -0.1f, -0.1f };

	unsigned int cubeIndices[] = { //top side
								   0, 1, 2,
								   0, 2, 3,
								   // bottom side,
								   4, 5, 6,
								   4, 6, 7,
								   // left side
								   1, 2, 5,
								   5, 6, 2,
								   // right side
								   0, 3, 4,
								   4, 7, 3,
								   // front side
								   0, 3, 7,
								   4, 7, 3,
								   //back side
								   1, 2, 6,
								   5, 6, 2 };

	glGenVertexArrays( 1, &cubeVAO );
	glBindVertexArray( cubeVAO );

	glGenBuffers( 1, &cubeVBO );
	glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( cubeVertices ), cubeVertices, GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glGenBuffers( 1, &cubeIBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, cubeIBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( cubeIndices ), cubeIndices, GL_STATIC_DRAW );


	glGenVertexArrays( 1, &XVAO );
	glBindVertexArray( XVAO );

	glGenBuffers( 1, &XVBO );
	glBindBuffer( GL_ARRAY_BUFFER, XVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( XVertices ), XVertices, GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

}


void Quadcopter::render( glm::mat4 mvp ) {
	glBindVertexArray( XVAO );
	glDrawArrays( GL_LINES, 0, 4 );

	glm::mat4 trans = glm::translate( 0.5f, 0.0f, 0.5f );
	glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &( mvp * trans )[0][0] );
	glBindVertexArray( cubeVAO );
	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );
	//model.render();
	trans = glm::translate( -0.5f, 0.0f, -0.5f );
	glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &( mvp * trans )[0][0] );
	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );


	trans = glm::translate( 0.5f, 0.0f, -0.5f );
	glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &( mvp * trans )[0][0] );
	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );


	trans = glm::translate( -0.5f, 0.0f, 0.5f );
	glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &( mvp * trans )[0][0] );
	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );
}

void Quadcopter::update( float elapsedTime ) {
	std::vector< float > state = path.getPositionInterpolation( elapsedTime );
	xPos = state[ 1 ];
	yPos = state[ 3 ];
	zPos = state[ 2 ];
	xRot = state[ 4 ];
	yRot = state[ 6 ];
	zRot = state[ 5 ];
	//std::cout << elapsedTime << " " << yPos << " " << zPos << std::endl;

}

glm::mat4 Quadcopter::getModelMatrix() {
	glm::mat4 translationMatrix = glm::translate( xPos, yPos, zPos );
	glm::mat4 rotX = glm::rotate( xRot * 180.0f / (float)M_PI, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	glm::mat4 rotY = glm::rotate( yRot * 180.0f / (float)M_PI, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	glm::mat4 rotZ = glm::rotate( zRot * 180.0f / (float)M_PI, glm::vec3( 0.0f, 0.0f, 1.0f ) );

	return translationMatrix * rotZ * rotY * rotX;
}
	
	
