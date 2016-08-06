for a in $(seq 117 1000); do make random_solutions_test && rm *.png && ./random_solutions_test $a 20 && rm ./$(ls *.png | tail -n 2) && gwenview $(ls *.png | tail -n 1); done
