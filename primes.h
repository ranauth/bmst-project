int* erathostene(int n, int* nb_primes); //returns the n first prime numbers using the sieve of erathostenes, and sets *nbrPrimes=pi(n)
int gen_prime(int low_bound, int up_bound);
bool witness(int n, int mant, int exp, int base);
bool miller_rabin(int n, int nb_base);
bool miller_rabin_deterministic(int n); // Works for n<2^64
bool fermat(int n, int nb_test);
bool naive_primality_base(int n, int *base, int base_size);
bool naive_primality(int n);
int fermat_factor(int N);
int rho_pollard(int n);


// unsigned long long gen_strong_prime(int low_bound, int up_bound); // Gordon's algorithm
// unsigned long long exp_mod_long(unsigned long long n, unsigned long long exp, unsigned long long mod);
// bool witness_long(unsigned long long n, unsigned long long mant, unsigned long long exp, int base);
// bool miller_rabin_deterministic_long(unsigned long long n);