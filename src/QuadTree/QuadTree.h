#pragma once

class Camera;

enum NodeType
{
	Node, Leaf
};

struct VertexData
{
	float x;
	float z;
};

struct QuadTreeNode
{

friend class QuadTree;





private:
	int MaximumError;

	//array identifier
	int ID = -1;
	int ParentID;

	//which level of the tree?
	int Level;

	//stores ID of children
	int children[4];

	bool bIsLeaf;
	bool bIsVisible;

	//the bounds of this square in the quadtree
	float LeftX;
	float RightX;
	float TopZ;
	float BottomZ;

	//the center of this square
	float CenterX;
	float CenterZ;

	//VertexData points[3][3]; 		
};



class QuadTree
{
public:
	QuadTree(float _LeftX, float _RightX, float _TopZ, float _BottomZ, int _Height);
	
	void CreateQuadTree(QuadTreeNode& node);

	int CalculateNumberNodes(int _Height);

	void Render(QuadTreeNode& node, Camera camera, float desiredError);

	void Render(Camera camera, float desiredError);


private:
	
	//the maximum height for the tree, the maximum detail that will be generated
	int TreeHeight;

	//the container for the quadtree.
	QuadTreeNode* Nodes;


};