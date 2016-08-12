Post Mortem
================

https://github.com/tilsche/icfpcontest2016/wiki/Post-Mortem

Build Everything
================

    sudo apt-get install libcgal-dev

    cd solver/
    mkdir BUILD
    cd BUILD
    cmake ..
    make

Notes
=====

- many used programs are located in solver/tests
  such as random_solutions_test.cpp which we used to generate problems

- most of the solver source code resides in solver/include/zebra (zebra is the namespace)
- solver/include/zebra/solver/brute.hpp and solver/include/zebra/solver/bfs.hpp are the main solvers used
  - solver/src/main.cpp contains the main function for the solver executable

- meta/ contains scripts for task / solution / submission management

