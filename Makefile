test: buffer.o jsonBase.o jsonToJpc.o jpcToJson.o jpc.o main.o
	g++ buffer.o jsonBase.o jsonToJpc.o jpcToJson.o jpc.o main.o -Iinclude\\ -Wno-multichar

main.o: src\main.cpp
	g++ -c src\main.cpp -Iinclude\ -Wno-multichar
buffer.o: src\buffer.cpp include\buffer.hpp
	g++ -c src\buffer.cpp -Iinclude\ -Wno-multichar
jsonBase.o: src\jsonBase.cpp include\jsonBase.hpp
	g++ -c src\jsonBase.cpp -Iinclude\ -Wno-multichar
jsonToJpc.o: src\jsonToJpc.cpp include\jsonToJpc.hpp
	g++ -c src\jsonToJpc.cpp -Iinclude\ -Wno-multichar
jpcToJson.o: src\jpcToJson.cpp include\jpcToJson.hpp
	g++ -c src\jpcToJson.cpp -Iinclude\ -Wno-multichar
jpc.o: src\jpc.cpp include\jpc.hpp 
	g++ -c src\jpc.cpp -Iinclude\ -Wno-multichar

clean:
	-rm -f *.o core *.core