//
// Created by shivani on 4/8/19.
//

/* sorts rectangles by area
 * searches area file to match with areas of rectangles
 * user can choose between linear and binary search
 * clock class to keep track of run time
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono> //NOLINT (build/c++11)

// Definition of the rectangles: includes area and coordinates of rectangles
class Rectangle {
 private:
  int area;
  // coordinates
  int left_x;
  int left_y;
  int right_x;
  int right_y;

 public:
  Rectangle(int Ax, int Ay, int Bx, int By) {
    area = (Bx-Ax)*(By-Ay);
    left_x = Ax;
    left_y = Ay;
    right_x = Bx;
    right_y = By;
  }
  ~Rectangle() {}
  // compares rectangles by area
  bool operator <(Rectangle second) {
    // if rectangles have same area, compare by left x coord
    if (this->area == second.area) {
      if (this->left_x < second.left_x)
        return true;
      else
        return false;
    } else if (this->area < second.area) {
      return true;
    } else {
      return false;
    }
  }
  // getter function for area
  int getArea() {
    return area;
  }

  // operator overloading to print rectangle coordinates
  friend std::ostream& operator <<(std::ostream &out, Rectangle rect){
    out << "((" << rect.left_x << ',' << rect.left_y << "), " << '(';
    out << rect.right_x << ',' << rect.right_y << "))";
    return out;
  }
};
// compares each area in area vector to area of rectangles in rectangle vector
// returns number of matches
int linear_search(std::vector<Rectangle> &rectangles, std::vector<int> &areas) {
  int count = 0;  // number of matches
  for (int &a : areas) {
    for (Rectangle &r : rectangles) {
      if (a == r.getArea()) {
        count++;
        break;
      }
    }
  }
  return count;
}
// finds matches between area in area vector and rectangles in rectangle vector
// returns number of matches
int binary_search(std::vector<Rectangle> &rectangles, std::vector<int> &areas) {
  int count = 0;  // keep track of matches
  for (int a : areas) {
    int low = 0;  // lower bound
    int high = rectangles.size()-1;  // upper bound
    int mid;  // middle index
    // as long as lower bound doesn't exceed upper bound loop continues
    while (low <= high) {
      mid = low + (high - low) / 2;
      if (a == rectangles[mid].getArea()) {
        count++;
        break;
      } else if (a < rectangles[mid].getArea()) {
        high = mid-1;
      } else {
        low = mid+1;
      }
    }
  }
  return count;
}

// Uses chrono to keep track of running time of program
class Clock {
 private:
  std::chrono::high_resolution_clock::time_point start;  // start time
  std::chrono::high_resolution_clock::time_point end;    // end time
 public:
  Clock() {}
  ~Clock() {}

  // start the clock
  void reset() {
    start = std::chrono::high_resolution_clock::now();
  }
  // stop the clock
  void endTime() {
    end = std::chrono::high_resolution_clock::now();
  }
  // calculates elapsed time between start and end
  // returns elapsed time as a double
  double elapsedTime() {
    auto temp = std::chrono::duration<double, std::micro>(end-start);
    double elapsed_us = temp.count();
    return elapsed_us;
  }
};


int main(int argc, char *argv[]) {
  // error management
  if (argc != 4) {
    std::cerr << "Usage: ./rectangle_search <rectangle_file.dat> ";
    std::cerr << "<area_file.dat> <result_file.dat>\n";
    return 0;
  }

  std::vector<Rectangle> rectangles;  // vector of Rectangle objects
  std::vector<int> areas;  // vector of area values

  // read rect_file into vector of Rectangles
  std::ifstream rect_file;
  rect_file.open(argv[1]);

  if (rect_file.is_open()) {
    while (!rect_file.eof()) {
      int Ax, Ay, Bx, By;
      rect_file >> Ax >> Ay >> Bx >> By;
      Rectangle rect(Ax, Ay, Bx, By);
      rectangles.push_back(rect);
    }
  } else {   // if file cannot be opened, output an error
    std::cerr << "Error: cannot open file " << argv[1] << std::endl;
    return 0;
  }
  rect_file.close();

  // read area file into vector of ints
  std::ifstream area_file;
  area_file.open(argv[2]);

  if (area_file.is_open()) {
    int value;
    while (area_file >> value) {
      areas.push_back(value);
    }
  } else {  // if area_file cannot be opened
    std::cerr << "Error: cannot open file " << argv[2] << std::endl;
    return 0;
  }

  area_file.close();

  // get choice of search
  std::cout << "Choice of search method ([l]inear, [b]inary)?\n";

  int matches;
  bool run_search = false;  // whether or not search has been run yet
  char choice;  // user's choice of l or b
  Clock ct;     // clock object used to find run time of search

  while (run_search != true) {
    std::cin >> choice;  // get user choice for linear or binary search
    // switch case for linear and binary search
    switch (choice) {
      case 'l':
        ct.reset();         // reset time
        matches = linear_search(rectangles, areas);
        ct.endTime();       // stop time
        run_search = true;  // search has been run, the while loop will break
        break;

      case 'b':
        ct.reset();
        // sort the rectangles vector by area before binary search
        std::sort(rectangles.begin(), rectangles.end());
        // matches contains number of matches found by binary_search
        matches = binary_search(rectangles, areas);
        ct.endTime();
        run_search = true;
        break;

      default:
        std::cerr << "Incorrect choice\n";  // user doesn't enter either l or b
    }
  }
  // print time
  std::cout << "CPU time: " << ct.elapsedTime() << " microseconds\n";

  // write number of matches to output file
  std::ofstream output;
  output.open(argv[3]);  // output file was given in 3rd command line argument

  // if file cannot be opened, error message output
  if (!output.is_open()) {
    std::cerr << "Error: cannot open file " << argv[3] << std::endl;
    return 0;
  }
  output << matches;
  output.close();

  return 0;
}

