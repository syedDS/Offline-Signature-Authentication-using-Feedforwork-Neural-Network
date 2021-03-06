#ifndef _DATAREADER
#define _DATAREADER

//standard libraries
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>

//custom includes
#include "dataEntry.h"

using namespace std;

//dataset retrieval approach enum
enum { NONE, STATIC, GROWING, WINDOWING };

class dataSet
{
public:

	vector<dataEntry*> trainingSet;
	vector<dataEntry*> generalizationSet;
	vector<dataEntry*> validationSet;

	dataSet()
	{		
	}

	~dataSet()
	{
		trainingSet.clear();
		generalizationSet.clear();
		validationSet.clear();
	}

	void clear()
	{
		trainingSet.clear();
		generalizationSet.clear();
		validationSet.clear();
	}
};

//data reader class
class dataReader
{
	
//private members
//----------------------------------------------------------------------------------------------------------------
private:

	//data storage
	vector<dataEntry*> data;
	int nInputs;
	int nTargets;

	//current data set
	dataSet dSet;

	//data set creation approach and total number of dataSets
	int creationApproach;
	int numDataSets;
	int trainingDataEndIndex;

	//creation approach variables
	double growingStepSize;			//step size - percentage of total set
	int growingLastDataIndex;		//last index added to current dataSet
	int windowingSetSize;			//initial size of set
	int windowingStepSize;			//how many entries to move window by
	int windowingStartIndex;		//window start index	
	
//public methods
//----------------------------------------------------------------------------------------------------------------
public:

	//constructor
	dataReader(): creationApproach(NONE), numDataSets(-1)
	{				
	}

	//destructor
	~dataReader()
	{
		//clear data
		for (int i=0; i < (int) data.size(); i++ ) delete data[i];
		data.clear();
	}

	//read data from file
	bool loadDataFile(int nI, int nT )
	{
		int i,j;
		//clear any previous data
		for (i=0; i < (int) data.size(); i++ ) delete data[i];		
		data.clear();
		dSet.clear();
		
		//set number of inputs and outputs
		nInputs = nI;
		nTargets = nT;

		//insert data
		double* pattern = new double[nInputs];
		double* target = new double[nTargets];

		for(i=0;i<NOP*5;i++)
		{
			pocessEntry(i);
		}

		for(int i=0;i<NOP*5;i+=5)
		{
			int j;
			for(j=i;j<i+3;j++)
				dSet.trainingSet.push_back( data[j] );
			dSet.generalizationSet.push_back( data[j] );
			j++;
			dSet.validationSet.push_back( data[j] );
		}
		cout<<dSet.generalizationSet.size()<<" "<<dSet.validationSet.size()<<" "<<dSet.trainingSet.size();
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////shuffle data
		//random_shuffle(data.begin(), data.end());

		////split data set
		//trainingDataEndIndex = (int) ( 0.6 * data.size() );
		//int gSize = (int) ( ceil(0.2 * data.size()) );
		//int vSize = (int) ( data.size() - trainingDataEndIndex - gSize );
		//						
		////generalization set
		//for ( int i = trainingDataEndIndex; i < trainingDataEndIndex + gSize; i++ ) dSet.generalizationSet.push_back( data[i] );
		//			
		////validation set
		//for ( int i = trainingDataEndIndex + gSize; i < (int) data.size(); i++ ) dSet.validationSet.push_back( data[i] );
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
		//print success
		cout << "Data File Read Complete >> Patterns Loaded: " << data.size() << endl;
			
		return true;
	}

	//select dataset approach
	void setCreationApproach( int approach, double param1 = -1, double param2 = -1 )
	{
		//static
		if ( approach == STATIC )
		{
			creationApproach = STATIC;
			
			//only 1 data set
			numDataSets = 1;
		}

		//growing
		else if ( approach == GROWING )
		{			
			if ( param1 <= 100.0 && param1 > 0)
			{
				creationApproach = GROWING;
			
				//step size
				growingStepSize = param1 / 100;
				growingLastDataIndex = 0;

				//number of sets
				numDataSets = (int) ceil( 1 / growingStepSize );				
			}
		}

		//windowing
		else if ( approach == WINDOWING )
		{
			//if initial size smaller than total entries and step size smaller than set size
			if ( param1 < data.size() && param2 <= param1)
			{
				creationApproach = WINDOWING;
				
				//params
				windowingSetSize = (int) param1;
				windowingStepSize = (int) param2;
				windowingStartIndex = 0;			

				//number of sets
				numDataSets = (int) ceil( (double) ( trainingDataEndIndex - windowingSetSize ) / windowingStepSize ) + 1;
			}			
		}

	}

	//return number of datasets
	int nDataSets()
	{
		return numDataSets;
	}
	
	dataSet* getDataSet()
	{		
		switch ( creationApproach )
		{
		
			case STATIC : createStaticDataSet(); break;
			case GROWING : createGrowingDataSet(); break;
			case WINDOWING : createWindowingDataSet(); break;
		}
		
		return &dSet;
	}

//private methods
//----------------------------------------------------------------------------------------------------------------
private:
	
	void createStaticDataSet()
	{
		//training set
		//for ( int i = 0; i < trainingDataEndIndex; i++ ) dSet.trainingSet.push_back( data[i] );		
	}

	void createGrowingDataSet()
	{
		//increase data set by step percentage
		growingLastDataIndex += (int) ceil( growingStepSize * trainingDataEndIndex );		
		if ( growingLastDataIndex > (int) trainingDataEndIndex ) growingLastDataIndex = trainingDataEndIndex;

		//clear sets
		dSet.trainingSet.clear();
		
		//training set
		for ( int i = 0; i < growingLastDataIndex; i++ ) dSet.trainingSet.push_back( data[i] );			
	}

	void createWindowingDataSet()
	{
		//create end point
		int endIndex = windowingStartIndex + windowingSetSize;
		if ( endIndex > trainingDataEndIndex ) endIndex = trainingDataEndIndex;		

		//clear sets
		dSet.trainingSet.clear();
						
		//training set
		for ( int i = windowingStartIndex; i < endIndex; i++ ) dSet.trainingSet.push_back( data[i] );
				
		//increase start index
		windowingStartIndex += windowingStepSize;
	}

	void pocessEntry(int i)
	{
		double* pattern = new double[nInputs];
		double* target = new double[nTargets];
		for(int j=0;j<10;j++)
			pattern[j]=bin_no[i][j];
		for(int j=0;j<nTargets;j++)
			target[j]=0;
		target[i/5]=1;
		data.push_back( new dataEntry( pattern, target ) );
	}
};

#endif
