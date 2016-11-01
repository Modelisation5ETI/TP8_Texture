// texture.cpp

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "texture.h"

/***************************************************************************\
 * Shader::Shader                                                          *
\***************************************************************************/
unsigned get_texture(const std::string &filename)
{
  std::string data_dir = DATA_DIR;
  data_dir.append( "/" );
  
  GLuint texture;
  int width, height;
  std::vector<char> data;
  std::ifstream stream( data_dir + filename );
  std::string buffer;

  std::getline(stream, buffer);
  if (buffer[0] == '#') std::getline(stream, buffer);
  std::getline(stream, buffer);
  if (buffer[0] == '#') std::getline(stream, buffer);
  std::stringstream ss(buffer);
  ss >> width >> height;
  data.resize(width * height * 3);
  std::getline(stream, buffer);
  if (buffer[0] == '#') std::getline(stream, buffer);
  stream.read(data.data(), width * height * 3);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



  return texture;
}

