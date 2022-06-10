#include<iostream>
#include<math.h>
using namespace std;

float drv(float s, int n, float z[],float c)
{
	float sum = 0;
	for (int i = 1; i <= n; i++)
	{
		sum = sum + ((i * c) / pow((1 + s + z[i - 1]), (i + 1)));
		if (i == n)
		{
			sum = sum + (100 * n / pow((1 + s + z[i - 1]), (i + 1)));
		}
	}

	return -sum;
}

float fun(float s, int n, float z[],float c)
{
	float sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum = sum + (c * pow((1 + z[i] + s), -i - 1));

		if (i == n-1)
		{
			sum = sum + (100 / pow((1 + s + z[i]), (i + 1)));
		}
	}
	return sum;
}

int main()
{
	int n;
	float ZeroRate[1000];
	float Yield[1000];
	float C, yieldSpread, staticSpread = 0.03; //預設0.03 
	cout << "輸入期數:";
	cin >> n;
	cout << "輸入債息:";
	cin >> C;
	cout << "輸入yield spread:";
	cin >> yieldSpread;
	for (int i = 0; i < n; i++)
	{
		cout << "輸入第" << i + 1 << "期殖利率:"; 
		cin >> Yield[i];
	}
	
	//第一期Zero rate=Yield 
	ZeroRate[0] = Yield[0];  
	
	//計算i+1期zero rate 
	for (int i = 1; i < n; i++)
	{
		float BondValue = 0;
		for (int j = 0; j <= i; j++)
		{
			float Discount = 1;
			for (int k = 0; k <= j; k++)
				Discount = Discount / (1 + Yield[i]);
			BondValue = BondValue + Discount * C;
			if (j == i)
			{
				BondValue = BondValue + Discount * 100;
			}
		}

		for (int j = 0; j < i; j++)
		{
			float PV = C;
			for (int k = 0; k <= j; k++)
			{
				PV = PV / (1 + ZeroRate[j]);
			}
			BondValue = BondValue - PV;
		}
		
		ZeroRate[i] = pow((C + 100) / BondValue, 1.0 / (i + 1)) - 1;
	}


	float price = 0;
	for (int i = 1; i <= n; i++)//計算債券價格
	{
		price = price + (C * pow(((1 + Yield[i - 1]) + yieldSpread), -i));
		if (i == n)
		{
			price = price + 100 * pow((1 + Yield[i - 1] + yieldSpread), -n);
		}
	}

	float s = 0.01;  //先假定的一個值 
	while (((fun(staticSpread, n, ZeroRate,C) -price)) != ((fun(s, n, ZeroRate,C) -price)))
	{
		s = staticSpread;
		
		staticSpread = staticSpread - (fun(staticSpread, n, ZeroRate, C) -price ) / drv(staticSpread, n, ZeroRate, C);

	}

	cout << "Static spread:" << staticSpread << endl;

	return 0;
}
