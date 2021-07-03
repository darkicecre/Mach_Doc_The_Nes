#include "CheckinRecord.h"

bool rawToBookRecord(BookRecord &book, Record &raw){
	if (raw.getDataCount() < 5) return false;

	// uid
	SDCSV.longTo4byte(SDCSV.stringToLong(raw.getData(0)), book.uid);

	// studentID
	book.studentID = SDCSV.stringToLong(raw.getData(1));

	// name
	strncpy(book.name, raw.getData(2), 10);

	// bookID
	book.studentID = SDCSV.stringToLong(raw.getData(3));

	// checkinTimes
	book.checkinTimes = SDCSV.stringToLong(raw.getData(4));

	return true;
}

bool bookToRawRecord(Record &raw, BookRecord &book){
	char *tmpStr;

	// uid
	tmpStr = SDCSV.longToString(SDCSV.byteTolong(book.uid));
	raw.addNextVal(tmpStr);
	free(tmpStr);

	// studentID
	tmpStr = SDCSV.longToString(book.studentID);
	raw.addNextVal(tmpStr);
	free(tmpStr);

	// name
	raw.addNextVal(book.name);

	// bookID
	tmpStr = SDCSV.longToString(book.bookID);
	raw.addNextVal(tmpStr);
	free(tmpStr);

	// checkinTimes
	tmpStr = SDCSV.longToString(book.checkinTimes);
	raw.addNextVal(tmpStr);
	free(tmpStr);

	return true;
}

bool rawToHistoryRecord(HistoryRecord &history, Record &raw){
	if (raw.getDataCount() < 3) return false;

	// date
	char *tmpDateStr = raw.getData(0);
	char tmpDateStrLen = strlen(tmpDateStr);
	history.date = (char*)malloc(tmpDateStrLen+1);
	strncpy(history.date, tmpDateStr, tmpDateStrLen+1);

	// uid
	SDCSV.longTo4byte(SDCSV.stringToLong(raw.getData(1)), history.uid);

	// studentID
	history.studentID = SDCSV.stringToLong(raw.getData(2));

	return true;
}

bool historyToRawRecord(Record &raw, HistoryRecord &history){
	char *tmpStr;

	raw.addNextVal(history.date);

	// uid
	tmpStr = SDCSV.longToString(SDCSV.byteTolong(history.uid));
	raw.addNextVal(tmpStr);
	free(tmpStr);

	// studentID
	tmpStr = SDCSV.longToString(history.studentID);
	raw.addNextVal(tmpStr);
	free(tmpStr);

	return true;
}
