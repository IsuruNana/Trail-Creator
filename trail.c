#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct pgm
{
  int rows;
  int cols;

  int** pixels;
}pgmPic;

int FindMax(pgmPic* pictureSent)
{
    int max = pictureSent->pixels[0][0];
    int i =0;
    int j =0;
    
    for(i = 0; i < pictureSent->rows; i++)
    {
        for(j = 0; j < pictureSent->cols; j++)
        {
            if(pictureSent->pixels[i][j] > max)
            {
                max = pictureSent->pixels[i][j];
            }
        }
    }
    
    return max;
}

int FindMin(pgmPic* pictureSent)
{
    int min = pictureSent->pixels[0][0];
    int i =0;
    int j =0;
    
    for(i = 0; i < pictureSent->rows; i++)
    {
        for(j = 0; j < pictureSent->cols; j++)
        {
            if(pictureSent->pixels[i][j] < min)
            {
                min = pictureSent->pixels[i][j];
            }
        }
    }
    
    return min;
}

void UpdateData(pgmPic* pictureSent, float multiplier, int minValue)
{
    int i = 0;
    int j =0;
    
    for(i = 0; i < pictureSent->rows; i++)
    {
        for(j = 0; j < pictureSent->cols; j++)
        {
        	//float newValue = (float) pictureSent->pixels[i][j] * scaledRange;
            float newValue = ( (float) pictureSent->pixels[i][j] - (float) minValue )* multiplier;
            pictureSent->pixels[i][j] = (int) round(newValue);
            
            pictureSent->pixels[i][j] = 255 - pictureSent->pixels[i][j];
            //pictureSent->pixels[i][j] = (int) round(pictureSent->pixels[i][j]);
        }
    }
}

void CreatePMG(char dataFileName[], pgmPic* pictureSent)
{
	char rowsString[10];
	char columnsString[10];
	char dataValueString[10];
	int i =0;
	int j =0;
	
	char pgmName[50];
    for(i = 0; dataFileName[i] != '.'&& i<strlen(dataFileName); i++)
    {
    	pgmName[i] = dataFileName[i];    	
    }
    
    strcat(pgmName, ".pgm");
   
	FILE* outputFile = fopen(pgmName, "w+");
	sprintf(columnsString, "%d", pictureSent->cols);
	sprintf(rowsString, " %d\n", pictureSent->rows);
	
	fputs("P2\n", outputFile);
	fputs(columnsString, outputFile);
	fputs(rowsString, outputFile);
	fputs("255\n",outputFile);
	
	for(i = 0; i < pictureSent->rows; i++)
    {
        for(j = 0; j < pictureSent->cols; j++)
        {
        	sprintf(dataValueString, "%d",  pictureSent->pixels[i][j]);
        	strcat(dataValueString, "\t");
        	//printf("%s", dataValueString);
            fputs(dataValueString, outputFile);
            //printf("Executed\n");
        }
        //printf("\n");
        fputs("\n", outputFile);
    }
	
	fclose(outputFile);
}

typedef struct Tracker
{
	int previousColumn;
	int currentColumn;
	int nextColumn;
	
	int highestValue;
}Tracker;

typedef struct Tracker2
{
	int previousRow;
	int currentRow;
	int nextRow;
	
	int highestValue;
}Tracker2;

