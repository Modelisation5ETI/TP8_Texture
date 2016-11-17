// main.cpp

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <sstream>
#include <iostream>

#include "shader.h"
#include "texture.h"
#include "vec3.h"



#define BUFFER_OFFSET(bytes) ((GLubyte*)nullptr + (bytes))

// Window size
static int width = 800, height = 600;

// Ground
static GLuint  ground_vao;
static GLuint  ground_vbos[2];
static Shader *ground_shader = nullptr;
// Stone
static GLuint  stone_vao;
static GLuint  stone_vbos[2];
static Shader *stone_shader = nullptr;
static unsigned stone_nb_indices;
static GLuint  stone_texture;
// Caustics
std::vector<GLuint> caustic_texture;
unsigned int current_caustic=0;

static float angle = 0;

/*=========================================================================*\
 * Load caustic textures                                                                    *
\*=========================================================================*/
void load_caustic_texture()
{
unsigned int nb_caustic = 32;
for (unsigned int i=0;i<nb_caustic;i++)
  {
  std::string filename="caust";
  std::stringstream caustic;
  caustic<<i;
  filename += caustic.str()+".ppm";
  caustic_texture.push_back( get_texture(filename) );
  }
}

/*=========================================================================*\
 * init                                                                    *
\*=========================================================================*/
static void init()
{
  { // Un sol blanc de 60 cm (x) par 40 cm (z), y up
    vec3 vertices[] =
    {
      vec3(-30, 0, -20), vec3(0, 1, 0),
      vec3( 30, 0, -20), vec3(0, 1, 0),
      vec3( 30, 0,  20), vec3(0, 1, 0),
      vec3(-30, 0,  20), vec3(0, 1, 0)
    };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
    glGenVertexArrays(1, &ground_vao);
    glBindVertexArray(ground_vao);
    glGenBuffers(2, ground_vbos);
    glBindBuffer(GL_ARRAY_BUFFER, ground_vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), BUFFER_OFFSET(sizeof(vec3)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ground_vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
    //
    ground_shader = new Shader();
    ground_shader->attach_vertex_from_file("ground.vert");
    ground_shader->attach_fragment_from_file("ground.frag");
    ground_shader->set_attributes({"position", "normal"});
    ground_shader->link();
    ground_shader->activate();
  }
  { // un caillou posé sur ce sol
    std::vector<vec3> vertices;
    std::vector<GLushort> indices;

    vertices.push_back(vec3(0,10,0));
    vertices.push_back(vec3(0,1,0));
    for (unsigned i = 1; i < 20; i ++)
    {
      for (unsigned j = 0; j < 20; j++)
      {
        // position
        vertices.push_back(vec3(10 * sin(i * M_PI / 20) * cos(j * 2.0 * M_PI / 20),
                                5  * cos(i * M_PI / 20) + 5,
                                10 * sin(i * M_PI / 20) * sin(j * 2.0 * M_PI / 20)));
        // normal
        vertices.push_back(vec3(sin(i * M_PI / 20) * cos(j * 2.0 * M_PI / 20),
                                cos(i * M_PI / 20),
                                sin(i * M_PI / 20) * sin(j * 2.0 * M_PI / 20)));
      }
    }
    vertices.push_back(vec3(0, 0, 0));
    vertices.push_back(vec3(0, -1, 0));

    // pôle nord
    for (unsigned i = 1; i < 20; i++)
    {
      indices.push_back(0);
      indices.push_back(i);
      indices.push_back(i + 1);
    }
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(20);

    // milieu
    for (unsigned i = 0; i < 18; i++)
    {
      for (unsigned j = 1; j < 20; j++)
      {
        indices.push_back(20*i + j);
        indices.push_back(20*i + j+1);
        indices.push_back(20*i + j+20);
        indices.push_back(20*i + j + 1);
        indices.push_back(20*i + j+20);
        indices.push_back(20*i + j+21);
      }
      indices.push_back(20*i+1);
      indices.push_back(20*i+20);
      indices.push_back(20*i+21);

      indices.push_back(20*i+20);
      indices.push_back(20*i+40);
      indices.push_back(20*i+21);
    }

    // pôle sud
    for (unsigned i = 1; i < 20; i++)
    {
      indices.push_back(vertices.size() - 1);
      indices.push_back(20*18 + i);
      indices.push_back(20*18 + i+1);
    }
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 2);
    indices.push_back(20*18+1);

    stone_nb_indices = indices.size();
    glGenVertexArrays(1, &stone_vao);
    glBindVertexArray(stone_vao);
    glGenBuffers(2, stone_vbos);
    glBindBuffer(GL_ARRAY_BUFFER, stone_vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), BUFFER_OFFSET(sizeof(vec3)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stone_vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), indices.data(), GL_STATIC_DRAW);
    //
    stone_shader = new Shader();
    stone_shader->attach_vertex_from_file("stone.vert");
    stone_shader->attach_fragment_from_file("stone.frag");
    stone_shader->set_attributes({"position", "normal"});
    stone_shader->link();
    stone_shader->activate();
    stone_shader->set("tex", 0);
    stone_shader->set("caustic_tex", 1);
    stone_texture = get_texture("stone.ppm");
  }

  load_caustic_texture();
}

