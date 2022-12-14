all:
	g++ -o producer producer.cpp
	g++ -o consumer consumer.cpp

clean:
	ipcrm -a
	rm -f producer consumer *.o
