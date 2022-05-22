int sqrti(int n);  //newton's method, returns floor(sqrt(n))
int log2_estimate(int n);
int exp_mod_slow(int n, int exp, int mod);
int exp_mod(int n, int exp, int mod);
int is_quad_res(int n, int p); // returns true if n is a quadratic residue mod p using euler criterion. NEEDS p odd AND gcd(n,p)=1
int gen_ran_int(int low_bound, int up_bound);
int gcd_extended(int x, int y, int *a, int *b); //returns gcd(a,b)=au+bv (Binary extended cgd algorithm)
int inv_mod(int n, int m);
int is_square(int n);
int gcd_bin(int a, int b);