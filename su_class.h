#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

class point
{
public:
	int x;
	int y;
	point()
	{
		x=0;
		y=0;
	}
	point(int x,int y)
	{
		this->x=x;
		this->y=y;
	}
	void print()
	{
		cout<<"("<<x<<","<<y<<")";
	}
};

class line
{
public:
	point start;
	point end;
	double m;
	double c;
	line()
	{
		m=0;
		c=0;
	}
	line(point &start,point &end)
	{
		this->start=start;
		this->end=end;
		convert();
	}
	void convert()
	{
		m=(double)(start.y-end.y)/(start.x-end.x);
		c=(start.x*end.y)-(end.x*start.y);
	}
	void print()
	{
		cout<<"y="<<m<<"x+"<<c;
	}
};

class fittedline
{
public:
	line bl;
	fittedline(point *p,int n)
	{
		int sx=0,sy=0,sxx=0,sxy=0;
		for(int i=0;i<n;i++)
		{
			sx+=p[i].x;
			sy+=p[i].y;
			sxx+=p[i].x*p[i].x;
			sxy+=p[i].x*p[i].y;
		}
		bl.m=((double)((sx*sy)-(n*sxy))/((sx*sx)-(sxx*n)));
		bl.c=((double)sy-sx*bl.m)/n;
	}
	line getline()
	{
		return bl;
	}
};

class component
{
public:
	PGMData *data;
	point comp[20][500];
	int no_of_pt[500];
	int no_of_comp;
	
	component(PGMData *d)
	{
		data=d;
		no_of_comp=0;
		for(int i=0;i<500;i++)
			no_of_pt[i]=0;
	}

	int neighbour(point a,point b)
	{
		if(abs(a.x-b.x)<=1 && abs(a.y-b.y)<=1)
			return 1;
		return 0;
	}

	void add(point p)
	{
		int i,j,flag=-1,m=0;
		for(i=0;i<no_of_comp;i++)
		{
			for(j=0;j<no_of_pt[i];j++)
			{
				if(neighbour(comp[i][j],p))
				{
					if(flag==-1)
					{
						flag=i;
						comp[i][no_of_pt[i]]=p;
						no_of_pt[i]++;
					}
					else
					{
						m=1;
						merge_comp(flag,i);
					}
					break;
				}
			}
		}
		if(m==1)
			optimize();
		if(flag==-1)
		{
			comp[no_of_comp][no_of_pt[no_of_comp]]=p;
			no_of_pt[no_of_comp]++;
			no_of_comp++;
		}
	}

	void analize()
	{
		point temp;
		for(int i=0;i<data->col;i++)
		{
			for(int j=0;j<data->row;j++)
			{
				if(data->matrix[j][i]==0)
				{
					temp.x=i;
					temp.y=j;
					add(temp);
				}
			}
		}
	}

	void merge_comp(int src,int dst)
	{
		for(int i=0;i<no_of_pt[dst];i++)
			comp[src][no_of_pt[src]++]=comp[dst][i];
		no_of_pt[dst]=0;
	}

	void optimize()
	{
		int blank=0;
		for(int i=0;i<no_of_comp;i++)
		{
			if(no_of_pt[i]==0)
			{
				blank++;
				for(int j=i;j<no_of_comp;j++)
					copy(j,j+1);
			}
		}
		no_of_comp-=blank;
	}

	void copy(int src,int dst)
	{
		int i;
		no_of_pt[src]=no_of_pt[dst];
		for(i=0;i<no_of_pt[dst];i++)
			comp[src][i]=comp[dst][i];
		no_of_pt[dst]=0;
	}
};

class dtw
{
	double f[50][50];
	int xlength,ylength;
	double sum;
public:
	void input(double *x1,double *y1,int *z1,int xl,double *x2,double *y2,int *z2,int yl)
	{
		xlength=xl+1;
		ylength=yl+1;
		for(int i=0;i<xlength;i++)
			for(int j=0;j<ylength;j++)
				f[i][j]=-1;
		for(int i=1;i<xlength;i++)
			f[i][0]=999;
		for(int i=1;i<ylength;i++)
			f[0][i]=999;
		f[0][0]=0;
		sum=computForward(x1,y1,z1,x2,y2,z2);
	}
	double computForward(double *x1,double *y1,int *z1,double *x2,double *y2,int *z2)
	{
		for(int i=1;i<xlength;i++)
		{
			for(int j=1;j<ylength;j++)
			{
				if(f[i-1][j]<=f[i-1][j-1] && f[i-1][j]<=f[i][j-1])
					f[i][j]=(fabs(x1[i-1]-x2[j-1]))+f[i-1][j];
				else if(f[i][j-1]<=f[i-1][j-1] && f[i][j-1]<=f[i-1][j])
					f[i][j]=(fabs(x1[i-1]-x2[j-1]))+f[i][j-1];
				else if(f[i-1][j-1]<=f[i][j-1] && f[i-1][j-1]<=f[i-1][j])
					f[i][j]=(fabs(x1[i-1]-x2[j-1]))+f[i-1][j-1];
			}
		}
		return f[xlength-1][ylength-1];
	}
	double get_index()
	{
		return sum;
	}
	void show_mat()
	{
		for(int i=0;i<xlength;i++)
		{
			for(int j=0;j<ylength;j++)
				cout<<setw(4)<<f[i][j];
			cout<<endl;
		}
	}
};

class dtw_adv
{
	int f[2][600];
	int xlength,ylength;
	int sum;
public:
	void input(int *x1,int xl,int *x2,int yl)
	{
		xlength=xl+1;
		ylength=yl;
		f[0][0]=0;
		for(int i=1;i<xlength;i++)
			f[0][i]=999;
		sum=computForward(x1,x2);
	}
	int computForward(int *x1,int *x2)
	{
		for(int j=0;j<ylength;j++)
		{
			f[1][0]=999;
			for(int i=1;i<xlength;i++)
			{
				if(f[1][i-1]<=f[0][i] && f[1][i-1]<=f[0][i-1])
					f[1][i]=(fabs((double)x1[i-1]-x2[j]))+f[1][i-1];

				else if(f[0][i]<=f[0][i-1] && f[0][i]<=f[1][i-1])
					f[1][i]=(fabs((double)x1[i-1]-x2[j]))+f[0][i];

				else if(f[0][i-1]<=f[0][i] && f[0][i-1]<=f[1][i-1])
					f[1][i]=(fabs((double)x1[i-1]-x2[j]))+f[0][i-1];
			}
			for(int m=0;m<xlength;m++)
			{
				//cout<<f[1][m]<<"\t";
				f[0][m]=f[1][m];
			}
			//cout<<endl;
		}
		return f[1][xlength-1];
	}
	int get_index()
	{
		return sum;
	}
	void show_mat()
	{
		for(int i=0;i<xlength;i++)
		{
			for(int j=0;j<ylength;j++)
				cout<<setw(4)<<f[i][j];
			cout<<endl;
		}
	}
};