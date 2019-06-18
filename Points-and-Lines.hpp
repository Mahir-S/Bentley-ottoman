#include <bits/stdc++.h>
using namespace std;
#define EPS 0.00000000000002
class Point
{
	public :
	long double x;/**< x coordinate of point*/
	long double y;/**< y coordinate of point*/
	// long double EPS  = 0.00000000000002;

	public :
	/**default constructor initializes everything to zero 
	*/
	Point() 
	{
		x = 0, y = 0;
	}
	/**parameterized constructor
	*@param x 
	*@param y
	*/
	Point(long double _x, long double _y)
	{
		x = _x, y = _y;
	}
	/**overloading equal to operator
	*@return 0 or 1
	*/
	bool operator == (Point &p)
	{
		return abs(x-p.x) < EPS and abs(y - p.y) < EPS;
		//return (x == p.x and y == p.y);
	}
	/**function to check whether the point lies between P and Q
	*@param P
	*@param Q
	*@return 0 or 1
	*/
	bool between(Point &P, Point &Q)
 	{
 		bool checkX = x < max(P.x, Q.x) + EPS && x + EPS > min(P.x, Q.x);
 		bool checkY = y < max(P.y, Q.y) + EPS && y + EPS > min(P.y, Q.y);
 		return checkX && checkY;
 	}
};

class Segment
{
	public :
	Point A;/**< Upper End Point of Segment*/
	Point B;/**< Lower End Point of Segment*/
	int idx;/**< index of Segment*/
	// long double EPS  = 0.00000000000002;
	static long double sweepLine;/**< sweepLine which is shared among all objects and represents sweep line status*/
	
	/**default constructor makes a segment with idx = -1
	*/
	Segment()
	{
		
		idx = -1;
	}
	/** constructor to make an object which has same values as the parameter
	*@param _S segment whose fields need to be copied
	*/
	Segment(const Segment &_S)
	{
		A = _S.A, B = _S.B, idx =  _S.idx;
	}
	/** constructor to make a Segment out of the parameters passed
	*@param _A point A
	*@param _B point B
	*@param _idx index
	*/
	Segment(Point _A, Point _B, int _idx)
	{
		A = _A, B = _B, idx = _idx;
	}
	/** computes and returns the intersection of segment with sweepLine
	*@return the intersection of segment with sweep line
	*/
	long double intersection_with_sweep_line()
	{
		//vertical segment
		if(abs(A.x - B.x) < EPS)
			return A.x;
		
		//horizontal segment means overlapping segment
		if(abs(A.y - B.y) < EPS)
			return A.x;

		long double slope = (A.y - B.y) / (A.x - B.x);
		return (sweepLine -  (A.y - slope * A.x)) / slope;

	}
	/** compares if the segment is less than the parameter segment based on intersection of them with sweepLine
	*@param S the segment to be compared with
	*@return 1 if this segment is less than parameter segment otherwise 0
	*/
	bool operator < ( Segment &S) 
	{
		long double x1 = intersection_with_sweep_line();
		long double x2 = S.intersection_with_sweep_line();
		// x1  = round(x1 * 10000000)/(long double)10000000;
		// x2  = round(x2 * 10000000)/(long double)10000000;
		return x1 < x2 + EPS;
		// if(abs(x1-x2) > EPS)
		// 	return x1 < x2 ;
		// else
		// 	return idx < S.idx;
	}
	/** compares if the segment is greater than the parameter segment based on intersection of them with sweepLine
	*@param S the segment to be compared with
	*@return 1 if this segment is greater than the parameter segment
	*/
	bool operator > ( Segment &S) 
	{
		long double x1 = intersection_with_sweep_line();
		long double x2 = S.intersection_with_sweep_line();
		// x1  = round(x1 * 10000000)/(long double)10000000;
		// x2  = round(x2 * 10000000)/(long double)10000000;
		return x1 + EPS > x2; 
		
		// if(abs(x1-x2) > EPS)
		// 	return x1 > x2 ;
		// else
		// 	return idx > S.idx;
		
	}
	/** compares the segments for equality
	*@param S the segment to be compared with
	*@return 1 if they are equal otherwise 0
	*/
	bool operator == (Segment &S)
	{
		if(A == S.A  and B == S.B)
			return true;
		else
			return false;
	}
	/** compares the segments for inequality
	*@param S the segment to be compared with
	*@return 1 if they are different otherwise 0
	*/
	bool operator != (Segment &S)
	{
		if(A == S.A  and B == S.B)
			return false;
		else
			return true;
	}
	/** computes intersection of this segment with the parameter segment
	*@param S the segment with which intersection has to be found
	*@return a pair of <bool,Point> where the first will indicate whether the intersection exists or not and second field will contain the Point in case of existenc
	*/
	pair<bool,Point> intersection_with_segment(Segment &S)
	{
		long double a,_a,b,_b,c,_c;
		if(abs(A.x - B.x) < EPS)
		{
			a = 1;
			b = 0;
			c = -A.x;
		}
		else
		{
			a = (A.y - B.y) / (B.x - A.x);
			b = 1.0;
			c = -(a * A.x + A.y);
		}

		if(abs(S.A.x - S.B.x) < EPS)
		{
			_a = 1;
			_b = 0;
			_c = -S.A.x;
		} 
		else
		{
			_a = (S.A.y - S.B.y) / (S.B.x - S.A.x);
			_b = 1.0;
			_c = -(_a * S.A.x + S.A.y);
		}
		if(abs(a-_a) < EPS and abs(b-_b) < EPS)
		{
			//parallel at end points
			return {0,Point()};
		}
		else
		{
			long double x = (b * _c - c * _b) / (a * _b - b * _a);
			long double y;
			if(abs(b) < EPS)
				y = -_a * x - _c;
			else
				y = -a * x - c;
			
			Point intersect = Point(x,y);
			if(intersect.between(A,B) and intersect.between(S.A,S.B))
				return {1,intersect};
			else
				return {0,Point()};
		}

	}

};

