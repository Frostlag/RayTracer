#include <iostream>
#include <string>
#include "scene_lua.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

int subdivisions = 2;
int threads = 1;
int cwindow = 1;

int main(int argc, char** argv)
{
  std::string filename = "Assets/hier.lua";
  if (argc >= 2) {
    filename = argv[1];
    if (argc >= 4){
        try{
            subdivisions = std::stoi(argv[2]);
            threads = std::stoi(argv[3]);
            if (argc >= 5)
                cwindow = std::stoi(argv[4]);
        }catch(...){}
    }
  }

  if (!run_lua(filename)) {
    std::cerr << "Could not open " << filename << std::endl;
    return 1;
  }
  //cout << to_string(min(vec3(0,1,0), vec3(1,0,0), vec3(0,0,1))) << endl;
}
