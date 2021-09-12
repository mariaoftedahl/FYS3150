#include "functions.hpp"

using namespace arma;
using namespace std;


int main(int argc, char* argv[]){
    int n = atoi(argv[1]);                 // length of array
    double h = 1./n;                       // step size
    vec a = vec(n).fill(-1.);              // defining a-vector filled with -1's
    vec b_gen = vec(n).fill(2.);           // defining b-vector filled with 2's for general algortihm
    vec b_spec = vec(n).fill(2.);          // defining b-vector filled with 2's for special algortihm
    vec c = vec(n).fill(-1.);              // defining c-vector filled with -1's
    vec x = arma::linspace(0+h, 1-h, n);   // defining array x in [0, 1]
    vec f = 100*exp(-10*x);                // defining source term
    vec g_gen = h*h*f;                     // defining solution vector g for general algortihm
    vec g_spec = h*h*f;                    // defining solution vector g for special algortihm
    vec v_gen = vec(n).fill(0.);           // creating empty solution vector v for general algortihm
    vec v_spec = vec(n).fill(0.);          // creating empty solution vector v gor special algorithm

    vec u = exact(x);                                    // calling exact solution function
    gauss_elim_gen(a, b_gen, c, g_gen, &v_gen, n);       // calling gaussian elimination function (general algorithm)
    gauss_elim_spec(a, b_spec, c, g_spec, &v_spec, n);   // calling gaussian elimination function (special algorithm)

    // Creates files exact_n.txt and approx_n.txt if true
    bool write_to_file = true;
    if (write_to_file){
        writetofile(x, u, v_gen, n);
    }

    return 0;
}