void GoEastToWest(pgmPic* pictureSent)
{
	Tracker2 trackDataWE;
	
	trackDataWE.currentRow = pictureSent->rows/2;
	trackDataWE.previousRow = trackDataWE.currentRow - 1;
	trackDataWE.nextRow = trackDataWE.currentRow + 1;
	
	int currentColumn = pictureSent->cols - 1;
	
	for(currentColumn; currentColumn >= 0; currentColumn--)
	{
		if(currentColumn == pictureSent->cols - 1)
		{
			pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
		}
		
		else
		{
			int i;
			
			if(trackDataWE.previousRow != -1 )
			{
				i = trackDataWE.previousRow;
			}
			
			else if(trackDataWE.previousRow == -1)
			{
				i = trackDataWE.currentRow;
			}
			
			int tieCount = -1;
			trackDataWE.highestValue = pictureSent->pixels[i][currentColumn];
			
			for(i; i<=trackDataWE.nextRow && trackDataWE.nextRow != pictureSent->rows; i++)
			{
				if(pictureSent->pixels[i][currentColumn] == trackDataWE.highestValue)
				{
					tieCount++;
				}
				
				if(pictureSent->pixels[i][currentColumn] > trackDataWE.highestValue)
				{
					trackDataWE.highestValue = pictureSent->pixels[i][currentColumn];
					//printf("Highest value: %d\n", trackDataWE.highestValue);
				}
			}
			
			if(tieCount <= 0)
			{
				if(trackDataWE.previousRow == -1)
				{
					if(pictureSent->pixels[trackDataWE.currentRow][currentColumn] == trackDataWE.highestValue)
					{
						pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
					}
					
					else
					{
						//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.nextRow][currentColumn],  trackDataWE.nextRow, currentColumn);
						pictureSent->pixels[trackDataWE.nextRow][currentColumn] = 0;
						trackDataWE.previousRow = trackDataWE.previousRow +1;
						trackDataWE.currentRow = trackDataWE.currentRow +1;
						trackDataWE.nextRow = trackDataWE.nextRow +1;
						//printf("CurrentRow = %d\n", trackDataWE.currentRow);
					}
				}
				
				else if(trackDataWE.currentRow == pictureSent->rows - 1)
				{
					if(pictureSent->pixels[trackDataWE.currentRow][currentColumn] >= trackDataWE.highestValue)
					{
						pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
					}
					
					else
					{
						//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.previousRow][currentColumn],  trackDataWE.previousRow, currentColumn);
						pictureSent->pixels[trackDataWE.previousRow][currentColumn] = 0;
						trackDataWE.previousRow = trackDataWE.previousRow -1;
						trackDataWE.currentRow = trackDataWE.currentRow -1;
						trackDataWE.nextRow = trackDataWE.nextRow -1;
						//printf("CurrentRow = %d\n", trackDataWE.currentRow);
					}
				}
				
				else
				{
					if(pictureSent->pixels[trackDataWE.previousRow][currentColumn] == trackDataWE.highestValue)
					{
						//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.previousRow][currentColumn],  trackDataWE.previousRow, currentColumn);
						pictureSent->pixels[trackDataWE.previousRow][currentColumn] = 0;
						trackDataWE.previousRow = trackDataWE.previousRow - 1;
						//printf("CurrentRow = %d\n", trackDataWE.currentRow);
						trackDataWE.currentRow = trackDataWE.currentRow - 1;
						trackDataWE.nextRow = trackDataWE.nextRow - 1;
						//printf("PreviousRow = %d\n", trackDataWE.previousRow);
					}
					
					else if(pictureSent->pixels[trackDataWE.currentRow][currentColumn] == trackDataWE.highestValue)
					{
						pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
					}
					
					else
					{
						pictureSent->pixels[trackDataWE.nextRow][currentColumn] = 0;
						trackDataWE.previousRow = trackDataWE.previousRow + 1;
						trackDataWE.currentRow = trackDataWE.currentRow + 1;
						trackDataWE.nextRow = trackDataWE.nextRow + 1;
					}
				}
			}
			
			else
			{
				//printf("Tie found\n");
				if(pictureSent->pixels[trackDataWE.currentRow][currentColumn] == trackDataWE.highestValue)
				{
					pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
				}
				
				else if(pictureSent->pixels[trackDataWE.previousRow][currentColumn] >= pictureSent->pixels[trackDataWE.nextRow][currentColumn])
				{
					//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.previousRow][currentColumn],  trackDataWE.previousRow, currentColumn);
					pictureSent->pixels[trackDataWE.previousRow][currentColumn] = 0;
					trackDataWE.previousRow = trackDataWE.previousRow - 1;
					trackDataWE.currentRow = trackDataWE.currentRow - 1;
					trackDataWE.nextRow = trackDataWE.nextRow - 1;
				}
				
				else
				{
					//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.nextRow][currentColumn],  trackDataWE.nextRow, currentColumn);
					pictureSent->pixels[trackDataWE.nextRow][currentColumn] = 0;
					trackDataWE.previousRow = trackDataWE.previousRow + 1;
					trackDataWE.currentRow = trackDataWE.currentRow + 1;
					trackDataWE.nextRow = trackDataWE.nextRow + 1;
				}
				
			}
		}
	}
}

