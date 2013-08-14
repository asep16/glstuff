#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <math.h>
#include "quadcopter.hpp"

Quadcopter::Quadcopter() {}

void Quadcopter::render() {

	model.render();
}

void Quadcopter::update( float elapsedTime ) {
	std::vector< float > state = path.getPositionInterpolation( elapsedTime );
	xPos = state[ 1 ];
	yPos = state[ 2 ];
	zPos = state[ 3 ];
	xRot = state[ 4 ];
	yRot = state[ 5 ];
	zRot = state[ 6 ];
	//std::cout << elapsedTime << " " << yPos << " " << zPos << std::endl;

}

glm::mat4 Quadcopter::getModelMatrix() {
	glm::mat4 translationMatrix = glm::translate( xPos, yPos, zPos );
	glm::mat4 rotX = glm::rotate( xRot * 180.0f / (float)M_PI, glm::vec3( 1.0f, 0.0f, 0.0f ) );
	glm::mat4 rotY = glm::rotate( yRot * 180.0f / (float)M_PI, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	glm::mat4 rotZ = glm::rotate( zRot * 180.0f / (float)M_PI, glm::vec3( 0.0f, 0.0f, 1.0f ) );

	return translationMatrix * rotZ * rotY * rotX;
}
	
	
