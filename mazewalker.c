/* mazewalker.c
   Program # 1
   CS201 Summer 2020
   Ashton Smith
   The purpose of this program is to create a solution to a maze represented by a 2-dimensional array of characters. 
   The algorithim used to solve the maze will rely on using the right hand side of the wall and has four cases.
   The Cases are as follows:
   1. No right side - turn right and walk one block.
   2. No wall forwards - walk one block.
   3. No left side - turn left and walk one.
   4. Else - 180 degrees and walk one.
   The program will first display the unsolved maze.
   Then once the maze has been solved it will display the solution.*/


#include <stdio.h>
#include <stdlib.h>
#define RED "\x1b[31m"
#define BLACK "\x1b[0m"


/*Prototypes*/
struct point;
int check_argument_count(int argc);
struct maze build_maze();
void display_maze(struct maze to_display);
void destroy(struct maze * to_destroy);
struct maze solve_maze(struct maze to_solve);
void solve_direction_0(struct maze * to_solve, struct point * current, int  * direction);
void solve_direction_1(struct maze * to_solve, struct point * current, int  * direction);
void solve_direction_2(struct maze * to_solve, struct point * current, int  * direction);
void solve_direction_3(struct maze * to_solve, struct point * current, int  * direction);



/*This struct is used to store an x and y coordinate in a maze. The x axis is the column and the y axis is the row.*/
struct point{
	int x;/*X coordinate;*/
	int y;/*Y coordinate;*/
};



/*This struct is used to store data for a maze.*/
struct maze{
	int columns; /*Number of columns in the maze.*/
	int rows; /*Number of rows in the maze.*/
	char ** maze; /*Dynamically allocated array to represent the maze. The size will be determined by the number of columns and rows.*/
	struct point start;/*Start location of the maze.*/
	struct point exit;/*Finish location of the maze.*/
};



/*Main - used to call the functions to construct and solve a maze.*/
int main(int argc, char * argv[]){
	struct maze my_maze;

	if(!check_argument_count(argc))
		return 0;

	my_maze = build_maze(argv[1]);

	if(my_maze.maze == NULL){
		printf("There was an issue loading the maze\n");
		return 0;
	}

	display_maze(my_maze);
	my_maze = solve_maze(my_maze);
	printf("\n\n\n");
	display_maze(my_maze);
	destroy(&my_maze);
	return 0;
}



/*Deallocates the dynamic memory in a maze object. The argument is the maze to deallocate.*/
void destroy(struct maze * to_destroy){
	int rows;
	for(rows = 0; rows < to_destroy -> rows; ++rows){
		free(to_destroy -> maze[rows]);
	}
	free(to_destroy -> maze);
}



/*This function is used to check the argument count and returns 0 if there are too many or too few, and 1 if it has one argument
  The function will also tell the user the proper format if there are the wrong number of arguments*/
int check_argument_count(int argc){
	if(argc == 1){
		printf("No file name provided. Please re-run the program with this format: ./mazewalker filename\n");
		return 0;
	}
	else if(argc > 2){
		printf("Too many arguments. Please re-run the program with this format: ./mazewalker filename\n");
		return 0;
	}
	else
		return 1;
}



/*This function loads data from an external data file into a maze struct and then returns it.*/
struct maze build_maze(char * file_name){
	struct maze new_maze;
	FILE *fp;
	int rows = 0;
	int columns = 0;
	fp = fopen(file_name, "r");
	if(fp == NULL){
		fclose(fp);
		return new_maze;
	}else{
		/*Line one: (Number of columns, Number of rows)*/
		fscanf(fp, "%d", &(new_maze.columns));
		fscanf(fp, ",");
		fscanf(fp, "%d", &(new_maze.rows));
		fscanf(fp, "\n");

		/*Line two: Start point (X, Y)*/
		fscanf(fp, "%d", &(new_maze.start.x));
		fscanf(fp, ",");
		fscanf(fp, "%d", &(new_maze.start.y));
		fscanf(fp, "\n");

		/*Line three: Exit point (X, Y)*/
		fscanf(fp, "%d", &(new_maze.exit.x));
		fscanf(fp, ",");
		fscanf(fp, "%d", &(new_maze.exit.y));
		fscanf(fp, "\n");

		/*Line four+: Maze data*/
		new_maze.maze = malloc(sizeof(char*) * new_maze.rows);
		for(rows = 0; rows < new_maze.rows; ++rows){
			new_maze.maze[rows] = malloc(new_maze.columns * sizeof(char));
			for(columns = 0; columns < new_maze.columns; ++columns)
				fscanf(fp, "%c", &(new_maze.maze[rows][columns]));
			fscanf(fp, "\n");
		}
	}
	fclose(fp);
	return new_maze;
}



/*This function displays a maze. The maze displayed is the argument.*/
void display_maze(struct maze to_display){
	int columns = 0;	
	int rows = 0;

	for(rows = 0; rows < to_display.rows; ++rows){
		for(columns = 0; columns < to_display.columns; ++columns){
			if(to_display.maze[rows][columns] == 'S'){
				printf(RED);		
			}
			printf("%c  ", to_display.maze[rows][columns]);
			printf(BLACK);
		}
		printf("\n");
	}
}



/*This function is used to solve the maze. The argument is the maze to solve and then it returns the solved maze.
  The solved maze will have a path of 'W's on the path taken. The start point will be a W and the end point will be
  an E.*/
struct maze solve_maze(struct maze to_solve){
	struct point current;
	int count = 0;
	int loop = 1;
	char display = 'y';
	int direction = 2;
	/* Directions: 0 == North, 1 == East, 2 == South, 3 == West*/
	
	current.x = to_solve.start.x;
	current.y = to_solve.start.y;