void GoWestToEast(pgmPic* pictureSent)
{
	Tracker2 trackDataWE;
	
	trackDataWE.currentRow = pictureSent->rows/2;
	trackDataWE.previousRow = trackDataWE.currentRow - 1;
	trackDataWE.nextRow = trackDataWE.currentRow + 1;
	
	int currentColumn = 0;
	
	for(currentColumn; currentColumn < pictureSent->cols; currentColumn++)
	{
		if(currentColumn == 0)
		{
			pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
		}
		
		else
		{
			int i;
			
			if(trackDataWE.previousRow != -1 )
			{
				i = trackDataWE.previousRow;
			}
			
			else if(trackDataWE.previousRow == -1)
			{
				i = trackDataWE.currentRow;
			}
			
			int tieCount = -1;
			trackDataWE.highestValue = pictureSent->pixels[i][currentColumn];
			
			for(i; i<=trackDataWE.nextRow && trackDataWE.nextRow != pictureSent->rows; i++)
			{
				if(pictureSent->pixels[i][currentColumn] == trackDataWE.highestValue)
				{
					tieCount++;
				}
				
				if(pictureSent->pixels[i][currentColumn] > trackDataWE.highestValue)
				{
					trackDataWE.highestValue = pictureSent->pixels[i][currentColumn];
					//printf("Highest value: %d\n", trackDataWE.highestValue);
				}
			}
			
			if(tieCount <= 0)
			{
				if(trackDataWE.previousRow == -1)
				{
					if(pictureSent->pixels[trackDataWE.currentRow][currentColumn] == trackDataWE.highestValue)
					{
						pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
					}
					
					else
					{
						//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.nextRow][currentColumn],  trackDataWE.nextRow, currentColumn);
						pictureSent->pixels[trackDataWE.nextRow][currentColumn] = 0;
						trackDataWE.previousRow = trackDataWE.previousRow +1;
						trackDataWE.currentRow = trackDataWE.currentRow +1;
						trackDataWE.nextRow = trackDataWE.nextRow +1;
						//printf("CurrentRow = %d\n", trackDataWE.currentRow);
					}
				}
				
				else if(trackDataWE.currentRow == pictureSent->rows - 1)
				{
					if(pictureSent->pixels[trackDataWE.currentRow][currentColumn] >= trackDataWE.highestValue)
					{
						pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
					}
					
					else
					{
						//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.previousRow][currentColumn],  trackDataWE.previousRow, currentColumn);
						pictureSent->pixels[trackDataWE.previousRow][currentColumn] = 0;
						trackDataWE.previousRow = trackDataWE.previousRow -1;
						trackDataWE.currentRow = trackDataWE.currentRow -1;
						trackDataWE.nextRow = trackDataWE.nextRow -1;
						//printf("CurrentRow = %d\n", trackDataWE.currentRow);
					}
				}
				
				else
				{
					if(pictureSent->pixels[trackDataWE.previousRow][currentColumn] == trackDataWE.highestValue)
					{
						//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.previousRow][currentColumn],  trackDataWE.previousRow, currentColumn);
						pictureSent->pixels[trackDataWE.previousRow][currentColumn] = 0;
						trackDataWE.previousRow = trackDataWE.previousRow - 1;
						//printf("CurrentRow = %d\n", trackDataWE.currentRow);
						trackDataWE.currentRow = trackDataWE.currentRow - 1;
						trackDataWE.nextRow = trackDataWE.nextRow - 1;
						//printf("PreviousRow = %d\n", trackDataWE.previousRow);
					}
					
					else if(pictureSent->pixels[trackDataWE.currentRow][currentColumn] == trackDataWE.highestValue)
					{
						pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
					}
					
					else
					{
						pictureSent->pixels[trackDataWE.nextRow][currentColumn] = 0;
						trackDataWE.previousRow = trackDataWE.previousRow + 1;
						trackDataWE.currentRow = trackDataWE.currentRow + 1;
						trackDataWE.nextRow = trackDataWE.nextRow + 1;
					}
				}
			}
			
			else
			{
				//printf("Tie found\n");
				if(pictureSent->pixels[trackDataWE.currentRow][currentColumn] == trackDataWE.highestValue)
				{
					pictureSent->pixels[trackDataWE.currentRow][currentColumn] = 0;
				}
				
				else if(pictureSent->pixels[trackDataWE.previousRow][currentColumn] <= pictureSent->pixels[trackDataWE.nextRow][currentColumn])
				{
					//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.previousRow][currentColumn],  trackDataWE.previousRow, currentColumn);
					pictureSent->pixels[trackDataWE.nextRow][currentColumn] = 0;
					trackDataWE.previousRow = trackDataWE.previousRow + 1;
					trackDataWE.currentRow = trackDataWE.currentRow + 1;
					trackDataWE.nextRow = trackDataWE.nextRow + 1;
				}
				
				else
				{
					//printf("Making %d at [%d][%d] = 0\n", pictureSent->pixels[trackDataWE.nextRow][currentColumn],  trackDataWE.nextRow, currentColumn);
					pictureSent->pixels[trackDataWE.previousRow][currentColumn] = 0;
					trackDataWE.previousRow = trackDataWE.previousRow - 1;
					trackDataWE.currentRow = trackDataWE.currentRow - 1;
					trackDataWE.nextRow = trackDataWE.nextRow - 1;
				}
			}
		}
	}
}

