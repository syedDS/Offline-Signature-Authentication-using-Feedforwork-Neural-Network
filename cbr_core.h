#include <iomanip>
#define NOP 68
#define M 10
int no_of_bins[10];
char name[NOP][10]={"AD","AG","AKH","AM","AN","AnkiM","AP","Arnas","AS","ASult","AT","AUM","AvisG",
	"BB","BK","BS",
	"CD",
	"DM",
	"KC","KL","KM","KR","KS",
	"MD","MDey","MG","MGR","MR",
	"ND","NK",
	"PA","PB","PBha","PH","PK","PM","PrB",
	"RC","RG","RRM","RSB",
	"SA","SaiC","SB","SBasu","SBes","SBha","SD","SDas","SDha","SDM","SG","ShilaB","ShowB","SJana","SKD","SKG","SM","SmritiG","SP","SPaL","SS","SSeng","SubM","SVS",
	"TanD","TD","TsheD"};

char code[100][11];
int cc;
int no_of_sign;
int maximum[10];
int minimum[10];
int breakup[10];
int nor;
int candidates[50][10];
int cndt_cnt;

int bin_no[500][11];

char cndt[1200][M];
int noc=0;
int arr[10]={1,2,0,1,1,2,3,2,1,1};

void rgstr(char *arr)
{
	for(int i=0;i<M;i++)
		cndt[noc][i]=arr[i];
	noc++;
}

void cndt_show()
{
	for(int i=0;i<noc;i++)
	{
		for(int j=0;j<M;j++)
			cout<<setw(3)<<cndt[i][j];
		cout<<endl;
	}
	cout<<"Total candidate : "<<noc;
}

void cndt_gen(char *arr)
{
	//LEVEL 1	
	for(int i=0,ie=1;i<M;)
	{
		arr[i]+=ie;
		rgstr(arr);
		arr[i]-=ie;
		ie*=-1;
		if(ie==1) i++;
	}
	//LEVEL 2
	for(int i=0,ie=1;i<M-1;)
	{
		arr[i]+=ie;
		for(int j=i+1,je=1;j<M;)
		{
			arr[j]+=je;
			rgstr(arr);
			arr[j]-=je;
			je*=-1;
			if(je==1) j++;
		}
		arr[i]-=ie;
		ie*=-1;
		if(ie==1) i++;
	}
	//LEVEL 3
	for(int i=0,ie=1;i<M-2;)
	{
		arr[i]+=ie;
		for(int j=i+1,je=1;j<M-1;)
		{
			arr[j]+=je;
			for(int k=j+1,ke=1;k<M;)
			{
				arr[k]+=ke;
				rgstr(arr);
				arr[k]-=ke;
				ke*=-1;
				if(ke==1) k++;
			}
			arr[j]-=je;
			je*=-1;
			if(je==1) j++;
		}
		arr[i]-=ie;
		ie*=-1;
		if(ie==1) i++;
	}
}

struct indexrec
{
	int pos;
	int neg;
	int count;
	char code[5][11];
	char signcode[20];
	char signcode1[20];
	char signcode2[20];
};

long long probing(long long index,int a,int m)
{
		return (index+(1*a))%m;							// LINEAR PROBING
		//return (index+(1*a)+(2*a*a))%m;				// QUADRATIC PROBING
		//return ((index%m)+a*((index%(m-1))+1))%m;		// DOUBLE HASHING
}

