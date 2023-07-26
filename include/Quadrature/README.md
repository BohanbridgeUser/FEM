# Integration Formula Derivataion
![Static Badge](https://img.shields.io/badge/NumericalMethod-Integration-blue)
![Static Badge](https://img.shields.io/badge/Math-FormulaDerivation-green)


- [Integration Formula Derivataion](#integration-formula-derivataion)
  - [Gauss Integration](#gauss-integration)
  - [Gauss Legendre Integration](#gauss-legendre-integration)
  - [Gauss Lobatto Integration](#gauss-lobatto-integration)
***
## Gauss Integration
$$ \int_a^b f(x) \approx \sum_{k=0}^{n} A_{k}f(x_{k}) k=0,1,...,n $$
where the selection of $x_{k}$ makes the integration accuracy reach $2n+1$.
***
## Gauss Legendre Integration
Legendre Polynomial:
$$  P_{n}(x) = \frac{1}{2^{n}n!}\ \frac{d^{n}}{dx^{n}}\ (x^{2}-1)^{n}, x\in[-1,1], n=1,2,... $$
$$  P_{0}(x) = 1 $$
$$ \int_{-1}^1 f(x) \approx \sum_{k=0}^{m} A_{k}f(x_{k}) k=0,1,...,m $$
where the selection of $x_{k}$ makes the integration accuracy reach $2m+1$.
Gauss Legendre Integration is a method that choose $x_{k}$ makes Legendre Polynomial equal to zero.\
\
Step1:\
$P_{0}(x) = 1$\
$P_{1}(x) = x$\
$P_{2}(x) = \frac{1}{2}\ (3x^{2}-1)$\
$P_{3}(x) = \frac{1}{2}\ (5x^{3}-3x)$\
$P_{4}(x) = \frac{1}{8}\ (35x^{4}-30x^{2}-3)$\
$P_{5}(x) = \frac{1}{8}\ (63x^{5}-70x^{3}-15x)$\
$P_{6}(x) = \frac{1}{8}\ (63x^{5}-70x^{3}-15x)$\
...\
\
Step2:
Solve $P_{n}(x) = 0$.\
| k | A0                   | A1                   | A2                  | A3                  | A4                  |
|---|----------------------|----------------------|---------------------|---------------------|---------------------|
| 1 | 2.0000000000000000   |                      |                     |                     |                     |
| 2 | 1.0000000000000000   | 1.0000000000000000   |                     |                     |                     |
| 3 | 0.5555555555555560   | 0.8888888888888890   | 0.5555555555555560  |                     |                     |
| 4 | 0.3478548451374540   | 0.6521451548625460   | 0.6521451548625460  | 0.3478548451374540  |                     |
| 5 | 0.2369268850561890   | 0.4786286704993660   | 0.5688888888888890  | 0.4786286704993660  | 0.2369268850561890  |
|   |
| k | x1                   | x2                   | x3                  | x4                  | x5                  |
| 1 | 0.0000000000000000   |                      |                     |                     |                     |
| 2 | -0.5773502691896260  | 0.5773502691896260   |                     |                     |                     |
| 3 | -0.7745966692414830  | 0.0000000000000000   | 0.7745966692414830  |                     |                     |
| 4 | -0.8611363115940530  | -0.3399810435848560  | 0.8611363115940530  | 0.3399810435848560  |                     |
| 5 | -0.9061798459386640  | -0.5384693101056830  | 0.0000000000000000  | 0.5384693101056830  | 0.9061798459386640  |
***
## Gauss Lobatto Integration
$$ \int_{-1}^1 f(x) \approx \frac{2}{n(n-1)}\ (f(-1)+f(1)) + \sum_{i=2}^{n-1} w_{i}f(x_{i}) $$
node: &nbsp;&nbsp; $x_{i} \ \ is \ \ the \ \ i-th \ \ zeros \ \ of \ \ P^{'}_{n-1}(x)$\
weight: $w_{i}=\frac{2}{n(n-1)[P_{n-1}(xi)]^{2}}$\
function:$f(x)$ is a polynomial of $2N-3$ order. $x_{1}$ and $x_{n}$ have to be $-1$ and $1$.
| i | x1          | x2           | x3           | x4          | x5          | x6          |
|---|-------------|--------------|--------------|-------------|-------------|-------------|
| 1 |             |              |              |             |             |             |
| 2 | -1          | 1            |              |             |             |             |
| 3 | -1          | 0            | 1            |             |             |             |
| 4 | -1          | -0.447213595 | 0.447213595  | 1           |             |             |
| 5 | -1          | -0.654653671 | 0            | 0.654653671 | 1           |             |
| 6 | -1          | -0.765055324 | -0.285231516 | 0.285231516 | 0.765055324 | 1           |
|   |             |              |              |             |             |             |
| i | w1          | w2           | w3           | w4          | w5          | w6          |
| 1 |             |              |              |             |             |             |
| 2 | 1           | 1            |              |             |             |             |
| 3 | 0.333333333 | 1.333333333  | 0.333333333  |             |             |             |
| 4 | 0.166666667 | 0.833333333  | 0.833333333  | 0.166666667 |             |             |
| 5 | 0.1         | 0.544444444  | 0.711111111  | 0.544444444 | 0.1         |             |
| 6 | 0.066666667 | 0.378474956  | 0.554858377  | 0.554858377 | 0.378474956 | 0.066666667 |
***