/*=========================================================================*\
 * display_callback                                                        *
\*=========================================================================*/
static void display_callback()
{
  matrix projection, view;

  projection.set_perspective(15, static_cast<float>(width) / height, 1, 1000);
  view.set_look_at(vec3(-70, 40, -50), vec3(0,0,0), vec3(0,1,0));
  view.rotate(angle, vec3(0, 1, 0));

  glClearColor(0.5, 0.6, 0.7, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // Dessin du caillou
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, stone_texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, caustic_texture[current_caustic]);
  current_caustic=(current_caustic+1)%caustic_texture.size();
  glBindVertexArray(stone_vao);
  stone_shader->activate();
  stone_shader->set("projection", projection * view);
  glDrawElements(GL_TRIANGLES, stone_nb_indices, GL_UNSIGNED_SHORT, nullptr);

  // Dessin du sol
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindVertexArray(ground_vao);
  ground_shader->activate();
  ground_shader->set("projection", projection * view);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

  glDisable(GL_BLEND);

  glutSwapBuffers();
}

/*=========================================================================*\
 * reshape_callback                                                        *
\*=========================================================================*/
static void reshape_callback(int w, int h)
{
  width = w;
  height = h;
  glViewport(0, 0, w, h);
  glutPostRedisplay();
}

/*=========================================================================*\
 * keyboard_callback                                                       *
\*=========================================================================*/
static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {
    case 'q':
    case 'Q':
    case 27:
      exit(0);
  }
//  glutPostRedisplay();
}

/*=========================================================================*\
 * special_callback                                                        *
\*=========================================================================*/
static void special_callback(int key, int, int)
{
  switch (key)
  {
    case GLUT_KEY_LEFT:
      angle -= 1; break;
    case GLUT_KEY_RIGHT:
      angle += 1; break;
    case GLUT_KEY_UP:
      break;
    case GLUT_KEY_DOWN:
      break;
  }
//  glutPostRedisplay();
}

/*=========================================================================*\
 * timer_callback                                                          *
\*=========================================================================*/
static void timer_callback(int)
{
  glutTimerFunc(25, timer_callback, 0);
  glutPostRedisplay();
}

/***************************************************************************\
 * main                                                                    *
\***************************************************************************/
int main(int argc, char *argv[])
{
  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(width, height);
  glutCreateWindow("caillou");
  glutDisplayFunc(display_callback);
  glutReshapeFunc(reshape_callback);

  glutKeyboardFunc(keyboard_callback);
  glutSpecialFunc(special_callback);

  // Mise à jour automatique du rendu
  glutTimerFunc(25, timer_callback, 0);
  glewExperimental=true; glewInit();glGetError();

  init();

  glutMainLoop();
}
