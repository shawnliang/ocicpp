
OBJS = DBConnection.o OCIException.o DBStatement.o DBDataReader.o DBColumnDefinition.o DBValueString.o DBValueNum.o
OUT_FILE = occitest
ORACLE_INSTATN_CLIENT_FOLDER = /home/shawn/Downloads/instantclient_11_2
INCLUDES = -I$(ORACLE_INSTATN_CLIENT_FOLDER)/sdk/include
LIBS = -L$(ORACLE_INSTATN_CLIENT_FOLDER) -lclntsh -lnnz11 -lboost_program_options
CXXFLAGS = -g

CHECK_PROGRAM = checkProgram
CHECK_OBJS = test/testMain.o test/testQuery.o
CHECK_CXXFLAGS = -g

check: $(CHECK_OBJS) $(OBJS)
	g++ -o $(CHECK_PROGRAM) $(CHECK_OBJS) $(OBJS) -lcppunit $(LIBS) $(CHECK_CXXFLAGS) 
	LD_LIBRARY_PATH=$(ORACLE_INSTATN_CLIENT_FOLDER) ./$(CHECK_PROGRAM)

all: $(OBJS)
	g++ -o $(OUT_FILE) $(OBJS) $(LIBS) $(CXXFLAGS)

clean:
	rm -rf $(OBJS) $(OUT_FILE) $(CHECK_PROGRAM) $(CHECK_OBJS)

 %.o: %.cpp
	g++ -c $< -o $@ $(INCLUDES) $(CXXFLAGS)
 
 
