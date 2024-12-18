﻿#include <iostream>
#include <mutex>


std::mutex m;

int coins = 1002;
int Bob_coins = 0;
int Tom_coins = 0;

void coin_sharing(std::string name, int& thief_coins, int& companion_coins, bool& flag)
{
	while (coins > 1)
	{
		m.lock();
		if (coins == 1 && Bob_coins == Tom_coins)
		{
			m.unlock();
			break;
		}
		if ((flag == true && name == "Tom") || (flag == false && name == "Bob"))
		{
			thief_coins++;
			coins--;
			std::cout << name << " :" << thief_coins << " " << companion_coins << std::endl;
			flag = !flag;
		}
		m.unlock();
	}
}

int main()
{
	std::string tom = "Tom";
	std::string bob = "Bob";
	bool flag = true;
	std::thread thread1(coin_sharing, tom, std::ref(Tom_coins), std::ref(Bob_coins), std::ref(flag));
	std::thread thread2(coin_sharing, bob, std::ref(Bob_coins), std::ref(Tom_coins), std::ref(flag));

	thread1.join();
	thread2.join();

	system("pause");
	int deadman = 0;
	if (coins % 2 == 1)
		deadman += 1;
	std::cout << "Общие деньги: " << Tom_coins + Bob_coins + deadman << std::endl;
	std::cout << tom << " :" << Tom_coins << std::endl;
	std::cout << bob << " :" << Bob_coins << std::endl;
	std::cout << "Мертвец: " << deadman << std::endl;
}
