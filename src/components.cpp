#include "../include/components.h"

void AddComponent(const std::string& rName, const Variable<bool>& rComponent)
{
    Components<Variable<bool>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<int>& rComponent)
{
    Components<Variable<int>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<unsigned int>& rComponent)
{
    Components<Variable<unsigned int>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<double>& rComponent)
{
    Components<Variable<double>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::array<double, 3>>& rComponent)
{
    Components<Variable<array_1d<double, 3>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::array<double, 4>>& rComponent)
{
    Components<Variable<array_1d<double, 4>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::array<double, 6>>& rComponent)
{
    Components<Variable<array_1d<double, 6>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::array<double, 9>>& rComponent)
{
    Components<Variable<array_1d<double, 9>>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<Eigen::Matrix<double,Eigen::Dynamic,1>>& rComponent)
{
    Components<Variable<Vector>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>>& rComponent)
{
    Components<Variable<Matrix>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<std::string>& rComponent)
{
    Components<Variable<std::string>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<Flags>& rComponent)
{
    Components<Variable<Flags>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Geometry<Node>& rComponent)
{
    Components<Geometry<Node>>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Element& rComponent)
{
    Components<Element>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Condition& rComponent)
{
    Components<Condition>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const ConstitutiveLaw& rComponent)
{
    Components<ConstitutiveLaw>::Add(rName, rComponent);
}
void AddComponent(const std::string& rName, const Variable<ConstitutiveLaw::Pointer>& rComponent)
{
    Components<Variable<ConstitutiveLaw::Pointer>>::Add(rName, rComponent);
}

template class Components<Variable<bool>>;
template class Components<Variable<int>>;
template class Components<Variable<unsigned int>>;
template class Components<Variable<double>>;
template class Components<Variable<std::array<double, 3>>>;
template class Components<Variable<std::array<double, 4>>>;
template class Components<Variable<std::array<double, 6>>>;
template class Components<Variable<std::array<double, 9>>>;
template class Components<Variable<Eigen::Matrix<double,Eigen::Dynamic,1>>>;
template class Components<Variable<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>>>;
template class Components<Variable<std::string>>;
template class Components<Variable<Flags>>;
template class Components<Flags>;

template class Components<Geometry<Node>>;
template class Components<Element>;
template class Components<Condition>;
template class Components<ConstitutiveLaw>;
template class Components<Variable<ConstitutiveLaw::Pointer>>;

Components<Variable_Data>::ComponentsContainerType Components<VariableData>::msComponents;