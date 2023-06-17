#ifndef _ELEMENT_H_
#define _ELEMENT_H_
#include <iostream>
#include <fstream>
#include <Eigen/Eigen>
#include <vector>
#include "node.h"
class Element
{
        friend class Force;
    public:
        int etype;
        double E;
        double v;
        int elenum;
        int nodenum;
        struct element_beam 
        {
            int enumber;
            int Node1;
            int Node2;
        };
        struct element_triangle
        {
            static double D0;
            static Eigen::Matrix3d D;
            static double t;
            static int stressorstrain;
            static Eigen::MatrixXd B;
            static Node* epnode;
            /* Nodes are counter clock wise direction */
            int enumber;
            int Node1;
            int Node2;
            int Node3;
            double J;
            Eigen::MatrixXd Ke;

            bool D_set(double v);
            bool B_set();
            bool J_set();
            bool Ke_set();
            double area();
        };
        struct element_quadrangle
        {
            static double D0;
            static Eigen::Matrix3d D;
            static double t;
            int enumber;
            int Node1;
            int Node2;
            int Node3;
            int Node4;
        };
        /* other element type can add here*/
        
        element_beam* pele_b;
        element_quadrangle* pele_q;
        element_triangle* pele_t;

        Element(char* filename, Node* pn);
        ~Element();
        Eigen::MatrixXd assemble_stiffness();
    private:
        double area(const int& ele)const;
};


#endif