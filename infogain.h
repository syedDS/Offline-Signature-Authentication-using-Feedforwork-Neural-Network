#define ATTRB  10
#define MAXBIN  10 // max of bin
// for evaluating a features by info gain

double info_gain2(int attrib)
{
	int freq_attr[MAXBIN];
	int freq_attr_class[MAXBIN][NOP];
	double temp;
	double info_gain=0;

	for(int i=0;i<MAXBIN;i++)
	{
		freq_attr[i]=0;
		for(int j=0;j<NOP;j++)
			freq_attr_class[i][j]=0;
	}
	
	for(int i=0;i<NOP*3;i++)
	{
		freq_attr[bin_no[i][attrib]]++;
		freq_attr_class[bin_no[i][attrib]][bin_no[i][ATTRB]]++;
	}

	//for(int i=0;i<=breakup[attrib]+1;i++)
	//{
	//	cout<<freq_attr[i]<<":"<<endl;
	//	for(int j=0;j<NOP;j++)
	//	{
	//		cout<<freq_attr_class[i][j]<<"\t";
	//	}
	//	cout<<endl;
	//}

	for(int i=0;i<=breakup[attrib]+1;i++)
	{
		temp=0;
		for(int j=0;j<NOP;j++)
		{
			if(freq_attr_class[i][j]!=0)
				temp+=((double)freq_attr_class[i][j]/freq_attr[i])*(log((double)freq_attr_class[i][j]/freq_attr[i])/log(2.0));
		}
		info_gain+=((double)freq_attr[i]/(NOP*3))*(-1)*temp;
	}
	return info_gain;
}

void info_gain()
{
	for(int i=0;i<10;i++)
	{
		cout<<info_gain2(i);
		cout<<endl;
	}
}