void GoNorthToSouth(pgmPic* pictureSent)
{
	Tracker trackDataSN;
	
	trackDataSN.currentColumn = pictureSent->cols/2;
	trackDataSN.previousColumn = trackDataSN.currentColumn - 1;
	trackDataSN.nextColumn = trackDataSN.currentColumn + 1;
	
	int currentRow = 0;
	
	for(currentRow; currentRow < pictureSent->rows; currentRow++)
	{
		if(currentRow == 0)
		{
			pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
		}
		
		else
		{
			int i;
			
			if(trackDataSN.previousColumn == -1)
			{
				i = trackDataSN.currentColumn;
			}
			
			else
			{
				i = trackDataSN.previousColumn;
			}
			
			int tieCount = -1;
			
			trackDataSN.highestValue = pictureSent->pixels[currentRow][i];
			
			for(i; i<=trackDataSN.nextColumn && trackDataSN.nextColumn != pictureSent->cols; i++)
			{
				if(pictureSent->pixels[currentRow][i] == trackDataSN.highestValue)
				{
					tieCount++;
				}
				
				if(pictureSent->pixels[currentRow][i] > trackDataSN.highestValue)
				{
					trackDataSN.highestValue = pictureSent->pixels[currentRow][i];
				}
			
			}
			
			if(tieCount <= 0)
			{
				//int j = trackDataSN.previousColumn;
				
				if(trackDataSN.previousColumn == -1)
				{
					if(pictureSent->pixels[currentRow][trackDataSN.currentColumn] >= pictureSent->pixels[currentRow][trackDataSN.nextColumn])
					{
						pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
					}
					
					else
					{
						pictureSent->pixels[currentRow][trackDataSN.nextColumn] = 0;
						trackDataSN.previousColumn = trackDataSN.previousColumn + 1;
						trackDataSN.currentColumn = trackDataSN.currentColumn + 1;
						trackDataSN.nextColumn = trackDataSN.nextColumn + 1;
					}
				}
				
				else if(trackDataSN.nextColumn == pictureSent->cols)
				{
					if(pictureSent->pixels[currentRow][trackDataSN.currentColumn] >= pictureSent->pixels[currentRow][trackDataSN.previousColumn])
					{
						//printf("Here\n");
						pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
					}
					
					else
					{
						//printf("Here\n");
						pictureSent->pixels[currentRow][trackDataSN.previousColumn] = 0;
						trackDataSN.previousColumn = trackDataSN.previousColumn - 1;
						trackDataSN.currentColumn = trackDataSN.currentColumn - 1;
						trackDataSN.nextColumn = trackDataSN.nextColumn - 1;
					}
				}
				
				else
				{
					if(pictureSent->pixels[currentRow][trackDataSN.previousColumn] == trackDataSN.highestValue)
					{
						pictureSent->pixels[currentRow][trackDataSN.previousColumn] = 0;
						trackDataSN.previousColumn = trackDataSN.previousColumn - 1;
						trackDataSN.currentColumn = trackDataSN.currentColumn - 1;
						trackDataSN.nextColumn = trackDataSN.nextColumn - 1;
					}
					
					else if(pictureSent->pixels[currentRow][trackDataSN.currentColumn] == trackDataSN.highestValue)
					{
						pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
					}
					
					else
					{
						pictureSent->pixels[currentRow][trackDataSN.nextColumn] = 0;
						trackDataSN.previousColumn = trackDataSN.previousColumn + 1;
						trackDataSN.currentColumn = trackDataSN.currentColumn + 1;
						trackDataSN.nextColumn = trackDataSN.nextColumn + 1;
					}
				}
			
			}
			
			else
			{
				if(pictureSent->pixels[currentRow][trackDataSN.currentColumn] == trackDataSN.highestValue)
				{
					pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
				}
				
				else if(pictureSent->pixels[currentRow][trackDataSN.previousColumn] >= pictureSent->pixels[currentRow][trackDataSN.nextColumn])
				{
					pictureSent->pixels[currentRow][trackDataSN.previousColumn] = 0;
					trackDataSN.previousColumn = trackDataSN.previousColumn - 1;
					trackDataSN.currentColumn = trackDataSN.currentColumn - 1;
					trackDataSN.nextColumn = trackDataSN.nextColumn - 1;
				}
				
				else
				{
					pictureSent->pixels[currentRow][trackDataSN.nextColumn] = 0;
					trackDataSN.previousColumn = trackDataSN.previousColumn + 1;
					trackDataSN.currentColumn = trackDataSN.currentColumn + 1;
					trackDataSN.nextColumn = trackDataSN.nextColumn + 1;
				}
				
			}
		}
	}
	
}

