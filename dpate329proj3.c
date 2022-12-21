/*
* project3.c
*
* CS 211 : Project 3
*
* Author: Deep Patel, UIC, Fall 2021
*
* Description: This program contains lots of functions to
* analyse the 2D chars array by implementing several functions
* over the array which by manipulating the array and creating
* what is required. Each function requires array and size as the
* parameters to work over the array and implement the description provided.
* Also, contains additional two string function to trim and find pattern
* main() implement all the functions for the user to know when what is being
* happenend.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "prog3.h"

/**
* 
*  Function "stubs" for Assignment 3 exercises.
*
*  See prog3.h for specifications of function behavior.
*
*/



/***********************************************
*                                             **
*   MATRIX FUNCTIONS                          **
*                                             **
*   NOTE:  parameter n ALWAYS expresses the   **
*     dimensions of the square matrix in      **
*     question.                               **
*                                             **
***********************************************/


/* Description:  Dynamically allocates a square nxn 2D
*       matrix of characters.  The individual matrix
*       elements need not be initialized.
*/
char ** alloc_square_mtx(int n) {
    // allocates 2D chars array of size n
    char ** sq_mtx = (char**)malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
    sq_mtx[i] = (char*)malloc(n*sizeof(char));
    }
    return sq_mtx;
}

/* deallocates all dyanamically allocated
*  components of a matrix previously produced by
*  alloc_square_mtx.
*/
void free_square_mtx(char **m, int n) {
  for (int i = 0; i < n; i++) {
    free(m[i]); // frees at each index of array
  }
  free(m);
}

/*  Description:  populates square matrix of characters
*		with random, lower-case letters a..z
*/
void pop_mtx_alpha(char **m, int n) {
  srand(time(NULL));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      m[i][j] = 'a' + (rand() % 26); // assigns random alphabet
    }
  }
}

/* prints given square matrix row-by-row (printing
*  a newline after each row).
*/
void display_mtx(char **m, int n) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      printf("%c", m[i][j]); // prints each element
    }
    printf("\n");
  }
}

/* swaps the contents of rows r1 and r2 of
*  square matrix m.  Ordering of characters
*  within rows are preserved.
*/
void swap_rows(char **m, int n, int r1, int r2) {
  char temp;
  if (r1 > n || r2 > n) {
    return;
  } else {
    for (int i = 0; i < n; i++) {
      temp = m[r1][i];
      m[r1][i] = m[r2][i]; // swaps with appropriate row
      m[r2][i] = temp; // replaces with the original
    }
  }
}

/*  swaps the contents of column c1 and c2 of
*   square matrix m.  Ordering of characters
*   within rows are preserved.
*/
void swap_cols(char **m, int n, int c1, int c2) {
  char temp;
  if (c1 > n || c2 > n) {
    return;
  } else {
    for (int i = 0; i < n; i++) {
      temp = m[i][c1];
      m[i][c1] = m[i][c2]; // swaps with appropriate column
      m[i][c2] = temp; // replaces witht the original
    }
  }
}


/* "rotates" square matrix m 90 degrees right
*   (clockwise).
*/
void rotate_right(char **m, int n) {
    char temp;
    for(int row = 0; row < n; row++) {
        for(int column = 0; column < n; column++) {
            temp = m[row][column];
            m[row][column] = m[column][row]; // horizontal to vertical
            m[column][row] = temp; // exchanges with elements
        }
    }
    for (int i = 0; i < n/2; i++) {
        // swaps after exchange
        int first_swap = i;
        int sec_swap = n-i-1;
        swap_cols(m, n, first_swap, sec_swap); // to transform to appropriate rotation
    }
}

/* popluates the given matrix with with the specified
*  number of  "boulders" randomly placed.
*  All other matrix entries are set to "air"
*/
void floating_boulders(char **m, int n, int nb) {
    // conditions in order to work
    if (nb < 0) {
        nb = 0;
    } else if (nb > n*n) {
        nb = n*n;
    }
    int count_boulders = 0;
    srand(time(NULL)); // gives different every run
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int val = rand(); // to assign randomly
            if(val % 2 == 0 && count_boulders < nb) {
                m[i][j] = '#';
                count_boulders += 1;
            }
            else {
                m[i][j] = '.';
            }
        }
    }
}

/* populates the given matrix with a random ASCII
* "landscape"
* Air is represented by just periods '.'
* Land is represnted by pound-symbols '#'
* Air is always above land
*/
void mountains(char **m, int n) {
    srand(time(NULL)); // different result every run
    for (int column = 0; column < n; column++) {
        int fill = rand() % n;
        for (int row = 0; row < n; row++) {
            if (row < fill) { // everything above land
                m[row][column] = '.';
            } else { // everything below air
                m[row][column] = '#';
            }
        }
    }
}


/*  "sinks" the given matrix/image by one layer and introduces
*   "air" as a new top layer.
*/
void sink(char **m, int n) {
    for (int row = n-1; row > 0; row--) { // since the bottom is sinking
        for (int column = 0; column < n; column++) {
            m[row][column] = m[row-1][column]; // sinks by one row
        }
    }
    for (int i = 0; i < n; i++) {
        m[0][i] = '.'; // top layer is air
    }
}


