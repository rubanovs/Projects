#include "geometry.hpp"

#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <iostream>

Vector::Vector() {
  point_x_ = 0;
  point_y_ = 0;
}

Vector::Vector(int64_t coordinate_x, int64_t coordinate_y) {
  point_x_ = coordinate_x;
  point_y_ = coordinate_y;
}

Vector::Vector(const Vector& other) {
  point_x_ = other.point_x_;
  point_y_ = other.point_y_;
}

Vector::Vector(Point& other) {
  point_x_ = other.GetX();
  point_y_ = other.GetY();
}

Vector::Vector(Point first, Point second) {
  point_x_ = second.GetX() - first.GetX();
  point_y_ = second.GetY() - first.GetY();
}

Vector& Vector::operator+=(const Vector& second) {
  point_x_ += second.GetX();
  point_y_ += second.GetY();
  return *this;
}

Vector& Vector::operator-=(const Vector& second) {
  point_x_ -= second.GetX();
  point_y_ -= second.GetY();
  return *this;
}

Vector Vector::operator*=(const int64_t& second) {
  point_x_ *= second;
  point_y_ *= second;
  return *this;
}

Vector Vector::operator-() const {
  Vector tmp(-point_x_, -point_y_);
  return tmp;
}

int64_t Vector::GetX() const { return point_x_; }

int64_t Vector::GetY() const { return point_y_; }

Vector operator+(const Vector& first, const Vector& second) {
  Vector copy = first;
  copy += second;
  return copy;
}

Vector operator-(const Vector& first, const Vector& second) {
  Vector copy = first;
  copy -= second;
  return copy;
}

int64_t operator^(const Vector& first, const Vector& second) {
  return first.GetX() * second.GetY() - first.GetY() * second.GetX();
}

Vector operator*(const Vector& first, const int64_t& value) {
  Vector copy = first;
  copy *= value;
  return copy;
}

Vector operator*(const int64_t& value, const Vector& second) {
  return second * value;
}
int64_t operator*(const Vector& first, const Vector& second) {
  return first.GetX() * second.GetX() + first.GetY() * second.GetY();
}

Point::Point() {
  point_x_ = 0;
  point_y_ = 0;
}

Point::Point(int64_t first, int64_t second) {
  point_x_ = first;
  point_y_ = second;
}

Point::Point(const Point& other) {
  point_x_ = other.point_x_;
  point_y_ = other.point_y_;
}

int64_t Point::GetX() const { return point_x_; }

int64_t Point::GetY() const { return point_y_; }

Point& Point::operator=(const Point& second) {
  point_x_ = second.point_x_;
  point_y_ = second.point_y_;
  return *this;
}

Point& Point::operator-=(const Point& second) {
  point_x_ -= second.point_x_;
  point_y_ -= second.point_y_;
  return *this;
}

Point Point::operator-(const Point& second) const {
  Point copy = *this;
  copy -= second;
  return copy;
}

void Point::Move(const Vector& vec) {
  point_x_ += vec.GetX();
  point_y_ += vec.GetY();
}

bool Point::ContainsPoint(const Point& point) {
  return (point_x_ == point.point_x_ && point_y_ == point.point_y_);
}

bool Point::CrossSegment(const Segment& seg) {
  Segment copy = seg;
  return copy.ContainsPoint(*this);
}

IShape* Point::Clone() { return new Point(*this); }

Segment::Segment() {}

Segment::Segment(Point first, Point second) {
  point_a_ = first;
  point_b_ = second;
}

Segment::Segment(const Segment& other) {
  point_a_ = other.point_a_;
  point_b_ = other.point_b_;
}

Point Segment::GetA() const { return point_a_; }

Point Segment::GetB() const { return point_b_; }

void Segment::Move(const Vector& vec) {
  point_a_.Move(vec);
  point_b_.Move(vec);
}

bool Segment::ContainsPoint(const Point& point) {
  Vector a_b(point_a_, point_b_);
  Vector a_p(point_a_, point);
  Vector b_p(point_b_, point);
  return ((a_p * b_p) <= 0 && (a_b ^ a_p) == 0);
}

bool Segment::CrossSegment(const Segment& seg) {
  Segment loc = seg;
  Vector a_b(point_a_, point_b_);
  Vector a_c(point_a_, seg.point_a_);
  Vector a_d(point_a_, seg.point_b_);
  Vector c_a(seg.point_a_, point_a_);
  Vector c_b(seg.point_a_, point_b_);
  Vector c_d(seg.point_a_, seg.point_b_);
  int64_t mult_one = (c_d ^ c_b) * (c_d ^ c_a);
  int64_t mult_two = (a_b ^ a_c) * (a_b ^ a_d);
  return (((mult_one < 0) && (mult_two < 0)) ||
          (ContainsPoint(seg.point_a_) || ContainsPoint(seg.point_b_) ||
           loc.ContainsPoint(point_a_) || loc.ContainsPoint(point_b_)));
}

