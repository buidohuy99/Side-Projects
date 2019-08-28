#pragma once
#include <glm/glm.hpp>
#include <vector>
struct Rect;

template <class objType>
struct TreeNode{
	glm::vec2 center;
	glm::vec2 distToEdge;
	std::vector <objType> objList;
	TreeNode* child[4];
};

template <class objType>
class QuadTree
{
public:
	QuadTree(void);
	~QuadTree(void);
	void createTree(glm::vec2 center ,glm::vec2 halfs, unsigned int height);
	//Use objRect to determine how to insert the Obj
	void insertObj(Rect objRect, objType actualObj);
	void clear();
	bool getInit() {return init;};
private:
	TreeNode<objType>* tree;
	bool init;
	//Build tree
	TreeNode<objType>* BuildTree(glm::vec2 center ,glm::vec2 halfs, unsigned int height);
	void DestroyTree(TreeNode<objType>*&);
	void ClearTree(TreeNode<objType>*);
};