void GoSouthToNorth(pgmPic* pictureSent)
{
	Tracker trackDataSN;
	
	trackDataSN.currentColumn = pictureSent->cols/2;
	trackDataSN.previousColumn = trackDataSN.currentColumn - 1;
	trackDataSN.nextColumn = trackDataSN.currentColumn + 1;
	
	int currentRow = pictureSent->rows -1;
	
	for(currentRow; currentRow>=0; currentRow--)
	{
		if(currentRow == pictureSent->rows -1)
		{
			pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
		}
		
		else
		{
			int i;
			
			if(trackDataSN.previousColumn == -1)
			{
				i = trackDataSN.currentColumn;
			}
			
			else
			{
				i = trackDataSN.previousColumn;
			}
			
			int tieCount = -1;
			
			trackDataSN.highestValue = pictureSent->pixels[currentRow][i];
			
			for(i; i<=trackDataSN.nextColumn && trackDataSN.nextColumn != pictureSent->cols; i++)
			{
				if(pictureSent->pixels[currentRow][i] == trackDataSN.highestValue)
				{
					tieCount++;
				}
				
				if(pictureSent->pixels[currentRow][i] > trackDataSN.highestValue)
				{
					trackDataSN.highestValue = pictureSent->pixels[currentRow][i];
				}
			
			}
			
			if(tieCount <= 0)
			{
				//int j = trackDataSN.previousColumn;
				
				if(trackDataSN.previousColumn == -1)
				{
					if(pictureSent->pixels[currentRow][trackDataSN.currentColumn] >= pictureSent->pixels[currentRow][trackDataSN.nextColumn])
					{
						pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
					}
					
					else
					{
						pictureSent->pixels[currentRow][trackDataSN.nextColumn] = 0;
						trackDataSN.previousColumn = trackDataSN.previousColumn + 1;
						trackDataSN.currentColumn = trackDataSN.currentColumn + 1;
						trackDataSN.nextColumn = trackDataSN.nextColumn + 1;
					}
				}
				
				else if(trackDataSN.nextColumn == pictureSent->cols)
				{
					if(pictureSent->pixels[currentRow][trackDataSN.currentColumn] >= pictureSent->pixels[currentRow][trackDataSN.previousColumn])
					{
						//printf("Here\n");
						pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
					}
					
					else
					{
						//printf("Here\n");
						pictureSent->pixels[currentRow][trackDataSN.previousColumn] = 0;
						trackDataSN.previousColumn = trackDataSN.previousColumn - 1;
						trackDataSN.currentColumn = trackDataSN.currentColumn - 1;
						trackDataSN.nextColumn = trackDataSN.nextColumn - 1;
					}
				}
				
				else
				{
					if(pictureSent->pixels[currentRow][trackDataSN.previousColumn] == trackDataSN.highestValue)
					{
						pictureSent->pixels[currentRow][trackDataSN.previousColumn] = 0;
						trackDataSN.previousColumn = trackDataSN.previousColumn - 1;
						trackDataSN.currentColumn = trackDataSN.currentColumn - 1;
						trackDataSN.nextColumn = trackDataSN.nextColumn - 1;
					}
					
					else if(pictureSent->pixels[currentRow][trackDataSN.currentColumn] == trackDataSN.highestValue)
					{
						pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
					}
					
					else
					{
						pictureSent->pixels[currentRow][trackDataSN.nextColumn] = 0;
						trackDataSN.previousColumn = trackDataSN.previousColumn + 1;
						trackDataSN.currentColumn = trackDataSN.currentColumn + 1;
						trackDataSN.nextColumn = trackDataSN.nextColumn + 1;
					}
				}
			
			}
			
			else
			{
				if(pictureSent->pixels[currentRow][trackDataSN.currentColumn] == trackDataSN.highestValue)
				{
					pictureSent->pixels[currentRow][trackDataSN.currentColumn] = 0;
				}
				
				else if(pictureSent->pixels[currentRow][trackDataSN.previousColumn] <= pictureSent->pixels[currentRow][trackDataSN.nextColumn])
				{
					pictureSent->pixels[currentRow][trackDataSN.nextColumn] = 0;
					trackDataSN.previousColumn = trackDataSN.previousColumn + 1;
					trackDataSN.currentColumn = trackDataSN.currentColumn + 1;
					trackDataSN.nextColumn = trackDataSN.nextColumn + 1;
				}
				
				else
				{
					pictureSent->pixels[currentRow][trackDataSN.previousColumn] = 0;
					trackDataSN.previousColumn = trackDataSN.previousColumn - 1;
					trackDataSN.currentColumn = trackDataSN.currentColumn - 1;
					trackDataSN.nextColumn = trackDataSN.nextColumn - 1;
				}
			}
		}
	}
}

