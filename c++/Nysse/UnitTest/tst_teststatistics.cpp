#include "../Game/core/statistics.hh"
#include <QtTest>


class TestStatistics : public QObject
{
    Q_OBJECT

public:
    TestStatistics();
    ~TestStatistics();

private Q_SLOTS:
    void createStatisticsInstance();

};

TestStatistics::TestStatistics()
{

}

TestStatistics::~TestStatistics()
{

}

void TestStatistics::createStatisticsInstance()
{
    Student::Statistics stats;
    QCOMPARE(stats.getNysses(), 0i);
    QCOMPARE(stats.getPassengers(), 0i);
    QCOMPARE(stats.getNyssesDestroyed(), 0i);
}


QTEST_APPLESS_MAIN(TestStatistics)

#include "tst_teststatistics.moc"