void sort(int *arr,int sz)
{
	int temp;
	for(int i=0;i<sz;i++)
		for(int j=0;j<sz-i-1;j++)
		{
			if(arr[j]<arr[j+1])
			{
				temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
}

void binning()
{
	int i,p,j,k,c,min,max,maxdiffer,differ[NOP],cd,univmax,univmin;
	nor=1;
	for(k=0;k<10;k++)
	{
		maxdiffer=0;
		cd=0;
		univmin=features[0][k];
		univmax=features[0][k];
		
		for(i=0;i<no_of_sign;i+=5)
		{
			for(j=0;j<3;j++)
			{
				p=i+j;
				if(j==0)
				{
					if(univmin>features[p][k])
						univmin=features[p][k];
					if(univmax<features[p][k])
						univmax=features[p][k];
					if(i!=0)
						differ[cd++]=abs(max-min);
					min=features[p][k];
					max=features[p][k];
				}
				else if(j==1 || j==2)
				{
					if(univmin>features[p][k])
						univmin=features[p][k];
					if(univmax<features[p][k])
						univmax=features[p][k];
					if(min>features[p][k])
						min=features[p][k];
					if(max<features[p][k])
						max=features[p][k];
				}
			}
		}
		differ[cd++]=abs(max-min);

		sort(differ,cd);

		i=0;
		do
		{
		maxdiffer+=differ[i++];
		}while((univmax-univmin)/maxdiffer>9);

		cout<<"For feature : "<<k;
		cout<<" Maximum interperson range : " <<maxdiffer;
		cout<<" Bins required : "<<(univmax-univmin)/maxdiffer+1<<endl;
		nor=nor*((univmax-univmin)/maxdiffer+1);
		no_of_bins[k]=maxdiffer;
		minimum[k]=univmin;
	}
}

int twopower(int num)
{
	int ret=0;
	for(int i=0;num>ret;i++)
		ret=pow(2,(double)i);
	return ret;
}

long long bintodec(char *str)
{
	char temp[100];
	strcpy_s(temp,str);
	strrev(temp);
	int i=0;
	long long num=0;
	while(temp[i]!='\0')
	{
		num+=(temp[i]-48)*pow(2,(double)i);
		i++;	
	}
	return num;
}

long long hashval(long long index,int a,int b,int p,int m)
{
	return (((index*a+b)%p)%m);
}

int smallestprime(int num)
{
	int i,j,flag;
	for(i=num;;i++)
	{
		flag=0;
		for(j=2;j<(i/2);j++)
		{
			if(i%j==0)
			{
				flag=1;
				break;
			}
		}
		if(flag==0)
			break;
	}
	return i;
}

void showfile()
{
		cout<<endl<<"File structure"<<endl;
		FILE *frec=fopen("index.txt","r");
		int i=0;
		int c=0;
		indexrec ir;
		while(fread(&ir,sizeof(indexrec),1,frec)!=NULL)
		{
			printf("%d %d %d %d %s %s %s %s %s %s %s %s\n",i++,ir.pos,ir.neg,ir.count,ir.code[0],ir.code[1],ir.code[2],ir.code[3],ir.code[4],ir.signcode,ir.signcode1,ir.signcode2);
			if(ir.count!=0)
				c++;
		}
		cout<<"Total non-zero entries : "<<c<<endl;
		fclose(frec);
}

void coding_trn()
{
	cout<<"Coding for signatures (per signature; sequentially each "<<THRESH<<" belongs to one person) : "<<endl;
	cc=0;
	for(int i=0;i<no_of_sign;i+=5)
	{
		for(int k=0;k<THRESH;k++)
		{
			for(int j=0;j<10;j++)
			{
				code[cc][j]=(features[i+k][j]-minimum[j])/no_of_bins[j]+48;
				cout<<(char)((features[i+k][j]-minimum[j])/no_of_bins[j]+48)<<",";
			}
			cc++;
			cout<<endl;
		}
	}
}

void place_trn()
{
	int i,j,p,m;
	FILE *frec;
	indexrec ir,irtemp;

	m=twopower(no_of_sign/5);
	//m=m*2;
	cout<<endl<<"Total slot to be created : "<<m<<endl;
	p=smallestprime(m);

	// PREPARING BLANK RECORD
	ir.count=0;
	ir.pos=0;
	ir.neg=0;
	strcpy_s(ir.signcode,"N/A");
	strcpy_s(ir.signcode1,"N/A");
	strcpy_s(ir.signcode2,"N/A");
	strcpy_s(ir.code[0],"0000000000");
	strcpy_s(ir.code[1],"0000000000");
	strcpy_s(ir.code[2],"0000000000");
	strcpy_s(ir.code[3],"0000000000");
	strcpy_s(ir.code[4],"0000000000");

	// OPENING THE INDEX FILE
	frec=fopen("index.txt","wb");
	if(!frec)
	{
		printf("File I/O error!");
		return;
	}

	// PLACING m NUMBERS BLANK RECORD IN INDEX FILE
	for(i=0;i<m;i++)
			fwrite(&ir,sizeof(indexrec),1,frec);
	cout<<"Blank file prepared"<<endl;
	fclose(frec);

	long long pos,hashindex,hashindex2;
	int sum[10],c,flag,chesta;

	frec=fopen("index.txt","rb+");
	if(!frec)
	{
		printf("File I/O error!");	
		return;
	}
	for(j=0;j<10;j++)
		sum[j]=0;

	for(i=0,c=1;i<cc;i++,c++)
	{
		for(j=0;j<10;j++)
			sum[j]+=code[i/THRESH][j]-48;			// Adding only first entry (3 times) from each set of 3 signs
		strcpy_s(ir.code[i%THRESH],code[i]);
		strcpy_s(ir.signcode,name[i/THRESH]);
		ir.count=THRESH;
		if(i!=0 && c%(THRESH)==0)
		{
			pos=0;
			for(j=0;j<10;j++)
				pos+=ceil((double)sum[j]/THRESH)*pow(10.0,j);
			cout<<"Position before hashing : "<<pos<<" for "<<name[i/THRESH]<<endl;
			/////////////////////////
			flag=0;
			hashindex2=hashval(pos,1,0,p,m);
			chesta=0;
			do
			{
				hashindex=probing(hashindex2,chesta,m);
				//printf("\t->%d",hashindex);
				fseek (frec,hashindex*sizeof(indexrec),SEEK_SET);
				fread(&irtemp,sizeof(indexrec),1,frec);
				chesta++;
			}while(irtemp.count!=0);
			//printf("\n");
			if(!flag)
			{
				fseek (frec,(-1)*sizeof(indexrec),SEEK_CUR);
				fwrite(&ir,sizeof(indexrec),1,frec);
			}
			/////////////////////////
			for(j=0;j<10;j++)
				sum[j]=0;
		}
	}
	fclose(frec);
}

void coding_tst()
{
	cout<<"Coding for signatures (per signature; sequentially each "<<(5-THRESH)<<" belongs to one person) : "<<endl;
	cc=0;
	for(int i=0;i<no_of_sign;i+=5)
	{
		for(int k=THRESH;k<5;k++)
		{
			for(int j=0;j<10;j++)
			{
				if((features[i+k][j]-minimum[j])/no_of_bins[j]==7)
					printf("*");
				code[cc][j]=(features[i+k][j]-minimum[j])/no_of_bins[j]+48;
				cout<<(char)((features[i+k][j]-minimum[j])/no_of_bins[j]+48)<<",";
			}
			cc++;
			cout<<endl;
		}
	}
}

void place_tst()
{
	int i,j,p,m;
	FILE *frec;
	indexrec ir,irtemp;

	m=twopower(no_of_sign/5);
	//m=m*2;
	cout<<endl<<"Total slots available : "<<m<<endl;
	p=smallestprime(m);

	long long pos,hashindex,hashindex2;
	int sum[10],flag,chesta,avg[10],mandis;

	frec=fopen("index.txt","rb+");
	if(!frec)
	{
		printf("File I/O error!");	
		return;
	}

	for(i=0;i<cc;i++)
	{
		strcpy_s(ir.code[0],code[i]);
		strcpy_s(ir.signcode,name[i/(5-THRESH)]);
		ir.count=THRESH;
		pos=0;
		for(j=0;j<10;j++)
			pos+=(code[i][j]-48)*pow(10.0,j);
		cout<<"Position before hashing : "<<pos<<" for "<<name[i/(5-THRESH)]<<endl;
		flag=0;
		hashindex2=hashval(pos,1,0,p,m);
		chesta=0;
		do
		{
			hashindex=probing(hashindex2,chesta,m);
			fseek (frec,hashindex*sizeof(indexrec),SEEK_SET);
			fread(&irtemp,sizeof(indexrec),1,frec);
			cout<<"*";
			/*for(j=0;j<10;j++)
				avg[j]=ceil((double)(irtemp.code[0][j]+irtemp.code[1][j]+irtemp.code[2][j])/3);*/
			mandis=0;
			for(j=0;j<10;j++)
				mandis+=abs(irtemp.code[0][j]-ir.code[0][j]);
			cout<<mandis<<"["<<irtemp.signcode<<"]"<<",";
			if(mandis==0)
			{
				flag=1;
				break;
			}
			chesta++;
		}while(irtemp.count!=0);	// LOOP UNTIL WE FIND AN EMPTY SLOT
		cout<<endl;
		if(flag==1)
		{
		//	 FOR EXACT MATCH WE WILL CONSIDER ITs AS A CORRECT CASE. SINCE, LEARNING AND TESTING DATA SETS ARE OVERLAPPED
		//	correct++;
		}
		if(flag==0)
		{
			//cndt_gen(code[i]);
			//cndt_show();
		}
			/////////////////////////
			//flag=0;
			//hashindex2=hashval(pos,1,0,p,m);
			//chesta=0;
			//do
			//{
			//	hashindex=probing(hashindex2,chesta,m);
			//	//printf("\t->%d",hashindex);
			//	fseek (frec,hashindex*sizeof(indexrec),SEEK_SET);
			//	fread(&irtemp,sizeof(indexrec),1,frec);
			//	chesta++;
			//}while(irtemp.count!=0);
			////printf("\n");
			//if(!flag)
			//{
			//	fseek (frec,(-1)*sizeof(indexrec),SEEK_CUR);
			//	fwrite(&ir,sizeof(indexrec),1,frec);
			//}
			///////////////////////////
			//for(j=0;j<10;j++)
			//	sum[j]=0;
	}
	fclose(frec);
	//showfile();
}

void binning2()
{
	int i,p,j,k,c,t,min,max,maxdiffer,differ[NOP],cd,univmax,univmin,temp[NOP],bin_size[NOP];
	nor=1;
	/*for(int i=0;i<no_of_sign;i++)
	{
		for(int k=0;k<10;k++)
			cout<<features[i][k]<<"\t";
		cout<<endl;
	}*/

	for(k=0;k<10;k++)
	{
		maxdiffer=0;
		cd=0;
		univmin=features[0][k];
		univmax=features[0][k];
		
		for(i=0;i<no_of_sign;i+=5)
		{
			for(j=0;j<3;j++)
			{
				p=i+j;
				if(j==0)
				{
					if(univmin>features[p][k])
						univmin=features[p][k];
					if(univmax<features[p][k])
						univmax=features[p][k];
					if(i!=0)
						differ[cd++]=abs(max-min);
					min=features[p][k];
					max=features[p][k];
				}
				else
				{
					if(univmin>features[p][k])
						univmin=features[p][k];
					if(univmax<features[p][k])
						univmax=features[p][k];
					if(min>features[p][k])
						min=features[p][k];
					if(max<features[p][k])
						max=features[p][k];
				}
			}
		 }
		differ[cd++]=abs(max-min);
		sort(differ,cd);
		
		maximum[k]=univmax;
		minimum[k]=univmin;
		breakup[k]=ceil(((double)univmax-univmin)/differ[0]);
		cout<<univmax<<"\t"<<univmin<<"\t"<<breakup[k]<<endl;

		temp[k]=abs(maximum[k]-minimum[k]);
	    bin_size[k]=temp[k]/breakup[k];
	    cd=0;
		for(i=0;i<no_of_sign;i+=5)
		{
			for(j=0;j<5;j++)
			{
				p=i+j;
				bin_no[cd++][k]=ceil(((double)features[p][k]-minimum[k])/bin_size[k]);
			}
		}
	 }

	for(i=0;i<NOP*5;i++)
	{ 
	  bin_no[i][10]=i/5;
	}
}

void create_directory(char *path,int level)
{
	if(level>3)
	{
		char fname[100];
		strcpy_s(fname,path);
		strcat_s(fname,"\\case.dat");
		FILE *f=fopen(fname,"w+");
		fclose(f);
		return;
	}
	char temp[100];
	char current[100];
	char command[100];
	for(int k=0;k<breakup[level]+2;k++)
	{
		strcpy_s(temp,path);
		strcat_s(temp,"\\");
		sprintf_s(current,"%d_%d",level,k);
		strcat_s(temp,current);
		strcpy_s(command,"md ");
		strcat_s(command,temp);
		//cout<<command<<endl;
		system(command);
		create_directory(temp,level+1);
	}
}

struct record
{
	char tag[20];
	int maxx[600];
	int minx[600];
	int maxx_count;
	int minx_count;
	char path1[100];
	char path2[100];
	char path0[100];
};

void place_trn2()
{
	char path1[100];
	char path2[100];
	char path3[100];
	char temp[100];
	FILE *f;

	for(int i=0;i<no_of_sign;i+=5)
	{
		strcpy_s(path1,"cb");
		strcpy_s(path2,"cb");
		strcpy_s(path3,"cb");
		for(int j=0;j<4;j++)
		{
			strcat_s(path1,"\\");
			if(features[i][j]==maximum[j])
				sprintf_s(temp,"%d_%d",j,breakup[j]);
			else if(features[i][j]>maximum[j])
				sprintf_s(temp,"%d_%d",j,breakup[j]+1);
			else if(features[i][j]<minimum[j])
				sprintf_s(temp,"%d_%d",j,0);
			else
				sprintf_s(temp,"%d_%d",j,((features[i][j]-minimum[j])/((maximum[j]-minimum[j])/breakup[j]))+1);
			strcat_s(path1,temp);
		}
		strcat_s(path1,"\\case.dat");

		for(int j=0;j<4;j++)
		{
			strcat_s(path2,"\\");
			if(features[i+1][j]==maximum[j])
				sprintf_s(temp,"%d_%d",j,breakup[j]);
			else if(features[i+1][j]>maximum[j])
				sprintf_s(temp,"%d_%d",j,breakup[j]+1);
			else if(features[i+1][j]<minimum[j])
				sprintf_s(temp,"%d_%d",j,0);
			else
				sprintf_s(temp,"%d_%d",j,((features[i+1][j]-minimum[j])/((maximum[j]-minimum[j])/breakup[j]))+1);
			strcat_s(path2,temp);
		}
		strcat_s(path2,"\\case.dat");

		for(int j=0;j<4;j++)
		{
			strcat_s(path3,"\\");
			if(features[i+2][j]==maximum[j])
				sprintf_s(temp,"%d_%d",j,breakup[j]);
			else if(features[i+2][j]>maximum[j])
				sprintf_s(temp,"%d_%d",j,breakup[j]+1);
			else if(features[i+2][j]<minimum[j])
				sprintf_s(temp,"%d_%d",j,0);
			else
				sprintf_s(temp,"%d_%d",j,((features[i+2][j]-minimum[j])/((maximum[j]-minimum[j])/breakup[j]))+1);
			strcat_s(path3,temp);
		}
		strcat_s(path3,"\\case.dat");
		
		record r1,r2,r3;
		for(int j=0;j<maxpoint_count[i];j++)
			r1.maxx[j]=maxpoints[i][j].x;
		for(int j=0;j<maxpoint_count[i];j++)
			r1.minx[j]=minpoints[i][j].x;
		r1.maxx_count=maxpoint_count[i];
		r1.minx_count=minpoint_count[i];
		strcpy_s(r1.path0,path1);
		strcpy_s(r1.path1,path2);
		strcpy_s(r1.path2,path3);
		strcpy_s(r1.tag,name[i/5]);

		for(int j=0;j<maxpoint_count[i+1];j++)
			r2.maxx[j]=maxpoints[i+1][j].x;
		for(int j=0;j<maxpoint_count[i+1];j++)
			r2.minx[j]=minpoints[i+1][j].x;
		r2.maxx_count=maxpoint_count[i+1];
		r2.minx_count=minpoint_count[i+1];
		strcpy_s(r2.path0,path2);
		strcpy_s(r2.path1,path1);
		strcpy_s(r2.path2,path3);
		strcpy_s(r2.tag,name[i/5]);

		for(int j=0;j<maxpoint_count[i+2];j++)
			r3.maxx[j]=maxpoints[i+2][j].x;
		for(int j=0;j<maxpoint_count[i+2];j++)
			r3.minx[j]=minpoints[i+2][j].x;
		r3.maxx_count=maxpoint_count[i+2];
		r3.minx_count=minpoint_count[i+2];
		strcpy_s(r3.path0,path3);
		strcpy_s(r3.path1,path1);
		strcpy_s(r3.path2,path2);
		strcpy_s(r3.tag,name[i/5]);

		f=fopen(path1,"ab");
		if(!f)
		{
			cout<<"file not available"<<endl;
			continue;
		}
		fwrite(&r1,sizeof(r1),1,f);
		fclose(f);

		f=fopen(path2,"ab");
		if(!f)
		{
			cout<<"file not available"<<endl;
			continue;
		}
		fwrite(&r2,sizeof(r2),1,f);
		fclose(f);

		f=fopen(path3,"ab");
		if(!f)
		{
			cout<<"file not available"<<endl;
			continue;
		}
		fwrite(&r3,sizeof(r3),1,f);
		fclose(f);
	}
}

char *path_construct(int *arr,int no)
{
	char path[100],temp[100];
	strcpy_s(path,"cb");
	for(int i=0;i<no;i++)
	{
		strcat_s(path,"\\");
		sprintf_s(temp,"%d_%d",i,arr[i]);
		strcat_s(path,temp);
	}
	return path;
}

void grnt_cndt(int *path_val,int no)
{
	cndt_cnt=0;
	for(int i=0;i<no;i++)
		candidates[cndt_cnt][i]=path_val[i];
	cndt_cnt++;
	for(int i=0;i<no;i++)
	{
		if(path_val[i]>0)
		{
			for(int j=0;j<no;j++)
				candidates[cndt_cnt][j]=path_val[j];
			candidates[cndt_cnt][i]--;
			cndt_cnt++;
		}
		if(path_val[i]<breakup[i]+1)
		{
			for(int j=0;j<no;j++)
				candidates[cndt_cnt][j]=path_val[j];
			candidates[cndt_cnt][i]++;
			cndt_cnt++;
		}
	}
}

double function1(record r,char *path)
{
	char temp[100];
	record tr;
	FILE *f;

	int min_found;
	record min_rec;
	int min_dtw_index;
	dtw_adv dt;

	record rec_pool[50];
	int record_processed;
	int rec_count;
	int dtw_value[50];
	int temp_value;

	double thresh=0.0;

	f=fopen(path,"rb");
		if(!f){cout<<"file not available"<<endl;return thresh;}

	record_processed=0;		
	rec_count=0;
	min_found=0;

	while(!feof(f))
	{
		if(!fread(&tr,sizeof(record),1,f))
			break;
		record_processed++;
		dt.input(r.maxx,r.maxx_count,tr.maxx,tr.maxx_count);
		rec_pool[rec_count]=tr;
		dtw_value[rec_count]=dt.get_index();
		rec_count++;

		if(!min_found)
		{
			min_rec=tr;
			min_dtw_index=dt.get_index();
			min_found=1;
		}
		else
		{
			if(min_dtw_index>dt.get_index())
			{
				min_rec=tr;
				min_dtw_index=dt.get_index();
			}
		}
	}

	if(record_processed)
	{
		int first=0;
		int second=0;
		if(!strcmp(path,min_rec.path1))
			first=1;
		if(!strcmp(path,min_rec.path2))
			second=1;
		if(!first)
		{
			f=fopen(min_rec.path1,"rb");
			if(!f){cout<<"file not available"<<endl;return thresh;}
			while(!feof(f))
			{
				if(!fread(&tr,sizeof(record),1,f))
					break;
				if(!strcmp(tr.tag,min_rec.tag))
				{
					rec_pool[rec_count]=tr;
					dt.input(r.maxx,r.maxx_count,tr.maxx,tr.maxx_count);
					dtw_value[rec_count]=dt.get_index();
					rec_count++;
					break;
				}
			}
			fclose(f);
		}
		if(!second)
		{
			f=fopen(min_rec.path2,"rb");
			if(!f){cout<<"file not available"<<endl;return thresh;}
			while(!feof(f))
			{
				if(!fread(&tr,sizeof(record),1,f))
					break;
				if(!strcmp(tr.tag,min_rec.tag))
				{
					rec_pool[rec_count]=tr;
					dt.input(r.maxx,r.maxx_count,tr.maxx,tr.maxx_count);
					dtw_value[rec_count]=dt.get_index();
					rec_count++;
					break;
				}
			}
			fclose(f);
		}
		for(int m=0;m<rec_count;m++)
		{
			for(int n=0;n<rec_count-1;n++)
			{
				if(dtw_value[n]>dtw_value[n+1])
				{
					temp_value=dtw_value[n];
					dtw_value[n]=dtw_value[n+1];
					dtw_value[n+1]=temp_value;

					tr=rec_pool[n];
					rec_pool[n]=rec_pool[n+1];
					rec_pool[n+1]=tr;
				}
			}
		}
		//for(int m=0;m<rec_count;m++)
			//cout<<rec_pool[m].tag<<"\t";//<<"("<<dtw_value[m]<<")"<<"\t";
		thresh=0;
		for(int m=1;m<rec_count;m++)
		{
			if(!strcmp(rec_pool[0].tag,rec_pool[m].tag))
				thresh+=m;
		}
		thresh/=3;
		cout<<"\t"<<thresh<<"("<<rec_pool[0].tag<<")";
	}
	else
	{
		cout<<thresh<<"(#NRF)"<<"\t";
	}
	fclose(f);
	return thresh;
}

void place_tst2()
{
	char path[100];
	char temp[100];
	record r,tr;
	FILE *f;
	int path_val[4];

	int min_found;
	char min_path[100];
	int min_dtw_index;
	dtw_adv dt;

	int record_processed;

	double thresh;

	for(int i=0;i<no_of_sign;i++)
	{
		if(i%5!=3 && i%5!=4)
			continue;
        cout<<endl<<"placing : "<<name[i/5]<<" : \t";

		for(int j=0;j<maxpoint_count[i];j++)
			r.maxx[j]=maxpoints[i][j].x;
		for(int j=0;j<maxpoint_count[i];j++)
			r.minx[j]=minpoints[i][j].x;
		r.maxx_count=maxpoint_count[i];
		r.minx_count=minpoint_count[i];
		strcpy_s(r.tag,name[i/5]);

		strcpy_s(path,"cb");
		for(int j=0;j<4;j++)
		{
			strcat_s(path,"\\");
			if(features[i][j]==maximum[j])
				sprintf_s(temp,"%d_%d",j,breakup[j]);
			else if(features[i][j]>maximum[j])
				sprintf_s(temp,"%d_%d",j,breakup[j]+1);
			else if(features[i][j]<minimum[j])
				sprintf_s(temp,"%d_%d",j,0);
			else
				sprintf_s(temp,"%d_%d",j,((features[i][j]-minimum[j])/((maximum[j]-minimum[j])/breakup[j]))+1);
			strcat_s(path,temp);
		}
		strcat_s(path,"\\case.dat");

		cout<<"["<<path<<"]";

		thresh=function1(r,path);

		if(thresh==0.0 || thresh>1.06 || 1)
		{
			for(int j=0;j<4;j++)
			{
				if(features[i][j]==maximum[j])
					path_val[j]=breakup[j];
				else if(features[i][j]>maximum[j])
					path_val[j]=breakup[j]+1;
				else if(features[i][j]<minimum[j])
					path_val[j]=0;
				else
					path_val[j]=((features[i][j]-minimum[j])/((maximum[j]-minimum[j])/breakup[j]))+1;
			}
			grnt_cndt(path_val,4);
			min_found=0;
			for(int j=0;j<cndt_cnt;j++)
			{
				strcpy(path,path_construct(candidates[j],4));
				strcat(path,"\\case.dat");
				f=fopen(path,"rb");
				if(!f){cout<<"file not available"<<endl;continue;}
				record_processed=0;
				while(!feof(f))
				{
					if(!fread(&tr,sizeof(record),1,f))
						break;
					record_processed++;
					dt.input(r.maxx,r.maxx_count,tr.maxx,tr.maxx_count);

					if(!min_found)
					{
						strcpy(min_path,path);
						min_dtw_index=dt.get_index();
						min_found=1;
					}
					else
					{
						if(min_dtw_index>dt.get_index())
						{
							strcpy(min_path,path);
							min_dtw_index=dt.get_index();
						}
					}
				}
				fclose(f);
			}
			function1(r,min_path);
		}
		cout<<endl;
	}
}

void test_a_file()
{
	char path[100];
	record tr;
	cout<<endl<<"test";
	strcpy_s(path,"cb\\0_2\\1_2\\2_1\\3_2\\case.dat");
	FILE *f=fopen(path,"rb");
	while(!feof(f))
	{
		if(!fread(&tr,sizeof(record),1,f))
			break;
		cout<<tr.tag<<endl;
	}
}
