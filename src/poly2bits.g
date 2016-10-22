#!/usr/bin/env gap -q

F := GF(2);
x := Indeterminate(F, "x");

input := InputTextFile("*stdin*");
g := x^180+x^130+x^56+x^26+x^10;

line := ReadLine(input);
while not line = fail do
    g := EvalString(line);
    coef := CoefficientsOfUnivariatePolynomial(g);
    list := [ 1 .. (Degree(g) + 1) ];

    Print("[");
    for i in list do
        if coef[i] = 0*Z(2) then
            Print("0");
        elif coef[i] = 1*Z(2) then
            Print("1");
        fi;
        
        if not i = Maximum(list) then
            Print(" ");
        fi;
    od;
    Print("]\n");

    line := ReadLine(input);
od;

