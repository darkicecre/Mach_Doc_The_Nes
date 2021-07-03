#ifndef SDCSV_H
#define SDCSV_H

#include <SD.h>
#include <Arduino.h>
#include <stdint.h>
#include <string.h>

class Record {
public:
	Record* next();
	void connectToNextNode(Record* nextRec);
	void setNextRecord(Record *next);
	char* getData(uint8_t index);
	char** getData();
	uint8_t getDataCount();
	void addNextVal(const char *val);
	void clear();
	void close();
	void remove(uint8_t colIndex);
private:
	Record *nextRec = NULL;
	char **data=NULL;
	uint8_t dataCount = 0;
};

class RecordList {
public:
	Record *getHead();
	Record *getTail();
	bool isEmpty();
	void close();
	void addNextRecord(Record *rec);
private:
	Record *head = NULL;
};

class SDCSVClass {
public:
	char* readln(File *myfile, int max = 100);
	long byteTolong(char a[]);
	void longTo4byte(long val,char a[]);

	char* longToString(uint32_t val);
	uint32_t stringToLong(const char *str);

	void createNewCSV(const char* fileName, const char** titleCols, int colsAmount);
	void addNewRecordToCSV(File *file, const char** strArr, int strAmount);
	void addNewRecordToCSV(File *file, Record *rec);
	RecordList getRecordsFromCSV(File *file, int8_t colIndex, const char *colVal);
	Record* getNextRecordInFile(File *file);
};

extern SDCSVClass SDCSV;

#endif
