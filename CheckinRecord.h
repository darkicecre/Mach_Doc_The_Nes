#ifndef _CheckinRecord_H_
#define _CheckinRecord_H_

#include "SDCSV.h"
#include <string.h>

struct BookRecord {
	byte uid[4];//uid
	long studentID;//msv
	char name[10];//ten
	byte bookID;//mã sổ điểm danh
	byte checkinTimes = 0;//số ngày sinh hoạt
}; // Bản ghi của file truy xuất tổng

struct HistoryRecord {
	char* date = NULL;//chuỗi biểu diễn ngày giờ
	byte uid[4];//uid
	long studentID;//msv
};// Bản ghi của file lịch sử quét. CHÚ Ý: quản lý, giải phóng char*

bool rawToBookRecord(BookRecord &book, Record &raw);
bool bookToRawRecord(Record &raw, BookRecord &book);
bool rawToHistoryRecord(HistoryRecord &history, Record &raw);
bool historyToRawRecord(Record &raw, HistoryRecord &history);

#endif
