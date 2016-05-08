# Minicas

*@authors* V.Constans L.Coniglio


Minicas is a simple, rudimental example of computer algebra system (CAS).


Exemples of use:

Simple instructions
```python
# Function call
summ(1, 2);

# Multiple inline instructions
summ(1, 2); quit

# Nested function call
summ(3, summ(2,1));
```

Declarations
```pyhon
# Declare a variable
my_float: 3;
my_matrix: [1,2,3];

# Declare a function
summ_copy: summ;
plus_two : summ(2,_);

# Nested declarations
add_age : summ(age:7,_);

# Create an alias for a command
p : print;
```

Force function referencing
```
# Is the same as: summ(2,_) with the only difference that
# it can be called without any parameter
plus_two : @summ(2);
plus_two(); # This will return 2

plus_two : summ(2,_);
plus_two(); # This will return a reference to a function with a missing argument
```

At this moment those are the functions and commands available in the 
internal module of minic@s
```python
# Summ of float arguments
summ(1,2,3); 

# Compose functions 
add_twice : @compose(@summ(a:0,2), @summ(a,2));
add_twice(); # a = 4
add_twice(); # a = 8 

# Print/show reference
show(a); 
show(a,b,c); 

# Functional assignement
eval_in(a:0, 10); # a = 10
eval_in(a, 1, b, 2, c, 3); # a = 10, b = 2, c = 3
eval_in(a:0, @mult_scal(10,10)); # a = 100

# Evaluate condition
eval_ifeq(a,b,  @eval_in(twice_a:0, @summ(a,b)) )
eval_ifneq(a,b, @eval_in(twice_a:0, @summ(a,a)) )
```

Module matrix
```python
# Create a matrix
m1 : matrix([1, 2, 1], [4, 2, 6], [2, 3, 1])

# Matrix product
m3 : mult(m1, m2)

# Scalar product
m3 : mult_scal(m1, 3.5, 4, m2)

# Matrix addition/subctration
m3 : addition(m1, m2)
m3 : sub(m1, m2)

# Matrix power
m3_cube : expo(m3, 3);

# Matrix transposition
m1 : transpose(m1);

# Matrix inversion
m4 : invert(m1)

# Determinant 
det : determinant(m1)

# Solve system of equations
b : matrix([4],[6],[5]);
x : solve(m1, b)

# Rank
r : rank(m1)

# Kernel of a linear application
ker : nullspace(m1); # or
ker : kernel(m1);

# LU/PLU decomposition
decomposition(m1);


# Speed test with chart generation
speedtest mult 1 300 2
speedtest mult 1 300 2 3
```


Example of function: recursive Fibonacci's sequence
```python
init_fibo : @eval_in(a:0, 0, b:0, 1, c:0, 1, i:0, 0, n:0, _);
fibo_step : @eval_in(c, @summ(a,b), a, b, b, c, i, @summ(i,1));
fibo_rec : @compose(@show(c), fibo_step, @eval_ifneq(i, n, fibo_rec:0));
fibo : @compose(init_fibo, _, fibo_rec);
fibo(30);
```

