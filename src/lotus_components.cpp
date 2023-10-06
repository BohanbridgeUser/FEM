#include "../include/lotus_components.h"
#include "../include/Geometry/geometry.h"
#include "../include/Element/element.h"
#include "../include/Condition/condition.h"
#include "../include/Constitutive_Law/Constitutive_law.h"

void AddComponent(const std::string& rName, const Variable<bool>& rComponent)
{
    Lotus_Components<Variable<bool>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<int>& rComponent)
{
    Lotus_Components<Variable<int>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<unsigned int>& rComponent)
{
    Lotus_Components<Variable<unsigned int>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<double>& rComponent)
{
    Lotus_Components<Variable<double>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::array<double, 3>>& rComponent)
{
    Lotus_Components<Variable<std::array<double, 3>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::array<double, 4>>& rComponent)
{
    Lotus_Components<Variable<std::array<double, 4>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::array<double, 6>>& rComponent)
{
    Lotus_Components<Variable<std::array<double, 6>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::array<double, 9>>& rComponent)
{
    Lotus_Components<Variable<std::array<double, 9>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<Eigen::Matrix<double,Eigen::Dynamic,1>>& rComponent)
{
    Lotus_Components<Variable<Eigen::Matrix<double,Eigen::Dynamic,1>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>>& rComponent)
{
    Lotus_Components<Variable<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::string>& rComponent)
{
    Lotus_Components<Variable<std::string>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<Flags>& rComponent)
{
    Lotus_Components<Variable<Flags>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Geometry<Node>& rComponent)
{
    Lotus_Components<Geometry<Node>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Element& rComponent)
{
    Lotus_Components<Element>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Condition& rComponent)
{
    Lotus_Components<Condition>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Constitutive_Law& rComponent)
{
    Lotus_Components<Constitutive_Law>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<Constitutive_Law::Pointer>& rComponent)
{
    Lotus_Components<Variable<Constitutive_Law::Pointer>>::Add(rName, rComponent);
}

template class Lotus_Components<Variable<bool>>;
template class Lotus_Components<Variable<int>>;
template class Lotus_Components<Variable<unsigned int>>;
template class Lotus_Components<Variable<double>>;
template class Lotus_Components<Variable<std::array<double, 3>>>;
template class Lotus_Components<Variable<std::array<double, 4>>>;
template class Lotus_Components<Variable<std::array<double, 6>>>;
template class Lotus_Components<Variable<std::array<double, 9>>>;
template class Lotus_Components<Variable<Eigen::Matrix<double,Eigen::Dynamic,1>>>;
template class Lotus_Components<Variable<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>>>;
template class Lotus_Components<Variable<std::string>>;
template class Lotus_Components<Variable<Flags>>;
template class Lotus_Components<Flags>;

template class Lotus_Components<Geometry<Node>>;
template class Lotus_Components<Element>;
template class Lotus_Components<Condition>;
template class Lotus_Components<Constitutive_Law>;
template class Lotus_Components<Variable<Constitutive_Law::Pointer>>;

Lotus_Components<Variable_Data>::ComponentsContainerType Lotus_Components<Variable_Data>::msComponents;