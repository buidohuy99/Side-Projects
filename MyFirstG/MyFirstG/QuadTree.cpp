#include "QuadTree.h"
#include "Rect.h"

template <class objType>
QuadTree<objType>::QuadTree(void)
{
	tree = nullptr;
	init = false;
}

template <class objType>
QuadTree<objType>::~QuadTree(void)
{
	DestroyTree(tree);
}

template <class objType>
void QuadTree<objType>::createTree(glm::vec2 center ,glm::vec2 halfs, unsigned int height){
	if(init) return;
	init = true;
	tree = BuildTree(center,halfs,height);
	if(!init) DestroyTree(tree);
}

template <class objType>
TreeNode<objType>* QuadTree<objType>::BuildTree(glm::vec2 center ,glm::vec2 halfs, unsigned int height){
	if (height <= 0) return nullptr;
	TreeNode<objType>* current = new TreeNode<objType>;
	if(!current) {init = false; return nullptr;}
	current->center[0] = center.x;
	current->distToEdge[0] = halfs.x;
	current->center[1] = center.y;
	current->distToEdge[1] = halfs.y;
	
	glm::vec2 nextHalf = glm::vec2(halfs.x/2,halfs.y/2); 
	for(int i=0;i<4;i++){
		glm::vec2 nCenter(center);
		nCenter.x += (i&1)? nextHalf.x : -nextHalf.x;
		nCenter.y += (i&2)? nextHalf.y : -nextHalf.y;
		current->child[i] = BuildTree(nCenter,nextHalf,height - 1);
	}

	return current;
}

template <class objType>
void QuadTree<objType>::DestroyTree(TreeNode<objType>*& current){
	for(int i=0;i<4;i++){
		if(current->child[i]) DestroyTree(current->child[i]);	
	}
	TreeNode<objType>* temp = current;
	delete temp;
	current = nullptr;
}

template <class objType>
void QuadTree<objType>::clear(){
	if(!init) return;
	ClearTree(tree);
}

template <class objType>
void QuadTree<objType>::ClearTree(TreeNode<objType>* current){
	current->objList.clear();
	for(int i=0;i<4;i++){
		if(current->child[i]) ClearTree(current->child[i]);	
	}
}

template <class objType>
void QuadTree<objType>::insertObj(Rect objRect, objType actualObj){
	if (!init) return;
	InsertTree(tree,objRect,actualObj);
}

template <class objType>
void QuadTree<objType>::InsertTree(TreeNode<objType>* current,Rect objRect, objType actualObj){
	float halfspan[2] = {objRect.span.x/2,objRect.span.y/2};
	float center[2] = {objRect.position.x + halfspan[0],objRect.position.y+ halfspan[1]};
	bool straddle = false;
	int currentQuadTreeCell = 0;
	for(int i = 0 ; i < 2 ; i++){
		float deltaSide = center[i] - current->center[i];
		if(abs(deltaSide) < halfspan[i] + current->distToEdge[i]){
			straddle = true;
			break;
		}
		if(deltaSide > 0.0f) currentQuadTreeCell |= (1 << i);
	}
	//if 
	if(!straddle && current->child[currentQuadTreeCell]) {
		
	}
}