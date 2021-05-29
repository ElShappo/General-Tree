#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <regex>
#include <deque>
#include <bits/stdc++.h>
#include "NodeException.hpp"

using namespace std;

template <class T>
class Node
{
    private:

        T value;
        vector<Node*> children;

        vector<int> parseInstructions(string instructions)
        {
            if (instructions.empty())
                throw NodeException("cannot parse empty instruction list.");

            vector<int> path;
            regex reg("[[[:digit:]]+]");
            smatch m;

            while (std::regex_search (instructions, m, reg))
            {
                for (auto x : m)
                {
                    //cout << x.str() << endl;
                    string temp = x.str().substr(1, x.str().size()-2);
                    path.push_back((stoi(temp)));
                    instructions = m.suffix().str();
                }
            }
            return path;
        }

        void movePtr(Node* &start, vector<int> path)
        {
            if (start->size() <= path[0])
                throw NodeException("path out of range (path doesn't exist)");

            start = start->children[path[0]];
            path.erase(path.begin());

            if (path.size() > 0)
                movePtr(start, path);
        }

        void LevelOrderTraversal(Node<T>* root, vector<Node*> & buffer, T* value = NULL)
        // Prints the n-ary tree level wise
        // when necessary, can also find all Nodes with a particular value
        {
            if (root == NULL)
                throw NodeException("corrupted root passed as an argument.");

            if (!buffer.empty() )
                throw NodeException("'LevelOrderTraversal' function requires passed buffer to be empty.");

            // Standard level order traversal code
            // using queue
            queue<Node *> q;  // Create a queue
            q.push(root); // Enqueue root
            while (!q.empty())
            {
                int n = q.size();

                // If this node has children
                while (n > 0)
                {
                    // Dequeue an item from queue and print it
                    Node* p = q.front();
                    q.pop();

                    if (value != NULL)
                    {
                        if (p->value == *value)
                            buffer.push_back(p);
                    }
                    else
                        cout << p->value << " ";

                    // Enqueue all children of the dequeued item
                    for (int i=0; i<p->children.size(); i++)
                        q.push(p->children[i]);
                    n--;
                }
                if (value == NULL)
                    cout << endl; // Print new line between two levels
            }
        }

        void LevelArbitraryTraversal(Node<T>* root, vector<Node*> & buffer, T* value = NULL, string instructions = string())
        // Prints the n-ary tree level wise
        // when necessary, can also find all Nodes with a particular value
        {
            if (root == NULL)
                throw NodeException("corrupted root passed as an argument.");

            if (!buffer.empty() )
                throw NodeException("'LevelOrderTraversal' function requires passed buffer to be empty.");

            // Standard level order traversal code
            // using deque
            deque<Node *> q;  // Create a deque
            q.push_back(root); // Enqueue root
            int iteration = 0;
            while (!q.empty())
            {
                int n = q.size();

                // If this node has children
                while (n > 0)
                {
                    // Dequeue an item from deque and print it
                    Node* p = q.front();
                    q.pop_back();

                    if (value != NULL)
                    {
                        if (p->value == *value)
                            buffer.push_back(p);
                    }
                    else
                        cout << p->value << " ";

                    // Enqueue all children of the dequeued item
                    for (int i=0; i<p->children.size(); i++)
                        q.push_back(p->children[i]);

                    ++iteration;
                    n--;

                    if (iteration == 1 && !instructions.empty() )
                    {
                        vector<int> path(parseInstructions(instructions));

                        for (int i=0; i<q.size(); ++i)
                            swap(q[i], q[path[i]]);
                    }
                }
                if (value == NULL)
                    cout << endl; // Print new line between two levels
            }
        }

    public:

        void erase(string instructions)
        // when "[0]" is passed, removes all the children from the root
        {
            if (instructions.empty())
                throw NodeException("empty instructions list passed as an argument.");

            vector<int> path(parseInstructions(instructions));
            vector<int> parentPath(path);

            parentPath.pop_back();

            if (parentPath.empty())
            {
                children.erase(children.begin(), children.end());
                return;
            }

            Node* buffer = find(parentPath);
            printChildren(buffer);

            buffer->eraseChild(path.back());
            printChildren(buffer);
        }

        void erase(vector<int> path)
        {
            if (path.empty())
                throw NodeException("empty path passed as an argument.");

            vector<int> parentPath(path);

            parentPath.pop_back();

            if (parentPath.empty())
            {
                children.erase(children.begin(), children.end());
                return;
            }

            Node* buffer = find(parentPath);
            printChildren(buffer);

            buffer->eraseChild(path.back());
            printChildren(buffer);
        }

