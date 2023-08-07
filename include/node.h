#ifndef _NODE_H_
#define _NODE_H_
#include "define.h"
#include "Geometry/Point.h"
#include "node_data.h"
#include "dof.h"
#include <vector>
#include "Geometry/Vector.h"
class Node : public Point<3>
{
    public:
        ///@name Define
        ///@{ 
        LOTUS_POINTER_DEFINE(Node)
        typedef Node NodeType;
        typedef Point<3> BaseType;
        typedef Point<3> PointType;

        typedef Node_Data NodeDataType;

        typedef std::vector<Dof> DofsContainerType;
        ///@}

        ///@name Life Circle
        ///@{
            // Constructor
            Node(const int& ID)
            :PointType(),
             mNodeData(ID),
             mDofsContainer(),
             mInitialPosition()
            {

            }  
            Node(const double& x, const double& y, const double& z)
                :PointType(x,y,z),
                mNodeData(),
                mDofsContainer(),
                mInitialPosition(x,y,z)
            {

            }
            Node(const double& x, const double& y, const double& z,
                const NodeDataType& ThisNodeData, const DofsContainerType& ThisDofs)
                :PointType(x,y,z),
                mNodeData(ThisNodeData),
                mDofsContainer(ThisDofs),
                mInitialPosition(x,y,z)
            {
                
            }  
            Node(const Node& another)
                :PointType(another.x(),another.y(),another.z()),
                mNodeData(another.mNodeData),
                mDofsContainer(another.mDofsContainer),
                mInitialPosition(another.x(),another.y(),another.z())
            {

            }
            Node(const Point<3>& anotherP)
                 :PointType(anotherP),
                 mNodeData(),
                 mDofsContainer(),
                 mInitialPosition(anotherP)
            {

            }
            Node(const Point<3>& anotherP,
                 const NodeDataType& ThisNodeData, 
                 const DofsContainerType& ThisDofs)
                 :PointType(anotherP),
                 mNodeData(ThisNodeData),
                 mDofsContainer(ThisDofs),
                 mInitialPosition(anotherP)
            {

            }
            // Destructor
            ~Node()
            {

            }    
        ///@}
        
        ///@name Operations {
            NodeDataType& GetNodeData()
            {
                return mNodeData;
            }
            NodeDataType GetNodeData()const
            {
                return mNodeData;
            }
            DofsContainerType& GetDofs()
            {
                return mDofsContainer;
            }
            DofsContainerType GetDofs()const
            {
                return mDofsContainer;
            }
            Vector<3> GetInitialPosition()const
            {
                return mInitialPosition;
            }
            Vector<3>& GetInitialPosition()
            {
                return mInitialPosition;
            }
        ///@}
    private:
        // NodeData stores ID and SolutionData of Nodes
        NodeDataType mNodeData;

        DofsContainerType mDofsContainer;

        Vector<3> mInitialPosition;
};

#endif