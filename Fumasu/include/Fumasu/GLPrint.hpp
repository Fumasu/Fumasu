/*
 * GLPrint.hpp
 *
 *  Created on: 16.03.2013
 *      Author: Fumasu
 */

#ifndef GLPRINT_HPP_
#define GLPRINT_HPP_

#include <string>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Poco/TextConverter.h>
#include <Poco/UTF8Encoding.h>
#include <Poco/TextIterator.h>

#include <Fumasu/GLTexture.hpp>
#include <Fumasu/Shader.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_IDS_H

using namespace std;

namespace Fumasu
{

class GLPrint
{
public:
	GLPrint(const string& font, int size, int width, int height);
	~GLPrint();

	void print(int x, int y, string text, glm::vec3 color =glm::vec3(1, 1, 1));

	void setFontSize(int size);
	void setScreenSize(int w, int h);
private:
	void createTexture();

	struct glyph_info {
		unsigned int width;
		unsigned int height;
		unsigned char *buffer;
		unsigned int advance_x;
		unsigned int advance_y;
		unsigned int left;
		unsigned int top;
	};

	int _width, _height, _num_glyphs;
	GLfloat _vertex_buffer_data[256 * (2 * 6)];
	GLfloat _uv_buffer_data[256 * (2 * 6)];
	GLfloat _extra_vertex_data[2 * 6];
	GLfloat _extra_uv_data[2 * 6];

	GLuint _vertexID;
	GLuint _extra_GlyphID;
	GLuint _vertexBuffer, _uvBuffer;
	GLuint _extra_vertexBuffer, _extra_uvBuffer;

	unsigned int _max_width, _max_height;
	int _size;
	vector<glyph_info> _glyphs;
	map<unsigned int, GLTexture*> _extra_glyphs;
	GLTexture *_texture;
	Shader *_shader;
	string _font;

	const static string _vertexShader, _fragmentShader;
};

} /* namespace Fumasu */
#endif /* GLPRINT_HPP_ */
