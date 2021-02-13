#include <stdio.h>
#include <math.h>

int main()
{
  int n;
  double k1, k2, k3, kl, t, us, ep, tt;
  scanf("%d %lf %lf %lf", &n, &k1, &k2, &k3);
  if( n == 1 )
  {
    kl = pow(k3 + 1, k1);
    printf("%f\n", (kl - 1) * k2 / (kl * k3) );
  }
  else if( n == 2 )
  {
    kl = k2 / (k1 * k3);
    kl = kl / (kl - 1);
    printf("%.0f\n", log(kl) / log(k3 + 1) );
  }
  else if( n == 3 )
  {
    kl = pow(k3 + 1, k2);
    printf("%f\n", k1 * kl * k3 / (kl - 1) );
  }
  else
  {
  	ep = 0.0001;
  	kl = 1.1;
  	us = pow(kl, k2);
  	t =  us * k1 - k3 * (us - 1) / (kl - 1);
  	while( t > ep || t < -ep )
  	{
  		tt = k2 * pow(kl, k2-1) * ( k1 - k3 / (kl-1) ) + k3 * (us - 1) / pow(kl - 1, 2);
  		kl = kl - t / tt;
  		us = pow(kl, k2);
  		t =  us * k1 - k3 * (us - 1) / (kl - 1);
	}
	printf("%f\n", kl-1);
  }
  return 0;
}
