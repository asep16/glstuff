#ifndef _QUADCOPTER_HPP
#define _QUADCOPTER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <SFML/OpenGL.hpp>
#include "tetrahedron.hpp"
#include "path.hpp"
class Quadcopter 
{
public:
	Quadcopter();

	Path path;
	Tetrahedron model;
	float xPos;
	float yPos;
	float zPos;
	float xRot;
	float yRot;
	float zRot;

	GLuint XVAO, cubeVAO;
	GLuint XVBO, cubeVBO;
	GLuint XIBO, cubeIBO;

	void update( float elapsedTime );
	void render( glm::mat4 mvp );
	glm::mat4 getModelMatrix();
};

#endif
