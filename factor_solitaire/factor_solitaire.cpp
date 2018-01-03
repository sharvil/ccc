#include <bitset>
#include <cstdio>
#include <iostream>

// 2236 = square root of 5000000 rounded down
typedef std::bitset<2236> primes_t;

// Marks all prime numbers in the passed in bitset.
void mark_primes(primes_t &primes) {
  primes.set();
  primes.set(0, false);
  primes.set(1, false);

  for (int i = 2; i < primes.size(); ++i)
    if (primes[i])
      for (int j = i * i; j < primes.size(); j += i)
        primes[j] = false;
}

// Returns the smallest divisor of n that is not 1.
int find_first_divisor(int n, const primes_t &primes) {
  for (int i = 2; i < primes.size(); ++i)
    if (primes[i] && (n % i) == 0)
      return i;

  return n;
}

int compute_cost(int n, const primes_t &primes) {
  int cost = 0;
  while (n > 1) {
    int b = find_first_divisor(n, primes);
    int a = n / b;
    n -= a;
    cost += n / a;
  }
  return cost;
}

int main(int argc, char **argv) {
  int n;
  std::cin >> n;

  primes_t primes;
  mark_primes(primes);
  std::cout << compute_cost(n, primes) << std::endl;

  return 0;
}
