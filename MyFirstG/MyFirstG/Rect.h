#pragma once

#include <glad/glad.h>

//The vertex definition
struct Rect{
    struct BottomLeft{
		float x;
		float y;
	};

	struct Span {
		float width;
		float height;
	};

    BottomLeft position;

    //UV texture coordinates.
    Span span;

    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }

    void setSpan(float w, float h) {
		span.width = w;
        span.height = h;
    }
};