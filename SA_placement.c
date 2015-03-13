#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include"SA_helper.h"


void main(int argc , char* argv[])
{
	
	int module_count=0;
	struct module_dim **module_array;
	struct module_dim **module_array_temp;
	struct module_dim *temp_module;
	struct module_dim *free_module;
	struct module *temp_modules;
	int i;
	int *polish_exp;
	int verticle, horizontal;
	unsigned int init_temp = 50000;		
	struct cost cost;
	struct cost cost_new;
	int *polish_exp_new;
	int delta_area;
	double random;
	double param_exp;
	
	/*pseudo-random number generator is initialized using time feed*/
	srand( (unsigned)time( NULL ) ); 
	/*design file parser*/
	module_count = parse_design(argv[1], &module_array);
	/*vertical and horizontal section identify with number*/
	verticle = module_count + 1;
	horizontal = module_count + 2;
	/*random polish expression generator*/
	polish_exp = getRandom(module_count);
	/*performing sizing slicing on floorplan generated by polish expression*/
	cost = sizing_slicing(module_array, polish_exp,module_count);
	/* SA algorithm Starts */
	while(init_temp > 0){
		for(i=0; i<80; i++){
		polish_exp_new = getRandom(module_count);
		cost_new = sizing_slicing(module_array, polish_exp_new,module_count);
		delta_area = cost_new.area - cost.area;
		/* generats random values between 0.1 and 0.9*/ 
		random = (rand() % 9) / 10;
		/* Acceptance probability calculation */
		param_exp = (delta_area/init_temp);
		/*If new cost is small */
		if(delta_area < 0){
			//free(polish_exp);
			polish_exp = polish_exp_new;
			cost = cost_new;
		}
		/*Accept with probability*/
		else if(random > exp(-(param_exp))){
			//free(polish_exp);
			polish_exp = polish_exp_new;
			cost = cost_new;
		}
		//free(polish_exp_new);
		}
		init_temp--;
	}
	/* SA algorith finish */
	printf("\ncost after SA\n");
	printf("\ncost.area = %d\n", cost.area);

	temp_modules = cost.final_modules;
	printf("\nfinal modules\n");
	for(i=0; i<module_count; i++){
		printf("(%d, %d)\t", temp_modules[i].module, temp_modules[i].size);
	}
	printf("\n");
	/*generates output desig file */
	optimal_design(module_count, cost, module_array,polish_exp);

	/*free allocated dynamic memory*/
	module_array_temp = module_array;
	for(i=0; i<module_count; i++){
		temp_module = module_array_temp[i];
		while(temp_module != NULL){
			free_module = temp_module;			
			temp_module = temp_module->next;
			free(free_module);
		}
	}

	free(module_array);
}

