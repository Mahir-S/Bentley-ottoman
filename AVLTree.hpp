#include "Node.hpp"

template <class T>
class AVLTree
{
	public :
	Node<T> * root;/**< root of subtree*/
	int _size = 0;/**< size of subtree*/
	
	AVLTree()
	{
		root = NULL;
		_size = 0;
	}
	/** balance the nodes in the vector path
	*@param path contains the nodes which might have to be balanced
	*/
	void balance(vector< Node<T> ** > path)
	{
		std::reverse(path.begin(),path.end());
		for (auto node : path)
		{
			
			(*node)->updateValues();
			if((*node)->balanceFactor() >= 2 and (*node)->left->balanceFactor() >= 0)//left-left
				(*node) = (*node)->right_rotate();
			else if((*node)->balanceFactor() <= -2 and (*node)->right->balanceFactor () <= 0)
				(*node) = (*node)->left_rotate();
			else if((*node)->balanceFactor() >= 2)//left-right
			{
				(*node)->left = (*node)->left->left_rotate();
				(*node) = (*node)->right_rotate();
			}
			else if ((*node)->balanceFactor() <= -2)//right-left
			{
				(*node)->right = (*node)->right->right_rotate();
				(*node) = (*node)->left_rotate();
			}

		}
	}
	/** insert a node
	*@param value the value to be inserted
	*/
	void insert(T value)
	{
		std::vector< Node<T>** > path; 
		Node<T> **  node = &root;

		while((*node) != NULL)
		{
			path.push_back(node);
			if((*node)->value > value)
				node = &((*node)->left);
			else
				node = &((*node)->right);
		}	
		*node = new Node<T>(value);

		path.push_back(node);

		balance(path);
		_size++;
	}
	/** erase a node
	*@param value the value to be erased
	*/
	void erase(T value)
	{
		Node<T> ** node = &root;
		std::vector< Node<T> ** > path;
		while(*node != NULL and (*node)->value != value)
		{
			path.push_back(node);
			// cout << (*node)->value.idx << endl;
			// cout << ((*node)->value > value) << endl;
			//cout << (*node)->value.intersection_with_sweep_line() << " " << value.intersection_with_sweep_line() << endl;
			if((*node) -> value > value)
				node = & (*node)->left;
			else
				node = & (*node)->right;
		}
		if(*node == NULL)
		{
			cout << "value not found";
			cout << value.idx << endl;
			assert(1 == 2);
			return ;//value not found
		}
		else
			path.push_back(node);
		
		
		int pathsize = path.size();//is it needed?
		
		if((*node)->left == NULL and (*node)->right == NULL)
		{
			delete *node;
			*node = NULL;
			path.pop_back();
		}
		else if((*node)->right == NULL)
		{
			Node<T> * toremove = *node;
			*node = (*node)->left;
			delete toremove;
			path.pop_back();
		}
		else
		{
			Node<T> ** successor  = & ((*node)->right);
			while((*successor)->left != NULL)
			{
				path.push_back(successor);
				successor = &((*successor)->left);
			}
			if(*successor == (*node)->right)
			{
				(*successor)->left = (*node)->left;
				Node<T> * toremove = *node;
				*node = *successor;
				delete toremove;
			}
			else
			{
				Node<T> *temp = *path.back(),*suc = *successor;
				//temp is the second last node
				temp->left = (*successor)->right;
				suc->left = (*node)->left;
				suc->right = (*node)->right;
				delete *node;
				*node = suc;
				path[pathsize] = & (suc->right);
			}

		}
		balance(path);
		_size--;
	}
	/** finding left neightbour
	*@param value the value whose left neighbour has to be found
	*@return the left neighbour
	*/
	T * find_left_neighbour(T value)
	{
		T* ans = NULL;
		Node<T> *cur = root;
		while(cur != NULL)
		{
			
			if(cur->value == value)
			{
				break;
			}

			if(cur-> value < value)
			{
				
				ans = &cur -> value;
				cur = cur -> right;
			}
			else
			{
				cur = cur -> left;
			}
		}
		if(cur != NULL)//find rightmost node in left subtree
		{
			cur = cur -> left;
			while(cur != NULL)
			{
				ans = &cur -> value;
				cur = cur -> right;
			}
		}
		return ans;
	}
	/** finding right neightbour
	*@param value the value whose right neighbour has to be found
	*@return the right neighbour value
	*/
	T* find_right_neighbour(T value)
	{
		T* ans = NULL;
		Node<T> *cur = root;
		std::vector< Node<T> ** > path;
		while(cur != NULL)
		{
			if(cur->value == value)
			{
				break;
			}
			if(cur-> value < value)
			{
				cur = cur -> right;
			}
			else
			{
				ans = &cur -> value;
				cur = cur -> left;
			}
		}
		if(cur != NULL)//find lefttmost node in right subtree
		{
			cur = cur -> right;
			while(cur != NULL)
			{
				ans = &cur -> value;
				cur = cur -> left;
			}
		}
		return ans;
	}
	/** finding left neightbour
	*@param value the value whose lower bound has to be found
	*@return the lower bound of value
	*/
	T* lower_bound(T value)//handle empty tree
	{
		Node<T> *cur = root;
		T* ans = NULL;
		while(cur != NULL)
		{
			if(cur->value < value)
			{
				cur = cur -> right;
				
			}
			else
			{
				ans = &cur->value;
				cur = cur->left;
			}
		}
		return ans;
	}
	/** get the minimum value in tree
	*@return the minimum value in tree
	*/
	T* get_min()
	{
		Node<T> *cur = root;
		T* ans = NULL;
		while(cur != NULL)
		{
			ans = &cur->value;
			cur = cur->left;
		}
		return ans;
	}

