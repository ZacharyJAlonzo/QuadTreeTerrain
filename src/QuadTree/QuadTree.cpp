#include <iostream>
#include "Camera/Camera.h"
#include <GL/glew.h>
#include "QuadTree.h"


QuadTree::QuadTree(float _LeftX, float _RightX, float _TopZ, float _BottomZ, int _Height)
{
	TreeHeight = _Height;
	Nodes = new QuadTreeNode[CalculateNumberNodes(TreeHeight)];

	QuadTreeNode first;

	first.ID = 0;
	first.ParentID = 0;

	//4 ^ level divides landscape
	first.Level = 1;

	first.LeftX = _LeftX;
	first.RightX = _RightX;
	first.TopZ = _TopZ;
	first.BottomZ = _BottomZ;

	first.MaximumError = 64;

	CreateQuadTree(first);
	std::cout << "done" << std::endl;
}

void QuadTree::CreateQuadTree(QuadTreeNode& node)
{	
		static int NodeID = 1;
		//std::cout << NodeID << " ";

		QuadTreeNode* current = &Nodes[node.ID];
		//at the beginning of the recursion, the first node is not yet set
		//node ID is 0, fill the array
		
		current->ID = node.ID;
		current->ParentID = node.ParentID;
		current->LeftX = node.LeftX;
		current->RightX = node.RightX;
		current->TopZ = node.TopZ;
		current->BottomZ = node.BottomZ;
		current->Level = node.Level;
		current->MaximumError = node.MaximumError;

		if (node.Level == TreeHeight)
		{
			//leaf
			return;
		}
	

		QuadTreeNode NW;
		NW.ID = NodeID;
		NW.ParentID = current->ID;
		NW.Level = current->Level + 1;
		//bounds calculation
		NW.LeftX = current->LeftX;
		NW.RightX = (current->RightX + current->LeftX) / 2.f;
		NW.TopZ = current->TopZ;
		NW.BottomZ = (current->TopZ + current->BottomZ) / 2.f;
		NW.MaximumError = current->MaximumError / 2;

		current->children[0] = NW.ID;
		//
		NodeID++;
		CreateQuadTree(NW);

		QuadTreeNode NE;
		NE.ID = NodeID;
		NE.ParentID = current->ID;
		NE.Level = current->Level + 1;
		//bounds calculation
		NE.LeftX = (current->LeftX + current->RightX) / 2.f;
		NE.RightX = current->RightX;
		NE.TopZ = current->TopZ;
		NE.BottomZ = (current->TopZ + current->BottomZ) / 2.f;
		NE.MaximumError = current->MaximumError / 2;

		current->children[1] = NE.ID;
		//
		NodeID++;
		CreateQuadTree(NE);

		QuadTreeNode SE;
		SE.ID = NodeID;
		SE.ParentID = current->ID;
		SE.Level = current->Level + 1;
		//bounds calculation
		SE.LeftX = (current->LeftX + current->RightX) / 2.f;
		SE.RightX = current->RightX;
		SE.TopZ = (current->TopZ + current->BottomZ) / 2.f;
		SE.BottomZ = current->BottomZ;
		SE.MaximumError = current->MaximumError / 2;

		current->children[2] = SE.ID;
		//
		NodeID++;
		CreateQuadTree(SE);

		QuadTreeNode SW;
		SW.ID = NodeID;
		SW.ParentID = current->ID;
		SW.Level = current->Level + 1;
		//bounds calculation
		SW.LeftX = current->LeftX;
		SW.RightX = (current->LeftX + current->RightX) / 2.f;
		SW.TopZ = (current->TopZ + current->BottomZ) / 2.f;
		SW.BottomZ = current->BottomZ;
		SW.MaximumError = current->MaximumError / 2;

		current->children[3] = SW.ID;
		//
		NodeID++;
		CreateQuadTree(SW);
}


int QuadTree::CalculateNumberNodes(int _Height)
{
	//formula for filled m-ary tree
	return (pow(4, (_Height + 1)) - 1) / 3;
}


//a visibility test will be performed before the actuall render
void QuadTree::Render(QuadTreeNode& node, Camera camera, float desiredError)
{
	float x1 = (node.RightX + node.LeftX) / 2.0f;
	float x2 = camera.CameraPos.x;
	//float y1 = 0.0;
	//float y2 = camera.CameraPos.y;
	float z1 = (node.TopZ + node.BottomZ) / 2.0f;
	float z2 = camera.CameraPos.z;
	
	float distance = abs(sqrt(pow((x2 - x1), 2) + pow((z2 - z1), 2)));

	//std::cout << "distance: " << distance << std::endl;

	float Error = abs((float)node.MaximumError / distance * (800.f / (2.0 * tan(45.f))));
	//std::cout << "Error: " << Error << std::endl;

	if (Error <= desiredError)
	{
		//std::cout << node.MaximumError << std::endl;
		//std::cout << "Error: " << Error << std::endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(node.LeftX, 0.f, node.TopZ);
		glVertex3f(node.RightX, 0.f, node.TopZ);
		glVertex3f(node.RightX, 0.f, node.BottomZ);
		glVertex3f(node.LeftX, 0.f, node.BottomZ);
		glVertex3f(node.LeftX, 0.f, node.TopZ);
		glEnd();
	}
	else
	{
		//std::cout << "else" << std::endl;
		if (node.Level != TreeHeight)
		{
			for (int x : node.children)
			{
				Render(Nodes[x], camera, desiredError);
			}
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(node.LeftX, 0.f, node.TopZ);
			glVertex3f(node.RightX, 0.f, node.TopZ);
			glVertex3f(node.RightX, 0.f, node.BottomZ);
			glVertex3f(node.LeftX, 0.f, node.BottomZ);
			glVertex3f(node.LeftX, 0.f, node.TopZ);
			glEnd();
		}
		
	}

}

void QuadTree::Render(Camera camera, float desiredError)
{
	Render(Nodes[0], camera, desiredError);
}
