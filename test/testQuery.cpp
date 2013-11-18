#include <cppunit/extensions/HelperMacros.h>
#include "../ociwrapper.h"
#include "testSetting.h"

namespace db = prophetstor::ociwrapper;


class QueryTest : public CppUnit::TestFixture {

CPPUNIT_TEST_SUITE( QueryTest );
CPPUNIT_TEST( testStringField );
CPPUNIT_TEST( testInt64Field );
CPPUNIT_TEST_SUITE_END();

    db::DBConnection* conn;
public:
    void setUp()
    {
        conn = new db::DBConnection(g_User, g_Password, g_ConnectionString);
        conn->connect();
    }

    void tearDown()
    {
        delete conn;
    }

    void testStringField()
    {
       db::DBStatement stmt(*conn, "select name from v$database");
       stmt.prepare();
       db::DBDataReader* reader = stmt.executeReader();
       CPPUNIT_ASSERT(reader->hasNext());
       CPPUNIT_ASSERT_EQUAL(std::string("XE"), reader->get(1).getString());
    }

    void testInt64Field()
    {
        db::DBStatement stmt(*conn, "select DBID from v$database");
        stmt.prepare();
        db::DBDataReader* reader = stmt.executeReader();
        CPPUNIT_ASSERT(reader->hasNext());
        CPPUNIT_ASSERT_EQUAL(2711468963L, reader->get(1).getLong());
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( QueryTest );
