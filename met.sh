#!/bin/bash

make metrics IMPL=hj


./metrics -t 50 -n 10000000
