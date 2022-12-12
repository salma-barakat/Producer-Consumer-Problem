all:
	g++ -o producer producer.cpp
	g++ -o consumer consumer.cpp

clean:
	rm -f producer consumer *.o