
OBJS = main.o DBConnection.o OCIException.o DBStatement.o DBDataReader.o DBColumnDefinition.o DBValueString.o
OUT_FILE = occitest
INCLUDES = -I/home/shawn/Downloads/instantclient_10_2/sdk/include
LIBS = -L/home/shawn/Downloads/instantclient_10_2 -lclntsh -lnnz10 -lboost_program_options
CXXFLAGS = -g


all: $(OBJS)
	g++ -o $(OUT_FILE) $(OBJS) $(LIBS) $(CXXFLAGS)

clean:
	rm -rf $(OBJS) $(OUT_FILE)

 %.o: %.cpp
	g++ -c $< -o $@ $(INCLUDES) $(CXXFLAGS)
 
 
