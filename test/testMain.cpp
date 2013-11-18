#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <string>

std::string g_User = "sys";
std::string g_Password = "123456";
std::string g_ConnectionString = "172.31.9.127/XE";

int main(int argc, char **argv)
{
    CppUnit::TextUi::TestRunner runner;

    TestFactoryRegistry: CppUnit::TestFactoryRegistry &registry =
            CppUnit::TestFactoryRegistry::getRegistry();

    runner.addTest( registry.makeTest() );
    runner.run();
    return 0;
}
