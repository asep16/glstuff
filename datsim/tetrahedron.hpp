#ifndef _TETRAHEDRON_HPP
#define _TETRAHEDRON_HPP
#include <SFML/OpenGL.hpp>
/******************************************************
 * The Tetrahedron class defines a regular tetrahedron
 * with it's own render method
 ******************************************************/
class Tetrahedron
{
private:
	float x;
	float y;
	float z;

	GLuint VBO;
	GLuint IBO;
public:
	Tetrahedron();
	float getX() { return x; };
	float getY() { return y; };
	float getZ() { return z; };
	void render();

};


#endif