	T* upper_bound(T value)//handle empty tree
	{
		
		T* ans = NULL;
		
		Node<T> ** node = &root;
		while(*node != NULL)
		{
			if((*node) -> value > value)
			{
				ans = & (*node)->value;
				node = & (*node)->left;
			}
			else
				node = & (*node)->right;
		}
		return ans;
	}
	void upper_bound_and_erase(T value)
	{
		Node<T> ** node = &root;
		std::vector< Node<T> ** > path;
		int sz ;  
		while(*node != NULL)
		{
			path.push_back(node);

			// cout << ((*node)->value > value) << endl;
			//cout << (*node)->value.intersection_with_sweep_line() << " " << value.intersection_with_sweep_line() << endl;
			if((*node) -> value > value)
			{
				sz = path.size();
				node = & (*node)->left;
			}
			else
				node = & (*node)->right;
		}
		if(sz == 0)
			cout << "early fail";
		path.resize(sz);
		node = path[sz - 1];
		if(*node == NULL)
		{
			cout << "value not found in upper_bound_and_erase";
			cout << value.idx << endl;
			assert(1 == 2);
			return ;//value not found
		}
		//cerr << (*node)->value.idx << endl;
		// path.push_back(node);
		
		
		int pathsize = path.size();//is it needed?
		if((*node)->left == NULL and (*node)->right == NULL)
		{
			delete *node;
			*node = NULL;
			path.pop_back();
		}
		else if((*node)->right == NULL)
		{
			Node<T> * toremove = *node;
			*node = (*node)->left;
			delete toremove;
			path.pop_back();
		}
		else
		{
			Node<T> ** successor  = & ((*node)->right);
			while((*successor)->left != NULL)
			{
				path.push_back(successor);
				successor = &((*successor)->left);
			}
			if(*successor == (*node)->right)
			{
				(*successor)->left = (*node)->left;
				Node<T> * toremove = *node;
				*node = *successor;
				delete toremove;
			}
			else
			{
				Node<T> *temp = *path.back(),*suc = *successor;
				//temp is the second last node
				temp->left = (*successor)->right;
				suc->left = (*node)->left;
				suc->right = (*node)->right;
				delete *node;
				*node = suc;
				path[pathsize] = & (suc->right);
			}

		}
		balance(path);
		_size--;
	}
	/** find if the value exists or not
	*@return 0 or 1
	*/
	bool find(T value)
	{
		Node<T> *cur = root;
		while(cur != NULL)
		{
			if(cur->value == value)
				return 1;
			if(cur->value < value)
			{
				cur = cur -> right;
			}
			else
			{
				cur = cur->left;
			}
		}
		return 0;
	}

	
	void display(Node<T> *cur,int depth = 0,int state = 0)
	{
		if (cur->left)
		        display(cur->left, depth + 1, 1);
		    
		    for (int i=0; i < depth; i++)
		        printf("     ");
		    
		    if (state == 1) // left
		        printf("┌───");
		    else if (state == 2)  // right
		        printf("└───");
		    
		    std::cout << "[" << cur->value.idx << "] - (" << cur->count << ", " << cur->height << ")" << std::endl;
		    
		    if (cur->right)
		display(cur->right, depth + 1, 2);
	}

	void display()
	{
		cout << endl;
		if(root != NULL)
			display(root);
		else
			cout << "Empty" << endl;

	}

};

//For debugging

// double Segment::sweepy = 1000000000;
// int main()
// {
// 	freopen("int.txt","r",stdin);

// 	// AVLTree<Segment> a;
// 	// Segment s1(Point(0,0),Point(1,1),0);
// 	// Segment s2(Point(1,0),Point(0,0.5),1);
// 	// s1.sweepy = 0.5;
// 	// a.insert_node(s1);
// 	// a.insert_node(s2);
// 	// cout << (s1 < s2);

// 	// a.display();
// 	// a.erase(s2);
// 	// a.erase(s1);
// 	// a.display();
// 	// s1.sweepy = 0;
// 	// a.insert_node(s1);
// 	// a.insert_node(s2);
// 	// a.display();
// 	// cout << (s1 < s2) << endl;
// 	// Segment ans = a.find_right_neighbour(s1);
// 	// cout << ans.idx << endl;
// 	// ans = a.find_left_neighbour(s1);
// 	// cout << ans.idx << endl;

// 	AVLTree<long long> tree;

//      tree.insert_node(1);
//     tree.insert_node(1);
//     tree.insert_node(2);
//     tree.insert_node(3);
//     tree.insert_node(1);
    
//     tree.display();
    
    


//     tree.erase(7);

    
    
//     tree.insert_node(2);
//     tree.display();
//     // long long * ans = tree.find_left_neighbour(2);
    
//     //     cout << *ans << endl;
//         cout << * tree.get_min();
//     //tree.display();



// }