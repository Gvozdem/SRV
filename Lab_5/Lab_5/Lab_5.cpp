#include <iostream>
#include <mutex>


std::mutex m;


void coin_sharing(std::string name, int& thief_coins, int& companion_coins, bool& flag_tom, bool& flag_bob, int& deadman, bool& flag_deadman, bool& first_flag)
{
	if (first_flag == true)
		flag_deadman = false;
	if (companion_coins % 2 == 0)
	{
		while (companion_coins > 0)
		{
			if (flag_tom == true && name == "Tom")
			{
				m.lock();
				thief_coins++;
				companion_coins--;
				flag_tom = false;
				flag_bob = true;
				std::cout << name << " :" << thief_coins << " " << companion_coins << std::endl;
				m.unlock();
			}
			else if (flag_bob == true && name == "Bob")
			{
				m.lock();
				thief_coins++;
				companion_coins--;
				flag_tom = true;
				flag_bob = false;
				std::cout << name << " :" << thief_coins << " " << companion_coins << std::endl;
				m.unlock();
			}
		}
	}

	else if (companion_coins % 2 == 1)
	{
		while (companion_coins > 1)
		{
			if (flag_tom == true && name == "Tom")
			{
				m.lock();
				thief_coins++;
				companion_coins--;
				flag_tom = false;
				flag_bob = true;
				std::cout << name << " :" << thief_coins << " " << companion_coins << std::endl;
				m.unlock();
			}
			else if (flag_bob == true && name == "Bob")
			{
				m.lock();
				thief_coins++;
				companion_coins--;
				flag_tom = true;
				flag_bob = false;
				std::cout << name << " :" << thief_coins << " " << companion_coins << std::endl;
				m.unlock();
			}
		}
		m.lock();
		if (flag_deadman == true)
		{
			deadman++;
			companion_coins--;
			flag_deadman = false;
		}
		m.unlock();
	}
}


int main()
{
	int coins = 1002;	
	int Bob_coins = 0;
	int Tom_coins = 0;
	int deadman = 0;
	std::string tom = "Tom";
	std::string bob = "Bob";
	bool flag_tom = true;
	bool flag_bob = false;
	bool flag_deadman = true;
	bool flag = (coins % 2 == 0);
	std::thread thread1(coin_sharing, tom, std::ref(Tom_coins), std::ref(coins), std::ref(flag_tom), std::ref(flag_bob), std::ref(deadman), std::ref(flag_deadman), std::ref(flag));
	std::thread thread2(coin_sharing, bob, std::ref(Bob_coins), std::ref(coins), std::ref(flag_tom), std::ref(flag_bob), std::ref(deadman), std::ref(flag_deadman), std::ref(flag));

	thread1.join();
	thread2.join();

	system("pause");
	std::cout << "Общие деньги: " << Tom_coins + Bob_coins + deadman << std::endl;
	std::cout << tom << " :" << Tom_coins << std::endl;
	std::cout << bob << " :" << Bob_coins << std::endl;
	std::cout << "Мертвец: " << deadman << std::endl;
}
