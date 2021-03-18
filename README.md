Playground repo for my OpenGL excursions

## Learning

### Tutorials

 * https://learnopengl.com/
 * https://open.gl/
 * https://antongerdelan.net/opengl/
 * http://ogldev.atspace.co.uk/
 * http://www.opengl-tutorial.org/
 * (legacy) https://lazyfoo.net/tutorials/OpenGL/index.php
 * (legacy) http://www.glprogramming.com/red/

### Examples

 * https://github.com/zuck/opengl-examples
 * (legacy) https://cs.lmu.edu/~ray/notes/openglexamples/
 * (legacy) https://users.auth.gr/~voyatzis/CG/
 * (legacy) https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/examples.html
 * (legacy) https://www.opengl.org/archives/resources/code/samples/simple/

## Libraries

Having   active  development   except  if   mentioned  otherwise.    (In
parentheses year development started or first version if I couldn't find
it, and implementation language.)

### Loaders

 * GLEW, http://glew.sourceforge.net/ (2002)
 * gl3w, https://github.com/skaslev/gl3w (2010)
 * glad, https://github.com/Dav1dde/glad (2013)

### Window management and event loop

 * freeglut, http://freeglut.sourceforge.net/ (1999, C; cont. GLUT)
 * SDL2, http://www.libsdl.org/ (2013, C; cont. SDL, 1999)
 * GLFW, https://www.glfw.org/ (2010, C)
 * SFML, https://www.sfml-dev.org/ (2007, C++)

### Mathematics

 * GLM, https://glm.g-truc.net/ (2005, C++)
 * Eigen, https://eigen.tuxfamily.org/ (2006, C++)

### User interface

 * (inactive) GLUI, https://github.com/libglui/glui (2003, C++)
 * Dear ImGui, https://github.com/ocornut/imgui (2014, C++)
 * NanoGUI, https://github.com/wjakob/nanogui (2015, C++)

### Generic (not OGL-related)

 * Boost, https://www.boost.org/ (1999, C++)

## Notes

### glad sans web interface

```
git clone --depth=1 https://github.com/Dav1dde/glad
cd glad
python -m glad \
  --profile compatibility \
  --out-path out \
  --api gl=3.0 \
  --generator c \
  --spec gl \
  --reproducible
```
