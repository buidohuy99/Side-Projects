#include "SpriteBatch.h"
#include "Camera2D.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Rect.h"
#include <algorithm>

bool CompareTexture(InfoQuad* a,InfoQuad* b){
	if(Texture2D::compareByIDSmaller(a->texture,b->texture)) return true;
	else
	{
		if(Texture2D::compareByIDDifferent(a->texture, b->texture)) return false;
		else {
			if(a->depth > b->depth) return true;
			return false;
		}
	}
}

void InfoQuad::Init(const Rect& destRect, const Rect& texRect, bool flip, Texture2D* tex,float Depth, const ColorRGBA8& color){
	texture=tex;
	depth=Depth;
	
	float flipTrue=(flip==true)? texRect.span.width : 0.0f;
	//Top Left
	TopLeft.setColor(color.r,color.g,color.b,color.a);
	TopLeft.setPosition(destRect.position.x,
						destRect.position.y + destRect.span.height,depth);
	TopLeft.setUV(texRect.position.x + flipTrue ,
				  texRect.position.y + texRect.span.height );
	//BL
	BottomLeft.setColor(color.r,color.g,color.b,color.a);
	BottomLeft.setPosition(destRect.position.x,destRect.position.y,depth);
	BottomLeft.setUV(texRect.position.x + flipTrue ,texRect.position.y);
	//BR
	BottomRight.setColor(color.r,color.g,color.b,color.a);
	BottomRight.setPosition(destRect.position.x + destRect.span.width,
							destRect.position.y,depth);
	BottomRight.setUV(texRect.position.x + texRect.span.width - flipTrue,
					  texRect.position.y);
	//TR
	TopRight.setColor(color.r,color.g,color.b,color.a);
	TopRight.setPosition(destRect.position.x + destRect.span.width,
						 destRect.position.y + destRect.span.height,depth);
	TopRight.setUV(texRect.position.x + texRect.span.width - flipTrue,
				   texRect.position.y + texRect.span.height);
}

void InfoQuad::ApplyViewMatrix(){
	//Apply view matrix
	glm::vec4 transform;
	glm::mat4 viewMat=Camera2D::instance()->getViewMatrix();
	//TL
	transform=glm::vec4(TopLeft.position.x,TopLeft.position.y,depth,1.0f);
	transform=viewMat*transform;
	TopLeft.setPosition(transform.x,transform.y,transform.z);
	//BL
	transform=glm::vec4(BottomLeft.position.x,BottomLeft.position.y,depth,1.0f);
	transform=viewMat*transform;
	BottomLeft.setPosition(transform.x,transform.y,transform.z);
	//BR
	transform=glm::vec4(BottomRight.position.x,BottomRight.position.y,depth,1.0f);
	transform=viewMat*transform;
	BottomRight.setPosition(transform.x,transform.y,transform.z);
	//TR
	transform=glm::vec4(TopRight.position.x,TopRight.position.y,depth,1.0f);
	transform=viewMat*transform;
	TopRight.setPosition(transform.x,transform.y,transform.z);
}

void InfoQuad::ScalePoints(const glm::vec2& scaleFactor){
	glm::mat4 scale(1.0f);
	scale=glm::scale(scale,glm::vec3(scaleFactor.x,scaleFactor.y,1.0f));
	glm::vec4 TL(TopLeft.position.x,TopLeft.position.y,depth,1.0f);
	TL=scale*TL;
	glm::vec4 BL(BottomLeft.position.x,BottomLeft.position.y,depth,1.0f);
	BL=scale*BL;
	glm::vec4 BR(BottomRight.position.x,BottomRight.position.y,depth,1.0f);
	BR=scale*BR;
	glm::vec4 TR(TopRight.position.x,TopRight.position.y,depth,1.0f);
	TR=scale*TR;
	//Set new positions
	TopLeft.setPosition(TL.x,TL.y,TL.z);
	BottomLeft.setPosition(BL.x,BL.y,BL.z);
	BottomRight.setPosition(BR.x,BR.y,BR.z);
	TopRight.setPosition(TR.x,TR.y,TR.z);
}

void InfoQuad::RotatePoints(float angle){
	//Rotate Points
	glm::mat4 rotate (1.0f);
	rotate=glm::rotate(rotate,angle,glm::vec3(0.0f,0.0f,1.0f));
	glm::vec4 TL(TopLeft.position.x,TopLeft.position.y,depth,1.0f);
	TL=rotate*TL;
	glm::vec4 BL(BottomLeft.position.x,BottomLeft.position.y,depth,1.0f);
	BL=rotate*BL;
	glm::vec4 BR(BottomRight.position.x,BottomRight.position.y,depth,1.0f);
	BR=rotate*BR;
	glm::vec4 TR(TopRight.position.x,TopRight.position.y,depth,1.0f);
	TR=rotate*TR;
	//Set new positions
	TopLeft.setPosition(TL.x,TL.y,TL.z);
	BottomLeft.setPosition(BL.x,BL.y,BL.z);
	BottomRight.setPosition(BR.x,BR.y,BR.z);
	TopRight.setPosition(TR.x,TR.y,TR.z);
}

