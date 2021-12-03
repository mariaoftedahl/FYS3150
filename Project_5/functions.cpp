#include "functions.hpp"

using namespace arma;
using namespace std;


cx_mat initial(double mean_x, double mean_y, double var_x, double var_y, double p_x, double p_y, double M, double h){

    vec x = linspace(0+h, 1-h, M-2);  // x vector from 0 to 1 with M-3 steps
    vec y = linspace(0+h, 1-h, M-2);  // y vector from 0 to 1 with M-3 steps

    cx_vec distr_x = cx_vec(M-2);
    cx_vec distr_y = cx_vec(M-2);
    cx_mat U = cx_mat(M-2, M-2);

    cx_double i = cx_double(0.0, 1.0);

    
    for (int i = 1; i < M-2; i++){
        distr_x(i) = exp(-(x(i) - mean_x)/(2.*var_x)*(x(i) - mean_x)/(var_x))*exp(i*p_x*(x(i) - mean_x));  // unnormalised Gaussian wave packet for x
        distr_y(i) = exp(-(y(i) - mean_y)/(2.*var_y)*(y(i) - mean_y)/(var_y))*exp(i*p_y*(y(i) - mean_y));  // unnormalised Gaussian wave packet for y
    }
    


    for (int i = 0; i < M-2; i++){
        for (int j = 0; j < M-2; j++){
            U(i, j) = distr_x(i)*distr_y(j);
        }
    }
    

    U = U/norm(U);  // making sure probability distribution starts out normalized to 1
    cout << U << endl;
    return U;
}


cx_mat potential(double v0, double M, int size_slit, int size_between_slit){
    cx_mat V = cx_mat(M-2, M-2, fill::zeros);
    for (int i = 0; i < M-2; i++){
        for (int j = 0; j < M-2; j++){
            if (j == (M-2)/2){
                V(i, j) = v0;

                for (int k = size_between_slit; k < size_between_slit + size_slit; k++){
                    if (i == (M-2)/2 + k){
                        V(i, j) = 0.0;
                    }
                    else if (i == (M-2)/2 - k){
                        V(i, j) = 0.0;
                    }
                }
            }
        }
    }
    return V;
}



void vector_ab(cx_double r, double dt, double M, cx_vec &a, cx_vec &b, cx_mat V){
    cx_vec v = V.as_col();
    cx_double i = cx_double(0.0, 1.0);

    for (int k = 0; k < pow(M-2, 2); k++){
        a(k) = 1. + 4.*r + i*dt/2.0 * v(k);
        b(k) = 1. - 4.*r - i*dt/2.0 * v(k);
    }
}


void matrix(cx_double r, cx_vec a, cx_vec b, sp_cx_mat &A, cx_mat &B, double M){
    for (int i = 0; i < (M-2)*(M-2); i++){
        A(i, i) = a(i);
        B(i, i) = b(i);
    }
    for (int i = 0; i < (M-2)*(M-2)-(M-2); i++){
        A(i, i + (M-2)) = -r;
        A(i + (M-2), i) = -r;
        B(i, i + (M-2)) = r;
        B(i + (M-2), i) = r;
    }
    for (int i = 0; i < (M-2)*(M-2)-1; i++){
        A(i, i + 1) = -r;
        A(i + 1, i) = -r;
        B(i, i + 1) = r;
        B(i + 1, i) = r;
        for (int j = 1; j < (M-2)*(M-2)-1; j++){
            if (i == j*(M-2) - 1){
                A(i, i + 1) = 0.0;
                A(i + 1, i) = 0.0;
                B(i, i + 1) = 0.0;
                B(i + 1, i) = 0.0;
            }
        }
    }
}


cx_cube CrankNicolson(cx_mat U, cx_mat B, sp_cx_mat A, double M, int N){
    cx_vec u = U.as_col();
    cx_vec b = cx_vec(u.size());
    cx_double p;

    cx_cube U_cube = cx_cube(M-2, M-2, N);
    for (int n = 0; n < N; n++){
        b = B*u;
        u = spsolve(A, b);
        for (int i = 0; i < (M-2); i++){
            for (int j = 0; j < (M-2); j++){
                U_cube(i, j, n) = u(i + j*(M-2));
            }
        }
    }
    return U_cube;
}


void write_to_file(cx_cube U_cube, string name){
    U_cube.save(name);
}
