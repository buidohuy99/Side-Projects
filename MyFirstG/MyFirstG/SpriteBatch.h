
/*CREDIT TO BENJAMIN ARNOLD'S SpriteBatch class*/

#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <queue>
#include "Vertex.h"

struct Rect;
class Texture2D;

class InfoQuad{
public:
	InfoQuad(const Rect& destRect,const Rect& texRect,Texture2D* tex,float Depth, const ColorRGBA8& color, 
				   float angle, const glm::vec2& scaleFactor,bool flip);
	Vertex TopLeft;
	Vertex BottomLeft;
	Vertex BottomRight;
	Vertex TopRight;

	Texture2D* texture;
	float depth;
private:
	void Init(const Rect& destRect,const Rect& texRect,bool flip,Texture2D* tex,float Depth, const ColorRGBA8& color);
	void ApplyViewMatrix();
	void RotatePoints(float angle);
	void ScalePoints(const glm::vec2& scaleFactor);
};

class RenderBatch{
public:
	RenderBatch(Texture2D* texTure,unsigned int S,unsigned int Amount):
		texture(texTure),start(S),amount(Amount)	
	{
		
	}

	Texture2D* texture;
	unsigned int start;
	unsigned int amount;
};

class SpriteBatch{
public:
	SpriteBatch();
	~SpriteBatch();

	void init();

	void begin();
	/*
	destRect is Rectangle bounding object with Origin on bottomLeft & width,height as Span from that Origin
	=> Therefore, you must calculate bottomLeft of an Object from its ScreenCoords (which is based on Center of that object)
	* Note: Object is thought to be in its original state, with out scaling, rotating when getting destRect
	=> Scaling and Rotating is done when rendering/changing Collision Shape through angle, scaleFactor variable
	* Note: destRect must be in normalized device coordinates(NDC) before passed in 
	* Note: you can get NDC by take Coords and divide by corresponding SCREEN_WIDTH/SCREEN_HEIGHT
	* Note: It's recommended to make function:  void getBottomLeftCoords() in GameObject class
	*/
	void draw(const Rect& destRect,const Rect& texRect, bool flipX, Texture2D* tex,float Depth, const ColorRGBA8& color,
			  float angle, const glm::vec2& scaleFactor);
	void end();
	
	void renderBatch();

	void destroy();
private:
	void sortInfoQuads();
	void createVertexArray();
	void createRenderBatches();
	
	GLuint _vbo,_vao;

	std::vector<RenderBatch> batches;
	std::vector<InfoQuad> _quads;
	std::vector<InfoQuad*> _pointersQuad;
};

#endif