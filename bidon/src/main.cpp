// main.cpp
// Fichier principal de l'exemple bidon

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "shader.h"
#include "texture.h"
#include "vec3.h"

#define BUFFER_OFFSET(bytes) ((GLubyte*)nullptr + (bytes))

static int width = 800, height = 600;
static GLuint  ground_vao;
static GLuint  ground_vbos[2];
static Shader *ground_shader = nullptr;

static GLuint  bidon_vao;
static GLuint  bidon_vbos[2];
static Shader *bidon_shader = nullptr;
static GLuint  bidon_decal_texture;
static GLuint  bidon_rust_texture;
static GLuint  bidon_brushed_texture;
static GLuint  bidon_shader_map_texture;

static unsigned bidon_nb_indices;

static GLfloat angle = 0.0f;

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
  { // un bidon posé sur ce sol
    // 20 x 10, 30 de haut
    struct Vertice
    {
      vec3 position;
      vec3 normal;
      vec2 tex_coords;
    };

    Vertice vertices[] =
    {
      // face avant
      { vec3(-10,  0, -5), vec3( 0, 0, -1), vec2(    0, 1) },
      { vec3( 10,  0, -5), vec3( 0, 0, -1), vec2(1./3., 1) },
      { vec3( 10, 30, -5), vec3( 0, 0, -1), vec2(1./3., 0) },
      { vec3(-10, 30, -5), vec3( 0, 0, -1), vec2(    0, 0) },
      // face droite
      { vec3( 10,  0, -5), vec3( 1, 0,  0), vec2(1./3., 1) },
      { vec3( 10, 30, -5), vec3( 1, 0,  0), vec2(1./3., 0) },
      { vec3( 10, 30,  5), vec3( 1, 0,  0), vec2(0.5,   0) },
      { vec3( 10,  0,  5), vec3( 1, 0,  0), vec2(0.5,   1) },
      // face arrière
      { vec3(-10,  0,  5), vec3( 0, 0,  1), vec2(5./6., 1) },
      { vec3( 10,  0,  5), vec3( 0, 0,  1), vec2(0.5,   1) },
      { vec3( 10, 30,  5), vec3( 0, 0,  1), vec2(0.5,   0) },
      { vec3(-10, 30,  5), vec3( 0, 0,  1), vec2(5./6., 0) },
      // face gauche
      { vec3(-10,  0, -5), vec3(-1, 0,  0), vec2(1,     1) },
      { vec3(-10, 30, -5), vec3(-1, 0,  0), vec2(1,     0) },
      { vec3(-10, 30,  5), vec3(-1, 0,  0), vec2(5./6., 0) },
      { vec3(-10,  0,  5), vec3(-1, 0,  0), vec2(5./6., 1) },
    };

    GLushort indices[] =
    {
      0, 2, 1, 0, 3, 2,
      4, 5, 6, 4, 6, 7,
      8, 9,10, 8,10,11,
      12,14,13,12,15,14
     //12,13,14,12,14,15
    };
    bidon_nb_indices = 24;

    glGenVertexArrays(1, &bidon_vao);
    glBindVertexArray(bidon_vao);
    glGenBuffers(2, bidon_vbos);
    glBindBuffer(GL_ARRAY_BUFFER, bidon_vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), BUFFER_OFFSET(offsetof(Vertice, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), BUFFER_OFFSET(offsetof(Vertice, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertice), BUFFER_OFFSET(offsetof(Vertice, tex_coords)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bidon_vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
    //
    bidon_shader = new Shader();
    bidon_shader->attach_vertex_from_file("bidon.vert");
    bidon_shader->attach_fragment_from_file("bidon.frag");
    bidon_shader->set_attributes({"position", "normal", "tex_coords"});
    bidon_shader->link();
    bidon_shader->activate();
    bidon_shader->set("decal_tex", 0);
    bidon_shader->set("rust_tex", 1);
    bidon_shader->set("brushed_tex", 2);
    bidon_shader->set("shader_map_tex", 3);

    bidon_decal_texture      = get_texture("decal.ppm");
    bidon_rust_texture       = get_texture("rust.ppm");
    bidon_brushed_texture    = get_texture("brushed.ppm");
    bidon_shader_map_texture = get_texture("shader_map.ppm");
  }
}

/*=========================================================================*\
 * display_callback                                                        *
\*=========================================================================*/
static void display_callback()
{
  matrix projection, view;

  projection.set_perspective(30, static_cast<float>(width) / height, 1, 1000);
  view.set_look_at(vec3(-70, 40, -50), vec3(0,15,0), vec3(0,1,0));
  view.rotate(angle, vec3(0, 1, 0));

  glClearColor(0.5, 0.6, 0.7, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  // Dessin du sol
  glBindVertexArray(ground_vao);
  ground_shader->activate();
  ground_shader->set("projection", projection * view);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

  // Dessin du bidon
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, bidon_decal_texture);
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, bidon_rust_texture);
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, bidon_brushed_texture);
  glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, bidon_shader_map_texture);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(bidon_vao);
  bidon_shader->activate();
  bidon_shader->set("projection", projection * view);
  glDrawElements(GL_TRIANGLES, bidon_nb_indices, GL_UNSIGNED_SHORT, nullptr);

  glutSwapBuffers();
}

/*=========================================================================*\
 * reshape_callback                                                        *
\*=========================================================================*/
static void reshape_callback(int w, int h)
{
  width = w; height = h;
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

  glutPostRedisplay();
}

/*=========================================================================*\
 * special_callback                                                       *
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

  glutPostRedisplay();
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
  glutCreateWindow("Bidon");
  glutDisplayFunc(display_callback);
  glutReshapeFunc(reshape_callback);

  glutKeyboardFunc(keyboard_callback);
  glutSpecialFunc(special_callback);

// Décommentez la ligne ci-dessous pour une mise à jour régulière du rendu.
//  glutTimerFunc(25, timer_callback, 0);
  
  glewExperimental=true; glewInit(); glGetError();

  init();

  glutMainLoop();
}
