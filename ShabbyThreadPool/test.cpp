#include "ShabbyThreadPool.h"
#include <string>

int main()
{
    ShabbyThreadPool* node = new ShabbyThreadPool();
    auto func_set = node->GetInstance()->GetInstance();

    func_set->SetFunctor<void, int, int>("ADD", std::function<void(int,int)>([](int a, int b) { printf(std::to_string(a + b).c_str()); }));
    func_set->GetFunctor<void, int, int>("ADD")(3, 4);

    

    return 0;
}