/* The character '.' represents Air
* - All other characters are assumed to be "heavier than air" --
* i.e., they will fall when the force of gravity is
* applied.
*/
void gravity(char **m, int n) {
    int air;
    for (int column = 0; column < n; column++) { // by each column
        air = 0;
        for (int row = n-1; row >= air; row--) {
            if (m[row][column] == '.') {
                air += 1; // increments air count in that column
            } else {
                m[row+air][column] = m[row][column]; // to get land beneath
            }
        }
        for (int row = 0; row < air; row++) {
            m[row][column] = '.'; // uses air count for particular column
        }
    }
}




/***********************************************
*                                             **
*   STRING FUNCTIONS                          **
*                                             **
***********************************************/

/* effectively removes all leading and trailing
*  whitespace from the given string s.
*/
void str_trim(char *s) {
    int length = strlen(s);
    int leading = 0; // spaces before string
    int trailing = 0; // spaces after string
    while (leading < length && s[leading] == ' ') {
        leading += 1; // increment leading space count
    }
    while (trailing < length && s[length - trailing - 1] == ' ') {
        trailing += 1; // increment trailing space count
    }
    for (int i = leading; i < length - trailing; i++) {
        s[i-leading] = s[i]; // trims by appropriate leading space count
    }
    s[length-leading-trailing] = '\0'; // for trailing
}

/* takes two C strings:  a "source string" s[] and
*  a "pattern string" and determines if the given
*  pattern is a substring of s. return index of first found
*/
int str_search(char *s, char *pattern) {
    int str_len = strlen(s); // string's length
    int pattern_len = strlen(pattern); // pattern's length
    int to_match = 0; // while consecutives matches
    int found = 0; // onces the length of pattern matches
    int first_match = 0; // index for first occurence of the pattern
    for (int i = 0; i < str_len; i++) {
        if (to_match == pattern_len) { // once pattern len matches
            found = i;
            break;
        } 
        if (pattern[to_match] == s[i]) { // by every element
            to_match += 1; // increments consecutive
        } else {
            to_match = 0; // pattern is broken
        }
    }
    first_match = found - to_match; // returns index of occurence
    if (to_match < pattern_len) { // pattern not found
        return -1;
    } else {
        return first_match; // pattern found
    }
}


/*

#ifndef _LIB  // DO NOT REMOVE THIS LINE!!!
              // IT DOESN"T CHANGE HOW YOUR PROGRAM
              // WORKS, BUT IT GIVES US A WAY TO CALL
              // YOUR FUNCTIONS FROM OUR CODE FOR
              // TESTING


* Write a test driver in main below to exercise and
*   your function implementations.
*
* Think carefully about your test cases!
*/

// main() for debugging and printing out the after each function
// to check if everything is correctly working.
// Calls every function outside the main() and prints the output
int main() {

    char str[] = "  hello there  ";
    printf("\nPrinting string:%s\n",str);
    str_trim(str);
    printf("\nPrinting string after trim:%s\n",str);
    printf("\nIndex for pattern %s in %s: %d\n", "'ello'", str, str_search(str,"ello"));
     printf("\nIndex for pattern %s in %s: %d\n", "'eot'", str, str_search(str,"eot"));
    printf("\nIndex for pattern %s in %s: %d\n", "'the'", str, str_search(str,"the")); 

    int size = 6;
    char ** sq_mtx;

    printf("\nDynamically allocating a square matrix: %d X ", size);
    printf("%d\n\n", size);
    sq_mtx = alloc_square_mtx(size);
    printf("Populating matrix of chars with random, lower letters a..z:" "\n\n");
    pop_mtx_alpha(sq_mtx, size);
    printf("Printing given matrix row-by-row:" "\n\n");
    display_mtx(sq_mtx, size);

    
    swap_rows(sq_mtx, size, 0, 1);
    printf("\nPrinting matrix after swapping rows r1 and r2:" "\n\n");
    display_mtx(sq_mtx, size);

    
    swap_cols(sq_mtx, size, 2, 3);
    printf("\nPrinting matrix after swapping columns c1 and c2:" "\n\n");
    display_mtx(sq_mtx, size);

    rotate_right(sq_mtx, size);
    printf("\nPrinting matrix after rotating right:" "\n\n");
    display_mtx(sq_mtx, size);
    
    printf("\nPrinting floating boulders:" "\n\n");
    floating_boulders(sq_mtx, size, 15);
    display_mtx(sq_mtx, size);

    printf("\nPrinting mountains:" "\n\n");
    mountains(sq_mtx, size);
    display_mtx(sq_mtx, size);

    printf("\nPrinting sink:" "\n\n");
    sink(sq_mtx, size);
    display_mtx(sq_mtx, size);

    printf("\nPrinting gravity:" "\n\n");
    gravity(sq_mtx, size);
    display_mtx(sq_mtx, size);

    free_square_mtx(sq_mtx, size);
    return 0;
}
