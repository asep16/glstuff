#ifndef _DATUTILS_HPP
#define _DATUTILS_HPP
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

std::vector< std::string > stringSplit( const std::string &source, const char *delimiter = " " ) {
	std::vector< std::string > results;

	size_t prev = 0;
	size_t next = 0;

	while( ( next = source.find_first_of( delimiter, prev ) ) != std::string::npos  ) {
		if ( (next - prev != 0 ) ) 
			results.push_back( source.substr( prev, next - prev ) );
		prev = next + 1;
	}

	return results;
}
	
double scientificToDouble( const std::string &stringnum ) {
	std::istringstream os( stringnum );
	double d;
	os >> d;
	return d;
}
std::vector< std::vector< float > > readScientificMatrix( const char* filename ) {

	std::ifstream infile( filename );
	std::string line;
	int count = 0;
	std::vector< std::vector< std::string > > strings;
	std::vector< std::vector<float> > nums;
	while( std::getline( infile, line ) ) {
		strings.push_back( stringSplit( line ) );
	}
	for ( int i = 0; i < strings.size(); i++ ) {
		std::vector< float > vec;
		for ( int j = 0; j < strings[ 0 ].size(); j++ ) {
			vec.push_back( scientificToDouble( strings[ i ][ j ] ) );
		}
		nums.push_back( vec );
	}

	return nums;
}
#endif
