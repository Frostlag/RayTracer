#pragma once
#include <list>
#include <thread>
#include <mutex>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include "Image.hpp"
#include "SceneNode.hpp"
#include "Light.hpp"

struct Changed{
    uint x, y;
    Changed(uint x, uint y):x(x), y(y){}
};

class RenderThread{
    bool done;
    std::thread thethread;
    SceneNode * root;
	Image * image;
	const glm::vec3 eye;
	const glm::vec3 view;
	const glm::vec3 up;
	double fovy;
	// Lighting parameters
	const glm::vec3 ambient;
	const std::list<Light *> lights;
	glm::vec4 E;
    glm::vec3 baseZ;
    glm::vec3 u;
    glm::vec3 r;
    glm::mat4 invV;

    void main();

    glm::vec3 generateBG(int x, int y, int width, int height);

    std::mutex lk;

    std::list<Changed> changes;

public:
    static std::list<RenderThread*> Threads;
    static std::list<RenderThread*> WorkingThreads;
    RenderThread(SceneNode * root, Image * image, const glm::vec3 eye, const glm::vec3 view, const glm::vec3 up, double fovy, glm::vec3 ambient, std::list<Light *> lights,
	       glm::vec4 E, glm::vec3 baseZ, glm::vec3 u, glm::vec3 r, glm::mat4 invV);
    ~RenderThread();
    bool isDone(){return done;}
    std::list<Changed> getChanges();

};
