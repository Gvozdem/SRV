#include <iostream>
#include <atomic>
#include <vector>
#include <memory>
#include <thread>
#include <Windows.h>

#include "lockfree_stack.h"
#include "node.h"


namespace {
    typedef int Data;
    size_t readers_num = 4;
    lf::LockFreeVersionedStack<Data> stack(readers_num);

    void writer() {
        for (size_t i = 0; i < 10000; i++)
        {
            for (int i = 0; i < 1000; i++)
            {
                stack.push(i);
            }
            for (int i = 0; i < 1000; i++)
            {
                if (!stack.pop())
                    throw std::runtime_error("can't delete element");
            }
            for (int i = 0; i < 500; i++)
            {
                stack.push(i);
            }
            for (int i = 0; i < 500; i++)
            {
                if (!stack.pop())
                {
                    throw std::runtime_error("can't delete element");
                }
            }
        }
        stack.stop();
        std::cout << "All version" << stack.last_version() << std::endl;
    }

    class Reader {
    public:
        Reader(unsigned int id, lf::LockFreeVersionedStack<Data>* stack): id_(id), stack_(stack){}

        void life() {
            while (!stack_->is_stopped()){
                auto data = read();
                check(data);
            }
        }

        std::vector<Data> read() {
            lf::LockFreeVersionedStack<Data>::NodePtr data_ptr;
            if (!stack_->subscribe(id_, data_ptr))
            {
                return {};
            }

            std::vector<Data> result;
            while (data_ptr != nullptr)
            {
                result.push_back(data_ptr->data);
                data_ptr = data_ptr->next;
            }
            versions_cnt++;
        }

        void check(std::vector<Data>& data) {
            if (data.size() > 10000)
            {
                throw std::logic_error("Wrong sequence size");
            }
            if (data.size() > 1) {
                for (size_t i = 0; i < data.size() - 1; i++)
                {
                    if (data[i] - data[i - 1] != -1) {
                        throw std::logic_error("Wrong sequence");
                    }
                }
            }
        }
        unsigned int versions_cnt = 0;
    private:
        int id_;
        lf::LockFreeVersionedStack<int>* stack_;
        std::thread thread_;
    };
}

int main()
{
    std::vector<Reader> readers;
    std::vector<std::thread> threads;
    for (int i = 0; i < readers_num; i++) {
        readers.emplace_back(i, &stack);
    }
    for (int i = 0; i < readers_num; i++)
    {
        threads.emplace_back(i, &stack);
    }

    writer();

    for (size_t i = 0; i < readers_num; i++)
    {
        threads[i].join();
    }
    for (size_t i = 0; i < readers_num; i++) {
        std::cout << "reader" << i << " : " << readers[i].versions_cnt << std::endl;
    }
}