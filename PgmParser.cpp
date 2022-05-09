// header declarations
#define THRESH 3
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <math.h>
#include <cctype>
#include <string.h>
#include "pgm.h"
#include "sumangal_class.h"
#include "sumangal_features.h"
#include "cbr_core.h"
#include "modify.h"
#include "infogain.h"
#include "dataEntry.h"
#include "dataReader.h"
#include "neuralNetwork.h"

using namespace std;

//main function
int main ()
{
	PGMData d;
	int no_of_person;
	char ifname[500][20];
	no_of_person=NOP;					// number of persons
	no_of_sign=0;						// number of signatures

	//generate input file name according to the each person's code
	for(int i=0;i<no_of_person;i++)
	{
		for(int j=1;j<=5;j++)
		{
			strcpy_s(ifname[no_of_sign],name[i]);
			strcat_s(ifname[no_of_sign],"_");
			int pos=strlen(ifname[no_of_sign]);
			ifname[no_of_sign][pos]=j+48;
			ifname[no_of_sign][pos+1]='\0';
			strcat_s(ifname[no_of_sign],".pgm");
			no_of_sign++;
		}
	}
	
	for(int i=0;i<no_of_sign;i++)
	{
		char in_path[40],out_path[40];
		strcpy_s(in_path,"../input/");
		strcat_s(in_path,ifname[i]);
		strcpy_s(out_path,"../output/");
		strcat_s(out_path,ifname[i]);

		readPGM(in_path,&d);
		cout<<ifname[i]<<endl;
		noiseremoval(&d);
		
		aligning(&d);

		//resize(&d,580);

		padding(&d,1);

		upper_contur_extration(&d,i);

		thining(&d);

		rotating(&d);

		padding(&d,1);

		global_features(&d,i);

		//maxima_charateristics(&d,i);
		
		writePGM(out_path,&d);
	}


	//cout<<"Raw fatures array : "<<endl;
	//for(int i=0;i<no_of_sign;i++)
	//{
	//	if(i%5==0)
	//		cout<<endl;
	//	for(int j=0;j<10;j++)
	//	{
	//		//cout<<features[i][j]<<",";
	//		cout<<ex_feature[i][j]<<",";
	//	}
	//	cout<<endl;
	//}
	
	//test_analysis();

	binning2();

	dataReader da;
	da.loadDataFile(10,68);
	da.setCreationApproach( STATIC );

	neuralNetwork nn(4, 16, 68);
	nn.enableLogging("trainingResults.csv");
	nn.setLearningParameters(0.03, 0.8);
	nn.setDesiredAccuracy(50);
	nn.setMaxEpochs(500);

	dataSet* dset;

	for ( int i=0; i < da.nDataSets(); i++ )
	{
		dset = da.getDataSet();
		nn.trainNetwork( dset->trainingSet, dset->generalizationSet, dset->validationSet );
	}
	//info_gain();
	//create_directory("cb",0);
	//cout<<"Directories are created"<<endl;
	//place_trn2();
	//place_tst2();

	//test_a_file();

	getch();

	return 0;
}