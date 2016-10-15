#!/usr/bin/env gap

# We construct the field F_2 and the indeterminate x
F := GF(2);
x := X(F, "x");

# We then get our polynomials in input (one for line)
# First line is the degree of the searched polynomial g(x)
# Then we expect double lines consisting of:
# - First line is the message m(x) (written as polynomial)
# - Second line is the CRC r(x) (as polynomial)
# We try to get g(x) such that, foreach m(x) and r(x) given
# the following equation holds:
# m(x) * x^(deg g) = g(x) * q(x) + r(x)

# All input is taken from std input stream
input := InputTextFile("*stdin*");
# Arrays of m- and r-polynomials follows
mpoly := [];
rpoly := [];

# We read the degree of the generating polynomial, gamma = deg g
gamma := EvalString(ReadLine(input));

# Then we start reading lines in pairs
line := ReadLine(input);
# We read the m-polynomial
while not (line = fail) do
    # If the stream hasn't ended we save the m-polynomial
    Add(mpoly, EvalString(line));
    # And then we read the r-polynomial. It MUST be there
    line := ReadLine(input);
    Add(rpoly, EvalString(line));
    line := ReadLine(input);
od;

# We then close stdin (it may not be very good to do)
CloseStream(input);

# We then compute the GCD of all polynomials we got
gcd := 0*x;
for i in [1 .. Length(mpoly)] do
    gcd := Gcd(gcd, mpoly[i] * x^gamma - rpoly[i]);
od;

# Test lines follow
# Print("The GCD is: ", gcd, "\n");
# Print("We now calculate all divisors of degree ", gamma, ":\n");

# For every subset in the factor decomposition multiset
for subset in EnumeratorOfCombinations(Factors(gcd)) do
    if not( subset = [ ]) then
        divisor := Product(subset);
        # If the product of the subset is of the right degree we print it
        if Degree(divisor) = gamma then
            Print(divisor, "\n");
        fi;
    fi;
od;

