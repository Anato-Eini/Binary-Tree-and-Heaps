#include "Kd_Tree.h"
#include <climits>

Kd_Tree::Kd_Tree() { Kd_Tree(1); }

Kd_Tree::Node::Node(const int point[], int size)
{
	this->point = new int[size];
	for (size_t i = 0; i < size; i++)
		this->point[i] = point[i];

	left = right = nullptr;
}

Kd_Tree::Node::Node(const std::vector<int> &point)
{
	const size_t sizeArray = point.size();
	this->point = new int[sizeArray];

	for (size_t i = 0; i < sizeArray; i++)
		this->point[i] = point[i];

	left = right = nullptr;
}

Kd_Tree::Node::~Node()
{
	delete point;
	delete right;
	delete left;
}

Kd_Tree::Kd_Tree(const int k_dimension) : root{nullptr}, size{0}
{
	if (k_dimension < 1)
		throw std::invalid_argument("Invalid k dimension");

	this->k_dimension = k_dimension;
}

void Kd_Tree::insertKey(const int point[])
{
	if (!root)
		root = new Node{point, k_dimension};
	else
	{
		Node *curr = root;

		for (int i = 0; true; i = ++i % k_dimension)
		{
			if (point[i] < curr->point[i])
			{
				if (curr->left)
					curr = curr->left;
				else
				{
					curr->left = new Node{point, k_dimension};
					break;
				}
			}
			else if (point[i] > curr->point[i])
			{
				if (curr->right)
					curr = curr->right;
				else
				{
					curr->right = new Node{point, k_dimension};
					break;
				}
			}
			else if (isEqual(curr->point, point))
				return;
			else if (curr->right)
				curr = curr->right;
			else
			{
				curr->right = new Node{point, k_dimension};
				break;
			}
		}
	}

	size++;
}

void Kd_Tree::insertKey(const std::vector<int> &point)
{
	if (!root)
		root = new Node{point};
	else
	{
		Node *curr = root;

		for (int i = 0; true; i = ++i % k_dimension)
			if (point[i] < curr->point[i])
			{
				if (curr->left)
					curr = curr->left;
				else
				{
					curr->left = new Node{point};
					break;
				}
			}
			else if (point[i] > curr->point[i])
			{
				if (curr->right)
					curr = curr->right;
				else
				{
					curr->right = new Node{point};
					break;
				}
			}
			else if (isEqual(curr->point, point))
				return;
			else if (curr->right)
				curr = curr->right;
			else
			{
				curr->right = new Node{point};
				break;
			}
	}
	size++;
}

void Kd_Tree::deleteKey(const int point[])
{
	if (!root)
		return;

	Node *parent = nullptr;
	Node *curr = root;
	int depth = 0;

	while (curr && !isEqual(curr->point, point))
	{
		parent = curr;
		int cd = depth % k_dimension;
		if (point[cd] < curr->point[cd])
			curr = curr->left;
		else
			curr = curr->right;
		depth++;
	}

	if (!curr)
		return;

	if (curr->right)
	{
		Node *minParent = curr;
		Node *min = curr->right;
		int minDepth = depth + 1;

		while (min->left)
		{
			minParent = min;
			min = min->left;
			minDepth++;
		}

		std::copy(min->point, min->point + k_dimension, curr->point);

		if (minParent->left == min)
			minParent->left = min->right;
		else
			minParent->right = min->right;

		delete min;
	}
	else if (curr->left)
	{
		Node *minParent = curr;
		Node *min = curr->left;
		int minDepth = depth + 1;

		while (min->right)
		{
			minParent = min;
			min = min->right;
			minDepth++;
		}

		std::copy(min->point, min->point + k_dimension, curr->point);

		if (minParent->right == min)
			minParent->right = min->left;
		else
			minParent->left = min->left;

		delete min;
	}
	else
	{
		if (!parent)
			root = nullptr;
		else if (parent->left == curr)
			parent->left = nullptr;
		else
			parent->right = nullptr;

		delete curr;
	}
}

bool Kd_Tree::isEqual(const int *point1, const int *point2) const
{
	for (int i = 0; i < k_dimension; i++)
		if (point1[i] != point2[i])
			return false;
	return true;
}

bool Kd_Tree::isEqual(const int *point1, const std::vector<int> &point2) const
{
	for (int i = 0; i < k_dimension; i++)
		if (point1[i] != point2[i])
			return false;
	return true;
}

Kd_Tree::Node *Kd_Tree::searchNode(const int *point) const
{
	if (!root)
		return nullptr;

	Node *curr = root;

	for (int i = 0; true; i = ++i % k_dimension)
	{
		if (point[i] < curr->point[i])
		{
			if (curr->left)
				curr = curr->left;
			else
				return nullptr;
		}
		else if (point[i] > curr->point[i])
		{
			if (curr->right)
				curr = curr->right;
			else
				return nullptr;
		}
		else if (isEqual(point, curr->point))
			return curr;
		else if (curr->right)
			curr = curr->right;
		else
			return nullptr;
	}
}

bool Kd_Tree::keyExist(const int *point) const { return searchNode(point); }

bool Kd_Tree::keyExist(const std::vector<int> &point) const
{
	if (root)
	{
		const Node *curr = root;

		for (int i = 0; true; i = ++i % k_dimension)
		{
			if (point[i] < curr->point[i])
			{
				if (curr->left)
					curr = curr->left;
				else
					return false;
			}
			else if (point[i] > curr->point[i])
			{
				if (curr->right)
					curr = curr->right;
				else
					return false;
			}
			else if (isEqual(curr->point, point))
				return true;
			else if (curr->right)
				curr = curr->right;
			else
				return false;
		}
	}
	return false;
}

#include <deque>

void Kd_Tree::print(std::ostream &ostream) const
{
	ostream << "Size: " << size << '\n';

	if (root)
	{
		std::deque<std::pair<int, Node *>> queue;
		queue.push_front({1, root});
		int depth = 0;

		while (!queue.empty())
		{
			std::pair<int, Node *> curr = queue.front();

			if (curr.first != depth)
				ostream << "\nDepth " << (depth = curr.first) << ": ";

			Node *node = curr.second;

			ostream << "| ";
			for (int i = 0; i < k_dimension; i++)
				ostream << node->point[i] << " ";
			ostream << "|";

			if (node->left)
				queue.push_back({depth + 1, node->left});
			if (node->right)
				queue.push_back({depth + 1, node->right});

			queue.pop_front();
		}

		ostream << '\n';
	}
}

int Kd_Tree::getMinimumRecursion(Node *node, int &dimension, int depth) const
{
	if (!node)
		return INT_MAX;

	if (dimension == depth % k_dimension + 1)
	{
		if (!node->left)
			return node->point[dimension - 1];
		return getMinimumRecursion(node->left, dimension, depth + 1);
	}
	else
		return std::min(node->point[dimension - 1],
						std::min(getMinimumRecursion(node->left, dimension, depth + 1),
								 getMinimumRecursion(node->right, dimension, depth + 1)));
}

int Kd_Tree::getMinimum(int &dimension) const
{
	if (dimension < 1 || dimension > k_dimension)
		throw std::invalid_argument("Invalid dimension");

	return getMinimumRecursion(root, dimension, 0);
}

std::ostream &operator<<(std::ostream &ostream, const Kd_Tree *tree)
{
	tree->print(ostream);

	return ostream;
}

std::ostream &operator<<(std::ostream &ostream, const Kd_Tree &tree)
{
	tree.print(ostream);

	return ostream;
}