void CreatePMGTrail(char dataFileName[], pgmPic* pictureSent, char orientation[])
{
	char rowsString[10];
	char columnsString[10];
	char dataValueString[10];
	int i =0;
	int j =0;
	char* token;
	const char delims[] = {'.'};
	
	int pathWay = -1;
	
	if(strcmp(orientation, "E-W") == 0)
	{
		pathWay = 0;
	}
	
	else if(strcmp(orientation, "W-E") == 0)
	{
		pathWay = 1;
	}
	
	else if(strcmp(orientation, "N-S") == 0)
	{
		pathWay = 2;
	}
	
	else if(strcmp(orientation, "S-N") == 0)
	{
		pathWay = 3;
	}
	
	char pgmName[30];
    token = strtok(dataFileName, delims);
    
    strcpy(pgmName, token);
    
    //strcpy(pgmCopy, pgmName);
    //printf("%s\n", pgmName);
    strcat(pgmName, "-trail.pgm");
    //printf("%s", pgmName);
   
	FILE* outputFile = fopen(pgmName, "w");
	sprintf(columnsString, "%d", pictureSent->cols);
	sprintf(rowsString, " %d\n", pictureSent->rows);
	
	fputs("P2\n", outputFile);
	fputs(columnsString, outputFile);
	fputs(rowsString, outputFile);
	fputs("255\n",outputFile);
	
	switch(pathWay)
	{
		case 0: GoEastToWest(pictureSent);
				break;
		case 1: GoWestToEast(pictureSent);
				break;
		case 2: GoNorthToSouth(pictureSent);
				break;
		case 3: GoSouthToNorth(pictureSent);
				break;
	}
	
	//printf("\n");
	
	for(i = 0; i < pictureSent->rows; i++)
    {
        for(j = 0; j < pictureSent->cols; j++)
        {
        	sprintf(dataValueString, "%d",  pictureSent->pixels[i][j]);
        	strcat(dataValueString, "\t");
        	//printf("%s", dataValueString);
            fputs(dataValueString, outputFile);
            //printf("Executed\n");
        }
        //printf("\n");
        fputs("\n", outputFile);
    }
	
	fclose(outputFile);
}

