#pragma once
#include <queue> 
#include <vector>

class Tree {
public:
	Tree();							// Default constructor
	Tree(std::vector<float> data, Tree *parent = nullptr, float depth = 0) {
		this->data = data;
		this->depth = depth;
		this-> parent = parent;
	}
	
	~Tree() {}				// Destructor
	std::vector<Tree> children;
	std::vector<float> data = {0, 0};
	Tree *parent;
	float depth = 0;

private:
	
};
