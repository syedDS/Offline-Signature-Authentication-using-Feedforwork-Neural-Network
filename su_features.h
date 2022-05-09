int features[500][10];
int ex_feature[500][10];
point maxpoints[500][1000];
int maxpoint_count[500];
point minpoints[500][1000];
int minpoint_count[500];
line strt_line;

void global_features(PGMData *d,int no_of_sign)
{
	int i,j;
	int ne_point,EP=0,CP=0,CL,EL=0;
	int black=0;
	int stat=0,count=0;

	for(i=1;i<d->row;i++)
	{
		for(j=1;j<d->col;j++)
		{
			ne_point=0;
			if(d->matrix[i][j]==0)
			{
				black++;
				if(d->matrix[i-1][j-1]==0)
					ne_point++;
				if(d->matrix[i-1][j]==0)
					ne_point++;
				if(d->matrix[i-1][j+1]==0)
					ne_point++;
				if(d->matrix[i][j-1]==0)
					ne_point++;
				if(d->matrix[i][j+1]==0)
					ne_point++;
				if(d->matrix[i+1][j-1]==0)
					ne_point++;
				if(d->matrix[i+1][j]==0)
					ne_point++;
				if(d->matrix[i+1][j+1]==0)
					ne_point++;

				if(ne_point==1)
				{
					EP++;
					d->matrix[i][j]=0;
				}
				if(ne_point>=3)
				{
					CP++;
					EL=EL+ne_point-2;
				}
			}
		}
	}
	CL=1+((EL-EP)/2);

	for(i=0;i<d->col;i++)
		for(j=0;j<d->row;j++)
			if(d->matrix[j][i]==0)
			{
				count++;
				break;
			}
	int vertical=0,horizontal=0,sumin;
	int maxvertical=0,maxhorizontal=0;
	for(i=0;i<d->row;i++)
	{
		sumin=0;
		for(j=0;j<d->col;j++)
		{
			sumin+=!d->matrix[i][j];
		}
		if(i==0)
			maxvertical=sumin;
		else if(maxvertical<sumin)
				maxvertical=sumin;
		vertical+=i*sumin;
	}
	vertical/=black;

	for(i=0;i<d->col;i++)
	{
		sumin=0;
		for(j=0;j<d->row;j++)
		{
			sumin+=!d->matrix[j][i];
		}
		if(i==0)
			maxhorizontal=sumin;
		else if(maxhorizontal<sumin)
				maxhorizontal=sumin;
		horizontal+=i*sumin;
	}
	horizontal/=black;

	//printf("\nEdge Points : %d",EP);
	features[no_of_sign][4]=EP;
	//printf("\nCross Points : %d",CP);
	features[no_of_sign][3]=CP;
	//printf("\nClosed Loops : %d",CL);
	features[no_of_sign][6]=CL;
	//printf("\nSignature Height : %d",d->row);
	features[no_of_sign][5]=d->row;
	//printf("\nImage area : %d",black);
	features[no_of_sign][0]=black;
	//printf("\nPure width : %d",count);
	features[no_of_sign][2]=count;
	//printf("\nVertical center : %d",vertical);
	features[no_of_sign][7]=vertical;
	//printf("\nHorizontal center : %d",horizontal);
	features[no_of_sign][1]=horizontal;
	//printf("\nMax vertical projection : %d",maxvertical);
	features[no_of_sign][8]=maxvertical;
	//printf("\nMax horizontal projection : %d",maxhorizontal);
	features[no_of_sign][9]=maxhorizontal;

	//	//printf("\nEdge Points : %d",EP);
	//features[no_of_sign][4]=EP;
	////printf("\nCross Points : %d",CP);
	//features[no_of_sign][5]=CP;
	////printf("\nClosed Loops : %d",CL);
	//features[no_of_sign][6]=CL;
	////printf("\nSignature Height : %d",d->row);
	//features[no_of_sign][3]=d->row;
	////printf("\nImage area : %d",black);
	//features[no_of_sign][0]=black;
	////printf("\nPure width : %d",count);
	//features[no_of_sign][8]=count;
	////printf("\nVertical center : %d",vertical);
	//features[no_of_sign][2]=vertical;
	////printf("\nHorizontal center : %d",horizontal);
	//features[no_of_sign][7]=horizontal;
	////printf("\nMax vertical projection : %d",maxvertical);
	//features[no_of_sign][1]=maxvertical;
	////printf("\nMax horizontal projection : %d",maxhorizontal);
	//features[no_of_sign][9]=maxhorizontal;

	no_of_sign++;
}

