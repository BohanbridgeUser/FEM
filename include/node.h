#ifndef _NODE_H_
#define _NODE_H_
#include <iostream>
#include <fstream>
class Node{
        friend class Element;
        friend class Force;
        friend class solver;
    public:
        struct node
        { 
            int number; 
            double x; 
            double y; 
            double z;
            node(int a,double b, double c, double d):number(a),x(b),y(c),z(d){};
            node() {number =0;x=0;y=0;z=0;}
            node(double a,double b, double c):number(0),x(a),y(b),z(c){};
            node operator+(const node& another)
            {
                return node(x+another.x,y+another.y,z+another.z);
            }
            node operator-(const node& another)
            {
                return node(x-another.x,y-another.y,z-another.z);
            }
            node operator=(node another)
            {
                return node(another.number,another.x,another.y,another.z);
            }
            node crossproduct(const node& a, const node& b) const;
            double dotproduct(const node& a, const node& b) const;
            double norm()const;
        };
        node* pnode;
        static int sum;
        Node(char* filename);
        ~Node();
};
#endif