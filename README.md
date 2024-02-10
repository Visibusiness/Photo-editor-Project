// Copyright Vișănescu Bogdan-Emilian 313CAb 2023 - 2024

# Image Editing Program

## Overview

This program is designed for editing images in the NETPBM format,
supporting both grayscale and RGB images. It provides various functionalities
for loading, selecting, manipulating, and saving images.

## Program Structure

The program is structured around an `Image` type that holds
information about the image, including format, dimensions, color depth,
and the image matrix. Key functionalities include loading images, selecting
parts of the image, generating histograms, saving images, equalizing grayscale
images, cropping, rotating, and applying filters to RGB images.

## Commands

- **Load**: The program can load images in both ASCII and binary formats.
- **Select Part**: Allows users to select a specific part of the image matrix.
- **Select All**: Selects the entire image.
- **Histogram**: Generates a histogram for grayscale images.
- **Save**: Saves the output photo in either ASCII or binary format.
- **Equalize**: Equalizes grayscale images.
- **Exit Program**: Frees allocated memory and exits the program.
- **Crop**: Crops the image based on the current selection.
- **Rotate**: Rotates the image at 90-degree angles.
- **Apply**: Applies filters EDGE, SHARPEN, BLUR, GAUSSIAN_BLUR to RGB images.

The structure contains:
- a char pointer that represents the format of 
the image: P2, P3, P5, P6.
- col represents the columns of the matrix
- row represents the rows of the matrix
- maxval represents the maximum value of a colour of a pixel
- **matrix represents the matrix of the image
- x1, x2, y1, y2 represent the selection of the matrix
- rgb is 3 if the matrix is rgb and 1 if the matrix is grayscale 

## Commands Explained

# LOAD 

The program can load a matrix into memory. The load function is
built from 2 functions: ascii and binary.
Ascii function opens the reading text mode and scans each variable
in order using fscanf because they
are in text format in the file.
Binary function opens the file in binary mode and scans
the using fscanf the format, col, row and maxval.
The matrix is loaded using fread because its format is binary.

## SELECT

    -select_part-
This is a function first checks if the matrix is loaded.
Then copies the selection parameters in coord using atoi.
Then changes the selection to the given parameters.

## SELECT ALL

    -select all-
Select all is a function that selects all image.

## Histogram

    -histogram-
Histogram function realises the histogram of an image using x
stars and y bins. In the array nr_stars will be calculated
the number of stars for each group of values. In freq will 
be calculated the frequance of each value of a color of a pixel.
Then using a math formula the number of pixels is calculated in
nr_stars and printed. It is working only with
grayscale pictures.

## SAVE

    -save-
This functione is used to save the output photo. It can be saved
ascii and binary. Save_ascii opens the output file and prints
using fprintf every part of the image begining with the magic
word and finishing with the matrix. It prints them in ascii format.
The binary function does the same thing, but in binary
mode.

## EQUALIZE

    -equalize-
The equalize function is used to equalize an image. It can
be used for old photoes, to restore them. It is working only with
grayscale pictures. Firstly it calculates the frequance of a value of
a color. Then it uses a formula to calculate the new value of a pixel color.
Because the value cannot be greater than 255 or less than 0 i've 
used a clamp function. I have also used a round function because the result
must be int.

## EXIT

    -exit program-
The exit function frees all allocated matrixes and then
exits the program.

## CROP

    -Crop-
The crop function crops the image by the current selection.
It uses an auxiliary matrix called aux. It builds the new matrix
into aux and then modifies the row and col to the selection.
Then reallocates the image with the new size and copies aux into
image. In the end, it frees aux and changes the selection to all
photo.

## ROTATE

    -rotate-
My rotate rotates repeatedly the image at 90 degrees depending
by the angle. If the angle is 360 degrees or 0 degrees the
rotation is useless and the program prints just "Rotated <angle>".
Else the image is rotated at 90 degrees repeteadly.
In Rotate_90 i've allocated a matrix called aux in wich i will rotate
image->matrix selection. Then if the selection is the
entire image it frees the old one and makes the image->matrix
to point to aux. Then swaps the coordinates. Else the selection
must be squared so i don't swap anything and just put the
aux values in place in image->matrix. Then i free aux.

## APPLY

    -apply-
This function aplies a filter to the selection of an image.
This filter is one of EDGE SHARPEN BLUR and GAUSSIAN_BLUR.
Also the photo must be rgb. 
First the program builds the kerner matrix into matrix variable.
Then it ensueres that not all the photo is selected. It does
that increment and decrement to ensure that it will not
crash when using the indexes from down. Then it replaces all
the pixel values from a 3 x 3 matrix of pixels from 
the big matrix with the sum of the old values multiplied each
with the kerner corespondent. It does this individual with red,
green and blue values of a pixel.

## MAIN

    -main-
In main i have declared an image type called img, a command in
wich i will read line by line the commands given from stdin,
a token, an index that will count the number of the words of
a comand, and words wich is a char** in wich i will keep all
the words of the command. Into an infinite loop i read from 
stdin the command using fgets, then i break it into words and
count them into index and copy the words into words variable.
Then i compare the first word to the different posible commands
from the program.



