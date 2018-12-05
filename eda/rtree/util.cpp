
namespace Util {
  struct Rectangle{
    double minX, minY;
    double maxX, maxY;
  };

  struct Point{
    double x,y;
  };

  double areaRectangle(const Rectangle &rectangle){
    return (rectangle.maxX - rectangle.minX) * (rectangle.maxY - rectangle.minY);
  }

  double costRectangle(Rectangle r, Point p){
      double area1 = areaRectangle(r);
      r.minX = min(r.minX, p.x);
      r.maxX = max(r.maxX, p.x);
      r.minY = min(r.minY, p.y);
      r.maxY = max(r.maxY, p.y);

      double area2 = areaRectangle(r);

      if(area2 <=area1)
        return 0.00;
      return area2 -area1

  }

  double costRectangle(Rectangle r, Rectangle rr){
      double area1 = areaRectangle(r);
      r.minX = min(r.minX, rr.minX);
      r.maxX = max(r.maxX, rr.maxX);
      r.minY = min(r.minY, rr.minY);
      r.maxY = max(r.maxY, rr.maxY);

      double area2 = areaRectangle(r);

      if(area2 <=area1)
        return 0.00;
      return area2 -area1

  }

  double getDistance(Rectangle group1, Rectangle group2, Point p){
    double d1 = costRectangle(group1, p);
    double d2 = costRectangle(group2, p);

    double answer = d1-d2;
    if(answer < 0) answer*=-1;
    return answer;
  }

  double getDistance(Rectangle group1, Rectangle group2, Rectangle r){
    double d1 = costRectangle(group1, r);
    double d2 = costRectangle(group2, r);

    double answer = d1-d2;
    if(answer < 0) answer*=-1;
    return answer;
  }

  Rectangle getBoudingRectangle(Rectangle r1, Rectangle r2){
    Rectangle answer;
    answer.minX = min(r1.minX, r2.minX);
    answer.maxX = max(r1.maxX, r2.maxX);
    answer.minY = min(r1.minY, r2.minY);
    answer.maxY = max(r1.maxY, r2.maxY);
  }


};
