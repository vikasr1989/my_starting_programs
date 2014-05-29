#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SUCCESS 0
#define FAIL    -1

//#define DEBUG
#define LOGD //printf
#define LOGE printf


void usage(char *progname)
{
	LOGE("Usage is %s <no_of_rows or columns> <no_of_mines>\n",progname);
}

void open_surrounding_pixels(char *out,char *game,int row_num,int col_num,int rows, int columns)
{
	int i = row_num;
	int j = col_num;
	{
		int temp_mine_count = 0;
		if(i-1 >=0 && j-1 >=0)
			if(out[(i-1)*columns + j-1] != 0)
			{
				game[(i-1)*columns + j-1] = out[(i-1)*columns + j-1];
				temp_mine_count +=1;
			}
			else
			{
				game[(i-1)*columns + j-1] = out[(i-1)*columns + j-1];
				//open_surrounding_pixels(out,game,i-1,j-1,rows,columns);
			}
		if(i >=0 && j-1 >=0)
			if(out[(i)*columns + j-1] != 0)
			{
				game[(i)*columns + j-1] = out[(i)*columns + j-1];
				temp_mine_count +=1;
			}
			else
			{
				game[(i)*columns + j-1] = out[(i)*columns + j-1];
				//open_surrounding_pixels(out,game,i,j-1,rows,columns);
			}

		if(i+1 < rows && j-1 >=0)
			if(out[(i+1)*columns + j-1] != 0)
			{
				game[(i+1)*columns + j-1] = out[(i+1)*columns + j-1];
				temp_mine_count +=1;
			}
			else
			{
				game[(i+1)*columns + j-1] = out[(i+1)*columns + j-1];
				//open_surrounding_pixels(out,game,i+1,j-1,rows,columns);
			}

		if(i-1 >=0 && j >=0)
			if(out[(i-1)*columns + j] != 0)
			{
				game[(i-1)*columns + j] = out[(i-1)*columns + j];
				temp_mine_count +=1;
			}
			else
			{
				game[(i-1)*columns + j] = out[(i-1)*columns + j];
				//open_surrounding_pixels(out,game,i-1,j,rows,columns);
			}

		if(i+1 < rows && j >=0)
			if(out[(i+1)*columns + j] != 0)
			{
				game[(i+1)*columns + j] = out[(i+1)*columns + j];
				temp_mine_count +=1;
			}
			else
			{
				game[(i+1)*columns + j] = out[(i+1)*columns + j];
				//open_surrounding_pixels(out,game,i+1,j,rows,columns);
			}

		if(i-1 >=0 && j+1 < columns)
			if(out[(i-1)*columns + j+1] != 0)
			{
				game[(i-1)*columns + j+1] = out[(i-1)*columns + j+1];
				temp_mine_count +=1;
			}
			else
			{
				game[(i-1)*columns + j+1] = out[(i-1)*columns + j+1];
				//open_surrounding_pixels(out,game,i-1,j+1,rows,columns);
			}

		if(i >=0 && j + 1 < columns)
			if(out[(i)*columns + j+1] !=  0)
			{
				game[(i)*columns + j+1] = out[(i)*columns + j+1];
				temp_mine_count +=1;
			}
			else
			{
				game[(i)*columns + j+1] = out[(i)*columns + j+1];
				//open_surrounding_pixels(out,game,i,j+1,rows,columns);
			}

		if(i+1 < rows && j+1 < columns)
			if(out[(i+1)*columns + j+1] != 0)
			{
				game[(i+1)*columns + j+1] = out[(i+1)*columns + j+1];
				temp_mine_count +=1;
			}
			else
			{
				game[(i+1)*columns + j+1] = out[(i+1)*columns + j+1];
				//open_surrounding_pixels(out,game,i+1,j+1,rows,columns);
			}

		//LOGD("temp_mine_count %d\n",temp_mine_count);
//				out[i*columns + j] = temp_mine_count;
	}
	return;
}
void print_data(char *out,int rows, int columns)
{
    int i,j;
    printf("\t");
    for(j=0;j<columns;j++)
    {
        printf("%d\t",j);
    }
    printf("\n");
    printf("\n");
    

    for(i=0;i<rows;i++)
    {
        printf("%d | \t",i);
        for(j=0;j<columns;j++)
        {
            if(out[i*columns + j] == 'X')
                printf("X\t",out[i*columns + j]);
            else if(out[i*columns + j] < 10)
                printf("%d\t",out[i*columns + j]);
            else if(out[i*columns + j] == 'M')
                printf("M\t",out[i*columns + j]);
            else if(out[i*columns + j] == 'F')
                printf("F\t",out[i*columns + j]);
            else
                printf("\t",out[i*columns + j]);
        }
        printf("\n");
        printf("\n");
    }

}
int play_game(char *out ,char *game,int rows, int columns)
{
	int i;
	int click=-1,row_num=0,col_num=0;
    char c;
#if 0
	/* Following code added to ensure that first one is not a mine */
	while(i<=0)
	{
		print_data(game,rows,columns);
		printf("Enter the next move\n");
		printf("1 for left click and 2 for right click followed by row number and then column number starting from 0 to (max-1)\n");
		scanf("%d",&click);
		scanf("%d",&row_num);
		scanf("%d",&col_num);
		printf("Click %d\t row_num %d\t col_num %d\n",click,row_num,col_num);
		if(row_num >= rows || row_num < 0)
		{
			LOGE("Invalid row_num\nPlease try again\n");
			i--;
			continue;
		}
		if(col_num >= columns || col_num < 0)
		{
			LOGE("Invalid col_num\nPlease try again\n");
			i--;
			continue;
		}
		if(click == 1)
		{
			game[row_num*columns + col_num] = out[row_num*columns + col_num];
			if(game[row_num*columns + col_num] == 0)
			{
				//open_surrounding_pixels(out,game,row_num,col_num,rows,columns);
			}
			if(game[row_num*columns + col_num] == 'M')
			{
				mine_index = rand() % (rows * columns);
				LOGD("Mine positions %d\n",mine_index);
				if(mine_index < (rows * columns))
				{
					LOGD("here\n");
					/* Check whether the newly generated number is already a mine, if not then make it a mine */
					while(out[mine_index] == 'M')
					{
						mine_index = rand() % (rows * columns);
					}
					{
						out[mine_index] = 'M';
					}
				}
//TODO Change present number and compute it for the new mine surrounding also
				{
					ii = row_num;
					j = col_num;
				temp_mine_count = 0;
				if(ii-1 >=0 && j-1 >=0)
					if(out[(ii-1)*columns + j-1] == 'M')
					{
						temp_mine_count +=1;
					}
				if(ii >=0 && j-1 >=0)
					if(out[(ii)*columns + j-1]== 'M')
					{
						temp_mine_count +=1;
					}
				if(ii+1 < rows && j-1 >=0)
					if(out[(ii+1)*columns + j-1] == 'M')
					{
						temp_mine_count +=1;
					}
				if(ii-1 >=0 && j >=0)
					if(out[(ii-1)*columns + j] == 'M')
					{
						temp_mine_count +=1;
					}
				if(ii+1 < rows && j >=0)
					if(out[(ii+1)*columns + j] == 'M')
					{
						temp_mine_count +=1;
					}
				if(ii-1 >=0 && j+1 < columns)
					if(out[(ii-1)*columns + j+1] == 'M')
					{
						temp_mine_count +=1;
					}
				if(ii >=0 && j + 1 < columns)
					if(out[(ii)*columns + j+1] ==  'M')
					{
						temp_mine_count +=1;
					}
				if(ii+1 < rows && j+1 < columns)
					if(out[(ii+1)*columns + j+1] == 'M')
					{
						temp_mine_count +=1;
					}
		//LOGD("temp_mine_count %d\n",temp_mine_count);
				out[ii*columns + j] = temp_mine_count;
			}
			}
						
		}
		i++;	
	}
#endif

	for(i=0;i<(rows*columns);i++)
	{
		print_data(game,rows,columns);
		printf("Enter the next move\n");
		printf("1 for left click and 2 for right click followed by row number and then column number starting from 0 to (max-1)\n");
		scanf("%d",&click);
		scanf("%d",&row_num);
		scanf("%d",&col_num);
        /* This is to ensure no extra arguments are read */
        while((c = getchar()) != '\n' && c != EOF)
        {
        }
        
		printf("Click %d\t row_num %d\t col_num %d\n",click,row_num,col_num);
		if(row_num >= rows || row_num < 0)
		{
			LOGE("Invalid row_num\nPlease try again\n");
			i--;
			continue;
		}
		if(col_num >= columns || col_num < 0)
		{
			LOGE("Invalid col_num\nPlease try again\n");
			i--;
			continue;
		}
		if(click == 1)
		{
			if(game[row_num*columns + col_num] != 'X')
			{
				if(game[row_num*columns + col_num] != 'F')
				{
					printf("Already clicked\n");
					i--;
					continue;
				}
				if(game[row_num*columns + col_num] == 'F')
				{
					i--;
				}
			}
			game[row_num*columns + col_num] = out[row_num*columns + col_num];
			if(game[row_num*columns + col_num] == 0)
			{
				//open_surrounding_pixels(out,game,row_num,col_num,rows,columns);
			}
			if(game[row_num*columns + col_num] == 'M')
			{
				printf("Mine found... Game over\n");
				print_data(out,rows,columns);
				return FAIL;
			}
						
		}
		else if(click == 2)
		{
			if(game[row_num*columns + col_num] == 'F')
			{
				printf("Already Flagged\n");
				i--;
				continue;
			}
			game[row_num*columns + col_num] = 'F';
		}
		else
		{
			LOGE("Invalid button for click...\n");
			return FAIL;
		}
	}
	print_data(out,rows,columns);
	return SUCCESS;
}

