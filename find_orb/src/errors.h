#ifndef ERRORS_H_INCLUDE
#define ERRORS_H_INCLUDE

void adjust_error_ellipse_for_timing_error(double* sigma_a, double* sigma_b,
    double* angle, const double vx, const double vy);  

void convert_ades_sigmas_to_error_ellipse(const double sig_ra,
    const double sig_dec, const double correl, double* major,
    double* minor, double* angle);   

static void adjust_quadratic_form_for_timing_error(const double A,
    const double B, const double C, const double vx, const double vy,
    double* A1, double* B1, double* C1);

static void convert_error_ellipse_to_quadratic_form(const double a,
    const double b, const double angle,
    double* A, double* B, double* C);

static void convert_quadratic_form_to_error_ellipse(const double A,
    const double B, const double C, double* a, double* b,
    double* angle);



#endif
