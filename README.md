# This is a structural FEM solver.
# Please read this doc before using.  
<br>
<br>

# Input
***
The Node class has method that can input nodes from `Nodes.txt`  files in the following format.
```
Nsum
Number1 X Y Z
Number2 X Y Z
Number3 X Y Z
...
```
Nsum indicates the total number of nodes.
Number%x% indicates the ID of node x.
X,Y,Z represents the coordinates of each nodes.
***
The element class has method that can generate elements from `Elements.txt` file and Node object in the following format.
```
Esum ETYPE E v t stressorstrain
# IF ETYPE == 0
ENumber1 N1 N2 N3 
...
# IF ETYPE == 1
Enumber1 N1 N2 N3 N4
...
```
Esum represents the total number of elements.\
ENumber%x% represents the ID of Element x.\
E represents the Modulus of elasticity.\
v represents the Poisson's ratio.\
t represents the thickness of plate.\
stressorstrain represents plane-stress or plane-strain.\
N%x% represents the coordinates of Elements' node.
***
The Force class has method that can generate force vector from 'Force.txt' file and Force object in the following format.
```
Fsum
x y z
ENumber1 ETYPE N1 N2 q
...
```
Fsum represents the total elements that been loaded.\
x,y,z represents the direction of rou.\
ENumber represetns the number of element.\
ETYPE represents the type of stress.
```
0 represents +Y direction uniform stress, 1 represents -Y direction uniform stress.
2 represents +X direction uniform stress, 3 represents -X direction uniform stress.
```
N1,N2 represent the edge of element.\
q represent the stress.
***
The Constrain class has method that can generate constrain from "Constrain.txt" file and constrain object in the following format.
```
Csum CTYPE
Nnum1 x y z
Nnum2 x y z
...
```
Csum represents numbers of constrain.\
CTYPE represents type of constrain.\
Nnum represents Node that be constrained.\
x,y,z represents constrain direction.
***