void mean_asc_desc(PGMData *d,int no_of_sign)
{
	//calcuating mean line's y value
	int start_y,end_y,mean_y=0;
	int valid_col=0;
	for(int i=0;i<d->col;i++)
	{
		int flag=0;
		for(int j=0;j<d->row;j++)
		{
			if(d->matrix[j][i]==0)
			{
				if(!flag)
				{
					start_y=j;
					flag=1;
				}
				end_y=j;
			}
		}
		if(flag)
		{
			valid_col++;
			mean_y+=(start_y+end_y)/2;
		}
	}
	mean_y/=valid_col;

	//for(int i=0;i<d->col;i++)
	//	d->matrix[mean_y][i]=150;

	//calculating signature height (actual)
	int lower=d->row,upper=0,flag=0,height;

	for(int i=0;i<d->row;i++)
	{
		for(int j=0;j<d->col;j++)
			if(d->matrix[i][j]==0)
			{
				flag=1;
				break;
			}
		if(flag)
		{
			upper=i;
			break;
		}
	}
	flag=0;
	for(int i=d->row-1;i>=0;i--)
	{
		for(int j=0;j<d->col;j++)
			if(d->matrix[i][j]==0)
			{
				flag=1;
				break;
			}
		if(flag)
		{
			lower=i;
			break;
		}
	}
	height=lower-upper;

	//defining two quarter lines
	int higher_quarter=height/4;
	int lower_quarter=(height*3)/4;

	//for upper quarter :: calculating mean ascender height
	int mean_asc_height;
	point local_max[1000];
	point temp;
	int count=0;
	flag=0;	// 0 = replacemnet allowed ; 1 = decreasing mode (replacement not allowed)

	int x,y,f;
	for(x=0;x<d->col;x++)
	{
		temp.x=x;
		temp.y=higher_quarter;
		for(y=0;y<=higher_quarter;y++)
		{
			if(d->matrix[y][x]==0)
			{
				temp.x=x;
				temp.y=y;
				break;
			}
		}
		if(flag==0)
		{
			local_max[count]=temp;
			count++;
			flag=1;
			continue;
		}

		if(temp.y<local_max[count-1].y && flag==1)
		{
			local_max[count-1]=temp;
			flag=2;
			continue;
		}
		if(temp.y<local_max[count-1].y && flag==2)
		{
			local_max[count-1]=temp;
			flag=2;
			continue;
		}
		if(temp.y==local_max[count-1].y)
		{
			local_max[count-1]=temp;
			continue;
		}
		if(temp.y>local_max[count-1].y && flag==2)
		{
			local_max[count]=temp;
			count++;
			flag=3;
			continue;
		}
		if(temp.y>local_max[count-1].y && flag==3)
		{
			local_max[count-1]=temp;
			flag=3;
			continue;
		}
		if(temp.y<local_max[count-1].y && flag==3)
		{
			local_max[count-1]=temp;
			flag=2;
			continue;
		}
	}


	mean_asc_height=0;
	for(int i=0;i<count;i++)
		mean_asc_height=mean_y-local_max[i].y;
	mean_asc_height/=count;

	//Mean ascender height
	ex_feature[no_of_sign][0]=mean_asc_height;

	//for(int i=0;i<d->col;i++)
	//	d->matrix[higher_quarter][i]=150;

	//for(int i=0;i<count;i++)
	//	d->matrix[local_max[i].y][local_max[i].x]=150;

	//for lower quarter :: calculating mean descender height
	int mean_desc_height;
	point local_min[1000];
	//point temp;
	count=0;
	flag=0;

	for(x=0;x<d->col;x++)
	{
		temp.x=x;
		temp.y=lower_quarter;
		for(y=d->row-1;y>=lower_quarter;y--)
		{
			if(d->matrix[y][x]==0)
			{
				temp.x=x;
				temp.y=y;
				break;
			}
		}
		if(flag==0)
		{
			local_min[count]=temp;
			count++;
			flag=1;
			continue;
		}

		if(temp.y>local_min[count-1].y && flag==1)
		{
			local_min[count-1]=temp;
			flag=2;
			continue;
		}
		if(temp.y>local_min[count-1].y && flag==2)
		{
			local_min[count-1]=temp;
			flag=2;
			continue;
		}
		if(temp.y==local_min[count-1].y)
		{
			local_min[count-1]=temp;
			continue;
		}
		if(temp.y<local_min[count-1].y && flag==2)
		{
			local_min[count]=temp;
			count++;
			flag=3;
			continue;
		}
		if(temp.y<local_min[count-1].y && flag==3)
		{
			local_min[count-1]=temp;
			flag=3;
			continue;
		}
		if(temp.y>local_min[count-1].y && flag==3)
		{
			local_min[count-1]=temp;
			flag=2;
			continue;
		}
	}


	mean_desc_height=0;
	for(int i=0;i<count;i++)
		mean_desc_height=mean_y-local_max[i].y;
	mean_desc_height/=count;

	//Mean descender height
	ex_feature[no_of_sign][1]=mean_desc_height;

	//for(int i=0;i<d->col;i++)
	//	d->matrix[lower_quarter][i]=150;

	//for(int i=0;i<count;i++)
	//	d->matrix[local_min[i].y][local_min[i].x]=150;
}