IShape* Segment::Clone() { return new Segment(*this); }

Line::Line() {}

Line::Line(Point first, Point second) {
  point_a_ = first;
  point_b_ = second;
}

Line::Line(const Line& other) {
  point_a_ = other.point_a_;
  point_b_ = other.point_b_;
}

int64_t Line::GetA() const { return point_b_.GetY() - point_a_.GetY(); }

int64_t Line::GetB() const { return point_a_.GetX() - point_b_.GetX(); }

int64_t Line::GetC() const {
  return (point_a_.GetY() * point_b_.GetX()) -
         (point_a_.GetX() * point_b_.GetY());
}

void Line::Move(const Vector& vec) {
  point_a_.Move(vec);
  point_b_.Move(vec);
}

bool Line::ContainsPoint(const Point& point) {
  Vector a_b(point_a_, point_b_);
  Vector a_p(point_a_, point);
  return (a_b ^ a_p) == 0;
}

bool Line::CrossSegment(const Segment& seg) {
  Vector a_b(point_a_, point_b_);
  Point seg_a = seg.GetA();
  Point seg_b = seg.GetB();
  Vector a_c(point_a_, seg_a);
  Vector a_d(point_a_, seg_b);
  return ((a_b ^ a_c) * (a_b ^ a_d)) <= 0;
}

IShape* Line::Clone() { return new Line(*this); }

Ray::Ray() {}

Ray::Ray(const Ray& other) {
  point_a_ = other.point_a_;
  point_b_ = other.point_b_;
}

Ray::Ray(Point first, Point second) {
  point_a_ = first;
  point_b_ = second;
}

Point Ray::GetA() const { return point_a_; }

Vector Ray::GetVector() {
  Vector vec(point_a_, point_b_);
  return vec;
}

void Ray::Move(const Vector& vec) {
  point_a_.Move(vec);
  point_b_.Move(vec);
}

bool Ray::ContainsPoint(const Point& point) {
  Vector a_b(point_a_, point_b_);
  Vector a_p(point_a_, point);
  return ((a_b * a_p) >= 0 && (a_b ^ a_p) == 0);
}

bool Ray::CrossSegment(const Segment& seg) {
  Segment copy = seg;
  Vector a_b(point_a_, point_b_);
  Point seg_a = seg.GetA();
  Point seg_b = seg.GetB();
  Vector a_c(point_a_, seg_a);
  Vector a_d(point_a_, seg_a);
  int64_t mult_one = (a_b ^ a_c) * (a_b ^ a_d);
  int64_t mult_two = (a_b * a_c);
  int64_t mult_three = (a_b * a_d);
  return ((mult_one <= 0 && mult_two >= 0 && mult_three >= 0) ||
          (ContainsPoint(seg_a) || ContainsPoint(seg_a) ||
           copy.ContainsPoint(point_a_) || copy.ContainsPoint(point_b_)));
}

IShape* Ray::Clone() { return new Ray(*this); }

Circle::Circle() { distance_from_o_to_circle_ = 0; }

Circle::Circle(Point first, int64_t second) {
  point_o_ = first;
  distance_from_o_to_circle_ = second;
}

Circle::Circle(const Circle& other) {
  point_o_ = other.GetCentre();
  distance_from_o_to_circle_ = other.GetRadius();
}

Point Circle::GetCentre() const { return point_o_; }

int64_t Circle::GetRadius() const { return distance_from_o_to_circle_; }

void Circle::Move(const Vector& vec) { point_o_.Move(vec); }

bool Circle::ContainsPoint(const Point& point) {
  int64_t centre_x = point_o_.GetX();
  int64_t centre_y = point_o_.GetY();
  int64_t point_x = point.GetX();
  int64_t point_y = point.GetY();
  int64_t squar_r = distance_from_o_to_circle_ * distance_from_o_to_circle_;
  return ((centre_x - point_x) * (centre_x - point_x) +
              (centre_y - point_y) * (centre_y - point_y) <=
          squar_r);
}

int64_t Lenght(Point first, Point second) {
  return (second.GetX() - first.GetX()) * (second.GetX() - first.GetX()) +
         (second.GetY() - first.GetY()) * (second.GetY() - first.GetY());
}

bool Circle::CrossSegment(const Segment& seg) {
  Vector a_o(seg.GetA(), point_o_);
  Vector a_b(seg.GetA(), seg.GetB());
  int64_t radius = distance_from_o_to_circle_ * distance_from_o_to_circle_;
  int64_t square = (a_b ^ a_o) * (a_b ^ a_o);
  return ((square <= radius * Lenght(seg.GetA(), seg.GetB())) &&
          (Lenght(point_o_, seg.GetA()) >= radius ||
           Lenght(point_o_, seg.GetB()) >= radius)) ||
         (((a_o * a_b) < 0) && !(Lenght(point_o_, seg.GetA()) > radius &&
                                 Lenght(point_o_, seg.GetB()) > radius));
}

IShape* Circle::Clone() { return new Circle(*this); }

