#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <regex>
#include <deque>
#include <variant>
#include "NodeException.hpp"

using namespace std;

bool isEmpty(variant<string, vector<int>> var)
// utility function to check whether variable of type 'variant<string, vector<int> >' is empty
{
    if (var.index() == 0)
    {
        if (get<string>(var).empty())
            return true;
        return false;
    }
    else
    {
        if (get<vector<int>>(var).empty())
            return true;
        return false;
    }
}

template <class T>
class Node
{
    private:

        T value;
        vector<Node*> children;

        vector<int> parseInstructions(variant<string, vector<int> > coordinates)
        {
            string instructions;

            if (coordinates.index() == 1)
                return get<vector<int>>(coordinates);
            else
                instructions = get<string>(coordinates);

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

        void movePtr(Node* &start, variant<string, vector<int>> coordinates)
        {
            vector<int> path = parseInstructions(coordinates);

            if (start->size() <= path[0])
                throw NodeException("path out of range (path doesn't exist)");

            start = start->children[path[0]];
            path.erase(path.begin());

            if (path.size() > 0)
                movePtr(start, path);
        }

        void LevelArbitraryTraversal(Node<T>* root, vector<Node*> & buffer, T* value = NULL, variant<string, vector<int>> coordinates = variant<string, vector<int>>() )
        // Prints the n-ary tree level wise
        // when necessary, can also find all Nodes with a particular value
        {
            if (root == NULL)
                throw NodeException("corrupted root passed as an argument.");

            if (!buffer.empty() )
                throw NodeException("'LevelArbitraryTraversal' function requires passed buffer to be empty.");

            // Standard level order traversal code
            // using deque
            deque<Node*> q;  // Create a deque
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
                    q.pop_front();

                    if (value != NULL)
                    // modification for finding node with specific value
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

                    if (iteration == 1 && !isEmpty(coordinates) )
                    // modification for changing reading order
                    // may also write get<vector<int>> as .empty is present in both STL-containers
                    {
                        vector<int> path(parseInstructions(coordinates));
                        deque<Node*> newDeque;
                        vector<int> newPath(path); // new path won't contain first "[0]"
                        newPath.erase(newPath.begin());

                        for (int i=0; i<q.size(); ++i)
                            newDeque.push_back(q[newPath[i]]);

                        q.swap(newDeque);

                        /*
                        cout << endl << "Printing values from deque: " << endl;

                        for (int i=0; i<q.size(); ++i)
                            cout << q[i]->getValue() << endl;

                        cout << "End of printing..." << endl;
                        */
                    }
                }
                if (value == NULL)
                // modification for finding node with specific value
                {
                    cout << endl; // Print new line between two levels
                }
            }
        }

    public:

        void erase(variant<string, vector<int> > coordinates)
        // when "[0]" is passed, removes all the children from the root
        {
            if (isEmpty(coordinates))
                throw NodeException("empty instructions list passed as an argument.");

            vector<int> path(parseInstructions(coordinates));
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

        vector<T> str(variant<string, vector<int> > coordinates)
        {
            if (isEmpty(coordinates))
                throw NodeException("empty instructions list passed as an argument.");

            vector<T> str;
            vector<int> path(parseInstructions(coordinates));
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

        Node* find(variant<string, vector<int> > coordinates)
        // find node with such coordinates
        {
            if (isEmpty(coordinates))
                throw NodeException("empty instructions list passed as an argument.");

            vector<int> path(parseInstructions(coordinates));

            Node<T>* temp = this;

            //if (path.size() == 1)
                //addChild(buffer);
            if (path.size() > 1)
            {
                path.erase(path.begin() );
                // deleting first "[0]" from path as we won't use it

                //cout << "This: " << path[0] << endl;
                movePtr(temp, path);
                //cout << "0 child: " << temp->getValue() << endl;
                //cout << "0 child: " << temp->getChild(0)->getValue() << endl;
            }
            return temp;
        }

        void traversal(Node<T>* root, variant<string, vector<int>> coordinates = variant<string, vector<int>>() )
        // print all nodes according to the specified order which is set by coordinates
        // here coordinates represent level 1 reading order
        {
            if (root == NULL)
                throw NodeException("corrupted root passed as an argument.");

            vector<Node*> emptyVector;
            LevelArbitraryTraversal(root, emptyVector, NULL, coordinates);
        }

        vector<Node*> findValue(Node* root, T value, variant<string, vector<int>> coordinates = variant<string, vector<int>>() )
        // find all nodes with such value
        {
            vector<Node*> buffer;
            T* val = new T(value);

            LevelArbitraryTraversal(root, buffer, val, coordinates);
            delete val;

            if (buffer.empty())
                throw NodeException("couldn't find node with specified value.");

            return buffer;
        }

        void push(Node<T>* buffer, variant<string, vector<int> > coordinates)
        // push node at the place with the passed coordinates (new node will be the child of a node with these coordinates)
        {
            if (isEmpty(coordinates))
                throw NodeException("empty instructions list passed as an argument.");

            find(coordinates)->addChild(buffer);
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
