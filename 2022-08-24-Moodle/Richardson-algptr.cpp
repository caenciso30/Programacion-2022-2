#include <iostream>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <cstdlib>

using fptr = double(double);
using algptr = double(double, double, fptr fun);

double f(double x);
double forward_deriv(double x, double h, fptr fun);
double central_deriv(double x, double h, fptr fun);
double richardson_deriv(double x, double h, int order, fptr fun, algptr alg);

int main(int argc, char **argv)
{
  std::ofstream fout;
  fout.precision(15); fout.setf(std::ios::scientific);
  fout.open("data2.txt");
  double h, x, df, dc, drf, drc, dexact;
  x = 3.7;
  for (int ii = 1; ii <= 10; ii++) {
    h = std::pow(10.0, -ii);
    df = forward_deriv(x, h, f); // pointer allows to deriv other functions: forward_deriv(x, h, std::cos)
    dc = central_deriv(x, h, f);
    drf = richardson_deriv(x, h, 1, f, forward_deriv);
    drc = richardson_deriv(x, h, 2, f, central_deriv);
    dexact = 2*std::cos(2*x);
    fout  << h << "\t"
	      << std::fabs(df-dexact)/dexact << "\t"
	      << std::fabs(dc-dexact)/dexact << "\t"
	      << std::fabs(drf-dexact)/dexact << "\t"
	      << std::fabs(drc-dexact)/dexact << "\n";
//std::printf("%25.16e%25.16E%25.16e%25.16e\n", h,
//            std::fabs(df-dexact)/dexact,
//            std::fabs(dc-dexact)/dexact,
//            std::fabs(dr-dexact)/dexact);

  }
  fout.close();

  return 0;
}

double f(double x)
{
  return std::sin(2*x);
}

double forward_deriv(double x, double h, fptr fun)
{
  return (fun(x+h) - fun(x))/h;
}

double central_deriv(double x, double h, fptr fun)
{
  return (fun(x + h/2) - fun(x - h/2))/h;
}

double richardson_deriv(double x, double h, int order, fptr fun, algptr alg)
{
  const double C = std::pow(2, order);
  double f1, f2;
  f1 = alg(x, h, fun);
  f2 = alg(x, h/2, fun);
  return (C*f2 - f1)/(C-1);
  
}
//plot "data2.txt" u 1:2 w lp lw 3 title "forward" , "data2.txt" u 1:3 w lp lw 3 title "central" , "data2.txt" u 1:4 w lp lw 3 title "richardson-forward", "data2.txt" u 1:5 w lp lw 3 title "richardson-central"
