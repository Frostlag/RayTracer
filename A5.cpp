#include <glm/ext.hpp>
#include <thread>
#include <vector>

#include "A5.hpp"
#include "GeometryNode.hpp"
#include "BlockList.hpp"
#include "Window.hpp"
#include "RenderThread.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <GL/glut.h>
using namespace std;
using namespace glm;

extern int subdivisions;
extern int threads;
extern int cwindow;
bool super = false;
int supersub = 4;
float reflectionThreshold = 0.1;
float myEpsilon = 0.1;
BlockList *blockList;
PhongMaterial defaultMat(vec3(0.2,0.2,0.2),vec3(0.1,0.1,0.1),40,0);


void A5_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {
	double aspect;
	aspect = (float)image.width() / (float)image.height();
	float n = (image.height() / 2) / tan(radians((float)fovy/2));
	float gridHeight = image.height() / n;
	float gridWidth = image.width() / n;

	vec3 u = (normalize(up * gridHeight)) / image.height();
	vec3 r = (normalize(cross(up,vec3(0,0,n)) * gridWidth) / image.width());

	int h = image.height();
	int w = image.width();

	//gl3wInit();
	//GLuint FramebufferName = 0;
	//glGenFramebuffers(1, &FramebufferName);
	//glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	//glViewport(0,0,w,h);
	//GLUquadric* quadric = gluNewQuadric();
	//gluSphere(quadric, 10,10,10);
	//glFlush();
	//glFinish();
	//CHECK_GL_ERRORS;



	Window *window;

	if (cwindow > 0){
		window = new Window(h, w, image);
	}

	mat4 V = lookAt(eye, view, up);
	mat4 invV = inverse(V);
	//cout << to_string(V) << endl;
	//cout << to_string(invV) << endl;

	vec3 baseZ = vec3(0,0,-1);
	if (h % 2 == 0){
		baseZ = baseZ + u*0.5;
	}
	if (w % 2 == 0){
		baseZ = baseZ + r*0.5;
	}

	vec4 E = vec4((float)eye.x,eye.y,eye.z,1);

	//cout << -h/2 << " " << h/2 << endl;

	blockList = new BlockList(h, w, subdivisions);
	root->reboundVolume();

	vector<RenderThread*> threadvector;
	for (int i = 0; i < threads; i++){
		RenderThread* temp =  new RenderThread(root, &image, eye, view, up, fovy, ambient, lights, E, baseZ, u, r, invV);

		threadvector.push_back(temp);
	}
	cout << "Threads: " << threadvector.size() << endl;
	if (cwindow >= 1){
		while(!window->isClosed()){
			if (!RenderThread::WorkingThreads.empty()){
				blockList->outputProgress();
			}
			window->tick();
			this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}else{
		while(!RenderThread::WorkingThreads.empty()){
			blockList->outputProgress();
			this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}

	for(int i = 0; i < threadvector.size(); i++){
		delete threadvector[i];
	}
	delete blockList;
	if (cwindow > 0){
		delete window;
	}

}
