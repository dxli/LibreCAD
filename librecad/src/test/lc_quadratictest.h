#ifndef LC_QUADRATICTEST_H
#define LC_QUADRATICTEST_H

#include <QObject>
#include <QtTest/QTest>

class LC_QuadraticTest : public QObject
{
	Q_OBJECT
public:
	LC_QuadraticTest(QObject *parent=nullptr);
	static void test();

private:
	static void testEigen2x2();

private slots:
	void initTestCase();
	void testLinearReduction();
};

#endif // LC_QUADRATICTEST_H