	do{
		printf("\n\nWould you like to display all steps or just the solution? Enter y for all steps or n for solution only.\n");
		display = getchar();
		while((getchar() != '\n'));
	}while(display != 'y' && display != 'n');

	if(display == 'y'){
		do{

			printf("\n\nAre you sure that you want to display every step? This may take a while for large mazes. Enter y for yes or n for no.\n");
			display = getchar();
			while((getchar() != '\n'));
		}while(display != 'y' && display != 'n');
	}

	to_solve.maze[current.y][current.x] = 'S';

	do{
		to_solve.maze[current.y][current.x] = 'S';
		/*Facing north*/
		if(direction == 0){	
			solve_direction_0(&to_solve, &current, &direction);
		}
		/*Facing east*/
		else if(direction == 1){	
			solve_direction_1(&to_solve, &current, &direction);
		}
		/*Facing south*/
		else if(direction == 2){	
			solve_direction_2(&to_solve, &current, &direction);
		}
		/*Facing west*/
		else if(direction == 3){	
			solve_direction_3(&to_solve, &current, &direction);
		}

		to_solve.maze[current.y][current.x] = 'C';
		++count;

		/*Display maze at each step*/
		if(display == 'y'){
			printf("Step %d. \n", count);
			printf("Exit Location:");
			printf("%d ,", to_solve.exit.x);
			printf("%d \n", to_solve.exit.y);
			printf("Current Location:");
			printf("%d ,", current.x); 
			printf("%d \n", current.y); 
			display_maze(to_solve);
			printf("\n\n");
		}

		if(current.x == to_solve.exit.x && current.y == to_solve.exit.y){
			--loop;
		}

	}while(loop);
	printf("X - Wall, S - Stepped, C - Current location, 0 - Not stepped, E - Exit\n");
	printf("Maze Solved in %d steps", count);
	to_solve.maze[current.y][current.x] = 'E';
	return to_solve;
}



/*Checks each case for facting North, then the function updates the current position and direction. */
void solve_direction_0(struct maze * to_solve, struct point * current, int * direction){
	/*Case number one: No wall to the right of the person - Turn the person right and walk one block */
	if(current -> x < to_solve -> columns -1 && to_solve -> maze[current -> y][current -> x +1] != 'X'){
		++current -> x;
		++*direction;
	}
	/*Case number two: no wall forwards - Walk one block.*/
	else if(current -> y - 1 >= 0 && to_solve -> maze[current -> y - 1][current -> x] != 'X'){
		--current -> y;
	}
	/*Case Three: No block to the left - Turn left and walk one.*/
	else if(current -> x - 1 >= 0 && to_solve -> maze[current -> y][current -> x -1] != 'X'){
		--current -> x;
		*direction +=3;
	}
	/*Case four: turn 180 degrees and walk one.*/
	else{
		*direction += 2;
		++current -> y;
	}
}



/*Checks each case for facting East, then the function updates the current position and direction. */
void solve_direction_1(struct maze * to_solve, struct point * current, int * direction){
	/*Case number one: No wall to the right of the person - Turn the person right and walk one block */
	if(current -> y < to_solve ->rows -1 && to_solve -> maze[current -> y + 1][current -> x] != 'X'){
		++current -> y;
		++*direction;
	}
	/*Case number two: no wall forwards - Walk one block.*/
	else if(current -> x < to_solve -> columns -1 && to_solve -> maze[current -> y][current -> x + 1] != 'X'){
		++current -> x;
	}
	/*Case Three: No block to the left - Turn left and walk one.*/
	else if(current -> y - 1 >= 0 && to_solve -> maze[current -> y -1][current -> x] != 'X'){
		--current -> y;
		--*direction;
	}
	/*Case four: turn 180 degrees and walk one.*/
	else{
		*direction += 2;
		--current -> x;
	}
}



/*Checks each case for facting South, then the function updates the current position and direction. */
void solve_direction_2(struct maze * to_solve, struct point * current, int * direction){
	/*Case number one: No wall to the right of the person - Turn the person right and walk one block */
	if(current -> x - 1 >= 0 && to_solve -> maze[current -> y][current -> x - 1] != 'X'){
		--current -> x;
		++*direction;
	}
	/*Case number two: no wall forwards - Walk one block.*/
	else if(current -> y < to_solve -> rows -1 && to_solve -> maze[current -> y + 1][current -> x] != 'X'){
		++current -> y;
	}
	/*Case Three: No block to the left - Turn left and walk one.*/
	else if(current -> x + 1 >= 0 && to_solve -> maze[current -> y][current -> x + 1] != 'X'){
		++current -> x;
		--*direction;
	}
	/*Case four: turn 180 degrees and walk one.*/
	else{
		*direction -= 2;
		--current -> y;
	}
}



/*Checks each case for facting West, then the function updates the current position and direction. */
void solve_direction_3(struct maze * to_solve, struct point * current, int  * direction){
	/*Case number one: No wall to the right of the person - Turn the person right and walk one block */
	if(current -> y - 1 >= 0 && to_solve -> maze[current -> y - 1][current -> x] != 'X'){
		--current -> y;
		*direction -= 3;
	}
	/*Case number two: no wall forwards - Walk one block.*/
	else if(current -> x - 1 >= 0 && to_solve -> maze[current -> y][current -> x - 1] != 'X'){
		--current -> x;
	}
	/*Case Three: No block to the left - Turn left and walk one.*/
	else if(current -> y < to_solve -> rows -1 && to_solve -> maze[current -> y + 1][current -> x] != 'X'){
		++current -> y;
		--*direction;
	}
	/*Case four: turn 180 degrees and walk one.*/
	else{
		*direction -= 2;
		++current -> x;
	}
}
