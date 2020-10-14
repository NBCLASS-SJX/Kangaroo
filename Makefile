cc = g++
library_dir = -L /usr/lib/oracle/11.2/client64/lib -L /usr/local/lib
include_dir = -I /usr/include/oracle/11.2/client64/ -I /usr/local/include
c_flags = -std=c++11 -g
libs = -lpthread -lnnz11 -lociei -lclntsh -lthrift -lrdkafka
objs = common.o \
	   kgr_bytes.o \
	   logger.o \
	   base64.o \
	   md5.o \
	   des.o \
	   rsa.o \
	   otldb.o \
	   kafka_producer.o \
	   tinyxml2.o \
	   xmlformat.o \
	   xmlparser.o \
	   threadpool.o 

test_objs = utils_test.o \
			bytes_test.o \
			encrypt_test.o \
			xmlcodec_test.o \
			kafka_test.o 


exe = test/bytes_test \
	  test/utils_test \
	  test/encrypt_test \
	  test/xmlcodec_test \
	  test/kafka_test 


$(exe):$(objs) $(test_objs)
	$(cc) utils_test.o $(objs) $(c_flags) -o test/utils_test $(libs) $(include_dir) $(library_dir)
	$(cc) bytes_test.o $(objs) $(c_flags) -o test/bytes_test $(libs) $(include_dir) $(library_dir)
	$(cc) encrypt_test.o $(objs) $(c_flags) -o test/encrypt_test $(libs) $(include_dir) $(library_dir)
	$(cc) xmlcodec_test.o $(objs) $(c_flags) -o test/xmlcodec_test $(libs) $(include_dir) $(library_dir)
	$(cc) kafka_test.o $(objs) $(c_flags) -o test/kafka_test $(libs) $(include_dir) $(library_dir)

common.o:
	$(cc) -c $(c_flags) src/common/common.cpp
logger.o:
	$(cc) -c $(c_flags) src/common/logger.cpp
kgr_bytes.o:
	$(cc) -c $(c_flags) src/common/kgr_bytes.cpp

base64.o: src/encrypt/base64.h src/encrypt/base64.cpp
	$(cc) -c $(c_flags) src/encrypt/base64.cpp
md5.o: src/encrypt/md5.h src/encrypt/md5.cpp
	$(cc) -c $(c_flags) src/encrypt/md5.cpp
des.o: src/encrypt/des.h src/encrypt/des.cpp
	$(cc) -c $(c_flags) src/encrypt/des.cpp
rsa.o: src/encrypt/rsa.h src/encrypt/rsa.cpp
	$(cc) -c $(c_flags) src/encrypt/rsa.cpp

otldb.o: src/database/otldb.h src/database/otldb.cpp src/database/otlv4.h
	$(cc) -c $(c_flags) src/database/otldb.cpp src/database/otlv4.h $(include_dir)

kafka_producer.o: src/kafka/kafka_producer.h src/kafka/kafka_producer.cpp
	$(cc) -c $(c_flags) src/kafka/kafka_producer.cpp

tinyxml2.o: src/xmlcodec/tinyxml2.h src/xmlcodec/tinyxml2.cpp
	$(cc) -c $(c_flags) src/xmlcodec/tinyxml2.cpp
xmlformat.o: src/xmlcodec/xmlformat.h src/xmlcodec/xmlformat.cpp
	$(cc) -c $(c_flags) src/xmlcodec/xmlformat.cpp
xmlparser.o: src/xmlcodec/xmlparser.h src/xmlcodec/xmlparser.cpp
	$(cc) -c $(c_flags) src/xmlcodec/xmlparser.cpp

threadpool.o: src/thread/threadpool.h src/thread/threadpool.cpp
	$(cc) -c $(c_flags) src/thread/threadpool.cpp


utils_test.o: test/utils_test.cpp
	$(cc) -c $(c_flags) test/utils_test.cpp
bytes_test.o: test/bytes_test.cpp
	$(cc) -c $(c_flags) test/bytes_test.cpp
encrypt_test.o: test/encrypt_test.cpp
	$(cc) -c $(c_flags) test/encrypt_test.cpp
xmlcodec_test.o: test/xmlcodec_test.cpp
	$(cc) -c $(c_flags) test/xmlcodec_test.cpp
kafka_test.o: test/kafka_test.cpp
	$(cc) -c $(c_flags) test/kafka_test.cpp


.PHONY: clean distclean
clean:
	rm -f $(objs) $(test_objs) $(exe)
distclean:
	rm -f $(objs) $(test_objs) $(exe) test/Configure.ini

