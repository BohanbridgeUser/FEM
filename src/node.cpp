#include "node.h"
#include <cmath>
int Node::sum = 0;
Node::Node(char* filename)
{
    using std::cout;
    using std::cin;
    using std::endl;
    using std::ios;
    std::fstream file;
    file.open(filename,ios::in);
    if(file.is_open()) {
        cout << "Node.txt opened successfully!\n";
        file >> sum;
        pnode = new node[sum];
        for (int i=0;i<sum;++i) {
            file >> pnode[i].number;
            file >> pnode[i].x;
            file >> pnode[i].y;
            file >> pnode[i].z; 
        }
        cout << "Total number of Nodes: " << sum << endl;
        for (int i=0;i<sum;++i) {
            cout << pnode[i].number << ' ';
            cout << pnode[i].x << ' ';
            cout << pnode[i].y << ' ';
            cout << pnode[i].z << ' '; 
            cout << endl;
        }
    } else {
        std::cout << "Failed open Node.txt!\n";
        exit(0);
    }
}
Node::~Node()
{
    delete pnode;
}
Node::node Node::node::crossproduct(const node& a, const node& b)const
{
    node temp;
    temp.number = 0;
    temp.x = a.y * b.z - b.y * a.z;
    temp.y = b.x * a.z - a.x * b.z;
    temp.z = a.x * b.y - a.y * b.x;
    return temp;
}
double Node::node::dotproduct(const node& a, const node& b)const
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
double Node::node::norm()const
{
    return sqrt(x * x + y * y + z * z);
}