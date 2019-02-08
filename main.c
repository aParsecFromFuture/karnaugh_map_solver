#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

typedef struct karnaugh karnaugh;

struct karnaugh{
	int inp_count;
	int width;
	int height;
	int **map;
};

karnaugh* create_karnaugh(int);
void set_data(karnaugh*,int,int,int);
void set_data_area(karnaugh*,int,int,int,int,int);
void solve(karnaugh*);
bool is_filled(karnaugh*,int,int,int,int);
int* get_inputs(karnaugh*,int,int);
int* get_formula(karnaugh*,int,int,int,int);

int main(){
	karnaugh *k = create_karnaugh(4);
	set_data(k,0,0,1);
	set_data(k,1,0,1);
	set_data(k,0,1,1);
	set_data(k,1,1,1);
	solve(k);
	
}

karnaugh* create_karnaugh(int inp_count){
	karnaugh *k = malloc(sizeof(karnaugh));
	k->inp_count = inp_count;
	k->width = pow(2,inp_count / 2 + inp_count % 2);
	k->height = pow(2,inp_count / 2);

	k->map = malloc(sizeof(int*) * k->width);
	for(int i = 0;i < k->width;i++){
		k->map[i] = calloc(k->height,sizeof(int));
	}

	return k;
}

void set_data(karnaugh *k,int x1,int y1,int v){
	k->map[x1][y1] = v;
}

void set_data_area(karnaugh *k,int x1,int y1,int w,int h,int v){
	if(x1 >= 0 && y1 >= 0 && x1 < k->width && y1 < k->height){
		for(int i = x1;i < (x1 + w);i++){
			for(int j = y1;j < (y1 + h);j++){
				k->map[i % k->width][j % k->height] = v;
			}
		}
	}
}

void solve(karnaugh *k){
	int area_w,area_h;
	bool is_first = 1;
	bool is_area_filled_zero = 1;
	bool is_area_filled_one = 1;
	for(int i = 0;i < k->width;i++){
		for(int j = 0;j < k->height;j++){
			if(k->map[i][j]){
				is_area_filled_zero = 0;
			}else{
				is_area_filled_one = 0;
			}
		}
	}

	if(is_area_filled_zero){
		printf("0");
		return;
	}

	if(is_area_filled_one){
		printf("1");
		return;
	}

	for(int i = 0;i < k->width;i++){
		if((k->width - i) % 2 != 0 && (k->width - i) != 1){
			continue;
		}
		for(int j = 0;j < k->height;j++){
			if((k->height - j) % 2 != 0 && (k->height - j) != 1){
				continue;
			}

			area_w = k->width - i;
			area_h = k->height - j;
			for(int m = 0;m < k->width;m++){
				for(int n = 0;n < k->height;n++){
					if(is_filled(k,m,n,area_w,area_h)){
						if(!is_first){
							printf(" v ");
						}
						int *inputs = get_formula(k,m,n,area_w,area_h);
						for(int o = 0;o < k->inp_count;o++){
							if(inputs[o] == 0){
								printf("x%d'",o + 1);
							}else if(inputs[o] == 1){
								printf("x%d",o + 1);
							}
						}
						set_data_area(k,m,n,area_w,area_h,-1);
						is_first = 0;
					}
				}
			}
		}
	}
}

bool is_filled(karnaugh *k,int x,int y,int w,int h){
	bool flag = 0;
	if(x > k->width || y > k->height || x < 0 || y < 0){
		return false;
	}else{
		for(int i = x;i < (x + w);i++){
			for(int j = y;j < (y + h);j++){
				if(k->map[i % k->width][j % k->height] == 1){
					flag = 1;
				}
				
				if(k->map[i % k->width][j % k->height] == 0){
					return false;
				}
			}
		}
		if(flag){
			return true;
		}else{
			return false;
		}
	}
}

int* get_inputs(karnaugh *k,int x1,int y1){
	int *inputs = malloc(sizeof(int) * k->inp_count);
	int input_count_width = log2(k->width);
	int input_count_height = log2(k->height);
	for(int i = 0;i < input_count_height;i++){
		inputs[i] = (int)((y1 + pow(2,input_count_height - i - 1)) / pow(2,input_count_height - i)) % 2;
	}
	for(int i = 0;i < input_count_width;i++){
		inputs[i + input_count_height] = (int)((x1 + pow(2,input_count_width - i - 1)) / pow(2,input_count_width - i)) % 2;
	}
	return inputs;
}

int* get_formula(karnaugh *k,int x1,int y1,int w,int h){
	int *inputs_cmp = calloc(k->inp_count,sizeof(int));
	int *inputs = malloc(sizeof(int) * k->inp_count);
	for(int i = x1;i < x1 + w;i++){
		for(int j = y1;j < y1 + h;j++){
			inputs = get_inputs(k,i % k->width,j % k->height);
			for(int m = 0;m < k->inp_count;m++){
				inputs_cmp[m] += (inputs[m])? 1 : -1;
			}	
		}
	}
	for(int i = 0;i < k->inp_count;i++){
		if(inputs_cmp[i] == 0){
			inputs_cmp[i] = -1;
		}else if(inputs_cmp[i] > 0){
			inputs_cmp[i] = 1;
		}else{
			inputs_cmp[i] = 0;
		}
	}
	return inputs_cmp;
}
