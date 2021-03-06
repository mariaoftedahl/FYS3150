#include "functions.hpp"

using namespace arma;
using namespace std;

int main(int argc, char const *argv[]){
    bool write = true;                             // writes internal state cube U(x, y, t) to bin-file if true
    string name = "animation.bin";                 // name of bin-file

    double h = 0.005;                              // step size in the x- and y-direction
    double M = 1.0/h + 1.0;                        // number of points along the x-axis of total state matrix, including boundary points
    double dt = 2.5*pow(10, -5);                   // time step size
    double T = 0.008;                              // total time
    double N = T/dt;                               // number of time steps
    double v0 = pow(10, 10);                       // constant potential value inside barriers
    cx_double r = cx_double(0.0, dt/(2*h*h));      // constant
    int n_slits = 2;                               // number of slits, must be either 0, 1, 2 or 3
    double wall_thickness = 0.02;                  // size of the wall thickness in the x-direction
    double separation_size = 0.05;                 // length of the wall piece separating the slits
    double slit_size = 0.05;                       // slit aperture (opening in the y-direction)


    cx_vec a = cx_vec((M-2)*(M-2));                       // initializing a vector
    cx_vec b = cx_vec((M-2)*(M-2));                       // initializing b vector
    sp_cx_mat A = sp_cx_mat((M-2)*(M-2), (M-2)*(M-2));    // initializing A matrix
    sp_cx_mat B = sp_cx_mat((M-2)*(M-2), (M-2)*(M-2));    // initializing B matrix

    double mean_x = 0.25;        // coordinates of the centre (mean) of the initial wave packet in the x-direction
    double mean_y = 0.5;         // coordinates of the centre (mean) of the initial wave packet in the y-direction
    double var_x = 0.05;         // initial width (variance) of the wave packet in the x-direction
    double var_y = 0.2;          // initial width (variance) of the wave packet in the y-direction
    double p_x = 200.0;          // wave packet momenta in the x-direction
    double p_y = 0.0;            // wave packet momenta in the y-direction

    cx_mat U = initial_state(mean_x, mean_y, var_x, var_y, p_x, p_y, M, h);           // initializing internal state matrix U(x, y, 0)
    cx_mat V = potential(v0, M, n_slits, slit_size, separation_size, wall_thickness); // generating potential matrix V(x, y)
    a_b_vectors(r, dt, M, a, b, V);                                                   // generating a and b vectors
    A_B_matrices(r, a, b, A, B, M);                                                   // generating A and B matrices
    cx_cube U_cube = CrankNicolson(U, B, A, M, N);                                    // generating internal state cube U(x, y, t) where each slize is a time step

    if (write){
        write_to_file(U_cube, name);                                                  // writes U_cube to bin-file
    }

    return 0;
}