class Event
{
public:
	Point P;/**< Event Point*/
	int idx;/**< index of Event*/
	int type;/**< type of Event*/
	/**default constructor makes a event with idx = 0 and type = - 1
	*/
	Event() 
	{
		
		idx = 0, type = -1;
	}
	/** constructor to make an Event out of the parameters passed
	*@param _P event point
	*@param  _idx index of the event
	*@param _type type of event
	*/
	Event(Point _P, int _idx, int _type)
	{
		P = _P, idx = _idx, type = _type;
	}
	//use eps
	/** compares if the event is less than the parameter event based primarily on y co-oordinate of Point P
	*@param E the event to be compared with
	*@return 1 if this event is less than parameter event otherwise 0
	*/
	bool operator < (const Event& E) const	
	{
		if(P.y != E.P.y)
			return P.y > E.P.y;
		else
			if(P.x != E.P.x)
				return P.x < E.P.x;
			else
				if(type != E.type)
					return type < E.type;
				else
					return idx < E.idx;
	}
	/** compares if the event is greater than the parameter event based primarily on y co-oordinate of Point P
	*@param E the event to be compared with
	*@return 1 if this event is greater than parameter event
	*/
	bool operator > (const Event& E) const	
	{
		if(P.y != E.P.y)
			return P.y < E.P.y;
		else
			if(P.x != E.P.x)
				return P.x > E.P.x;
			else
				if(type != E.type)
					return type > E.type;
				else
					return idx > E.idx;
	}
	/** compares the segments for inequality
	*@param E the event to be compared with
	*@return 1 if they are different otherwise 0
	*/
	bool operator != (Event &E) 
	{
		if(P == E.P and type == E.type and idx == E.idx)
			return false;
		else
			return true;
	}
	/** compares the segments for equality
	*@param E the event to be compared with
	*@return 1 if they are equal otherwise 0
	*/
	bool operator == (Event &E) 
	{
		if(P == E.P and type == E.type and idx == E.idx)
			return true;
		else
			return false;
	}
};
