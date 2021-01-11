#include "../Course/CourseLib/creategame.hh"
#include "core/city.hh"

std::shared_ptr<Interface::ICity> Interface::createGame()
{
    std::shared_ptr<Student::City> gameCity = std::make_shared<Student::City>();

    return gameCity;
}

