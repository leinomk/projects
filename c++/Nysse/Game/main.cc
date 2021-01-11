#include "../Course/CourseLib/creategame.hh"
#include "../Course/CourseLib/core/logic.hh"
#include "core/city.hh"

#include <QApplication>
#include <QImage>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(offlinedata);

    std::shared_ptr<Interface::ICity> gameCity = Interface::createGame();

    std::shared_ptr<CourseSide::Logic> gamelogic = std::make_shared<CourseSide::Logic>();

    if (std::static_pointer_cast<Student::City>(gameCity)
            ->window->startDialog->exec() == QDialog::Accepted)
    {
        gamelogic->fileConfig();
        gamelogic->takeCity(gameCity);
        gamelogic->finalizeGameStart();
    }
    else {
        return 0;
    }

    return a.exec();
}