int main(int argc, char *argv[])
{
	char *game;
	char *out;
	int columns,rows,mines;
	int i,j;
	int option=0;
	int mine_index;
	int temp_mine_count=0;
	if(argc != 3)
	{
		usage(argv[0]);
		return FAIL;
	}
	columns = rows = atoi(argv[1]);
	mines = atoi(argv[2]);
	printf("Number of rows %d\nNumber of columns %d\nNumber of mines %d\n",columns,rows,mines);
    if(columns > 20)
    {
		LOGE("Too many columns and rows (greater than 20) \n");
		return FAIL;
	}
        
	if(mines == 0)
	{
		LOGE("Zero mines present \n");
		return FAIL;
	}
    
	if(mines >= 60*rows*columns/100)
	{
		LOGE("Too many mines requested (More than 60 percent ) \n");
		return FAIL;
	}
		srand(time(&j));
	out = (char *)(calloc(rows*columns,1));
	if(out == NULL)
	{
		LOGE("Error allocationg memory\n");
		return FAIL;
	}
	game = (char *)(calloc(rows*columns,1));
	if(game == NULL)
	{
		LOGE("Error allocationg memory\n");
		return FAIL;
	}
	memset(game,'X',rows*columns);
	//print_data(out,rows,columns);
	for(i=0,j=i;i<mines;i++)
	{
		mine_index = rand() % (rows * columns);
#ifdef DEBUG
		if(i==9)
			mine_index = 50;
#endif
		LOGD("Mine positions %d\n",mine_index);
		if(mine_index < (rows * columns))
		{
			LOGD("here\n");
			if(out[mine_index] == 'M')
			{
				i--;
			}
			else
			{
				out[mine_index] = 'M';
				temp_mine_count++;
			}
		}
	}
	//print_data(out,rows,columns);
	if(temp_mine_count != mines)
	{
		LOGE("Could not create game with %d mines\nExiting....\n",mines);
		return FAIL;
	}
	temp_mine_count = 0;
	for(i=0;i<rows;i++)
	{
		for(j=0;j<columns;j++)
		{
			if(out[i*columns + j] != 'M')
			{
				temp_mine_count = 0;
				if(i-1 >=0 && j-1 >=0)
					if(out[(i-1)*columns + j-1] == 'M')
					{
						temp_mine_count +=1;
					}
				if(i >=0 && j-1 >=0)
					if(out[(i)*columns + j-1]== 'M')
					{
						temp_mine_count +=1;
					}
				if(i+1 < rows && j-1 >=0)
					if(out[(i+1)*columns + j-1] == 'M')
					{
						temp_mine_count +=1;
					}
				if(i-1 >=0 && j >=0)
					if(out[(i-1)*columns + j] == 'M')
					{
						temp_mine_count +=1;
					}
				if(i+1 < rows && j >=0)
					if(out[(i+1)*columns + j] == 'M')
					{
						temp_mine_count +=1;
					}
				if(i-1 >=0 && j+1 < columns)
					if(out[(i-1)*columns + j+1] == 'M')
					{
						temp_mine_count +=1;
					}
				if(i >=0 && j + 1 < columns)
					if(out[(i)*columns + j+1] ==  'M')
					{
						temp_mine_count +=1;
					}
				if(i+1 < rows && j+1 < columns)
					if(out[(i+1)*columns + j+1] == 'M')
					{
						temp_mine_count +=1;
					}
		//LOGD("temp_mine_count %d\n",temp_mine_count);
				out[i*columns + j] = temp_mine_count;
			}
		}
	}
	//print_data(out,rows,columns);

	printf("Do you want to play minesweeper?\n");
	printf("Press 1 to play or any other button to exit\n");
	scanf("%d",&option);
	if(option != 1)
	{
		LOGE("Exiting... Thank you\n");
		return SUCCESS;
	}
	printf("Starting game\n");
	if(play_game(out,game,rows,columns))
	{
		LOGE("Exiting\n");
		return FAIL;
	}
	else
	{
		printf("You won the game congrats\n");
	}
	return 0;
}

