#include <iostream>
#include "src/Node.hpp"
#include "src/NodeException.hpp"

using namespace std;

int main()
{
    try
    {
        /*
        Node<int> *root = newNode(10);
        root->addChild((newNode(2)));
        root->addChild(newNode(34));
        root->addChild(newNode(56));
        root->addChild(newNode(100));
        root->push(newNode(6874), "[0][1]"); // child for the element with path '[0][1]' is added
        root->push(newNode(6875), "[0][1]");
        root->push(newNode(79557), "[0][1]");
        root->push(newNode(6876), "[0][1][0]");
        root->push(newNode(6877), "[0][1][1]");
        root->push(newNode(56), "[0][1][2]");
        root->push(newNode(56), "[0][1][2]");
        root->push(newNode(104), "[0][1][2]");
        root->push(newNode(56), "[0][1][2]");
        root->push(newNode(56), "[0][1][2][3]");

        vector<int> buffer(root->str("[0][2]"));

        //root->erase("[0]");

        for (int i=0; i<buffer.size(); ++i)
            cout << buffer[i] << endl;

        cout << "Level order traversal\n";
        root->traversal(root);

        root->erase("[0][1][2]");

        cout << "Level order traversal\n";
        root->traversal(root);

        cout << "Searching for elements: " << endl;

        vector<Node<int>*> buffer1(root->findValue(root, 56));

        for (int i=0; i<buffer1.size(); ++i)
            cout << buffer1[i]->getValue() << endl;
        */

        Node<int> *root = newNode(10);

        root->push(newNode(2), "[0]");
        root->push(newNode(34), "[0]");
        root->push(newNode(56), "[0]");
        root->push(newNode(100), "[0]");

        root->push(newNode(6874), "[0][1]"); // child for the element with path '[0][1]' is added
        root->push(newNode(6875), "[0][1]");
        root->push(newNode(79557), "[0][1]");

        root->traversal(root);

        //cout << endl;
        //root->arbitraryTraversal(root, "[0][3][2][1][0]");
        cout << endl;

        root->erase("[0][1][0]");
        cout << root->find("[0][1][2]")->getValue() << endl;

        root->traversal(root);
    }
    catch(NodeException & ex)
    {
        cout << ex.what() << endl;
    }
    return 0;
}
