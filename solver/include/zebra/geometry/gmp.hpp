#ifndef SOLVER_GMP_HPP
#define SOLVER_GMP_HPP

inline double gmpq_to_double(CGAL::Gmpq q)
{
    return q.numerator().to_double() / q.denominator().to_double();
}

#endif // SOLVER_GMP_HPP
