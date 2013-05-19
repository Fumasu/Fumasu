/*
 * GLPrint.cpp
 *
 *  Created on: 16.03.2013
 *      Author: Fumasu
 */

#include "Fumasu/GLPrint.hpp"

namespace Fumasu
{

const string GLPrint::_vertexShader ="#version 330 core\n"
		"\n"
		"layout(location = 0) in vec2 inVert;\n"
		"layout(location = 1) in vec2 uvCoord;\n"
		"\n"
		"out vec2 UV;\n"
		"uniform float posX;\n"
		"uniform float posY;\n"
		"uniform float inWidth;\n"
		"uniform float inHeight;\n"
		"\n"
		"void main() {\n"
		"	float scaleX =2.0 / inWidth;\n"
		"	float scaleY =-2.0 / inHeight;\n"
		"\n"
		"	gl_Position = vec4( ((posX + inVert.x) * scaleX) - 1.0, ((posY + inVert.y) * scaleY) + 1.0, 0.0, 1.0);\n"
		"	UV =uvCoord;\n"
		"}\n"
		"\n";
const string GLPrint::_fragmentShader ="#version 330 core\n"
		"\n"
		"out vec4 color;\n"
		"in vec2 UV;\n"
		"\n"
		"uniform sampler2D Texture;\n"
		"uniform vec3 textColor;\n"
		"\n"
		"void main() {\n"
		"	color.rgb =textColor;\n"
		"	color.a =texture(Texture, UV).a;\n"
		"}\n"
		"\n";

GLPrint::GLPrint(const string& font, int size, int w, int h) {
	this->_height =h;
	this->_width =w;
	this->_size =size;
	this->_max_width =this->_max_height =0;
	this->_vertexID =0;
	this->_vertexBuffer =this->_uvBuffer =0;
	this->_extra_GlyphID =0;
	this->_extra_vertexBuffer =this->_extra_uvBuffer =0;

	this->_texture =NULL;
	this->_shader =NULL;

	this->_font =font;

	createTexture();

	_shader =new Shader(_vertexShader, _fragmentShader, false);

	cout <<_shader->getErrorLog() <<endl;
}

void GLPrint::createTexture() {
	int error;
	FT_Library library;
	FT_Face face;

	error =FT_Init_FreeType(&library);
	if(error) {
		 throw("FT_Init_FreeType()");
	}

	error =FT_New_Face(library, _font.c_str(), 0, &face);
	if(error ==FT_Err_Unknown_File_Format) {
		throw("FT_New_Face() unknown file format");
	}
	else if(error) {
		throw("FT_New_Face()");
	}

	FT_CharMap found =0;
	FT_CharMap charmap;
	int n;
	for(n =0;n <face->num_charmaps;n++) {
		charmap =face->charmaps[n];
		if((charmap->platform_id ==TT_PLATFORM_MICROSOFT && charmap->encoding_id ==ft_encoding_unicode) ||
				(charmap->platform_id ==TT_PLATFORM_MACINTOSH && charmap->encoding_id ==ft_encoding_none)) {
			found =charmap;
			break;
		}
	}
	if(!found) {
		throw("FT_CharMap(): No Encoding found");
	}

	error =FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	if(error) {
		throw("FT_Select_Charmap()");
	}

	error =FT_Set_Pixel_Sizes(face, 0, _size);
	if(error) {
		throw("FT_Set_Pixel_Sizes()");
	}

	vector<unsigned long> buffer;
	for(int i =0;i <face->num_glyphs;i++)
		buffer.push_back(i);

	_max_width =0;
	_max_height =0;

	_num_glyphs =face->num_glyphs;
	for(int i =0;i <_num_glyphs;i++) {
		error =FT_Load_Char(face, buffer[i], FT_LOAD_RENDER);
		if(error) {
			continue;
		}

		glyph_info tmp;
		tmp.width =face->glyph->bitmap.width;
		tmp.height =face->glyph->bitmap.rows;
		tmp.buffer =new unsigned char[tmp.width * tmp.height * 4];
		tmp.advance_x =face->glyph->advance.x >>6;
		tmp.advance_y =face->glyph->advance.y >>6;
		tmp.top =face->glyph->bitmap_top;
		tmp.left =face->glyph->bitmap_left;

		memset(tmp.buffer, 0, tmp.width * tmp.height * 4);
		/*for(unsigned int i =0;i <tmp.width * tmp.height * 4;i++) {
			tmp.buffer[i] =face->glyph->bitmap.buffer[i/4];
		}*/
		for(int y =tmp.height-1;y >=0;y--) {
			for(unsigned int x =0;x <tmp.width;x++) {
				int real_y =(tmp.height - 1) - y;
				/*tmp.buffer[(x + (real_y * tmp.width)) * 4 + 0] =face->glyph->bitmap.buffer[x + (y * tmp.width)];
				tmp.buffer[(x + (real_y * tmp.width)) * 4 + 1] =face->glyph->bitmap.buffer[x + (y * tmp.width)];
				tmp.buffer[(x + (real_y * tmp.width)) * 4 + 2] =face->glyph->bitmap.buffer[x + (y * tmp.width)];*/
				tmp.buffer[(x + (real_y * tmp.width)) * 4 + 3] =face->glyph->bitmap.buffer[x + (y * tmp.width)]; //nur Alpha-Kanal
			}
		}

		_glyphs.push_back(tmp);

		_max_width =(_max_width >tmp.width ? _max_width : tmp.width);
		_max_height =(_max_height >tmp.height ? _max_height : tmp.height);
	}

	_glyphs[' '].width =_max_width;
	_glyphs[' '].height =_max_height;
	delete _glyphs[' '].buffer;
	_glyphs[' '].buffer =new unsigned char[_glyphs[' '].width * _glyphs[' '].height * 4];
	memset(_glyphs[' '].buffer, 0, _glyphs[' '].width * _glyphs[' '].height * 4);

	_texture =new GLTexture(_max_width * 16, _max_height * 16);

	for(int i =0;i <256;i++) {

		float uvX =(i % 16) / 16.0f;
		float uvY =1.0f - ((i / 16) / 16.0f);
		float uvHeight =_glyphs[i].height / (float)_texture->GetHeight();
		float uvWidth =_glyphs[i].width / (float)_texture->GetWidth();

		uvY -=(_max_height - _glyphs[i].height) / (float)_texture->GetHeight();
		//uvY -=1 / (float)texture->GetWidth();

		_uv_buffer_data[i * (2 * 6) + 0] =uvX;
		_uv_buffer_data[i * (2 * 6) + 1] =uvY;
		_uv_buffer_data[i * (2 * 6) + 2] =uvX;
		_uv_buffer_data[i * (2 * 6) + 3] =uvY - uvHeight;
		_uv_buffer_data[i * (2 * 6) + 4] =uvX + uvWidth;
		_uv_buffer_data[i * (2 * 6) + 5] =uvY - uvHeight;

		_uv_buffer_data[i * (2 * 6) + 6] =uvX;
		_uv_buffer_data[i * (2 * 6) + 7] =uvY;
		_uv_buffer_data[i * (2 * 6) + 8] =uvX + uvWidth;
		_uv_buffer_data[i * (2 * 6) + 9] =uvY - uvHeight;
		_uv_buffer_data[i * (2 * 6) + 10] =uvX + uvWidth;
		_uv_buffer_data[i * (2 * 6) + 11] =uvY;

		_vertex_buffer_data[i * (2 * 6) + 0] =0.0f;
		_vertex_buffer_data[i * (2 * 6) + 1] =0.0f;

		_vertex_buffer_data[i * (2 * 6) + 2] =0.0f;
		_vertex_buffer_data[i * (2 * 6) + 3] =1.0f * _glyphs[i].height;

		_vertex_buffer_data[i * (2 * 6) + 4] =1.0f * _glyphs[i].width;
		_vertex_buffer_data[i * (2 * 6) + 5] =1.0f * _glyphs[i].height;

		_vertex_buffer_data[i * (2 * 6) + 6] =0.0f;
		_vertex_buffer_data[i * (2 * 6) + 7] =0.0f;

		_vertex_buffer_data[i * (2 * 6) + 8] =1.0f * _glyphs[i].width;
		_vertex_buffer_data[i * (2 * 6) + 9] =1.0f * _glyphs[i].height;

		_vertex_buffer_data[i * (2 * 6) + 10] =1.0f * _glyphs[i].width;
		_vertex_buffer_data[i * (2 * 6) + 11] =0.0f;

		int x =i % 16, y =15 - (i / 16);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x * _max_width, (y * _max_height), _glyphs[i].width, _glyphs[i].height, GL_RGBA, GL_UNSIGNED_BYTE, _glyphs[i].buffer);
	}

