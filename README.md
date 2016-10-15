# Bruteforce CRC

Given some strings and their expected CRC finds the possible generating
polynomials. We use GAP (A Computer Algebra System) and some C programs.

## How does it work?

Calculating the CRC code of a given "message" is really simple: we think
of the message as if it were a polynomial $m(x)$ with coefficients in
the field with two elements GF(2).

A type of CRC code is defined by a polynomial $g(x)$. Let's call its
degree $d = Deg g$. The degree it's usually a power of two for reasons
that will be soon clear.

The CRC of the message $m(x)$ is the remainder in the Euclidean Division
of the polynomial $m(x) * x^d$ by $g(x)$. That is, we can write
$$ m(x) * x^d = g(x) q(x) + r(x) $$, where $r(x)$ is the remainder, that
is to be interpreted as the final CRC.

We simply noticed that $g(x) \mid m(x) * x^d - r(x)$, that is the
defining polynomial divides the message minus the CRC number. So we can
compute, given enough messages and CRC, the MCD of all the polynomials
$m(x) * x^d - r(x)$, knowing that $g(x)$ must be one of the divisors of
it of degree d.

## How to use it

Basic step are:

* Using `...` convert your files to the bit representation:
  
  
* Using `...` generate all possible modes of operation of the machine
  generating the CRCs.
  
* Using `...` finally solve all CRCs challenges and hope there will be
  some polynomials remaining.


