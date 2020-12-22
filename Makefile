all:
	g++ -g -Wall -Wextra -o load_mri cpp/load_mri.cc

optim2:
	g++ -O2 -Wall -Wextra -o load_mri cpp/load_mri.cc
	
optim3:
	g++ -O3 -Wall -Wextra -o load_mri cpp/load_mri.cc