int main(int argc, char* argv[ ])
{
	int numRows = 0;
	int numColumns = 0;
	int lineNumber = 0;
	pgmPic *myPic;
	int maxValue = 0;
	int minValue = 0;
	int range = 0;
	float scaledRange = 0;
	int i =0;
	int j =0;
	char dataFileNameCopy[30];
	
	strcpy(dataFileNameCopy, argv[1]);
	
	int dimension = 0;
	
	FILE* inputFile;
	
	inputFile = fopen(argv[1] , "r");
	
	if ( ! inputFile )
	{
	 printf("File %s does not exist\n", argv[1]);
	 exit(1);
	}
	
	else if(strcmp(argv[2], "W-E") != 0 && strcmp(argv[2], "E-W") != 0 && strcmp(argv[2], "N-S") != 0 && strcmp(argv[2], "S-N") != 0)
	{
		printf("Error. Incorrect formatting. Use \"W-E\", \"E-W\", \"N-S\" OR \"S-N\"");
	}
	
	else
	{
		fscanf(inputFile, "%d", &dimension);
	
		while(!feof(inputFile))
		{
		  if(lineNumber == 0)
		  {
		    numRows = dimension;
		    lineNumber++;
		
		    fscanf(inputFile, "%d", &dimension);
		  }
		
		  else if(lineNumber == 1)
		  {
		    numColumns = dimension;
		    lineNumber++;
		    int a = 0;
		
		    myPic = (pgmPic*) malloc ( sizeof(pgmPic) );
		    myPic->rows = numRows;
		    myPic->cols = numColumns;
		
		    myPic->pixels = (int **) malloc ( sizeof(int *) * numRows);
		
		    for (a=0; a<myPic->rows; a++)
		    {
		      myPic->pixels[a] = (int *) malloc ( sizeof(int) * numColumns );
		    }
		  }
		
		  else
		  {
		    int i = 0;
		    int j = 0; 
		    
		    for(i = 0; i < numRows; i++)
		    {
		        for(j = 0; j < numColumns; j++)
		        {
		            fscanf(inputFile, "%d", &dimension);
		            myPic->pixels[i][j] = dimension;
		        }
		    }
		  }
		}
	    
	    maxValue = FindMax(myPic);
	    minValue = FindMin(myPic);
	    
	    /*printf("%s\n", argv[1]);
	    printf("Max value = %d\n", maxValue);
	    printf("Min value = %d\n", minValue);*/
	    
	    range = maxValue - minValue;
	    scaledRange = (float) 200 / (float) range;
	    
	    //printf("Scaled range = %f\n", scaledRange);
	    
	    UpdateData(myPic, scaledRange, minValue);
	    
	    CreatePMG(argv[1], myPic);
	    CreatePMGTrail(dataFileNameCopy, myPic, argv[2]);
	}

  return 0;
}
