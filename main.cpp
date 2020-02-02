#include <iostream>
#include <fstream>
#include <random>
#include <stdlib.h>
#include <future>
#include <cstddef>
#include<vector>
#include <algorithm>
#include <chrono>
#include<thread>
#include <mutex>
#include <string>
#include <cstring>
#include<cmath>


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

std::mutex mutex;
std::vector<point> result;

//creates svg image
void file(std::vector<point> points,size_t range){
  std::ofstream myfile;
  myfile.open ("convexhull.svg");

  myfile<< "<svg width='"<<range<<"' height='"<<range<<"'> \n";
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
  myfile.close();

}


double get_random_double(int min,int max) {
  double f = (double)rand() / RAND_MAX;
  return min + f * (max - min);
}

//function to generate points
std::vector<point> generate(size_t size,size_t range){
  std::vector<point> points;
  srand(time(NULL));
  size= size;
  for( size_t i =0;i<size;i++) {
    point p;
    p.x=get_random_double(0,range);
    p.y=get_random_double(0,range);
    points.push_back(p);
  }
  return points;

}
 double distance(point a, point b,point p){
    return (p.y-a.y)*(b.x-a.x)-(p.x-a.x)*(b.y-a.y);
}

//reccursive part of quick hull algorithm
void findhull (std::vector<point>&  points,point a,point b,point dist){

  if(points.empty()){
    mutex.lock();
    mutex.unlock();
    return;
  }

  mutex.lock();
  result.insert(std::find (result.begin(), result.end(), a),dist);
  mutex.unlock();

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

  findhull(aDistRight,a,dist,maxp);
  findhull(DistBRight,dist,b,minp);
}

template <typename TimePoint>
std::chrono::milliseconds to_ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp);
}

//finds minimum and maximum of vector
std::pair<point,point> minmax(std::vector<point>& points,int from,int to){
      std::pair<point,point> mm;
      mm.first = points[from];
      mm.second=points[from];
      for (int i = from+1;i<to;i++){
        if(points[i]<mm.first){
          mm.first=points[i];
        }
        if(mm.second<points[i]){
          mm.second=points[i];
        }
      }
      return mm;
}

//main function of quick hull algorithm
 void hull(std::vector<point>& points,int from,int to){
  std::pair<point,point> mm = minmax(points,from,to);

  mutex.lock();
  result.push_back(mm.first);
  result.push_back(mm.second);
  mutex.unlock();

  point maxp=mm.second;
  point minp=mm.first;
  std::vector<point> left;
  std::vector<point> right;
  double max =0;
  double min =0;

  for (int i = from;i<to;i++){
    auto dist=distance(mm.first,mm.second,points[i]);
    if(dist>0){
      left.push_back(points[i]);
      if(dist>max){
        max=dist;
        maxp=points[i];
      }
    }
    else if(dist<0){
      right.push_back(points[i]);
      if(dist<min){
        min=dist;
        minp=points[i];
      }
    }
  }

  findhull(right,mm.first,mm.second,minp);
  findhull(left,mm.second,mm.first,maxp);

  }



void print_usage(){
  std::cout << "specifying '--thread' or '--compare' without this argument will start in one thread" << '\n'
  <<"compare first is multy thread then , one thread"
  << "then you can write number of points and range, in this order."<< '\n'<<
  "if range is greater then 2000 display image is meaningless and will not be made.";
}
bool is_help(std::string const& argument) {
    return argument == "--help" || argument == "-h";
}
bool is_thread(std::string const& argument) {
    return argument == "--thread" || argument == "-t";
}
bool is_comp(std::string const& argument) {
    return argument == "--compare" || argument == "-c";
}


int main (int argc, char** argv) {
  size_t size =20;
  size_t range =1000;

  //next ifs dicide terminal arguments
  if (std::any_of(argv, argv+argc, is_help)) {
        print_usage();
        return 0;
  }
  bool t =0;
  if (std::any_of(argv, argv+argc, is_thread)) {
        t=1;
  }
  if (std::any_of(argv, argv+argc, is_comp)) {
        t=1;
  }

  if (argc == 1) {
          std::cout << "No arguments given, using default value of 20 points and 1000 range.\n";

  } else if (argc >= 4+t) {
        std::cout << "Wrong arguments given.\n";
        print_usage();
        return 1;

  }else if(argc == 1 + t){
    std::cout << "No numbers given, using default value of 20 points and 1000 range.\n";
  }

  else {
      std::size_t endpos1 =0;
      std::size_t endpos2 =0;
      try{
        size = std::stoi(argv[1+t], &endpos1);
        range = std::stoi(argv[2+t], &endpos2);
      }catch(...){
      }
      if (endpos1 != std::strlen(argv[1+t]) || endpos2!=std::strlen(argv[2+t]) || range < 0) {
            std::cout << "The arguments given are not valid numbers.\n";
            return 2;
      }
  }
  if(size<3){
    std::cout << "no convex hull for two points\n";
    return 3;
  }


  std::vector<point> points = generate(size,range);
  auto start = std::chrono::high_resolution_clock::now();

  if(t==1){ // thread mode - divides points vector to threads, then computes convex hull from semiresults
    std::vector<std::thread> ts;
    for (size_t i = 0; i < std::thread::hardware_concurrency()-1; ++i) {
          ts.emplace_back(std::thread(hull,std::ref(points),i*std::floor(points.size()/std::thread::hardware_concurrency()),(i+1)*std::floor(points.size()/std::thread::hardware_concurrency())));
    }ts.emplace_back (std::thread(hull,std::ref(points),(std::thread::hardware_concurrency()-1)*std::floor(points.size()/std::thread::hardware_concurrency()),points.size()));
    for(auto& th:ts){
      th.join();
    }

    std::vector<point> v = std::move(result);
    result.clear();
    hull(v,0,v.size());

  }else{//non thread mode
    hull(points,0,points.size());
  }


  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Needed " << to_ms(end - start).count() << " ms to finish.\n";


  if (std::any_of(argv, argv+argc, is_comp)) { //compare mode use this for single thread
    result.clear();
    auto start1 = std::chrono::high_resolution_clock::now();
    hull(points,0,points.size());
    auto end1 = std::chrono::high_resolution_clock::now();
    std::cout << "Needed " << to_ms(end1 - start1).count() << " ms to finish.\n";
  }

  if(range<2000){
    file(points,range);
  }

  return 0;
}
