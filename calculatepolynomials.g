#!/bin/env gap

# Costruiamo il campo F_2 e l'indeterminata x
F := GF(2);
x := X(F, "x");

# Ora ci basta un modo di prendere i vari polinomi in input (uno per riga come stringhe di coefficienti)
# Essi saranno scritti nel seguente modo:
# 0101010101010100101010101011 SPC 10010101010 \n
# Dove il primo numero è il polinomio m(x) ed il secondo è il resto r(x)
# m(x) * x^(deg g) = g(x) * q(x) + r(x)

input := InputTextFile("test-data/dati2.txt");
mpoly := [];
rpoly := [];

# Qui leggiamo il gamma iniziale, gamma = deg g
gamma := EvalString(ReadLine(input));

line := ReadLine(input);
# Qui moralmente abbiamo già letto il polinomio m
while not (line = fail) do
    Add(mpoly, EvalString(line));
    # Qui stiamo leggendo il polinomio r
    line := ReadLine(input);
    Add(rpoly, EvalString(line));
    line := ReadLine(input);
od;

CloseStream(input);

# A questo punto possiamo passare a computare i GCD
gcd := 0*x;
for i in [1 .. Length(mpoly)] do
    gcd := Gcd(gcd, mpoly[i] * x^gamma - rpoly[i]);
od;

Print("Il gcd totale è: ", gcd, "\n");
Print("Ora calcoliamo tutti i polinomi di grado ", gamma, " che lo dividono:\n");

for subset in EnumeratorOfCombinations(Factors(gcd)) do
    if not( subset = [ ]) then
        divisor := Product(subset);
        if Degree(divisor) = gamma then
            Print(divisor, "\n");
        fi;
    fi;
od;