        vector<T> str(string instructions)
        {
            if (instructions.empty())
                throw NodeException("empty instructions list passed as an argument.");

            vector<T> str;
            vector<int> path(parseInstructions(instructions));

            vector<int> new_path;

            for (int i=0; i<path.size(); ++i)
            {
                new_path.push_back(path[i]);
                str.push_back(find(new_path)->getValue());
            }

            return str;
        }

        vector<T> str(vector<int> path)
        {
            if (path.empty())
                throw NodeException("empty path passed as an argument.");

            vector<T> str;
            vector<int> new_path;

            for (int i=0; i<path.size(); ++i)
            {
                new_path.push_back(path[i]);
                str.push_back(find(new_path)->getValue());
            }

            return str;
        }

        void printChildren(Node<T>* node = NULL)
        {
            if (node != NULL)
                for (int i=0; i<node->size(); ++i)
                    cout << i << " child: " << node->getChild(i)->getValue() << endl;
            else
                for (int i=0; i<size(); ++i)
                    cout << i << " child: " << getChild(i)->getValue() << endl;
        }

        Node* getChild(int index)
        {
            if (size() <= index)
                throw NodeException("index out or range");
            return children[index];
        }

        vector<Node*> getChildren()
        {
            return children;
        }

        int size()
        // represents number of children (not entire number of all the elements)
        {
            return children.size();
        }

        T getValue()
        {
            return value;
        }

        void addChild(Node* child)
        {
            children.push_back(child);
        }

        void eraseChild(int index)
        {
            if (size() <= index)
                throw NodeException("index out or range");

            children.erase(children.begin()+index);
        }

        void set(T value)
        {
            this->value = value;
        }

        Node* find(string instructions)
        {
            if (instructions.empty())
                throw NodeException("empty instructions list passed as an argument.");

            vector<int> path(parseInstructions(instructions));

            Node<T>* temp = this;

            //if (path.size() == 1)
                //addChild(buffer);
            if (path.size() > 1)
            {
                path.erase(path.begin() );
                //cout << "This: " << path[0] << endl;
                movePtr(temp, path);
                //cout << "0 child: " << temp->getValue() << endl;
                //cout << "0 child: " << temp->getChild(0)->getValue() << endl;
            }
            return temp;
        }

        Node* find(vector<int> path)
        {
            if (path.empty())
                throw NodeException("empty path passed as an argument.");

            Node<T>* temp = this;

            //if (path.size() == 1)
                //addChild(buffer);
            if (path.size() > 1)
            {
                path.erase(path.begin() );
                //cout << "This: " << path[0] << endl;
                movePtr(temp, path);
                //cout << "0 child: " << temp->getValue() << endl;
                //cout << "0 child: " << temp->getChild(0)->getValue() << endl;
            }
            return temp;
        }

        vector<Node*> findValue(Node* root, T value)
        {
            vector<Node*> buffer;
            T* val = new T(value);

            LevelOrderTraversal(root, buffer, val);
            delete val;

            if (buffer.empty())
                throw NodeException("couldn't find node with specified value.");

            return buffer;
        }

        void traversal(Node<T>* root)
        // incapsulates LevelOrderTraversal
        {
            if (root == NULL)
                throw NodeException("corrupted root passed as an argument.");

            vector<Node*> emptyVector;
            LevelOrderTraversal(root, emptyVector, NULL);
        }

        void arbitraryTraversal(Node<T>* root, string instructions)
        // incapsulates LevelOrderTraversal
        {
            if (root == NULL)
                throw NodeException("corrupted root passed as an argument.");

            vector<Node*> emptyVector;
            LevelArbitraryTraversal(root, emptyVector, NULL, instructions);
        }

        void push(Node<T>* buffer, string instructions)
        // [0][3][1][2] // 2nd index of first index
        {
            if (instructions.empty())
                throw NodeException("empty instructions list passed as an argument.");

            find(instructions)->addChild(buffer);
        }

        void push(Node* buffer, vector<int> path)
        // [0][3][1][2] // 2nd index of first index
        {
            if (path.empty())
                throw NodeException("empty path passed as an argument.");

            find(path)->addChild(buffer);
        }
};

template <class T>
Node<T>* newNode(T value)
// Utility function to create a new tree node
{
    Node<T>* temp = new Node<T>;
    temp->set(value);
    return temp;
}

#endif // NODE_HPP
