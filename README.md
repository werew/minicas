# Minicas

*@authors* V.Constans L.Coniglio


Minicas is a simple, rudimental example of computer algebra system (CAS).


Exemples of use:

Recursive Fibonacci
```
init_fibo : @eval_in(a:0, 0, b:0, 1, c:0, 1, i:0, 0, n:0, _);
fibo_step : @eval_in(c, @summ(a,b), a, b, b, c, i, @summ(i,1));
fibo_rec : @compose(@show(c), fibo_step, @eval_ifneq(i, n, fibo_rec:0));
fibo : @compose(init_fibo, _, fibo_rec);
fibo(30);
```

