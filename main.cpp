#include <iostream>
#include <fstream>
#include <random>
#include <stdlib.h>
#include <cstddef>


int size =0;

void cleanup(double ** points){
  for (int   h = 0; h < size; h++)
      {
            delete [] points[h];
      }
      delete [] points;
      points = 0;
}
void file(double  **points){
  std::ofstream myfile;
  myfile.open ("example.svg");
  //myfile << " <html> \n <body> \n <h1>My first SVG</h1> \n
  myfile<< "<svg width='500' height='500'> \n";
  for (size_t x = 0; x < size; ++x){
    myfile << "<circle cx='"<< points[x][0]<<"' cy='"<<points[x][1]<<"' r='4'   fill='yellow' /> \n" ;
  }
  myfile << "</svg>";
  myfile << "</body> \n </html> \n";
  myfile.close();
  cleanup(points);
}


double get_random_double(int min,int max) {
  double f = (double)rand() / RAND_MAX;
  return min + f * (max - min);
}

double ** generate(){
  size = (rand() % 10)+3 ;
  double**points = 0;
  points = new double*[size];
  for( int i =0;i<size;i++) {
    points[i] = new double[2];
    points[i][0]=get_random_double(0,1000);
    points[i][1]=get_random_double(0,1000);
  }
  return points;

}



int main () {
  double ** points = generate();

  file(points);
  return 0;
}
