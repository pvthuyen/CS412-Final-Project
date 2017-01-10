# CS 412 Final Project

## Dependencies
	OpenCV 2.4.9 or higher
	HDF5 library
	Armadillo
	CMake 3.2 or higher
	Node JS

## Quick Installation (Ubuntu)
```bash
	# Make sure that your repos are updated 
	sudo apt-get update

	# Install OpenCV
	sudo apt-get install libopencv-dev
	# Install HDF5
	sudo apt-get install libhdf5-dev

	# Install CMake 
	https://cmake.org/install/

	# Install Nodejs
	sudo apt-get install node npm
```

## Build the code
	```bash
	cd core
	cmake .
	make
	```

## Run the code
	To run the code you need to download the codebook clusters here: https://drive.google.com/file/d/0BymJbdWH_Vu_ZXhnZ3l4SDlfZ2s/view and put it in data folder.
	```bash
		# Run the C++ binary. This might takes one or two hours if this is the first time running.
		./ImageRetrievalCpp

		# Open another terminal, run
		node server.js
	```

## Test the code by open http://localhost:8081/ using your browser.
