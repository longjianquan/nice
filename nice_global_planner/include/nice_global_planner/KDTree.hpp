#ifndef KD_TREE_HPP
#define KD_TREE_HPP
#include <iostream>
#include <vector>
#include <assert.h>
#include <map>
#include <limits.h>
#include <math.h>

#include "BinaryDecision.hpp"
#include "KDNode.hpp"


namespace nice_global_planner{

template<typename T>
class KDTree{
public:
	KDTree(unsigned dim)
	: dim(dim){
		root = NULL;
	}

	KDTree(unsigned dim, const std::vector<T>& initData)
	: dim(dim) {
		root = new KDNode<T>(initData,0);
	}

	~KDTree(void){
		this->pruneTree(this->root);
	}

	void insert(const std::vector<T> &data){
		if(root == NULL){
			root = new KDNode<T>(data, 0);
		} else {
			this->insertHelper(root, data);
		}
	}

	/**
	 * @brief Inserts a new node with data given
	 *  this method may be unsafe as it is expected
	 *  that the node is first found out with nearest
	 *  neigbourSearch or findNode method. This method
	 *  should be used with caution.
	 * @param node Node to start traversing from
	 * @param data New data to be inserted
	 */
	void insertFromNode(KDNode<T> *node, const std::vector<T> &data){
		if(node != NULL){
			this->insertHelper(node, data);
		}
	}

	void printTreeFromRoot(void){
		std::map<KDNode<T> *, bool> printed;
		printTree(this->root, printed);
	}

	KDNode<T> * findNode(const std::vector<T> &data){
		KDNode<T> searchNode = KDNode<T>::createTempNode(data);
		return findNodeHelper(root, const_cast<KDNode<T> *>(&searchNode));
	}


	KDNode<T> * nearestNeighbour(const std::vector<T> &data){
		KDNode<T> newNode = KDNode<T>::createTempNode(data);
		KDNode<T> *currentBest = root;
		double currentBestDistance = root->sqDistance(const_cast<KDNode<T> *> (&newNode));
		this->nearestNeighbour(root, &currentBest, currentBestDistance, const_cast<KDNode<T> *> (&newNode));
		return currentBest;
	}

	// void nearestNeighbourInsert(const std::vector<T> &data){
	// 	KDNode<T> *nearestNeighbour = this->nearestNeighbour(data);
	// 	if(!nearestNeighbour->equals(data)){
	// 		this->insertHelper(nearestNeighbour, data);
	// 	}
	// }

private:

	void nearestNeighbour(KDNode<T> *node, KDNode<T> **currentBest, double &currentBestDistance, const KDNode<T> *newNode){
		if(node == NULL){
			return;
		} else {
			double sqDistance = node->sqDistance(newNode);
			if(sqDistance < currentBestDistance){
				(*currentBest) = node;
				currentBestDistance = sqDistance;
			}
						
			BinaryDecision binaryDecision = node->getNextDirection(newNode);

			T newNodeSplitData = (*newNode).data[node->getSplittingAxis()];
			T nodeSplitData = node->getSplittingValue();
			if(binaryDecision == LEFT){
				if(newNodeSplitData - currentBestDistance <= nodeSplitData) {
					nearestNeighbour(node->left, currentBest, currentBestDistance, newNode);
				}

				if(newNodeSplitData + currentBestDistance > nodeSplitData) {
					nearestNeighbour(node->right, currentBest, currentBestDistance, newNode);
				}

			} else {
				if(newNodeSplitData + currentBestDistance > nodeSplitData) {
					nearestNeighbour(node->right, currentBest, currentBestDistance, newNode);
				}

				if(newNodeSplitData - currentBestDistance <= nodeSplitData) {
					nearestNeighbour(node->left, currentBest, currentBestDistance, newNode);
				}
			}
		}
	}


	KDNode<T> *findNodeHelper(KDNode<T> *node, const KDNode<T> *searchNode){
		if(node == NULL){
			return NULL;
		} else if(node->equals(searchNode)){
			return node;
		} else {
			BinaryDecision binaryDecision = node->getNextDirection(searchNode);
			if(binaryDecision == LEFT){
				return findNodeHelper(node->left, searchNode);
			} else {
				return findNodeHelper(node->right, searchNode);
			}
		}
	}

	void printTree(KDNode<T> *node, std::map<KDNode<T> *, bool> &printed){
		if(node == NULL){
			return;
		} else {
			if(printed.find(node) == printed.end()){
				printed[node] = false;
			} 			

			printTree(node->left, printed);

			if(printed[node] == false){
				node->printNode();
			}	

			printTree(node->right, printed);		
		}
	}

	void insertHelper(KDNode<T> *node, const std::vector<T>& data){
		BinaryDecision binaryDecision;
		if(node->getNextStorageNode(data) == NULL){
			binaryDecision = node->getNextDirection(data);
			if(binaryDecision == LEFT){
				if(!node->equals(data)){
					node->left = new KDNode<T>(data, node->getNextDecisionDim(this->dim));
					KDNode<T> *child = node->left;
					child->parent = node;
				}
			} else {
				if(!node->equals(data)){
					node->right = new KDNode<T>(data, node->getNextDecisionDim(this->dim));
					KDNode<T> *child = node->right;
					child->parent = node;
				}
			}
			return;
		} else {
			binaryDecision = node->getNextDirection(data);
			if(binaryDecision == LEFT){
				this->insertHelper(node->left, data);
			} else {
				this->insertHelper(node->right, data);
			}
		}
	}

	void pruneTree(KDNode<T> *node){
		if(node == NULL){
			return;
		} else {
			pruneTree(node->left);
			pruneTree(node->right);
			delete node;
		}
	}

	KDNode<T> *root;

private:
	unsigned dim;
};

}

#endif