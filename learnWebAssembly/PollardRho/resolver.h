#include <string>

typedef long long LL;
class Resolver
{
public:
	Resolver();
	~Resolver();
	std::string GetResolvedResult(LL x);

protected:
	void find(LL n,LL c);
	bool Miller_rabin(LL p);
	LL Pollard_rho(LL n,LL c);
	LL quick_mod(LL a,LL p,LL m);
	LL multi(LL a,LL b,LL m);
	LL gcd(LL a,LL b);

	const int N=6e5;
	int cnt;
	LL *fac;
	LL *num;
};