#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <algorithm>
#include <cstring>

#include "resolver.h"

Resolver::Resolver()
{
	cnt = 0;
	fac = new LL[N];
	num = new LL[N];
}

Resolver::~Resolver()
{
    delete [] fac;
    delete [] num;
}

std::string Resolver::GetResolvedResult(int x)
{
	char str[10005] = { 0 };
	cnt=0;
    find(x,120);
    std::sort(fac,fac+cnt);
    memset(num,0,sizeof(LL)*N);
    memset(fac,0,sizeof(LL)*N);
    int k=0;
    for(int i=0;i<cnt;i++){
        if(i==0){
            num[k]++;
            continue;
        }

        if(fac[i]==fac[i-1]){
            num[k]++;
        }
        else {
            k++;
            fac[k]=fac[i];
            num[k]++;
        }
    }
    for(int i=0;i<k;i++){
    	if( num[i] != 1 )
    	{
	        sprintf(str + strlen(str), "%lld^{%lld} \\times ",fac[i], num[i]);
    	}
    	else {
	    	sprintf(str + strlen(str), "%lld \\times ",fac[i]);	
    	}
    }
    if( num[k] != 1 )
    {
	    sprintf(str + strlen(str), "%lld^{%lld}", fac[k], num[k]);
    }
    else {
	    sprintf(str + strlen(str), "%lld", fac[k]);
    }
    return std::string( str );
}

void Resolver::find(LL n,LL c)
{
	if(n==1) return ;
    if(Miller_rabin(n)){
        fac[cnt++]=n;
        return ;
    }
    LL p=n;
    while(p>=n) p=Pollard_rho(p,c--);
    find(p,c);
    find(n/p,c);
}

bool Resolver::Miller_rabin(LL p)
{
	if(p==2) return 1;
    if(p<2 || (p&1)==0) return 0;
    LL m=p-1;
    int sum=0;
    while((m&1)==0){
        m>>=1;
        sum++;
    }
    for(int i=0;i<10;i++){
        LL a=rand()%(p-1)+1;
        LL x=quick_mod(a,m,p);
        LL g=0;
        for(int j=0;j<sum;j++){
            g=multi(x,x,p);
            if(g==1&&x!=1&&x!=p-1)  return 0;
            x=g;
        }
        if(g!=1) return 0;
    }
    return 1;
}

LL Resolver::Pollard_rho(LL n,LL c)
{
	LL x,y,k=2,i=1;
    x=rand()%(n-1)+1;
    y=x;
    while(1>0){
        i++;
        x=(multi(x,x,n)+c)%n;
        LL d=gcd((y-x+n)%n,n);
        if(1<d&&d<n) return d;
        if(y==x)  return n;
        if(i==k) {
            y=x;
            k<<=1;
        }
    }
}

LL Resolver::quick_mod(LL a,LL p,LL m)
{
	LL ans=1;
    while(p){
        if(p&1) ans=multi(ans,a,m);
        a=multi(a,a,m);
        p>>=1;
    }
    return ans;
}

LL Resolver::multi(LL a,LL b,LL m)
{
	LL ans=0;
    while(b){
        if(b&1) ans=(ans+a)%m;
        a=(a+a)%m;
        b>>=1;
    }
    return ans;
}

LL Resolver::gcd(LL a,LL b)
{
	return b==0?a:gcd(b,a%b);
}