	glGenVertexArrays(1, &_vertexID);
	glBindVertexArray(_vertexID);

	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertex_buffer_data), _vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_uv_buffer_data), _uv_buffer_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   2,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
}

void GLPrint::print(int x, int y, string text, glm::vec3 color) {
	_shader->Use();
	glUniform1f(glGetUniformLocation(_shader->getHandle(), "posY"), y);
	glUniform3fv(glGetUniformLocation(_shader->getHandle(), "textColor"), 3, glm::value_ptr(color));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glUniform1f(glGetUniformLocation(_shader->getHandle(), "inWidth"), _width);
	glUniform1f(glGetUniformLocation(_shader->getHandle(), "inHeight"), _height);

	Poco::UTF8Encoding utf8;
	string utf8string;
	Poco::TextConverter converter(utf8, utf8);
	converter.convert(text, utf8string);
	Poco::TextIterator it(utf8string, utf8);
	Poco::TextIterator end(utf8string);

	for(;it !=end;it++) {
		unsigned int unicode =*it;

		glUniform1f(glGetUniformLocation(_shader->getHandle(), "posX"), x);
		glUniform1f(glGetUniformLocation(_shader->getHandle(), "posY"), y + (_max_height - _glyphs[/*text[i]*/unicode].top));

		if(unicode >255) {
			//cout <<"Unicode Character" <<endl;
			if(_extra_glyphs.find(unicode) ==_extra_glyphs.end()) {
				//cout <<"noch nicht" <<endl;
				_extra_glyphs.insert(pair<unsigned int, GLTexture*>(unicode, new GLTexture(_glyphs[unicode].width, _glyphs[unicode].height)));
				_extra_glyphs[unicode]->Bind();

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _glyphs[unicode].width, _glyphs[unicode].height, GL_RGBA, GL_UNSIGNED_BYTE, _glyphs[unicode].buffer);

				_extra_glyphs[unicode]->saveToTGA("extra.tga");
			}
			if(_extra_GlyphID ==0) {
				glGenVertexArrays(1, &_extra_GlyphID);
			}
			if(_extra_vertexBuffer ==0) {
				glGenBuffers(1, &_extra_vertexBuffer);
			}
			if(_extra_uvBuffer ==0) {
				glGenBuffers(1, &_extra_uvBuffer);
			}
			glBindVertexArray(_extra_GlyphID);
			_extra_vertex_data[0] =0.0f;
			_extra_vertex_data[1] =_glyphs[unicode].height;
			_extra_vertex_data[2] =_glyphs[unicode].width;
			_extra_vertex_data[3] =0.0f;
			_extra_vertex_data[4] =0.0f;
			_extra_vertex_data[5] =0.0f;
			_extra_vertex_data[6] =0.0f;
			_extra_vertex_data[7] =_glyphs[unicode].height;
			_extra_vertex_data[8] =_glyphs[unicode].width;
			_extra_vertex_data[9] =_glyphs[unicode].height;
			_extra_vertex_data[10] =_glyphs[unicode].width;
			_extra_vertex_data[11] =0.0f;

			_extra_uv_data[0] =0.0f;
			_extra_uv_data[1] =0.0f;
			_extra_uv_data[2] =1.0f;
			_extra_uv_data[3] =1.0f;
			_extra_uv_data[4] =0.0f;
			_extra_uv_data[5] =1.0f;
			_extra_uv_data[6] =0.0f;
			_extra_uv_data[7] =0.0f;
			_extra_uv_data[8] =1.0f;
			_extra_uv_data[9] =0.0f;
			_extra_uv_data[10] =1.0f;
			_extra_uv_data[11] =1.0f;

			glBindBuffer(GL_ARRAY_BUFFER, _extra_vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(_extra_vertex_data), _extra_vertex_data, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, _extra_uvBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(_extra_uv_data), _extra_uv_data, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, _extra_vertexBuffer);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, _extra_uvBuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			_extra_glyphs[unicode]->Bind();

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			glBindBuffer(GL_VERTEX_ARRAY, 0);
			glBindVertexArray(0);
		}
		else {
			_texture->Bind();
			glBindVertexArray(_vertexID);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glDrawArrays(GL_TRIANGLES, /*text[i]*/unicode * 6, 6);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}

		x +=_glyphs[/*text[i]*/unicode].advance_x;
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);

	_texture->UnBind();
	glUseProgram(0);
}

void GLPrint::setScreenSize(int w, int h) {
	this->_width =w;
	this->_height =h;
}

void GLPrint::setFontSize(int size) {
	this->_size =size;
}

GLPrint::~GLPrint() {
	glDeleteVertexArrays(1, &_vertexID);
	glDeleteBuffers(1, &_vertexBuffer);
	glDeleteBuffers(1, &_uvBuffer);

	for(int i =0;i <_num_glyphs;i++)
		delete _glyphs[i].buffer;

	for(map<unsigned int, GLTexture*>::iterator it =_extra_glyphs.begin();it !=_extra_glyphs.end();it++) {
		delete it->second;
	}

	delete _texture;
	delete _shader;
}

} /* namespace Fumasu */
