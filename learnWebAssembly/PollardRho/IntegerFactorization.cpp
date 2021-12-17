#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cstring>
#include <algorithm>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;
typedef long long LL;

const int N=6e5;
LL fac[N],num[N];
int cnt;

LL gcd(LL a,LL b){
    return b==0?a:gcd(b,a%b);
}
LL multi(LL a,LL b,LL m){
    LL ans=0;
    while(b){
        if(b&1) ans=(ans+a)%m;
        a=(a+a)%m;
        b>>=1;
    }
    return ans;
}
LL quick_mod(LL a,LL p,LL m){
    LL ans=1;
    while(p){
        if(p&1) ans=multi(ans,a,m);
        a=multi(a,a,m);
        p>>=1;
    }
    return ans;
}
LL Pollard_rho(LL n,LL c){
    LL x,y,k=2,i=1;
    x=rand()%(n-1)+1;
    y=x;
    while(1>0){
        i++;
        x=(multi(x,x,n)+c)%n;
        LL d=gcd((y-x+n)%n,n);
        if(1<d&&d<n) return d;
        if(y==x)  return n;   // 出现循环
        if(i==k) {
            y=x;
            k<<=1;  //x 比y多跑一圈
        }
    }
}
bool Miller_rabin(LL p){
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
 
void find(LL n,LL c){
    if(n==1) return ;
    if(Miller_rabin(n)){
        fac[cnt++]=n;
        return ;
    }
    LL p=n;
    while(p>=n) p=Pollard_rho(p,c--);  //循环避免没找到
    find(p,c);
    find(n/p,c);
}

#ifdef __EMSCRIPTEN__
extern "C"
{
EMSCRIPTEN_KEEPALIVE
#endif
string IntegerFactorization(int x)
{
    char str[10005] = { 0 };
    cnt=0;
    find(x,120);
    sort(fac,fac+cnt);
    memset(num,0,sizeof(num));
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
        sprintf(str+strlen(str), "%lld^%lld*", fac[i], num[i]);
    }
    sprintf(str+strlen(str), "%lld^%lld\n", fac[k], num[k]);
    return string(str);
}

}