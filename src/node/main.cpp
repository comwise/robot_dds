#include <cstdint>
#include <iostream>
#include <string>
#include <exception>

int main()
{
    int quit_code = 0;
    std::string cfg_file = "$/robot_dds.json";
    bool is_exit_ok = false;

    try {

    } catch (const std::exception &e) {
        std::cout << "robot dds run exception, " << e.what();
        quit_code = -2;
        is_exit_ok = true;
    }
    return quit_code;
}
