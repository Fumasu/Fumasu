/*
 * VertexArray.h
 *
 *  Created on: 24.12.2011
 *      Author: Fumasu
 */

#ifndef VERTEXARRAY_H_
#define VERTEXARRAY_H_

#include <vector>
#include <GL/glew.h>

//using namespace std;

namespace Fumasu {

class VertexArray
{
public:
	~VertexArray();

	void AddTexCoord(double x = 0, double y = 0);
	void AddVertexCoord(double x = 0, double y = 0, double z = 0);
	void AddNormalCoord(double x = 0, double y = 0, double z = 0);
	void AddColorValue(double r = 0, double g = 0, double b = 0, double a = 1);

	void Activate();

	int size()
	{
		return vertex_data.size() / 3;
	}
	;
	void clear();
private:
	std::vector<GLdouble> vertex_data;
	std::vector<GLdouble> tex_data;
	std::vector<GLdouble> normal_data;
	std::vector<GLdouble> color_data;
};

} //namespace

#endif /* VERTEXARRAY_H_ */
