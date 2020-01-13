#include <iostream>
#include <fstream>
#include <random>
#include <stdlib.h>
#include <cstddef>
#include<vector>
#include <algorithm>    // std::find

int size =0;

struct point{
  double x;
  double y;
  bool operator<(const point &rhs) const {
        return rhs.x < x;
    }
    bool operator==(const point &rhs) const {
          return rhs.x == x && rhs.y == y;
      }
};
std::vector<point> result;
char c=1;
void file(std::vector<point> points){
  std::ofstream myfile;
  myfile.open (std::to_string(c)+".svg");
  c++;
  //myfile << " <html> \n <body> \n <h1>My first SVG</h1> \n
  myfile<< "<svg width='500' height='500'> \n";
  for (auto p:points){

    myfile << "<circle cx='"<< p.x<<"' cy='"<<p.y<<"' r='4'   fill='yellow' /> \n" ;
  }
  myfile << "<polyline points='";
  for (auto p:result){
    myfile << p.x<<","<<p.y<<" ";
  }
  myfile<< result[0].x<<","<<result[0].y<<" ";
  myfile<<"' style='fill:none;stroke:black;stroke-width:3' /> \n";
  myfile << "</svg>";
//  myfile << "</body> \n </html> \n";
  myfile.close();

}


double get_random_double(int min,int max) {
  double f = (double)rand() / RAND_MAX;
  return min + f * (max - min);
}

std::vector<point> generate(){
  std::vector<point> points;
  srand(time(NULL));
  size = (rand() % 10)+3 ;
  for( int i =0;i<size;i++) {
    point p;
    p.x=get_random_double(0,500);
    p.y=get_random_double(0,500);
    points.push_back(p);
  }
  return points;

}
double distance(point a, point b,point p){
    return (p.y-a.y)*(b.x-a.x)-(p.x-a.x)*(b.y-a.y);
}
void findhull(std::vector<point> points,point a,point b,point dist,std::vector<point> control){
  if(points.empty()){
    return;
  }
  result.insert(std::find (result.begin(), result.end(), a),dist);
  std::vector<point> aDistRight;
  std::vector<point> DistBRight;
  point maxp=dist;
  double max =0;
  point minp=dist;
  double min =0;
  for (auto p:points){
    auto dist1=distance(a,dist,p);
    auto dist2=distance(dist,b,p);
    if(dist1<0){
      aDistRight.push_back(p);
      if(dist1<max){
        max=dist1;
        maxp=p;
      }
    }   else if(dist2<0){
      DistBRight.push_back(p);
      if(dist2<min){
        min=dist2;
        minp=p;
      }
    }
  }
  file(control);
findhull(aDistRight,a,dist,maxp,control);
findhull(DistBRight,dist,b,minp,control);

}


void hull(std::vector<point> points){
  std::sort(points.begin(), points.end());
  result.push_back(points[0]);
  result.push_back(points[points.size()-1]);
  std::vector<point> left;
  std::vector<point> right;
  point maxp=result[0];
  point minp=result[0];
  double max =0;
  double min =0;
  for (auto p:points){
    auto dist=distance(result[0],result[1],p);
    if(dist>0){
      left.push_back(p);
      if(dist>max){
        max=dist;
        maxp=p;
      }
    }
    else if(dist<0){
      right.push_back(p);
      if(dist<min){
        min=dist;
        minp=p;
      }
    }
  }
  auto a =result[0];
  auto b =result[1];
  file(points);
  findhull(right,result[0],result[1],minp,points);
  findhull(left,b,a,maxp,points);
  return;
}

int main () {
  std::vector<point> points = generate();
  hull(points);
  file(points);
  return 0;
}
