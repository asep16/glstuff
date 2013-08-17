#ifndef _PATH_HPP
#define _PATH_HPP
#include <vector>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Path
{
private:
	GLuint VBO;
	GLuint VAO;
public:
	int maxPathIndex;
	int pathIndex;
	Path();
	std::vector< float > getPositionInterpolation( float elapsedTime );
	std::vector< std::vector< float > > pathData;
	glm::mat4 getModelMatrix();
	void render();
};

#endif
