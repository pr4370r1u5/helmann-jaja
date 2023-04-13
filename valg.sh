#!/bin/bash

make metrics IMPL=hj
valgrind --leak-check=full --error-exitcode=1 --tool=memcheck --track-origins=yes --errors-for-leak-kinds=definite --show-leak-kinds=definite ./metrics -t50 -n 10000
