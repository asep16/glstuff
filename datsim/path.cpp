#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include "path.hpp"
#include "datutils.hpp"
#include <iostream>

Path::Path() {
	//read the path data from file
	pathData = readScientificMatrix( "comb.txt" );

	//generate vertices from data
	float Vertices[ pathData[ 0 ].size() * 3 ];
	for ( int i = 0; i < pathData[ 0 ].size(); i++ ) {
		Vertices[ 3 * i ] = pathData[ 1 ][ i ];
		Vertices[ 3 * i + 1 ] = pathData[ 2 ][ i ];
		Vertices[ 3 * i + 2 ] = pathData[ 3 ][ i ];
		//if ( i < 4000 && i > 3000 )
			//cout << Vertices[ i ] << " " << Vertices[ i + 1 ] << " " << Vertices[ i + 2 ] << endl;
		//cout << i << endl;
	}	

	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );
 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

void Path::render() {
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glDrawArrays( GL_LINE_STRIP, std::max( pathIndex - 500, 0 ), ( pathIndex - 500 ) > 0 ? 500 : pathIndex  );

	glDisableVertexAttribArray( 0 );
}

std::vector< float > Path::getPositionInterpolation( float elapsedTime ) {
	//calculate the index
	int maxIndex = pathData[ 0 ].size() - 1;
	int index = 0;

	std::vector< float > result;
	bool found = false;
	while ( !found ) {
		//check out of bounds
		if ( index >= maxIndex ) {
			index = maxIndex; 
			break;
		}

		if ( pathData[ 0 ][ index ] > elapsedTime ) {
			found = true;
			index--;
		}
		else
			index++;
	}

	int deltaTime = 0;

	if ( index <= maxIndex - 1 ) {
		deltaTime = pathData[ 0 ][ index ] - elapsedTime;
		//add time to result
		result.push_back( elapsedTime );

		//add position and rotation to result
		for ( int i = 1; i < 7; i++ ) {
			result.push_back( pathData[ i ][ index ] + ( pathData[ i ][ index + 1 ] - pathData[ i ][ index ] ) * deltaTime );
		}
	}
	else {
		result.push_back( pathData[ 0 ][ maxIndex ] );
		for ( int i = 1; i < 7; i++ ) {
			result.push_back( pathData[ i ][ maxIndex ] );
		}
	}
	std::cout <<  result[ 0 ] << " " << elapsedTime << " " << index << " " << result[ 3 ] << std::endl;
	pathIndex = index;
	maxPathIndex = maxIndex;
	return result;
}
		


glm::mat4 Path::getModelMatrix() {
	return glm::mat4( 1.0f );
}