void sort(point *arr,int no_of_ele)
{
	point temp;
	for(int i=0;i<no_of_ele;i++)
		for(int j=i;j<no_of_ele-1;j++)
		{
			if(arr[j].x>arr[j+1].x)
			{
				temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
}

void area_extraction(PGMData *d,int no_of_sign)
{
	component comp(d);
	int gray=50;
	int topy,lowy;
	int current_x,last_x;
	int subarea;
	int area=0;
	comp.analize();
	for(int i=0;i<comp.no_of_comp;i++)
	{
		sort(comp.comp[i],comp.no_of_pt[i]);
		subarea=0;
		current_x=comp.comp[i][0].x;
		topy=comp.comp[i][0].y;
		lowy=comp.comp[i][0].y;
		last_x=current_x;
		for(int j=0;j<comp.no_of_pt[i];j++)
		{
			if(current_x==comp.comp[i][j].x)
			{
				if(topy>=comp.comp[i][j].y)
					topy=comp.comp[i][j].y;
				if(lowy<=comp.comp[i][j].y)
					lowy=comp.comp[i][j].y;
			}
			else
			{
				for(int m=topy+1;m<lowy;m++)
					d->matrix[m][last_x]=150;
				subarea+=(lowy-topy)+1;

				current_x=comp.comp[i][j].x;
				topy=comp.comp[i][j].y;
				lowy=comp.comp[i][j].y;
				last_x=current_x;
			}
		}
		for(int m=topy+1;m<lowy;m++)
			d->matrix[m][last_x]=150;
		subarea+=(lowy-topy)+1;
		area+=subarea;
	}
	ex_feature[no_of_sign][2]=area;
	ex_feature[no_of_sign][3]=comp.no_of_comp+1;
}

void avg_height_per_segment(PGMData *d,int no_of_sign)
{
	int section;
	int inc=d->col/5;
	for(int section=0;section<5;section++)
	{
		int start=section*inc;
		int end=start+inc;


		int sum=0,count=0,avg,flag,top,low;
		for(int j=start;j<end;j++)
		{
			flag=0;
			for(int m=0;m<d->row;m++)
			{
				if(d->matrix[m][j]==0 && flag==0)
				{
					flag=1;
					top=m;
					low=m;
				}
				if(d->matrix[m][j]==0 && flag==1)
				{
					low=m;
				}
			}
			if(flag!=0)
			{
				sum+=((low-top)+1);
				count++;
			}
		}
		if(count!=0)
			avg=sum/count;
		else
			avg=0;
		cout<<"part = "<<section<<" sum = "<<sum<<" count = "<<count<<" avg = "<<avg<<endl;
		ex_feature[no_of_sign][section]=avg;
	}
}

int decision(float m)
{
	//0=flat
	//1=up
	//-1=down
	float thresh=0.05;
	if(m<(-1*thresh))
		return 1;
	if(m>thresh)
		return -1;
	return 0;
}

void center_grvt_segment(PGMData *d,int no_of_sign)
{
	point p[5];
	int inc=d->col/5;
	for(int section=0;section<5;section++)
	{
		int start=section*inc;
		int end=start+inc;	
		int x=0,y=0,area=0;
		for(int j=start;j<end;j++)
		{
			for(int m=0;m<d->row;m++)
			{
				if(d->matrix[m][j]==0)
				{
					area++;
					x+=j;
					y+=m;
				}
			}
		}
		if(area!=0)
		{
			x/=area;
			y/=area;
		}
		else
		{
			x=(start+end)/2;
			y=d->row/2;
		}
		p[section].x=x;
		p[section].y=y;
	}

	int cat=5;
	for(int i=0;i<4;i++)
	{
		line l;
		l.start=p[i];
		l.end=p[i+1];
		l.convert();
		//cout<<(-1)*l.m<<",";
		d->matrix[l.start.y][l.start.x]=100;
		d->matrix[l.end.y][l.end.x]=100;
		switch(cat)
		{
		case 5:
			if(decision(l.m)==1)
				cat=3;
			else if(decision(l.m)==-1)
				cat=4;
			break;
		case 3:
			if(decision(l.m)==-1)
				cat=1;
			break;
		case 4:
			if(decision(l.m)==1)
				cat=2;
			break;
		case 1:
			if(decision(l.m)==1)
				cat=7;
			break;
		case 2:
			if(decision(l.m)==-1)
				cat=8;
			break;
		case 7:
			if(decision(l.m)==-1)
				cat=6;
			break;
		case 8:
			if(decision(l.m)==1)
				cat=6;
			break;
		}
	}
	cout<<cat<<",";
	if((no_of_sign+1)%5==0)
		cout<<endl;
}

void maxima_charateristics(PGMData *d,int no_of_sign)
{
	int flag=0;								// 0 = confirmed; 1 = update; 2 = falling
	int current_y;
	point max(-1,-1);
	point maxima[200];
	int count=0;
	for(int x=0;x<d->col;x++)
	{
		current_y=-1;
		for(int y=0;y<d->row;y++)
			if(d->matrix[y][x]==0)
			{
				current_y=y;
				break;
			}
		if(current_y==-1)
		{
			if(flag==1)
			{
				maxima[count++]=max;
				flag=0;
			}
			continue;
		}
		if(flag==0)
		{
			max.y=current_y;
			max.x=x;
			flag=1;
		}
		else if(flag==1)
		{
			if(max.y>=current_y)
			{
				max.y=current_y;
				max.x=x;
			}
			else
			{
				maxima[count++]=max;
				max.y=current_y;
				max.x=x;
				flag=2;
			}
		}
		else if(flag==2)
		{
			if(max.y>current_y)
				flag=1;
			max.y=current_y;
			max.x=x;
		}
	}
	for(int i=0;i<count;i++)
	{
		maxpoints[no_of_sign][i]=maxima[i];
	}
	maxpoint_count[no_of_sign]=count;
	
	/*int inc=d->col/5;
	inc++;
	for(int section=0;section<5;section++)
	{
		int cat[3]={0,0,0};
		int start=section*inc;
		int end=start+inc;
		for(int i=0;i<count;i++)
		{
			if(maxima[i].x<=start || maxima[i].x>end)
				continue;
			d->matrix[maxima[i].y][maxima[i].x]=100;
			int plateau=0;
			for(int j=maxima[i].x;j>=0 && d->matrix[maxima[i].y][j]!=255;j--)
				plateau++;
			switch(plateau)
			{
				case 1:	cat[0]++;break;
				case 2:	cat[0]++;break;
				case 3:	cat[1]++;break;
				case 4:	cat[1]++;break;
				default:cat[2]++;
			}
		}
		cout<<cat[0]<<","<<cat[1]<<","<<cat[2]<<",";
	}
	cout<<endl;*/
}

void minima_charateristics(PGMData *d,int no_of_sign)
{
	int flag=0;								// 0 = confirmed; 1 = update; 2 = falling
	int current_y;
	point min(-1,-1);
	point minima[200];
	int count=0;
	for(int x=0;x<d->col;x++)
	{
		current_y=-1;
		for(int y=d->row-1;y>=0;y--)
			if(d->matrix[y][x]==0)
			{
				current_y=y;
				break;
			}
		if(current_y==-1)
		{
			if(flag==1)
			{
				minima[count++]=min;
				flag=0;
			}
			continue;
		}
		if(flag==0)
		{
			min.y=current_y;
			min.x=x;
			flag=1;
		}
		else if(flag==1)
		{
			if(min.y<=current_y)
			{
				min.y=current_y;
				min.x=x;
			}
			else
			{
				minima[count++]=min;
				min.y=current_y;
				min.x=x;
				flag=2;
			}
		}
		else if(flag==2)
		{
			if(min.y<current_y)
				flag=1;
			min.y=current_y;
			min.x=x;
		}
	}
	for(int i=0;i<count;i++)
	{
		minpoints[no_of_sign][i]=minima[i];
	}
	minpoint_count[no_of_sign]=count;
	
	/*int inc=d->col/5;
	inc++;
	for(int section=0;section<5;section++)
	{
		int cat[3]={0,0,0};
		int start=section*inc;
		int end=start+inc;
		for(int i=0;i<count;i++)
		{
			if(maxima[i].x<=start || maxima[i].x>end)
				continue;
			d->matrix[maxima[i].y][maxima[i].x]=100;
			int plateau=0;
			for(int j=maxima[i].x;j>=0 && d->matrix[maxima[i].y][j]!=255;j--)
				plateau++;
			switch(plateau)
			{
				case 1:	cat[0]++;break;
				case 2:	cat[0]++;break;
				case 3:	cat[1]++;break;
				case 4:	cat[1]++;break;
				default:cat[2]++;
			}
		}
		cout<<cat[0]<<","<<cat[1]<<","<<cat[2]<<",";
	}
	cout<<endl;*/
}

void dtw_analysis_maxima(PGMData *d,int no_of_sign)
{
	static double x1[400];
	static double y1[400];
	static int z1[400];
	static int x1c;
	double x2[400];
	double y2[400];
	int z2[400];
	int x2c;
	dtw dt;
	int flag=0;								// 0 = confirmed; 1 = update; 2 = falling
	int current_y;
	point max(-1,-1);
	int m,n,f;
	if(no_of_sign==0)
	{
		x1c=0;
		for(int x=0;x<d->col;x++)
		{
			current_y=-1;
			for(int y=0;y<d->row;y++)
				if(d->matrix[y][x]==0)
				{
					current_y=y;
					break;
				}
			if(current_y==-1)
			{
				if(flag==1)
				{
					int plateau=0;
					/*for(int j=max.x;j>=0 && d->matrix[max.y][j]!=255;j--)
						plateau++;*/
					z1[x1c]=plateau;
					x1[x1c]=((double)max.x/d->col)*100;
					y1[x1c++]=((double)max.y/d->row)*100;
					flag=0;
				}
				continue;
			}
			if(flag==0)
			{
				max.y=current_y;
				max.x=x;
				flag=1;
			}
			else if(flag==1)
			{
				if(max.y>=current_y)
				{
					max.y=current_y;
					max.x=x;
				}
				else
				{
					int plateau=0;
					/*for(int j=max.x;j>=0 && d->matrix[max.y][j]!=255;j--)
						plateau++;*/
					z1[x1c]=plateau;
					x1[x1c]=((double)max.x/d->col)*100;
					y1[x1c++]=((double)max.y/d->row)*100;
					max.y=current_y;
					max.x=x;
					flag=2;
				}
			}
			else if(flag==2)
			{
				if(max.y>current_y)
					flag=1;
				max.y=current_y;
				max.x=x;
			}
		}
	}
	flag=0;
	x2c=0;
	for(int x=0;x<d->col;x++)
	{
		current_y=-1;
		for(int y=0;y<d->row;y++)
			if(d->matrix[y][x]==0)
			{
				current_y=y;
				break;
			}
		if(current_y==-1)
		{
			if(flag==1)
			{
				int plateau=0;
				/*for(int j=max.x;j>=0 && d->matrix[max.y][j]!=255;j--)
					plateau++;*/
				z2[x2c]=plateau;
				x2[x2c]=((double)max.x/d->col)*100;
				y2[x2c++]=((double)max.y/d->row)*100;
				flag=0;
			}
			continue;
		}
		if(flag==0)
		{
			max.y=current_y;
			max.x=x;
			flag=1;
		}
		else if(flag==1)
		{
			if(max.y>=current_y)
			{
				max.y=current_y;
				max.x=x;
			}
			else
			{
				int plateau=0;
				/*for(int j=max.x;j>=0 && d->matrix[max.y][j]!=255;j--)
					plateau++;*/
				z2[x2c]=plateau;
				x2[x2c]=((double)max.x/d->col)*100;
				y2[x2c++]=((double)max.y/d->row)*100;
				max.y=current_y;
				max.x=x;
				flag=2;
			}
		}
		else if(flag==2)
		{
			if(max.y>current_y)
				flag=1;
			max.y=current_y;
			max.x=x;
		}
	}
	dt.input(x1,y1,z1,x1c,x2,y2,z2,x2c);
	cout<<dt.get_index()<<endl;
}

void dtw_analysis_minima(PGMData *d,int no_of_sign)
{
	static double x1[400];
	static int x1c;
	static double x2[400];
	static int x2c;
	dtw dt;
	int flag=0;								// 0 = confirmed; 1 = update; 2 = falling
	int current_y;
	point min(-1,-1);
	int m,n,f;
	if(no_of_sign==0)
	{
		x1c=0;
		for(int x=0;x<d->col;x++)
		{
			current_y=-1;
			for(int y=d->row-1;y>=0;y--)
				if(d->matrix[y][x]==0)
				{
					current_y=y;
					break;
				}
			if(current_y==-1)
			{
				if(flag==1)
				{
					x1[x1c++]=((double)min.y/d->row)*100;
					flag=0;
				}
				continue;
			}
			if(flag==0)
			{
				min.y=current_y;
				min.x=x;
				flag=1;
			}
			else if(flag==1)
			{
				if(min.y<=current_y)
				{
					min.y=current_y;
					min.x=x;
				}
				else
				{
					x1[x1c++]=((double)min.y/d->row)*100;
					min.y=current_y;
					min.x=x;
					flag=2;
				}
			}
			else if(flag==2)
			{
				if(min.y<current_y)
					flag=1;
				min.y=current_y;
				min.x=x;
			}
		}
	}

	x2c=0;
	for(int x=0;x<d->col;x++)
	{
		current_y=-1;
		for(int y=d->row-1;y>=0;y--)
			if(d->matrix[y][x]==0)
			{
				current_y=y;
				break;
			}
		if(current_y==-1)
		{
			if(flag==1)
			{
				x2[x2c++]=((double)min.y/d->row)*100;
				flag=0;
			}
			continue;
		}
		if(flag==0)
		{
			min.y=current_y;
			min.x=x;
			flag=1;
		}
		else if(flag==1)
		{
			if(min.y<=current_y)
			{
				min.y=current_y;
				min.x=x;
			}
			else
			{
				x2[x2c++]=((double)min.y/d->row)*100;
				min.y=current_y;
				min.x=x;
				flag=2;
			}
		}
		else if(flag==2)
		{
			if(min.y<current_y)
				flag=1;
			min.y=current_y;
			min.x=x;
		}
	}
	//dt.input(x1,x1c,x2,x2c);
	cout<<dt.get_index()<<endl;
}

void upper_contur_extration(PGMData *d,int no_of_sign)
{
	int flag;
	int found_upper,found_lower;
	point p;
	maxpoint_count[no_of_sign]=0;
	minpoint_count[no_of_sign]=0;
	for(int i=0;i<d->col;i++)
	{
		found_upper=0;
		found_lower=0;
		int j,k;
		for(j=0;j<d->row;j++)
		{
			if(d->matrix[j][i]==0)
			{
				found_upper=1;
				break;
			}
		}
		for(k=d->row-1;k>=0;k--)
		{
			if(d->matrix[k][i]==0)
			{
				found_lower=1;
				break;
			}
		}
		if(found_upper && found_lower)
		{
			if((k-j)<=5)
			{
				if(d->row-k<j)
				{
					p.x=i;
					p.y=k;
					minpoints[no_of_sign][minpoint_count[no_of_sign]++]=p;
				}
				else
				{
					p.x=i;
					p.y=j;
					maxpoints[no_of_sign][maxpoint_count[no_of_sign]++]=p;
				}
			}
			else
			{
				p.x=i;
				p.y=j;
				maxpoints[no_of_sign][maxpoint_count[no_of_sign]++]=p;
				p.y=k;
				minpoints[no_of_sign][minpoint_count[no_of_sign]++]=p;
			}
		}
	}
}