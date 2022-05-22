#include <sys/random.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "misc.h"

int sqrti(int n){  //newton's method, returns ceil(sqrt(n))
    int x = n/2;
    if (x==0)
    	return n;
    int y = (x+n/x)/2;		
	while (y<x){
		x = y;
		y = (x+n/x)/2;
	}
	return x;
}

int log2_estimate(int n){
	int shift = 31-__builtin_clz(n);
	return shift+((n>>(shift-1))&1);
}

int exp_mod_slow(int n, int exp, int mod){// returns n^exp [mod]
	int res;
	for(res=1; exp>0; --exp)
		res = (res*n)%mod;
	return res;
}

int exp_mod(int n, int exp, int mod){
	int res = 1;
	while (exp>0){
		if (exp&1)
			res = (res*n)%mod;
    	exp >>= 1;
    	n = (n*n)%mod;
	}
	return res;
}

int is_quad_res(int n, int p){ // returns true if n is a quadratic residue mod p using euler criterion. NEEDS p odd AND gcd(n,p)=1
	int res = exp_mod(n,(p-1)/2,p);
	return ((res==1)||(res==1-p));
}

int gen_ran_int(int low_bound, int up_bound){
	int res;
	if (getrandom(&res, sizeof(int), 0)<=0){
		printf("ERR: INSUFFICIENT ENTROPY\n");
		exit(0);
	}
	res = (res<0) ? -res : res;
	return low_bound + (int)( res / (INT_MAX / (double)(up_bound - low_bound + 1.0) + 1.0));
}

int gcd_bin(int a, int b){ // PGCD algorithme binaire (le half gcd est plus rapide je crois)
	int g, tmp;
	g = (__builtin_ctz(a)<__builtin_ctz(b)) ? __builtin_ctz(a) : __builtin_ctz(b); 
	a >>= g;
	b>>= g;
	while(1){
		if (a>b){
			tmp = a;
			a = b;
			b = tmp;
		}
		b -= a;
		if (!b)
			return a<<g;
		b >>= __builtin_ctz(b);
	}
}

int gcd_extended(int x, int y, int *a, int *b){ //returns gcd(a,b)=au+bv (Binary extended cgd algorithm)
	int g;
	*a = __builtin_ctz(x);
	*b = __builtin_ctz(y);
	g = (*a<*b) ? *a : *b; 
	x >>= g;
	y >>= g;
	g = 1 << g;
	int u =x, v=y, A = 1, B = 0, C = 0, D = 1;

	while(u){
		while (!(u&1)){
			u >>= 1;
			if (!(A&1) && !(B&1)){
				A >>= 1;
				B >>= 1;
			}
			else{
				A = (A+y)/2;
				B = (B-x)/2;
			}
		}

		while (!(v&1)){
			v >>= 1;
			if (!(C&1) && !(D&1)){
				C >>= 1;
				D >>= 1;
			}
			else{
				C = (C+y)/2;
				D = (D-x)/2;
			}
		}

		if(u>=v){
			u = u-v;
			A = A-C;
			B = B-D;
		}
		else{
			v = v-u;
			C = C-A;
			D = D-B;
		}
	}

	*a = C;
	*b = D;
	return g*v;
}

int inv_mod(int n, int m){
	int res, tmp;
	if(gcd_extended(n, m, &res, &tmp)!=1){
		printf("not coprime");
		exit(0);
	}
	return res;
}

int is_square(int n){
	int r;
	r = __builtin_ctz(n);
	if (r%2)
		return false;
	n >>= r;
	r = sqrti(n);
	return n == r*r; // Optimisable en réimplémentant ici la méthode d'Héron ce qui évite de mettre au carré 
}