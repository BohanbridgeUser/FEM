***
# Lotus Structural FEM Solver
![Static Badge](https://img.shields.io/badge/FEM-Solver-green)
![Static Badge](https://img.shields.io/badge/Language-C%2B%2B17-red)
![Static Badge](https://img.shields.io/badge/Frame-Kratos-blue)
![Static Badge](https://img.shields.io/badge/Linear%20algebra-Eigen-purple)\
A structural FEM Solver developed by Bohan Zhang. The code framework for this finite element program is based on the Kratos multi-physics software framework. Currently, this FEM solver is only used for structural analysis, so the framework has been simplified. And the data structure is also adjusted for performance considerations.

- [Lotus Structural FEM Solver](#lotus-structural-fem-solver)
  - [Framework](#framework)
  - [Basic Tools](#basic-tools)
    - [Vector and Matrix](#vector-and-matrix)
    - [Geometry](#geometry)
    - [Quadrature](#quadrature)
    - [VariableBaseInterface](#variablebaseinterface)
- [To be continued....](#to-be-continued)
    - [LinearSolver](#linearsolver)
    - [DataContainers](#datacontainers)
  - [Base Finite Element](#base-finite-element)
    - [Node](#node)
    - [Properties](#properties)
    - [Element](#element)
    - [Condition](#condition)
    - [Dof](#dof)

## Framework
The framework of this solver has two layers, basic Tools and base finite element. Basic tools consists of geometry, linearsolver, datacontainers, vector, quadrature and matrix.
Base finite element consists of node, properties, element, condition and dof.\
Two layers structure is shown beblow.
![Two layers structure](pic/pic1.jpg)

## Basic Tools
### Vector and Matrix
Currently, `Eigen` linear algebra library is used for algebraic operations. `Eigen` is an open source cpp template library that supports vectors, dense matrix and sparse matrix operations and calculations. I'm also developping a matrix calculation library by myself. In the future, the matrix library will replace `Eigen`. 
### Geometry
Inorder to separating geometric operations and element operations, geometry tool includes dimension, points, edges and operations like central point, area, volume, and Jacobian matrix, etc. 
There are two different types of data in the `Geometry` module, one is constant data like Dimension, integration points, shape functions values, etc. and the other is nonconstant data like points, jacobian, etc.
Therefore, a `Geometry Data` class is used for storing all constant data.\
The Geometry structure is shown beblow.
![Geometry module](pic/pic2.jpg)
### Quadrature
Obviouly, it's hard to integrating a function analytically in practice. 
So, numerial integration method were developed with the development of computer.
In fact, there are lots of effective numerial integrations methods. But `Gaussion Quadrature` is most commonly used method in finite element method.
```
There are two reasons why `Gaussion Quadrature` is suite for finite element method:
1.The integrand function has to be polynomial or approximate polynomial in `Gaussion Quadrature`.
2.The finite element method usually use complete polynomial shape funtion.
```
The generally formula for the `Gaussion Quadrature` is as follows:
![Gaussion Quadrature1](pic/pic4.jpg)
Integration points can be calculated as follows:
![Gaussion Quadrature2](pic/pic3.jpg)
The rationality of `Gaussion Quadrature` is not proved here.

### VariableBaseInterface
The Variable Base Interface is the most creative feature of this framework.


# To be continued....

### LinearSolver
### DataContainers

## Base Finite Element
### Node
### Properties
### Element
### Condition
### Dof

