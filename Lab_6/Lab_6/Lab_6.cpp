#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

class Vector_Dish {
public:
    std::vector<int> dishes; //массив тарелок
    std::mutex mutex; //просто чтобы был, как один для всех классов, т.к. везде используется Dish

    Vector_Dish() : dishes(3, 3000) {}
};


class Fatty {
private:
    int id; //номер толстяка
    int gluttony; //прожорливость 
    int count_eating; //сколько съел наггетсов
    Vector_Dish& dish; //ссылка на тарелки
    std::atomic<int>& death_count; //сколько погибло бойцов
    std::atomic<bool>& all_death; //проверка, что погибли
public:
    Fatty(int id, Vector_Dish& dish, int gluttony, std::atomic<int>& death_count, std::atomic<bool>& all_death) : id(id), dish(dish), gluttony(gluttony), count_eating(0), death_count(death_count), all_death(all_death) {}

    void eat() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::lock_guard<std::mutex> lock(dish.mutex);
            //Если остались наггетсы на тарелке, то толстяк их съедает
            if (dish.dishes[id] > 0) {
                dish.dishes[id] -= gluttony;
                count_eating += gluttony;
                std::cout << "Толстяк " << id + 1 << " съел " << gluttony << " наггетсов, осталось: " << dish.dishes[id] << "\n";
            }
            //Смотрим, чтобы толстяк не хлопнулся и если хлопнулся, то увеличиваем счётчик смертей
            if (count_eating >= 10000) {
                std::cout << "Толстяк " << id + 1 << " самоуничтожился!\n";
                ++death_count;
                if (death_count == 3) {
                    all_death = true;
                }
                break;
            }
            //Если наггетсов не осталось, то кук недоработал
            else if (dish.dishes[id] <= 0) {
                std::cout << "Толстяк " << id + 1 << " остался без наггетсов! Кук уволен.\n";
                exit(0);
            }
        }
    }
};

class Cook {
private:
    Vector_Dish& dish; //ссылка на тарелки
    int efficiency; //сколько производит наггетсов
    std::atomic<bool>& all_death; //проверка на смерть всех пухликов

public:
    Cook(Vector_Dish& dish, int efficiency, std::atomic<bool>& all_death)
        : dish(dish), efficiency(efficiency), all_death(all_death) {}

    void cook() const {
        //Делаем небольшую задержку, чтобы кук и толстяки работали в разное время
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //Засекаем для того, чтобы кук не перетруждался
        const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        while (true) {

            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::lock_guard<std::mutex> lock(dish.mutex);
            //Проверяем что хоть кто-то жив
            if (all_death) {
                std::cout << "Все толстяки погибли! Кук не получил зарплату.\n";
                break;
            }
            
            //Добавляем всем по покушать
            for (int i = 0; i < dish.dishes.size(); ++i) {
                dish.dishes[i] += efficiency;
                std::cout << "Кук добавил " << efficiency << " наггетсов на тарелку " << i + 1 << ", всего: " << dish.dishes[i] << "\n";
            }
            //Смотрим перетрудился ли кук
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
            if (elapsed.count() >= 6)
            {
                std::cout << "Кук уволился!\n";
                exit(0);
            }
        }
    }
};

int main() {
    Vector_Dish dish_mas;
    int gluttony = 500;
    int efficiency = 50;

    std::atomic<int> dead_count(0);
    std::atomic<bool> all_dead(false);

    Fatty fatty1(0, dish_mas, gluttony, dead_count, all_dead);
    Fatty fatty2(1, dish_mas, gluttony, dead_count, all_dead);
    Fatty fatty3(2, dish_mas, gluttony, dead_count, all_dead);
    Cook cook(dish_mas, efficiency, all_dead);

    std::thread t1(&Fatty::eat, &fatty1);
    std::thread t2(&Fatty::eat, &fatty2);
    std::thread t3(&Fatty::eat, &fatty3);
    std::thread t4(&Cook::cook, &cook);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
