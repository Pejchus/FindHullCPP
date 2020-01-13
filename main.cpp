#include <iostream>
#include <fstream>
#include <random>
#include <stdlib.h>
#include <cstddef>
#include<set>

int size =0;

struct point{
  double x;
  double y;
  bool operator<(const point &rhs) const {
        return rhs.x < x;
    }
};


void file(std::set<point> points){
  std::ofstream myfile;
  myfile.open ("example.svg");
  //myfile << " <html> \n <body> \n <h1>My first SVG</h1> \n
  myfile<< "<svg width='500' height='500'> \n";
  for (auto p:points){
    myfile << "<circle cx='"<< p.x<<"' cy='"<<p.y<<"' r='4'   fill='yellow' /> \n" ;
  }
  myfile << "</svg>";
//  myfile << "</body> \n </html> \n";
  myfile.close();

}


double get_random_double(int min,int max) {
  double f = (double)rand() / RAND_MAX;
  return min + f * (max - min);
}

std::set<point> generate(){
  std::set<point> points;
  size = (rand() % 10)+3 ;
  for( int i =0;i<size;i++) {
    point p;
    p.x=get_random_double(0,500);
    p.y=get_random_double(0,500);
    points.insert(p);
  }
  return points;

}

double ** hull(double ** points){
}

int main () {
  std::set<point> points = generate();

  file(points);
  return 0;
}
