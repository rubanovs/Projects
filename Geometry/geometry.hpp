#include <stdint.h>

class Point;
class Segment;

class Vector {
 public:
  Vector();
  Vector(int64_t coordinate_x, int64_t coordinate_y);
  Vector(const Vector& other);
  Vector(Point& other);
  Vector(Point first, Point second);
  Vector& operator+=(const Vector& second);
  Vector& operator-=(const Vector& second);
  Vector operator*=(const int64_t& second);
  Vector operator-() const;
  int64_t GetX() const;
  int64_t GetY() const;

 private:
  int64_t point_x_;
  int64_t point_y_;
};
Vector operator+(const Vector& first, const Vector& second);
int64_t operator^(const Vector& first, const Vector& second);
Vector operator*(const Vector& first, const int64_t& value);
Vector operator*(const int64_t& value, const Vector& second);
int64_t operator*(const Vector& first, const Vector& second);

class IShape {
 public:
  virtual void Move(const Vector& other) = 0;
  virtual bool ContainsPoint(const Point& point) = 0;
  virtual bool CrossSegment(const Segment& seg) = 0;
  virtual IShape* Clone() = 0;
  virtual ~IShape() = default;
};

class Point : public IShape {
 public:
  Point();
  Point(int64_t first, int64_t second);
  Point(const Point& other);
  int64_t GetX() const;
  int64_t GetY() const;
  Point& operator=(const Point& second);
  Point& operator-=(const Point& second);
  Point operator-(const Point& second) const;
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& point) override;
  bool CrossSegment(const Segment& seg) override;
  IShape* Clone() override;

 private:
  int64_t point_x_;
  int64_t point_y_;
};

class Segment : public IShape {
 public:
  Segment();
  Segment(Point first, Point second);
  Segment(const Segment& other);
  Point GetA() const;
  Point GetB() const;
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& point) override;
  bool CrossSegment(const Segment& seg) override;
  IShape* Clone() override;

 private:
  Point point_a_;
  Point point_b_;
};

class Line : public IShape {
 public:
  Line();
  Line(Point first, Point second);
  Line(const Line& other);
  int64_t GetA() const;
  int64_t GetB() const;
  int64_t GetC() const;
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& point) override;
  bool CrossSegment(const Segment& seg) override;
  IShape* Clone() override;

 private:
  Point point_a_;
  Point point_b_;
};

class Ray : public IShape {
 public:
  Ray();
  Ray(Point first, Point second);
  Ray(const Ray& other);
  Point GetA() const;
  Vector GetVector();
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& point) override;
  bool CrossSegment(const Segment& seg) override;
  IShape* Clone() override;

 private:
  Point point_a_;
  Point point_b_;
};

class Circle : public IShape {
 public:
  Circle();
  Circle(Point first, int64_t second);
  Circle(const Circle& other);
  Point GetCentre() const;
  int64_t GetRadius() const;
  void Move(const Vector& vec) override;
  bool ContainsPoint(const Point& point) override;
  bool CrossSegment(const Segment& seg) override;
  IShape* Clone() override;

 private:
  Point point_o_;
  int64_t distance_from_o_to_circle_;
};

int64_t Lenght(Point first, Point second);
