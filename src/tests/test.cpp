#define CATCH_CONFIG_MAIN
#include "../../vendor/catch.hpp"
#include "../../src/Node.hpp"

TEST_CASE("general test")
{
    Node<int> *root = newNode(10);

    CHECK(root->size() == 0);
    CHECK_THROWS(root->getChild(0));

    root->push(newNode(2), "[0]");
    root->push(newNode(34), "[0]");
    root->push(newNode(56), "[0]");
    root->push(newNode(100), "[0]");

    CHECK(root->size() == 4);

    root->push(newNode(6874), "[0][1]"); // child for the element with path '[0][1]' is added
    root->push(newNode(6875), "[0][1]");
    root->push(newNode(79557), "[0][1]");

    CHECK(root->size() == 4);
    root->erase("[0][1][0]");

    CHECK(root->find("[0][1][0]")->getValue() == 6875);
    CHECK(root->find("[0][1][1]")->getValue() == 79557);

    CHECK_THROWS(root->find("[0][1][2]"));
    CHECK_THROWS(root->findValue(root, 6874));



    /*
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
}

TEST_CASE("general test[vector<int>]")
{
    Node<int> *root = newNode(10);

    CHECK(root->size() == 0);
    CHECK_THROWS(root->getChild(0));

    vector<int> path;

    path.push_back(0);

    root->push(newNode(2), path);
    root->push(newNode(34), path);
    root->push(newNode(56), path);
    root->push(newNode(100), path);

    CHECK(root->size() == 4);

    path.push_back(1);

    root->push(newNode(6874), path); // child for the element with path '[0][1]' is added
    root->push(newNode(6875), path);
    root->push(newNode(79557), path);

    path.push_back(0);

    CHECK(root->size() == 4);
    root->erase(path);

    CHECK(root->find(path)->getValue() == 6875);

    path[path.size()-1] = 1;
    CHECK(root->find(path)->getValue() == 79557);

    CHECK_THROWS(root->find("[0][1][2]"));
    CHECK_THROWS(root->findValue(root, 6874, "[0][2][3][0][1]"));
}
