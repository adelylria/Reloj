# CMakeList.txt: proyecto de CMake para Reloj, incluya el origen y defina
# la lógica específica del proyecto aquí.
#

# Agregue un origen al ejecutable de este proyecto.
add_executable (Reloj WIN32  "src/main.cpp" "src/Reloj.cpp" "src/reloj.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Reloj PROPERTY CXX_STANDARD 20)
endif()

# TODO: Agregue pruebas y destinos de instalación si es necesario.
