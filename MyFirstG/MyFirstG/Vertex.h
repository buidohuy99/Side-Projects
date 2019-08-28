#pragma once

#include <glad/glad.h>

struct Position {
	float x;
	float y;
	float z;
};

struct ColorRGBA8 {
    ColorRGBA8() : r(0), g(0), b(0), a(0) { }
    ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
        r(R), g(G), b(B), a(A) { }
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
};

struct UV {
    float u;
    float v;
};

//The vertex definition
struct Vertex{
    Position position;
    //4 bytes for r g b a color.
    ColorRGBA8 color;

    //UV texture coordinates.
    UV uvcoords;

    void setPosition(float x, float y,float z) {
        position.x = x;
        position.y = y;
		position.z = z;
    }

    void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }

    void setUV(float u, float v) {
		uvcoords.u= u;
        uvcoords.v = v;
    }
};