InfoQuad::InfoQuad(const Rect& destRect,const Rect& texRect,Texture2D* tex,float Depth, const ColorRGBA8& color, 
				   float angle, const glm::vec2& scaleFactor, bool flip){
	Init(destRect,texRect,flip,tex,Depth,color);
	if (scaleFactor!=glm::vec2(1.0f,1.0f)) ScalePoints(scaleFactor);
	if (unsigned int(floor(angle))%360!=0) RotatePoints(angle);
	ApplyViewMatrix();
}

SpriteBatch::SpriteBatch():_vbo(0),_vao(0){

}

SpriteBatch::~SpriteBatch(){
	destroy();
}

void SpriteBatch::init(){
	createVertexArray();
}

void SpriteBatch::destroy(){
	if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }
}

void SpriteBatch::begin(){
	_quads.clear();
	_pointersQuad.clear();
	batches.clear();
}
	
void SpriteBatch::draw(const Rect& destRect,const Rect& texRect,bool flipX,Texture2D* tex,float Depth, const ColorRGBA8& color,
	float angle, const glm::vec2& scaleFactor){
		_quads.emplace_back(InfoQuad(destRect,texRect,tex,Depth,color,angle,scaleFactor,flipX));
}

void SpriteBatch::end(){
	_pointersQuad.resize(_quads.size());
	for(unsigned int i=0;i<_quads.size();i++){
		_pointersQuad[i]=&(_quads[i]);
	}
	sortInfoQuads();
	createRenderBatches();
}

void SpriteBatch::createRenderBatches(){
	 // This will store all the vertices that we need to upload
    std::vector <Vertex> vertices;
    // Resize the buffer to the exact size we need so we can treat
    // it like an array
    vertices.resize(_pointersQuad.size() * 6);

    if (_pointersQuad.empty()) {
        return;
    }
	//Vertices overally 
	unsigned int numOfVertices=0;
	//Vertices per render batch (by texture)
	unsigned int vertexPerBatch=0;
	unsigned int currentVertex=0;
	//Divide to batches
	for(unsigned int i=0;i<_pointersQuad.size();i++){
		vertexPerBatch +=6;
		if(i==0){
			batches.emplace_back(RenderBatch(_pointersQuad[i]->texture,0,vertexPerBatch));
		}else
		{			
			if(Texture2D::compareByIDDifferent(_pointersQuad[i]->texture,_pointersQuad[i-1]->texture)){
				vertexPerBatch=0;
				batches.emplace_back(RenderBatch(_pointersQuad[i]->texture,numOfVertices,vertexPerBatch+6));
			}else
			{
				batches.back().amount = vertexPerBatch;
			}
		}
		vertices[currentVertex++]=_pointersQuad[i]->TopLeft;
		vertices[currentVertex++]=_pointersQuad[i]->BottomLeft;
		vertices[currentVertex++]=_pointersQuad[i]->BottomRight;
		vertices[currentVertex++]=_pointersQuad[i]->BottomRight;
		vertices[currentVertex++]=_pointersQuad[i]->TopRight;
		vertices[currentVertex++]=_pointersQuad[i]->TopLeft;
		numOfVertices +=6;
	}
	glBindBuffer(GL_ARRAY_BUFFER,_vbo);
    // Orphan the buffer (for speed)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), NULL, GL_STREAM_DRAW);
    // Upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void SpriteBatch::renderBatch(){
	glBindVertexArray(_vao);
	
	for(unsigned int i=0;i<batches.size();i++){
		glActiveTexture(GL_TEXTURE0);
		batches[i].texture->Bind();
		glDrawArrays(GL_TRIANGLES,batches[i].start,batches[i].amount);
	}

	glBindVertexArray(0);
}

void SpriteBatch::sortInfoQuads(){
  std::sort(_pointersQuad.begin(), _pointersQuad.end(), CompareTexture);
}

void SpriteBatch::createVertexArray(){
	if(_vao==0){
		glGenVertexArrays(1,&_vao);
	}
	if(_vbo==0){
		glGenBuffers(1,&_vbo);
	}

	glBindVertexArray(_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
	glVertexAttribPointer(1,4,GL_UNSIGNED_BYTE,GL_TRUE,sizeof(Vertex),(void*)sizeof(Position));
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(sizeof(Position)+sizeof(ColorRGBA8)));

	glBindVertexArray(0);

	
}