				GENERATE COLOR HISTOGRAMS, SIFT VECTORS AND MOTION VECTORS
				```````````````````````````````````````````````````````````

GENERATE COLOR HISTOGRAMS
`````````````````````````
1. Color histograms are generated using MATLAB and it is recommended that you install MATLAB R2016a.
2. In order to generate color histograms, place the .m files 'generate_color_histogram.m', 'split_frames.m', and the
'generate_block_histogram.m' files in a MATLAB directory. 
3. Make sure that the IMAGE PROCESSING TOOLBOX is installed along with MATLAB.
4. If not, install it by running the MATLAB installer again. In order for the install to be successful, you need a valid licence
  for the toolbox.
5. Once inside MATLAB, run the 'generate_block_histogram.m' file by passing the input directory, resolution, bin size(n), and output 
  file as parameters.
6. Alternatively you can call the 'generate_block_histogram()' function by passing the above parameters.
7. The color histograms are generated in the same folder as the videos.
8. The program generates color histograms for all the videos in the folder.

GENERATE SIFT VECTORS
`````````````````````

1. In order to generate SIFT vectors, you need a MATLAB installation. It is recommended that you use the latest version R2016a.
2. In order to generate SIFT vectors, you need the IMAGE PROCESSING TOOLBOX installed with you MATLAB installation. 
3. To install the IMAGE PROCESSING TOOLBOX, run the MATLAB isntaller again.
4. Next, configure the SIFT for MATLAB library by Andrea Vedaldi.
5. To do this, download the SIFT library suitable for your architecture.
6. Set the path to the SIFT library in MATLAB.
7. Compile the SIFT files using mex utility.
8. Now, you can call SIFT library files as MATLAB functions.
9. Run the 'generate_sift_vectors.m' file by passing the input directory, resolution, and output file as parameters.
10. Or run the 'generate_sift_vectors()' function by passing the input direcory, resolution and output file as parameters.

GENERATE MOTION VECTORS
```````````````````````

1. To generate the motion vectors, run the ffmpegMV.cpp file in Visual Studio 2015.
2. In order to run the file, configure the ffmepg library in visual studio.
3. Add ffmpeg/dev/include folder to Additional Include Directories in General tab of C/C++ configuration properties of Visual Studio.
4. Add ffmpeg/dev/lib folder to Additional Library directories in General Linker Configuration properties.
5. Then add all .lib files in  fmmpeg/dev/lib to Additional Dependencies under Linker>Input tab.
6. In C\C++ under All options, add 4996 to Disable Specific Warnings.
7. In C\C++ under All options, set Warning Level to Level4(/W4).
8. After the steps, compile ffmpegMV.cpp and copy the .dll files in ffmpeg/shared/bin into the project output directory.
9. Run the file from Visual Studio 2015.


