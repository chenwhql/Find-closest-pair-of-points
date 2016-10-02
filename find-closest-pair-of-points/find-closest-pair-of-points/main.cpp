#include<cstdio>
#include<ctime>
#include<cstdlib>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

#define LOCAL

/****************Point******************/
//point struct
struct Point
{
	int x, y;

	Point(int x = 0, int y = 0) :x(x), y(y){}

	Point& operator = (const Point& p)
	{
		this->x = p.x;
		this->y = p.y;
		return *this;
	}

	static bool cmp_by_x(const Point& p1, const Point& p2)
	{
		return p1.x < p2.x;
	}
	static bool cmp_by_y(const Point& p1, const Point& p2)
	{
		return p1.y < p2.y;
	}
};

ostream& operator << (ostream& out, const Point& p)
{
	out << "(" << p.x << "," << p.y << ")";
}

//计算两点间的距离
double calc_distance(const Point& p1, const Point& p2)
{
	int x = abs(p2.x - p1.x);
	int y = abs(p2.y - p1.y);
	return sqrt((double)(x*x + y*y));
}
/******************************************/

/************Point pair*****************/
struct PointPair
{
	Point A;
	Point B;
	
	PointPair(Point A, Point B) :A(A), B(B){};

};
/**************************************/

vector<Point> point_set;  //存放输入的点集
vector<PointPair> closest_pair_of_points;  //存放寻找的结果
double min_distance = 100000;  //存放最短距离

/*************select 第i小的元素*****************/
//算法导论9.3节 SELECT算法 找出能够将点集等分的x轴
//1. 获取中位数  注意这是数值上的中位数，用作划分，而不是划分点集的中间轴
int get_median(vector<int>& A, int p, int r)
{
	vector<int> median_set; //存放各组的中位数
	int n = r - p + 1;
	int last_group_member_num = n % 5;
	int five_member_group_num = n / 5; //n个元素划分为（n/5）向上取整个组

	for (int i = p; i < n / 5; i++)
	{
		sort(A[p + i * 5], A[p + i * 5 + 4]);  //排序
		median_set.push_back(A[p + i * 5 + 2]);
	}
	if (last_group_member_num != 0)
	{
		sort(A[n - last_group_member_num + 1], A[r]);
		median_set.push_back(A[n - last_group_member_num + (last_group_member_num/2)]);
	}

	//递归结束条件
	if (median_set.size() == 1)
	{
		return median_set[0];
	}
	else
	{
		return get_median(median_set, 0, median_set.size()-1);
	}

	return 0; //不会执行到
}

//2. 用中位数对数组进行划分
int partition(vector<int>& A, int p, int r, int key)
{
	//找出x坐标中位数对应的点的位置,并与最后一个点交换
	for (int i = p; i <= r; ++i)
	{
		if (A[i] == key)
		{
			swap(A[i], A[r]);
		}
	}
	//记录主键
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (A[j] <= key)
		{
			i++;
			swap(A[j], A[i]);
		}
	}
	swap(A[i + 1], A[r]);

	return i + 1;
}

//3. 选择第i小的元素
int select(vector<int>& A, int p, int r, int i)
{
	//获取数集的中位数
	int median = get_median(A, p, r);
	int q = partition(A, p, r, median);
	int k = q - p + 1;
	if (i == k)
	{
		return A[i];
	}
	else if (i < k)
	{
		select(A, p, q-1, i);
	}
	else
	{
		select(A, q + 1, r, i - k);
	}
}
/*********************************************/

/***************点集划分函数********************/
int point_set_partition(vector<Point>& P, int p, int r, int m)
{
	//找出x坐标中位数对应的点的位置,并与最后一个点交换
	for (int i = p; i <= r; ++i)
	{
		if (P[i].x == m)
		{
			swap(P[i], P[r]);
		}
	}
	//记录主键
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (P[j].x <= m)
		{
			i++;
			swap(P[j], P[i]);
		}
	}
	swap(P[i + 1], P[r]);

	return i + 1;
}
/*********************************************/

//寻找最近点对函数
double find_closest_pair_of_points(vector<Point>& P, int p, int r)
{
	if (r - p + 1 == 2)  //还剩2个点
	{
		double dis = calc_distance(P[p], P[r]);
		if (dis < min_distance)
		{
			min_distance = dis;
			closest_pair_of_points.clear();
			closest_pair_of_points.push_back(PointPair(P[p], P[r]));
		}
		else if (dis == min_distance)
		{
			closest_pair_of_points.push_back(PointPair(P[p], P[r]));
		}
		return dis;
	}
	else if (r - p + 1 == 3) //还剩3个点
	{
		double dis1 = calc_distance(P[p], P[p + 1]);
		double dis2 = calc_distance(P[p + 1], P[r]);
		double dis3 = calc_distance(P[p], P[r]);
		double min_dis = min(min(dis1, dis2), dis3);  //注意着三个距离值可能相等
		if (min_dis < min_distance)
		{
			min_distance = min_dis;
			closest_pair_of_points.clear();
			if (dis1 == min_dis)
			{
				closest_pair_of_points.push_back(PointPair(P[p], P[p + 1]));
			}
			else if (dis2 == min_dis)
			{
				closest_pair_of_points.push_back(PointPair(P[p + 1], P[r]));
			}
			else
			{
				closest_pair_of_points.push_back(PointPair(P[p], P[r]));
			}
		}
		else if (min_dis == min_distance)
		{
			if (dis1 == min_dis)
			{
				closest_pair_of_points.push_back(PointPair(P[p], P[p + 1]));
			}
			else if (dis2 == min_dis)
			{
				closest_pair_of_points.push_back(PointPair(P[p + 1], P[r]));
			}
			else
			{
				closest_pair_of_points.push_back(PointPair(P[p], P[r]));
			}
		}
	}
	else  //还剩不止3个点
	{
		//提取点集的x坐标集
		vector<int> point_x;
		for (int i = 0; i < P.size(); ++i)
		{
			point_x.push_back(P[i].x);
		}
		//获取划分点集的轴
		int x_median = select(point_x, 0, point_x.size(), point_x.size()/2);
		int divide_x = point_set_partition(P, p, r, x_median);
		//递归求解两边的最短距离
		double min_dis_left = find_closest_pair_of_points(P, p, divide_x);
		double min_dis_right = find_closest_pair_of_points(P, divide_x + 1, r);
		double min_x = min(min_dis_left, min_dis_right);
		//这里是否再需要更新min_distance的值？
		//将以轴为中心左右min_x范围内的点提取出来
		vector<Point> point_set_middle;
		for (int i = p; i <= divide_x; ++i)
		{
			if (abs(P[i].x - divide_x) <= min_x)
			{
				point_set_middle.push_back(P[i]);
			}
		}
		//将这些点按Y轴大小排序
		sort(point_set_middle.begin(), point_set_middle.end(), Point::cmp_by_y);
		//每一个点与其后的7个点相比
		for (int i = 0; i < point_set_middle.size(); i++)
		{
			if ()
		}
	}
}

int main()
{
#ifdef LOCAL
	freopen("in.txt","r",stdin);
#endif


	//初始化随机数种子 
	srand(time(NULL));

	//


	return 0;
}